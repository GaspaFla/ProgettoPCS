#include "Utils.hpp"
#include "Utils2.hpp"
#include "GeometryLibrary.hpp"
#include "Eigen/Eigen"
#include "Sort.hpp"
#include "UCDUtilities.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <array>

using namespace std;
using namespace Gedim;


namespace DFN{
//La traccia madre ha già i vertici numerati con id
void Taglia(Frattura& F,Frattura & FMadre,vector<Traccia>& Tracce, double tol,double tol2){
    if(!F.TraccePass.empty()){
        bool TracciaSulBordo = false;
        vector<Frattura> figlie = calcoloSottoPoligoniPass(F,tol,tol2,TracciaSulBordo,Tracce,FMadre);
        if(TracciaSulBordo){
            Taglia(figlie[0],FMadre,Tracce,tol,tol2);
        }
        else{
            Taglia(figlie[0],FMadre,Tracce,tol,tol2);
            Taglia(figlie[1],FMadre,Tracce,tol,tol2);
        }

    }
    else if(!F.TracceNoPass.empty()){
        bool UnicaFrattura=false;
        vector<Frattura> figlie = calcoloSottoPoligoniNoPass(F,tol, tol2, Tracce, FMadre, UnicaFrattura);
        if(UnicaFrattura){
            Taglia(figlie[0],FMadre,Tracce,tol,tol2);
        }
        else{
            Taglia(figlie[0],FMadre,Tracce,tol,tol2);
            Taglia(figlie[1],FMadre,Tracce,tol,tol2);
        }

    }
    else{//Vuol dire che non ha più tracce e posso salvarlo come elemento della mesh
        converteInCelle(F,FMadre);
    }
}

void converteInCelle(Frattura& F,Frattura &FMadre){
    //Devo aggiungere solo Cell1D e la Cell2D, ovvero la frattura stessa F
    //Le celle0D le aggiunge CalcoloSottoPoligoni
    //Ogni Frattura F ha il vettore contenente gli id dei suoi vertici

    //Aggiungo cell1D ==> Aggiungo Id, e gli id degli estremi, poi incremento il numero di Cell1D della mesh
    vector<unsigned int> IdLatiCell2D;

    //Prima di inserire un lato devo controllare se esiste già un lato che ha quei due vertici come id
    //Se esiste uso l'id di quel lato, altrimenti ne creo uno nuovo

    for(unsigned int i= 0;i<F.NumVertici;i++){
        bool esisteLato = false;
        array<unsigned int,2> IdVerticiCell1D;
        if(i == F.NumVertici -1){
            IdVerticiCell1D= {F.IdVertici[i],F.IdVertici[0]};
        }
        else{
            IdVerticiCell1D = {F.IdVertici[i],F.IdVertici[i+1]};

        }

        unsigned int IdCell1D ;
        for(unsigned int j = 0;j<FMadre.SottoPoligoni.VerticiCell1D.size();j++){
            if((FMadre.SottoPoligoni.VerticiCell1D[j][0] == IdVerticiCell1D[0] && FMadre.SottoPoligoni.VerticiCell1D[j][1] == IdVerticiCell1D[1]) || (FMadre.SottoPoligoni.VerticiCell1D[j][0] == IdVerticiCell1D[1] && FMadre.SottoPoligoni.VerticiCell1D[j][1] == IdVerticiCell1D[0]) ){
                esisteLato = true; //Non devo aggiungerlo
                IdCell1D = FMadre.SottoPoligoni.IdCell1D[j];

            }
        }
        if(!esisteLato){//Se non esiste, crea una nuova Cell1D
            IdCell1D =  FMadre.SottoPoligoni.NumeroCell1D;
            FMadre.SottoPoligoni.IdCell1D.push_back(IdCell1D);
            FMadre.SottoPoligoni.VerticiCell1D.push_back (IdVerticiCell1D);
            FMadre.SottoPoligoni.NumeroCell1D++;
        }

        IdLatiCell2D.push_back(IdCell1D);

    }

    //Aggiungo Cell2D ==>Aggiungo ID, vettori con gli id dei vertici e il vettore con gli id dei Lati
    unsigned int IdCell2D = FMadre.SottoPoligoni.NumeroCell2D;
    FMadre.SottoPoligoni.IdCell2D.push_back(FMadre.SottoPoligoni.NumeroCell2D);
    FMadre.SottoPoligoni.VerticiCell2D.push_back(F.IdVertici);
    FMadre.SottoPoligoni.LatiCell2D.push_back(IdLatiCell2D);

    FMadre.SottoPoligoni.NumeroCell2D++;
}

vector<Frattura> calcoloSottoPoligoniPass(Frattura& F,double tol, double tol2,bool& TracciaSulBordo, vector<Traccia>& Tracce, Frattura& FMadre){
    vector<unsigned int> NuoviIndiciPositivi;//salvataggio indici corrispondenti
    vector<unsigned int> NuoviIndiciNegativi;//salvataggio indici corrispondenti
    vector<Frattura> Figli;//output
    vector<Vector3d>PuntiPositivi;//vettore in cui mettere i punti sul semipiano positivo
    vector<Vector3d>PuntiNegativi;//vettore in cui mettere i punti sul semipiano negativo
    unsigned int PuntiNuovi=0; //quanti nuovi vertici ho trovato
    array<Vector3d,2> VerticiTraccia={ Tracce[F.TraccePass[0]].VerticiTraccia[0],Tracce[F.TraccePass[0]].VerticiTraccia[1]};//vettore con dentro i due vertici della traccia
    int SegnoPrec=1;
    double segno;
    Vector3d Direzione=(VerticiTraccia[0]-VerticiTraccia[1]);
    unsigned int NuovoId;
    for (unsigned int i=0; i<F.NumVertici; i++){

        if(PuntiNuovi<2){//se devo ancora trovarne faccio controlli altrimenti faccio un'assegnazione veloce
            //calcolo dove sta rispetto alla traccia, calcolandone il segno
            segno=((Direzione).cross(VerticiTraccia[0]-F.CoordinateVertici[i])).dot(F.vecNormale);
            if(i==0){//sono nel primo valore
                //il segno ha valore sotto la tolleranza
                if(abs(segno)<tol2){
                    //caso 1,  la traccia è compresa nel lato precedente
                    if((Direzione).cross(F.CoordinateVertici[0]-F.CoordinateVertici[F.NumVertici-1]).squaredNorm()<tol){
                        TracciaSulBordo=true;
                        vector<Vector3d>VerticiTracciaOrdinati={};
                        SegnoPrec=1;
                        //guardo quale è il più vicino e controllo se ci sono dei punti che coincidono. Se non è così inserisco punti nuovi ordinati
                        if((VerticiTraccia[1]-F.CoordinateVertici[F.NumVertici-1]).squaredNorm()<(VerticiTraccia[0]-F.CoordinateVertici[F.NumVertici-1]).squaredNorm()){
                            if((VerticiTraccia[1]-F.CoordinateVertici[i]).squaredNorm() <tol2 || (VerticiTraccia[1]-F.CoordinateVertici[F.NumVertici-1]).squaredNorm() <tol2){
                                if((VerticiTraccia[0]-F.CoordinateVertici[F.NumVertici-1]).squaredNorm() >tol2 && (VerticiTraccia[0]-F.CoordinateVertici[i]).squaredNorm() >tol2){
                                    //visto che i punti coincidono assegno solo l'altro
                                    VerticiTracciaOrdinati={VerticiTraccia[0]};
                                }

                            }
                            else{

                                VerticiTracciaOrdinati={VerticiTraccia[1],VerticiTraccia[0]};

                            }
                        }
                        else {//non era il più vicino l'1 ma il vertice 0
                            if((VerticiTraccia[0]-F.CoordinateVertici[i]).squaredNorm() <tol2 || (VerticiTraccia[0]-F.CoordinateVertici[F.NumVertici-1]).squaredNorm() <tol2){
                                if((VerticiTraccia[1]-F.CoordinateVertici[F.NumVertici-1]).squaredNorm() >tol2 && (VerticiTraccia[1]-F.CoordinateVertici[i]).squaredNorm() >tol2){
                                    //visto che i punti coincidono assegno solo l'altro
                                    VerticiTracciaOrdinati={VerticiTraccia[1]};

                                }
                            }
                            else{
                                VerticiTracciaOrdinati={VerticiTraccia[0],VerticiTraccia[1]};
                                //guardo solo se sta sulla retta con il precedente sempre
                            }

                        }
                        //salvo tutti nel positivo
                        for (unsigned int j=0; j<VerticiTracciaOrdinati.size(); j++){

                            PuntiPositivi.push_back(VerticiTracciaOrdinati[j]);
                            NuoviIndiciPositivi.push_back(RicercaIdVertice(FMadre, VerticiTracciaOrdinati[j], tol2));

                        }
                        PuntiPositivi.push_back(F.CoordinateVertici[i]);
                        NuoviIndiciPositivi.push_back(F.IdVertici[i]);
                        PuntiNuovi++;
                        PuntiNuovi++;
                    }
                    //caso 2, il vertice coincide con il vertice traccia ma il lato non è compreso
                    //essendo che la traccia è passante sono sicuro che se anche il vertice 1 coincidesse con il vertice del poligono ho zero, perchè avrei uguale la differenza tra una traccia e l'altra
                    else if(((VerticiTraccia[0]-F.CoordinateVertici[i]).squaredNorm() <tol2) || ((VerticiTraccia[1]-F.CoordinateVertici[i]).squaredNorm() <tol2) ){

                        //il vertice del poligono sta al bordo del semipiano
                        PuntiNuovi++;
                        PuntiPositivi.push_back(F.CoordinateVertici[i]);
                        NuoviIndiciPositivi.push_back(F.IdVertici[i]);
                        PuntiNegativi.push_back(F.CoordinateVertici[i]);
                        NuoviIndiciNegativi.push_back(F.IdVertici[i]);

                        if((Direzione).cross(VerticiTraccia[0]-F.CoordinateVertici[i+1]).dot(F.vecNormale)>0){
                            SegnoPrec=1;
                        }
                        else{
                            SegnoPrec=-1;
                        }

                    }
                     //caso 3, la traccia stia sul lato successivo e non il precedente, metto i vertici e vado avanti
                    else{

                        TracciaSulBordo=true;
                        if(SegnoPrec>0){
                            PuntiPositivi.push_back(F.CoordinateVertici[i]);
                            NuoviIndiciPositivi.push_back(F.IdVertici[i]);

                        }
                        else{
                            PuntiNegativi.push_back(F.CoordinateVertici[i]);
                            NuoviIndiciNegativi.push_back(F.IdVertici[i]);

                        }
                    }
                }
                //il segno non è nullo, posso dare un semipiano al vertice
                else{
                    if(segno>0){
                        PuntiPositivi.push_back(F.CoordinateVertici[i]);
                        NuoviIndiciPositivi.push_back(F.IdVertici[i]);
                        SegnoPrec=1;

                    }
                    else{
                        PuntiNegativi.push_back(F.CoordinateVertici[i]);
                        NuoviIndiciNegativi.push_back(F.IdVertici[i]);
                        SegnoPrec=-1;

                    }
                }



            }
            //non siamo nel primo
            else {
                if(abs(segno)<tol2){
                    //caso 1,  il lato  è compreso
                    if((Direzione).cross(F.CoordinateVertici[i]-F.CoordinateVertici[i-1]).squaredNorm()<tol){
                        TracciaSulBordo=true;
                        vector<Vector3d>VerticiTracciaOrdinati={};
                        SegnoPrec=1;
                        //guardo quale è il più vicino e controllo se ci sono delle sovrapposizioni tra i vertici della frattura e i vertici della traccia


                        if((VerticiTraccia[1]-F.CoordinateVertici[i-1]).squaredNorm()<(VerticiTraccia[0]-F.CoordinateVertici[i-1]).squaredNorm()){
                            //controllo se uno dei due è uguale a un vertice del poligono
                            if((VerticiTraccia[1]-F.CoordinateVertici[i]).squaredNorm() <tol2 || (VerticiTraccia[1]-F.CoordinateVertici[i-1]).squaredNorm() <tol2){
                                if((VerticiTraccia[0]-F.CoordinateVertici[i-1]).squaredNorm() >tol2 && (VerticiTraccia[0]-F.CoordinateVertici[i]).squaredNorm() >tol2){
                                    //visto che i punti coincidono assegno solo l'altro
                                    VerticiTracciaOrdinati={VerticiTraccia[0]};
                                }

                            }
                            else{

                                VerticiTracciaOrdinati={VerticiTraccia[1],VerticiTraccia[0]};

                            }
                        }
                        else {
                            if((VerticiTraccia[0]-F.CoordinateVertici[i]).squaredNorm() <tol2 || (VerticiTraccia[0]-F.CoordinateVertici[i-1]).squaredNorm() <tol2){
                                if((VerticiTraccia[1]-F.CoordinateVertici[i-1]).squaredNorm() >tol2 && (VerticiTraccia[1]-F.CoordinateVertici[i]).squaredNorm() >tol2){
                                    //visto che i punti coincidono assegno solo l'altro
                                    VerticiTracciaOrdinati={VerticiTraccia[1]};

                                }
                            }
                            else{
                                VerticiTracciaOrdinati={VerticiTraccia[0],VerticiTraccia[1]};

                            }

                        }
                        //salvo tutti nel positivo, se ci sono elementi nel negativo il trasferisco tutti nel positivo
                        if(PuntiNegativi.size()>1){
                            PuntiPositivi=PuntiNegativi;
                            NuoviIndiciPositivi=NuoviIndiciNegativi;
                        }
                        for (unsigned int j=0; j<VerticiTracciaOrdinati.size(); j++){

                            PuntiPositivi.push_back(VerticiTracciaOrdinati[j]);
                            NuoviIndiciPositivi.push_back(RicercaIdVertice(FMadre, VerticiTracciaOrdinati[j], tol2));

                        }
                        PuntiPositivi.push_back(F.CoordinateVertici[i]);
                        NuoviIndiciPositivi.push_back(F.IdVertici[i]);
                        PuntiNuovi++;
                        PuntiNuovi++;
                    }

                    //caso 2, il vertice coincide con il vertice traccia ma il lato non è compreso

                    else if(((VerticiTraccia[0]-F.CoordinateVertici[i]).squaredNorm() <tol2) || ((VerticiTraccia[1]-F.CoordinateVertici[i]).squaredNorm() <tol2) ){


                        //dico che ci sono vertici traccia che funzionano, rimane solo più l'altro punto
                        PuntiNuovi++;
                        PuntiPositivi.push_back(F.CoordinateVertici[i]);
                        NuoviIndiciPositivi.push_back(F.IdVertici[i]);
                        PuntiNegativi.push_back(F.CoordinateVertici[i]);
                        NuoviIndiciNegativi.push_back(F.IdVertici[i]);

                        SegnoPrec=-SegnoPrec;
                    }
                    //caso 3, la traccia non sta sul lato precedente ma sta sul successivo

                    else{
                        if(SegnoPrec>0){
                            PuntiPositivi.push_back(F.CoordinateVertici[i]);
                            NuoviIndiciPositivi.push_back(F.IdVertici[i]);

                        }
                        else{
                            PuntiNegativi.push_back(F.CoordinateVertici[i]);
                            NuoviIndiciNegativi.push_back(F.IdVertici[i]);

                        }
                    }
                }

                //la traccia non è sul lato e non ha vertici coincidenti, posso calcolare il semipiano
                else if(segno*SegnoPrec>0){

                    //hanno lo stesso segno, allora lo inserisco nel vettore giusto
                    if(segno>0){
                        PuntiPositivi.push_back(F.CoordinateVertici[i]);
                        NuoviIndiciPositivi.push_back(F.IdVertici[i]);
                    }
                    else{
                        PuntiNegativi.push_back(F.CoordinateVertici[i]);
                        NuoviIndiciNegativi.push_back(F.IdVertici[i]);
                    }
                }
                //se invece hanno segni diversi, guardo quale vertice della traccia è nell'intersezione e lo inserisco
                else {
                    SegnoPrec=-SegnoPrec;
                    PuntiNuovi++;
                    //essendo passanti ho già il punto di intersezione
                    //guardo quale sta sulla retta giusta
                    //aggiungo prima il vertice della traccia e poi del vertice
                    if(((F.CoordinateVertici[i]-F.CoordinateVertici[i-1]).cross(F.CoordinateVertici[i]-VerticiTraccia[0])).squaredNorm()<tol2){
                        NuovoId=RicercaIdVertice(FMadre, VerticiTraccia[0], tol2);
                        if (segno>0){

                            //aggiungo il punto nuovo sia ad una, sia all'altra


                            PuntiNegativi.push_back(VerticiTraccia[0]);
                            NuoviIndiciNegativi.push_back(NuovoId);
                            PuntiPositivi.push_back(VerticiTraccia[0]);
                            NuoviIndiciPositivi.push_back(NuovoId);


                            PuntiPositivi.push_back(F.CoordinateVertici[i]);
                            NuoviIndiciPositivi.push_back(F.IdVertici[i]);
                        }
                        else{

                            PuntiNegativi.push_back(VerticiTraccia[0]);
                            NuoviIndiciNegativi.push_back(NuovoId);
                            PuntiPositivi.push_back(VerticiTraccia[0]);
                            NuoviIndiciPositivi.push_back(NuovoId);
                            PuntiNegativi.push_back(F.CoordinateVertici[i]);
                            NuoviIndiciNegativi.push_back(F.IdVertici[i]);
                        }

                    }
                    //scopro che era il punto 1 e non il punto 0, rifaccio tutto
                    else{
                        NuovoId=RicercaIdVertice(FMadre, VerticiTraccia[1], tol2);

                        if (segno>0){

                            //aggiungo il punto nuovo sia a una sia all'altra

                            PuntiNegativi.push_back(VerticiTraccia[1]);
                            NuoviIndiciNegativi.push_back(NuovoId);
                            PuntiPositivi.push_back(VerticiTraccia[1]);
                            NuoviIndiciPositivi.push_back(NuovoId);
                            PuntiPositivi.push_back(F.CoordinateVertici[i]);
                            NuoviIndiciPositivi.push_back(F.IdVertici[i]);


                        }
                        else{


                            PuntiNegativi.push_back(VerticiTraccia[1]);
                            NuoviIndiciNegativi.push_back(NuovoId);
                            PuntiPositivi.push_back(VerticiTraccia[1]);
                            NuoviIndiciPositivi.push_back(NuovoId);
                            PuntiNegativi.push_back(F.CoordinateVertici[i]);
                            NuoviIndiciNegativi.push_back(F.IdVertici[i]);


                        }

                    }

                }

            }

        }
//ho già trovato i due punti che fanno avvenire il cambio di semipiano, inserisco gli altri in base al segno precedente
        else{
            if(SegnoPrec>0){
                PuntiPositivi.push_back(F.CoordinateVertici[i]);
                NuoviIndiciPositivi.push_back(F.IdVertici[i]);
            }
            else{
                PuntiNegativi.push_back(F.CoordinateVertici[i]);
                NuoviIndiciNegativi.push_back(F.IdVertici[i]);
            }
        }
    }
    //sono uscito dal for, vado a controllare se c'è un punto da aggiungere tra l'ultimo e il primo vertice, che non avevo ancora analizzato
    if(PuntiNuovi<2){
        segno=((Direzione).cross(VerticiTraccia[0]-F.CoordinateVertici[0])).dot(F.vecNormale);
        if(segno*SegnoPrec<0){
            if(((F.CoordinateVertici[F.NumVertici-1]-F.CoordinateVertici[0]).cross(F.CoordinateVertici[0]-VerticiTraccia[0])).squaredNorm()<tol2){
                NuovoId=RicercaIdVertice(FMadre, VerticiTraccia[0], tol2);
                if (segno>0){

                    //aggiungo il punto nuovo sia a una sia all'altra


                    PuntiNegativi.push_back(VerticiTraccia[0]);
                    NuoviIndiciNegativi.push_back(NuovoId);
                    PuntiPositivi.push_back(VerticiTraccia[0]);
                    NuoviIndiciPositivi.push_back(NuovoId);
                }
                else{



                    PuntiNegativi.push_back(VerticiTraccia[0]);
                    NuoviIndiciNegativi.push_back(NuovoId);
                    PuntiPositivi.push_back(VerticiTraccia[0]);
                    NuoviIndiciPositivi.push_back(NuovoId);

                }

            }
            //scopro che era il punto 1 e non il punto 0, rifaccio tutto
            else{
                NuovoId=RicercaIdVertice(FMadre, VerticiTraccia[1], tol2);

                if (segno>0){

                    //aggiungo il punto nuovo sia a una sia all'altra

                    PuntiNegativi.push_back(VerticiTraccia[1]);
                    NuoviIndiciNegativi.push_back(NuovoId);
                    PuntiPositivi.push_back(VerticiTraccia[1]);
                    NuoviIndiciPositivi.push_back(NuovoId);

                }
                else{


                    PuntiNegativi.push_back(VerticiTraccia[1]);
                    NuoviIndiciNegativi.push_back(NuovoId);
                    PuntiPositivi.push_back(VerticiTraccia[1]);
                    NuoviIndiciPositivi.push_back(NuovoId);
                }

            }
        }
    }

    //ora divido le tracce e controllo se le fratture stanno da una parte o dall'altra
    double segnoVerticeTraccia0;
    double segnoVerticeTraccia1;
    vector<unsigned int>TracceNegative={};//tracce nel semipiano negativo passanti
    vector<unsigned int>TraccePositive={};//tracce nel semipiano positivo passanti
    vector<unsigned int>TracceNegativeNopass;//tracce nel semipiano negativo non passanti
    vector<unsigned int> TraccePositiveNopass;//tracce nel semipiano positivo non passanti
    Vector3d NuovoVerticeTraccia;
    array<Vector3d,2> VerticiTracciaNuova;
    unsigned int IdPositivo;
    unsigned int IdNegativo;
    //se ho un solo poligono aggiungo tutte le tracce in ordine tranne la prima dei passanti
    if(TracciaSulBordo){
        for (unsigned int j=1; j<F.TraccePass.size();j++){
            TraccePositive.push_back(F.TraccePass[j]);

        }
        for (unsigned int j=0; j<F.TracceNoPass.size();j++){
            TraccePositive.push_back(F.TracceNoPass[j]);

        }
    }
    else{
        //se non ho caso degenere vado avanti in ordine, prima con le passanti poi le non passanti
        if(!F.TraccePass.empty()){
            for (unsigned int i=1; i<F.TraccePass.size(); i++){
                //calcolo i segni e i vertici della traccia da analizzare
                VerticiTracciaNuova={ Tracce[F.TraccePass[i]].VerticiTraccia[0],Tracce[F.TraccePass[i]].VerticiTraccia[1]};
                segnoVerticeTraccia0=Direzione.cross(VerticiTraccia[0]-Tracce[F.TraccePass[i]].VerticiTraccia[0]).dot(F.vecNormale);
                segnoVerticeTraccia1=Direzione.cross(VerticiTraccia[0]-Tracce[F.TraccePass[i]].VerticiTraccia[1]).dot(F.vecNormale);
                //caso 1, abbiamo che la traccia è divisa dal semipiano, cerco intersezione e la aggiungo
                if(segnoVerticeTraccia0*segnoVerticeTraccia1<0 && abs(segnoVerticeTraccia0)>tol && abs(segnoVerticeTraccia1)>tol){
                    NuovoVerticeTraccia=IncontroTraRette(Tracce[F.TraccePass[i]].VerticiTraccia[0]-Tracce[F.TraccePass[i]].VerticiTraccia[1], Tracce[F.TraccePass[i]].VerticiTraccia[0],Direzione , VerticiTraccia[0]);
                    if(segnoVerticeTraccia0>0){

                        if((VerticiTracciaNuova[0]-NuovoVerticeTraccia).squaredNorm()>tol2){
                            IdPositivo=Tracce.size();
                            Tracce.push_back(Traccia({VerticiTracciaNuova[0],NuovoVerticeTraccia}));
                            TraccePositive.push_back(IdPositivo);
                        }

                        if((VerticiTracciaNuova[1]-NuovoVerticeTraccia).squaredNorm()>tol2){
                            IdNegativo=Tracce.size();
                            Tracce.push_back(Traccia({NuovoVerticeTraccia,VerticiTracciaNuova[1]}));
                            TracceNegative.push_back(IdNegativo); ;
                        }




                        //inserisco un vertice nelle positive, l'altro nelle negative
                        //qua vado a inserire la traccia nuova.

                    }
                    else{

                        if((VerticiTracciaNuova[0]-NuovoVerticeTraccia).squaredNorm()>tol2){
                            IdNegativo=Tracce.size();
                            Tracce.push_back(Traccia({VerticiTracciaNuova[0],NuovoVerticeTraccia}));
                            TracceNegative.push_back(IdNegativo);
                        }
                        if((VerticiTracciaNuova[1]-NuovoVerticeTraccia).squaredNorm()>tol2){
                            IdPositivo=Tracce.size();
                            Tracce.push_back(Traccia({NuovoVerticeTraccia,VerticiTracciaNuova[1]}));
                            TraccePositive.push_back(IdPositivo);
                        }

                        //cambio inserimento, ora una da una parte ora dall'altra
                    }
                }
                //vado a vedere se sta dalla parte positiva o negativa
                else {
                    if(segnoVerticeTraccia0>0){
                        TraccePositive.push_back(F.TraccePass[i]);

                    }
                    else{
                        TracceNegative.push_back(F.TraccePass[i]);
                    }


                }
            }
        }

        //rifaccio tutto per le non passanti

        if(!F.TracceNoPass.empty()){
            for (unsigned int i=0; i<F.TracceNoPass.size(); i++){
                VerticiTracciaNuova={ Tracce[F.TracceNoPass[i]].VerticiTraccia[0],Tracce[F.TracceNoPass[i]].VerticiTraccia[1]};
                segnoVerticeTraccia0=Direzione.cross(VerticiTraccia[0]-Tracce[F.TracceNoPass[i]].VerticiTraccia[0]).dot(F.vecNormale);
                segnoVerticeTraccia1=Direzione.cross(VerticiTraccia[0]-Tracce[F.TracceNoPass[i]].VerticiTraccia[1]).dot(F.vecNormale);

                if(segnoVerticeTraccia0*segnoVerticeTraccia1<0 && abs(segnoVerticeTraccia0)>tol && abs(segnoVerticeTraccia1)>tol){
                    NuovoVerticeTraccia=IncontroTraRette(Tracce[F.TracceNoPass[i]].VerticiTraccia[0]-Tracce[F.TracceNoPass[i]].VerticiTraccia[1], Tracce[F.TracceNoPass[i]].VerticiTraccia[0],Direzione , VerticiTraccia[0]);
                    if(segnoVerticeTraccia0>0){

                        if((VerticiTracciaNuova[0]-NuovoVerticeTraccia).squaredNorm()>tol2){
                            IdPositivo=Tracce.size();
                            Tracce.push_back(Traccia({VerticiTracciaNuova[0],NuovoVerticeTraccia}));
                            TraccePositiveNopass.push_back(IdPositivo);
                        }

                        if((VerticiTracciaNuova[1]-NuovoVerticeTraccia).squaredNorm()>tol2){
                            IdNegativo=Tracce.size();
                            Tracce.push_back(Traccia({NuovoVerticeTraccia,VerticiTracciaNuova[1]}));
                            TracceNegativeNopass.push_back(IdNegativo); ;
                        }

                        //inserisco un vertice nelle positive, l'altro nelle negative
                        //qua vado a inserire la traccia nuova.

                    }
                    else{

                        if((VerticiTracciaNuova[0]-NuovoVerticeTraccia).squaredNorm()>tol2){
                            IdNegativo=Tracce.size();
                            Tracce.push_back(Traccia({VerticiTracciaNuova[0],NuovoVerticeTraccia}));
                            TracceNegativeNopass.push_back(IdNegativo);
                        }
                        if((VerticiTracciaNuova[1]-NuovoVerticeTraccia).squaredNorm()>tol2){
                            IdPositivo=Tracce.size();
                            Tracce.push_back(Traccia({NuovoVerticeTraccia,VerticiTracciaNuova[1]}));
                            TraccePositiveNopass.push_back(IdPositivo);
                        }

                        //cambio inserimento, ora una da una parte ora dall'altra
                    }
                }
                else {
                    if(segnoVerticeTraccia0>0 || segnoVerticeTraccia1>0){
                        TraccePositiveNopass.push_back(F.TracceNoPass[i]);

                    }
                    else{
                        TracceNegativeNopass.push_back(F.TracceNoPass[i]);
                    }


                }
            }
        }

    }

    if(NuoviIndiciPositivi.size()>2){
        Frattura F1=Frattura(PuntiPositivi.size(),PuntiPositivi);
        F1.TraccePass=TraccePositive;
        F1.TracceNoPass=TraccePositiveNopass;
        F1.IdVertici=NuoviIndiciPositivi;
        Figli.push_back(F1);

    }
    else{
        TracciaSulBordo=true;
    }
    if(NuoviIndiciNegativi .size()>2){

        Frattura F2=Frattura(PuntiNegativi.size(),PuntiNegativi);
        F2.TraccePass=TracceNegative;
        F2.TracceNoPass=TracceNegativeNopass;
        F2.IdVertici=NuoviIndiciNegativi;
        Figli.push_back(F2);
    }
    else{
        TracciaSulBordo=true;
    }


    //qua creo le fratture che mi servono



    return Figli;

}

unsigned int RicercaIdVertice(Frattura& FMadre, Vector3d PuntodaControllare, double tol2){

    for (unsigned int i=0; i<FMadre.SottoPoligoni.CoordinateCell0D.size(); i++){
        if((FMadre.SottoPoligoni.CoordinateCell0D[i]-PuntodaControllare).squaredNorm()<tol2){
            return i;
        }
    }
    int j=FMadre.SottoPoligoni.CoordinateCell0D.size();
    FMadre.SottoPoligoni.IdCell0D.push_back(j);
    FMadre.SottoPoligoni.CoordinateCell0D.push_back(PuntodaControllare);
    FMadre.SottoPoligoni.NumeroCell0D ++;
    return j;

}


vector<Frattura> calcoloSottoPoligoniNoPass(Frattura& F,double tol, double tol2, vector<Traccia>& Tracce, Frattura& FMadre,bool& BassaTolleranza){
    vector<unsigned int> NuoviIndiciPositivi;
    vector<unsigned int> NuoviIndiciNegativi;
    vector<Frattura> Figli;
    vector<Vector3d>PuntiPositivi;//vettore in cui mettere i punti a destra
    vector<Vector3d>PuntiNegativi;//vettore in cui mettere i punti a sinistra
    unsigned int PuntiNuovi=0; //quanti nuovi vertici ho trovato
    array<Vector3d,2> VerticiTraccia={ Tracce[F.TracceNoPass[0]].VerticiTraccia[0],Tracce[F.TracceNoPass[0]].VerticiTraccia[1]};//vettore con dentro i due vertici della traccia
    double SegnoPrec=1;
    double segno;
    Vector3d PuntoIntersezione;
    Vector3d Direzione= (VerticiTraccia[0]-VerticiTraccia[1]);
    unsigned int NuovoId;

    for (unsigned int i=0; i<F.NumVertici; i++){

        if(PuntiNuovi<2){
            //se devo ancora trovarne faccio controlli altrimenti faccio un'assegnazione veloce


            if((VerticiTraccia[1]-F.CoordinateVertici[i]).squaredNorm()<tol2){
                segno=((Direzione).cross(VerticiTraccia[1]-F.CoordinateVertici[i])).dot(F.vecNormale);
            }
            else{
                segno=((Direzione).cross(VerticiTraccia[0]-F.CoordinateVertici[i])).dot(F.vecNormale);
            }


            if(i==0){
                if(abs(segno)<tol2){

                    //dico che ci sono vertici traccia che funzionano, rimane solo più l'altro punto
                    PuntiNuovi++;
                    PuntiPositivi.push_back(F.CoordinateVertici[i]);
                    NuoviIndiciPositivi.push_back(F.IdVertici[i]);
                    PuntiNegativi.push_back(F.CoordinateVertici[i]);
                    NuoviIndiciNegativi.push_back(F.IdVertici[i]);
                    if((Direzione).cross(VerticiTraccia[0]-F.CoordinateVertici[i+1]).dot(F.vecNormale)>0){
                        SegnoPrec=1;
                    }
                    else{
                        SegnoPrec=-1;
                    }
                }
                else{//Il primo vertice non da 0 quindi mi salvo solo il suo segno
                    if(segno>0){
                        PuntiPositivi.push_back(F.CoordinateVertici[i]);
                        NuoviIndiciPositivi.push_back(F.IdVertici[i]);
                        SegnoPrec=1;
                    }
                    else{
                        PuntiNegativi.push_back(F.CoordinateVertici[i]);
                        NuoviIndiciNegativi.push_back(F.IdVertici[i]);
                        SegnoPrec=-1;
                    }
                }
            }
            //non siamo nel primo
            else {
                if(abs(segno)<tol2){


                    //caso 1, il vertice coincide con il vertice traccia ma il lato non è compreso
                    //potrei togliere anche questo if;

                    //dico che ci sono vertici traccia che funzionano, rimane solo più l'altro punto

                    PuntiNuovi++;
                    PuntiPositivi.push_back(F.CoordinateVertici[i]);
                    NuoviIndiciPositivi.push_back(F.IdVertici[i]);
                    PuntiNegativi.push_back(F.CoordinateVertici[i]);
                    NuoviIndiciNegativi.push_back(F.IdVertici[i]);

                    SegnoPrec=-SegnoPrec;

                }
                //la traccia non è sul lato e non ha vertici coincidenti
                else if(segno*SegnoPrec>0){

                    //hanno lo stesso segno, allora faccio che inserirlo nel vettore giusto
                    if(segno>0){
                        PuntiPositivi.push_back(F.CoordinateVertici[i]);
                        NuoviIndiciPositivi.push_back(F.IdVertici[i]);
                    }
                    else{
                        PuntiNegativi.push_back(F.CoordinateVertici[i]);
                        NuoviIndiciNegativi.push_back(F.IdVertici[i]);
                    }
                }
                //se invece hanno segni diversi
                else {

                    SegnoPrec=-SegnoPrec;
                    PuntiNuovi++;
                    //essendo  non passanti calcolo il punto di intersezione che starà in mezzo
                    PuntoIntersezione=IncontroTraRette((F.CoordinateVertici[i]-F.CoordinateVertici[i-1]),F.CoordinateVertici[i],Direzione,VerticiTraccia[0]);

                    NuovoId=RicercaIdVertice(FMadre, PuntoIntersezione, tol2);

                    if (segno>0){

                        //aggiungo il punto nuovo sia a una sia all'altra

                        PuntiNegativi.push_back(PuntoIntersezione);
                        NuoviIndiciNegativi.push_back(NuovoId);
                        PuntiPositivi.push_back(PuntoIntersezione);
                        NuoviIndiciPositivi.push_back(NuovoId);


                        PuntiPositivi.push_back(F.CoordinateVertici[i]);
                        NuoviIndiciPositivi.push_back(F.IdVertici[i]);
                    }
                    else{


                        PuntiNegativi.push_back(PuntoIntersezione);
                        NuoviIndiciNegativi.push_back(NuovoId);
                        PuntiPositivi.push_back(PuntoIntersezione);
                        NuoviIndiciPositivi.push_back(NuovoId);
                        PuntiNegativi.push_back(F.CoordinateVertici[i]);
                        NuoviIndiciNegativi.push_back(F.IdVertici[i]);
                    }
                }

            }

        }


        else{
            if(SegnoPrec>0){
                PuntiPositivi.push_back(F.CoordinateVertici[i]);
                NuoviIndiciPositivi.push_back(F.IdVertici[i]);

            }
            else{
                PuntiNegativi.push_back(F.CoordinateVertici[i]);
                NuoviIndiciNegativi.push_back(F.IdVertici[i]);

            }
        }
    }
    if(PuntiNuovi<2){
        segno=(Direzione.cross(VerticiTraccia[0]-F.CoordinateVertici[0])).dot(F.vecNormale);
        if(segno*SegnoPrec<0){
            PuntoIntersezione=IncontroTraRette((F.CoordinateVertici[0]-F.CoordinateVertici[F.NumVertici-1]),F.CoordinateVertici[0],Direzione,VerticiTraccia[0]);
            NuovoId=RicercaIdVertice(FMadre, PuntoIntersezione, tol2);
            if (segno>0){
                //aggiungo il punto nuovo sia a una sia all'altra

                PuntiNegativi.push_back(PuntoIntersezione);
                NuoviIndiciNegativi.push_back(NuovoId);
                PuntiPositivi.push_back(PuntoIntersezione);
                NuoviIndiciPositivi.push_back(NuovoId);
            }
            else{

                PuntiNegativi.push_back(PuntoIntersezione);
                NuoviIndiciNegativi.push_back(NuovoId);
                PuntiPositivi.push_back(PuntoIntersezione);
                NuoviIndiciPositivi.push_back(NuovoId);

            }
        }

        //scopro che era il punto 1 e non il punto 0, rifaccio tutto

    }


    //devo fare la funzione che mi divide le tracce e mi dice se sono di una frattura o dell'altra
    double segnoVerticeTraccia0;
    double segnoVerticeTraccia1;
    vector<unsigned int>TracceNegativeNopass={};
    vector<unsigned int>TraccePositiveNopass={};
    Vector3d NuovoVerticeTraccia;
    array<Vector3d,2> VerticiTracciaNuova;
    unsigned int IdPositivo;
    unsigned int IdNegativo;

    if(!F.TracceNoPass.empty()){
        for (unsigned int i=1; i<F.TracceNoPass.size(); i++){
            VerticiTracciaNuova={ Tracce[F.TracceNoPass[i]].VerticiTraccia[0],Tracce[F.TracceNoPass[i]].VerticiTraccia[1]};
            segnoVerticeTraccia0=Direzione.cross(VerticiTraccia[0]-Tracce[F.TracceNoPass[i]].VerticiTraccia[0]).dot(F.vecNormale);
            segnoVerticeTraccia1=Direzione.cross(VerticiTraccia[0]-Tracce[F.TracceNoPass[i]].VerticiTraccia[1]).dot(F.vecNormale);

            if(segnoVerticeTraccia0*segnoVerticeTraccia1<0 && abs(segnoVerticeTraccia0)>tol && abs(segnoVerticeTraccia1)>tol){
                NuovoVerticeTraccia=IncontroTraRette(Tracce[F.TracceNoPass[i]].VerticiTraccia[0]-Tracce[F.TracceNoPass[i]].VerticiTraccia[1], Tracce[F.TracceNoPass[i]].VerticiTraccia[0],Direzione , VerticiTraccia[0]);
                if(segnoVerticeTraccia0>0){

                    if((VerticiTracciaNuova[0]-NuovoVerticeTraccia).squaredNorm()>tol2){
                        IdPositivo=Tracce.size();
                        Tracce.push_back(Traccia({VerticiTracciaNuova[0],NuovoVerticeTraccia}));
                        TraccePositiveNopass.push_back(IdPositivo);
                    }

                    if((VerticiTracciaNuova[1]-NuovoVerticeTraccia).squaredNorm()>tol2){
                        IdNegativo=Tracce.size();
                        Tracce.push_back(Traccia({NuovoVerticeTraccia,VerticiTracciaNuova[1]}));
                        TracceNegativeNopass.push_back(IdNegativo);
                    }


                    //inserisco un vertice nelle positive, l'altro nelle negativo

                }
                else{

                    if((VerticiTracciaNuova[0]-NuovoVerticeTraccia).squaredNorm()>tol2){
                        IdNegativo=Tracce.size();
                        Tracce.push_back(Traccia({VerticiTracciaNuova[0],NuovoVerticeTraccia}));
                        TracceNegativeNopass.push_back(IdNegativo);
                    }

                    if((VerticiTracciaNuova[1]-NuovoVerticeTraccia).squaredNorm()>tol2){
                        IdPositivo=Tracce.size();
                        Tracce.push_back(Traccia({NuovoVerticeTraccia,VerticiTracciaNuova[1]}));
                        TraccePositiveNopass.push_back(IdPositivo);
                    }

                    //cambio inserimento, ora una da una parte ora dall'altra
                }
            }
            else {
                if(segnoVerticeTraccia0>0 || segnoVerticeTraccia1>0){
                    TraccePositiveNopass.push_back(F.TracceNoPass[i]);

                }
                else{
                    TracceNegativeNopass.push_back(F.TracceNoPass[i]);
                }


            }
        }
    }



    if(NuoviIndiciPositivi.size()>2){
        Frattura F1=Frattura(PuntiPositivi.size(),PuntiPositivi);

        F1.TracceNoPass=TraccePositiveNopass;
        F1.IdVertici=NuoviIndiciPositivi;
        Figli.push_back(F1);
    }
    else{
        BassaTolleranza=true;
    }
    if(NuoviIndiciNegativi .size()>2){

    Frattura F2=Frattura(PuntiNegativi.size(),PuntiNegativi);

    F2.TracceNoPass=TracceNegativeNopass;
    F2.IdVertici=NuoviIndiciNegativi;
    Figli.push_back(F2);
    }
    else{
        BassaTolleranza=true;
    }


    return Figli;

}

bool stampaMesh(vector<Frattura>& Fratture){
    ofstream file("stampaMesh.txt");
    if(file.fail()){
        cerr << "Errore";
        return false;
    }
    for(const auto& f : Fratture){
        if(f.IdFrattura!=Fratture.size()+1){
            file<<"FractureId "<<f.IdFrattura<< endl << "NumCell0Ds "<< f.SottoPoligoni.NumeroCell0D<<endl;
            file <<"IdCell0D; X; Y; Z"<<endl;
            for(unsigned int i = 0;i<f.SottoPoligoni.NumeroCell0D;i++){
                file << f.SottoPoligoni.IdCell0D[i]<<"; "<< f.SottoPoligoni.CoordinateCell0D[i][0]<< "; "<< f.SottoPoligoni.CoordinateCell0D[i][1]<<
                    "; "<< f.SottoPoligoni.CoordinateCell0D[i][2]<<endl;
            }
            file <<"NmCell1Ds "<< f.SottoPoligoni.NumeroCell1D<<endl;
            file << "IdCell1D; Origin; End "<<endl;
            for(unsigned int i = 0;i<f.SottoPoligoni.NumeroCell1D;i++){
                file << f.SottoPoligoni.IdCell1D[i]<<"; "<< f.SottoPoligoni.VerticiCell1D[i][0]<<"; "<<f.SottoPoligoni.VerticiCell1D[i][1]<< endl;
            }
            file <<"NmCell2Ds "<< f.SottoPoligoni.NumeroCell2D<<endl;
            file<< "IdCell2d; NumVertices; Vertices; NumEdges; Edges"<< endl;
            for(unsigned int i = 0;i<f.SottoPoligoni.NumeroCell2D;i++){
                file<<f.SottoPoligoni.IdCell2D[i]<<"; "<<f.SottoPoligoni.VerticiCell2D[i].size();
                for(unsigned int j = 0;j<f.SottoPoligoni.VerticiCell2D[i].size();j++){
                    file<<"; "<<f.SottoPoligoni.VerticiCell2D[i][j];
                }
                file <<"; "<<f.SottoPoligoni.LatiCell2D[i].size();
                for(unsigned int j = 0;j<f.SottoPoligoni.LatiCell2D[i].size();j++){
                    file<<"; "<<f.SottoPoligoni.LatiCell2D[i][j];
                }
                file<<endl;
            }
        }
    }
    file.close();
    return true;
}

void Progetto2(vector<Frattura>& Fratture,vector<Traccia>& Tracce, double tol,double tol2){
    unsigned int LunghezzaTraccia=Tracce.size();
    for(auto& f :Fratture){
        if(f.IdFrattura!=Fratture.size()+1){
            //AGGIUNGO CELL0D
            for(unsigned int i = 0;i<f.NumVertici;i++){
                unsigned int IdCell0D = f.SottoPoligoni.NumeroCell0D;//inizializzato a 0
                f.SottoPoligoni.IdCell0D.push_back(IdCell0D);
                f.IdVertici.push_back(IdCell0D);
                f.SottoPoligoni.CoordinateCell0D.push_back(f.CoordinateVertici[i]);
                f.SottoPoligoni.NumeroCell0D ++;
            }
            Taglia( f,f, Tracce,tol, tol2);
            Tracce.resize(LunghezzaTraccia);
            Tracce.reserve(LunghezzaTraccia);
        }
    }
    stampaMesh(Fratture);
    esportaMesh(Fratture[0]);
}

void esportaMesh(Frattura& F){
    int numColonne = F.SottoPoligoni.NumeroCell0D;

    // Creo la matrice risultante di dimensioni 3xN
    MatrixXd matricePunti(3, numColonne);

    // Riempio la matrice con i dati dal vettore di Vector3d
    for (int i = 0; i < numColonne; ++i) {
        matricePunti.col(i) = F.SottoPoligoni.CoordinateCell0D[i];
    }

    UCDUtilities U;
    string fileName = "./DFN/EsportoMeshPunti.inp";
    U.ExportPoints( fileName, matricePunti,{},{});


    // Calcola il numero totale di colonne nella matrice risultante
    int numCol2 = F.SottoPoligoni.NumeroCell1D;

    // Crea la matrice risultante di dimensioni 2xN
      MatrixXi matriceSegmenti(2, numCol2);

    // Copia i dati dal vettore di array nella matrice
    for (int i = 0; i < numCol2; ++i) {
        matriceSegmenti.col(i) = Vector2i(F.SottoPoligoni.VerticiCell1D[i][0],F.SottoPoligoni.VerticiCell1D[i][1]);
    }
    string fileName1 = "./DFN/EsportoMeshSegmenti.inp";
    U.ExportSegments( fileName1, matricePunti,matriceSegmenti);

    vector<vector<unsigned int>> triangoli;
    VectorXi materials;

    F.SottoPoligoni.GedimInterface( triangoli,materials);
    string fileName2 = "./DFN/EsportoMeshPoligoni.inp";
    U.ExportPolygons(fileName2,matricePunti,triangoli);
}

}
