#include "Utils.hpp"
#include "Utils2.hpp"
#include "GeometryLibrary.hpp"
#include "Eigen/Eigen"
#include "Sort.hpp"
#include <iostream>
#include <sstream> // controlla se servono tutte
#include <fstream>
#include <string>
#include <array>

using namespace std;

namespace DFN{                                         //La traccia madre deve avere già i vertici numerati con id

void Taglia(Frattura& F,Frattura & FMadre,vector<Traccia> Tracce, double tol,double tol2){//Passare un contatore che mi dice a quanto sono arrivata a numerare gli id
    if(!F.TraccePass.empty()){
        //Chiama calcoloPassante
        bool TracciaSulBordo = false;
        //in calcolo figlie quando creo le nuove fratture do già gli id ai suoi vertici, poi chiamo taglia

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
        //Chiama calcoloNonPassante
        //vector<Frattura> figlie = calcoloSottoPoligoniNoPass(F,tol);
        //Taglia(figlie[0],FMadre,Tracce,tol,tol2);
        //Taglia(figlie[1],FMadre,Tracce,tol,tol2);

    }
    else{//Vuol dire che non ha più tracce e posso salvarlo come elemento della mesh
        converteInCelle(F,FMadre);
    }
}
    void converteInCelle(Frattura& F,Frattura &FMadre){
            //Devo aggiungere solo Cell1D e la Cell2D, ovvero la frattura stessa F
            //Le celle0D le aggiunge flavio
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
            for(unsigned int j = 0;j<FMadre.SottoPoligoni.VerticesCell1Ds.size();j++){
                 if((FMadre.SottoPoligoni.VerticesCell1Ds[j][0] == IdVerticiCell1D[0] && FMadre.SottoPoligoni.VerticesCell1Ds[j][1] == IdVerticiCell1D[1]) || (FMadre.SottoPoligoni.VerticesCell1Ds[j][0] == IdVerticiCell1D[1] && FMadre.SottoPoligoni.VerticesCell1Ds[j][1] == IdVerticiCell1D[0]) ){
                    esisteLato = true; //Non devo aggiungerlo
                    IdCell1D = FMadre.SottoPoligoni.IdCell1Ds[j];

                }
            }
            if(!esisteLato){//Se non esiste, crea una nuova Cell1D
                IdCell1D =  FMadre.SottoPoligoni.NumberofCell1Ds;
                FMadre.SottoPoligoni.IdCell1Ds.push_back(IdCell1D);
                FMadre.SottoPoligoni.VerticesCell1Ds.push_back (IdVerticiCell1D);
                FMadre.SottoPoligoni.NumberofCell1Ds++;
            }

            IdLatiCell2D.push_back(IdCell1D);

        }

        //Aggiungo Cell2D ==>Aggiungo ID, vettori con gli id dei vertici e il vettore con gli id dei Lati
        unsigned int IdCell2D = FMadre.SottoPoligoni.NumberofCell2Ds;//Ricordati di inizializzarlo a 0!!
        FMadre.SottoPoligoni.IdCell2Ds.push_back(FMadre.SottoPoligoni.NumberofCell2Ds);
        FMadre.SottoPoligoni.VerticesCell2Ds.push_back(F.IdVertici);
        FMadre.SottoPoligoni.EdgesCell2Ds.push_back(IdLatiCell2D);

        FMadre.SottoPoligoni.NumberofCell2Ds++;
    }



    //Inizio codice Flavio


    vector<Frattura> calcoloSottoPoligoniPass(Frattura& F,double tol, double tol2,bool& TracciaSulBordo, vector<Traccia>& Tracce, Frattura& FMadre){
        vector<unsigned int> NuoviIndiciPositivi;
        vector<unsigned int> NuoviIndiciNegativi;
        vector<Frattura> Figli;
        vector<Vector3d>PuntiPositivi;//vettore in cui mettere i punti a destra
        vector<Vector3d>PuntiNegativi;//vettore in cui mettere i punti a sinistra
        unsigned int PuntiNuovi=0; //quanti nuovi vertici ho trovato
        //Sostituisci con array dim 2 :)
        vector<Vector3d> VerticiTraccia={ Tracce[F.TraccePass[0]].VerticiTraccia[0],Tracce[F.TraccePass[0]].VerticiTraccia[1]};//vettore con dentro i due vertici della traccia
        int SegnoPrec=1;
        double segno;
        for (unsigned int i=0; i<F.NumVertici; i++){
            if(PuntiNuovi<2){
                //se devo ancora trovarne faccio controlli altrimenti faccio un'assegnazione veloce
                segno=((VerticiTraccia[0]-VerticiTraccia[1]).cross(VerticiTraccia[0]-F.CoordinateVertici[i])).dot(F.vecNormale);
                if(i==0){
                    //caso 1, il vertice coincide con il vertice traccia ma il lato non è compreso
                    if((VerticiTraccia[0]-F.CoordinateVertici[i]).squaredNorm() <tol2 && abs(segno)>tol ){
                        //dico che ci sono vertici traccia che funzionano, rimane solo più l'altro punto
                        PuntiNuovi++;
                        PuntiPositivi.push_back(F.CoordinateVertici[i]);
                        NuoviIndiciPositivi.push_back(F.IdVertici[i]);
                        PuntiNegativi.push_back(F.CoordinateVertici[i]);
                        NuoviIndiciNegativi.push_back(F.IdVertici[i]);

                        SegnoPrec=(VerticiTraccia[0]-VerticiTraccia[1]).cross(VerticiTraccia[0]-F.CoordinateVertici[i+1]).dot(F.vecNormale);
                    }
                    else if ((VerticiTraccia[1]-F.CoordinateVertici[i]).squaredNorm() <tol2 && abs(segno)>tol){
                        PuntiNuovi++;
                        PuntiPositivi.push_back(F.CoordinateVertici[i]);
                        NuoviIndiciPositivi.push_back(F.IdVertici[i]);
                        PuntiNegativi.push_back(F.CoordinateVertici[i]);
                        NuoviIndiciNegativi.push_back(F.IdVertici[i]);
                        SegnoPrec=((VerticiTraccia[0]-VerticiTraccia[1]).cross(VerticiTraccia[0]-F.CoordinateVertici[i+1])).dot(F.vecNormale);
                    }
                    // caso 2, la retta della traccia appartiene al lato
                    else if(abs(segno)<tol){
                        TracciaSulBordo=true;
                        vector<Vector3d>VerticiTracciaOrdinati;
                        SegnoPrec=1;
                        //quale è il più vicino?

                        if((VerticiTraccia[1]-F.CoordinateVertici[i]).squaredNorm()<(VerticiTraccia[0]-F.CoordinateVertici[i]).squaredNorm()){
                            if((VerticiTraccia[1]-F.CoordinateVertici[i]).squaredNorm() <tol2){
                                //visto che i punti coincidono assegno solo l'altro
                                VerticiTracciaOrdinati={VerticiTraccia[0]};

                            }
                            else{

                                VerticiTracciaOrdinati={VerticiTraccia[1],VerticiTraccia[0]};

                            }
                        }
                        else{
                            if((VerticiTraccia[0]-F.CoordinateVertici[i]).squaredNorm() <tol2){
                                //visto che i punti coincidono assegno solo l'altro
                                VerticiTracciaOrdinati={VerticiTraccia[1]};

                            }
                            else{

                                VerticiTracciaOrdinati=VerticiTraccia;

                            }
                        }
                        //sta sulla retta con il successivo?

                        if((F.CoordinateVertici[i+1]-F.CoordinateVertici[i]).cross(VerticiTraccia[0]-F.CoordinateVertici[i]).squaredNorm()<tol2){
                            //salvo tutti nel positivo
                            PuntiPositivi.push_back(F.CoordinateVertici[i]);
                            NuoviIndiciPositivi.push_back(F.IdVertici[i]);
                            for (unsigned int j=0; j<VerticiTracciaOrdinati.size(); j++){

                                PuntiPositivi.push_back(VerticiTracciaOrdinati[j]);
                                NuoviIndiciPositivi.push_back(RicercaIdVertice(FMadre, VerticiTracciaOrdinati[j], tol2));
                                PuntiNuovi++;
                            }



                        }
                        else{
                            //sta nel precedente allora aggiungo prima da una parte e poi dall'altra
                            for (unsigned int j=0; j<VerticiTracciaOrdinati.size(); j++){

                                PuntiPositivi.push_back(VerticiTracciaOrdinati[j]);
                                NuoviIndiciPositivi.push_back(RicercaIdVertice(FMadre, VerticiTracciaOrdinati[j], tol2));
                                PuntiNuovi++;
                            }


                            PuntiPositivi.push_back(F.CoordinateVertici[i]);
                            NuoviIndiciPositivi.push_back(F.IdVertici[i]);

                        }
                    }
                    else{
                        //va tutto bene
                        SegnoPrec=segno;
                    }
                }
                //non siamo nel primo nè nell'ultimo
                else if (i!=F.NumVertici-1){
                    //il vertice della traccia lo è anche del Poligono?
                    if((VerticiTraccia[0]-F.CoordinateVertici[i]).squaredNorm() <tol2 && abs(segno)>tol ){
                        //dico che ci sono vertici traccia che funzionano, rimane solo più l'altro punto
                        PuntiNuovi++;
                        PuntiPositivi.push_back(F.CoordinateVertici[i]);
                        NuoviIndiciPositivi.push_back(F.IdVertici[i]);
                        PuntiNegativi.push_back(F.CoordinateVertici[i]);
                        NuoviIndiciNegativi.push_back(F.IdVertici[i]);
                        SegnoPrec=((VerticiTraccia[0]-VerticiTraccia[1]).cross(VerticiTraccia[0]-F.CoordinateVertici[i+1])).dot(F.vecNormale);
                    }
                    else if ((VerticiTraccia[1]-F.CoordinateVertici[i]).squaredNorm() <tol2 && abs(segno)>tol){
                        PuntiNuovi++;
                        PuntiPositivi.push_back(F.CoordinateVertici[i]);
                        NuoviIndiciPositivi.push_back(F.IdVertici[i]);
                        PuntiNegativi.push_back(F.CoordinateVertici[i]);
                        NuoviIndiciNegativi.push_back(F.IdVertici[i]);
                        SegnoPrec=((VerticiTraccia[0]-VerticiTraccia[1]).cross(VerticiTraccia[0]-F.CoordinateVertici[i+1])).dot(F.vecNormale);
                    }
                    //la traccia è nel lato?
                    else if(abs(segno)<tol){
                        TracciaSulBordo=true;
                        vector<Vector3d>VerticiTracciaOrdinati;
                        //quale è il più vicino?

                        if((VerticiTraccia[1]-F.CoordinateVertici[i]).squaredNorm()<(VerticiTraccia[0]-F.CoordinateVertici[i]).squaredNorm()){
                            if((VerticiTraccia[1]-F.CoordinateVertici[i]).squaredNorm() <tol2){
                                //visto che i punti coincidono assegno solo l'altro
                                VerticiTracciaOrdinati={VerticiTraccia[0]};

                            }
                            else{

                                VerticiTracciaOrdinati={VerticiTraccia[1],VerticiTraccia[0]};

                            }
                        }
                        else{
                            if((VerticiTraccia[0]-F.CoordinateVertici[i]).squaredNorm() <tol2){
                                //visto che i punti coincidono assegno solo l'altro
                                VerticiTracciaOrdinati={VerticiTraccia[1]};

                            }
                            else{

                                VerticiTracciaOrdinati=VerticiTraccia;

                            }
                        }
                        //guardo in quale stavo già inserendo tutti i miei dati e continuo a farlo
                        if(SegnoPrec>0){
                            PuntiPositivi.push_back(F.CoordinateVertici[i]);
                            NuoviIndiciPositivi.push_back(F.IdVertici[i]);
                            for (unsigned int j=0; j<VerticiTracciaOrdinati.size(); j++){

                                PuntiPositivi.push_back(VerticiTracciaOrdinati[j]);
                                NuoviIndiciPositivi.push_back(RicercaIdVertice(FMadre, VerticiTracciaOrdinati[j], tol2));
                                PuntiNuovi++;
                            }
                        }
                        else{
                            PuntiNegativi.push_back(F.CoordinateVertici[i]);
                            NuoviIndiciNegativi.push_back(F.IdVertici[i]);
                            for (unsigned int j=0; j<VerticiTracciaOrdinati.size(); j++){

                                PuntiNegativi.push_back(VerticiTracciaOrdinati[j]);
                                NuoviIndiciNegativi.push_back(RicercaIdVertice(FMadre, VerticiTracciaOrdinati[j], tol2));
                                PuntiNuovi++;

                            }
                        }

                    }



                    //la traccia non è sul lato e non ha vertici coincidenti
                    else {
                        if(segno*SegnoPrec>0){
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
                        //se invece hanno segni diversi...
                        else {
                            PuntiNuovi++;
                            //essendo passanti ho già il punto di intersezione
                            //guardo quale sta sulla retta giusta
                            //aggiungo prima il vertice della traccia e poi se stesso
                            if((F.CoordinateVertici[i]-F.CoordinateVertici[i+1]).cross(F.CoordinateVertici[i]-VerticiTraccia[0]).squaredNorm()<tol2){
                                if (segno>0){
                                    //aggiungo il punto nuovo sia a una sia all'altra

                                    PuntiNegativi.push_back(VerticiTraccia[0]);
                                    NuoviIndiciNegativi.push_back(RicercaIdVertice(FMadre, VerticiTraccia[0], tol2));
                                    PuntiPositivi.push_back(VerticiTraccia[0]);
                                    NuoviIndiciPositivi.push_back(RicercaIdVertice(FMadre, VerticiTraccia[0], tol2));


                                    PuntiPositivi.push_back(F.CoordinateVertici[i]);
                                    NuoviIndiciPositivi.push_back(F.IdVertici[i]);
                                }
                                else{

                                    PuntiNegativi.push_back(VerticiTraccia[0]);
                                    NuoviIndiciNegativi.push_back(RicercaIdVertice(FMadre, VerticiTraccia[0], tol2));
                                    PuntiPositivi.push_back(VerticiTraccia[0]);
                                    NuoviIndiciPositivi.push_back(RicercaIdVertice(FMadre, VerticiTraccia[0], tol2));
                                    PuntiNegativi.push_back(F.CoordinateVertici[i]);
                                    NuoviIndiciNegativi.push_back(F.IdVertici[i]);
                                }

                            }
                            //scopro che era il punto 1 e non il punto 0, rifaccio tutto
                            else{
                                if (segno>0){
                                    //aggiungo il punto nuovo sia a una sia all'altra
                                    PuntiNegativi.push_back(VerticiTraccia[1]);
                                    NuoviIndiciNegativi.push_back(RicercaIdVertice(FMadre, VerticiTraccia[1], tol2));
                                    PuntiPositivi.push_back(VerticiTraccia[1]);
                                    NuoviIndiciPositivi.push_back(RicercaIdVertice(FMadre, VerticiTraccia[1], tol2));
                                    PuntiPositivi.push_back(F.CoordinateVertici[i]);
                                    NuoviIndiciPositivi.push_back(F.IdVertici[i]);


                                }
                                else{
                                    PuntiNegativi.push_back(VerticiTraccia[1]);
                                    NuoviIndiciNegativi.push_back(RicercaIdVertice(FMadre, VerticiTraccia[1], tol2));
                                    PuntiPositivi.push_back(VerticiTraccia[1]);
                                    NuoviIndiciPositivi.push_back(RicercaIdVertice(FMadre, VerticiTraccia[1], tol2));
                                    PuntiNegativi.push_back(F.CoordinateVertici[i]);
                                    NuoviIndiciNegativi.push_back(F.IdVertici[i]);


                                }

                            }

                        }
                    }
                }

                //sono all'ultimo, faccio i controlli uguali al secondo ma al posto del successivo prendo lo 0
                //ne ho cambiati solo due forse posso fare un if solo per rendere tutto più pulito?
                else{

                    //il vertice della traccia lo è anche del Poligono?
                    if((VerticiTraccia[0]-F.CoordinateVertici[i]).squaredNorm() <tol2 && abs(segno)>tol ){
                        //dico che ci sono vertici traccia che funzionano, rimane solo più l'altro punto
                        PuntiNuovi++;
                        PuntiPositivi.push_back(F.CoordinateVertici[i]);
                        NuoviIndiciPositivi.push_back(F.IdVertici[i]);
                        PuntiNegativi.push_back(F.CoordinateVertici[i]);
                        NuoviIndiciNegativi.push_back(F.IdVertici[i]);

                    }
                    else if ((VerticiTraccia[1]-F.CoordinateVertici[i]).squaredNorm() <tol2 && abs(segno)>tol){
                        PuntiNuovi++;
                        PuntiPositivi.push_back(F.CoordinateVertici[i]);
                        NuoviIndiciPositivi.push_back(F.IdVertici[i]);
                        PuntiNegativi.push_back(F.CoordinateVertici[i]);
                        NuoviIndiciNegativi.push_back(F.IdVertici[i]);

                    }
                    //la traccia è nel lato?
                    else if(abs(segno)<tol){
                        TracciaSulBordo=true;
                        vector<Vector3d>VerticiTracciaOrdinati;
                        //quale è il più vicino?

                        if((VerticiTraccia[1]-F.CoordinateVertici[i]).squaredNorm()<(VerticiTraccia[0]-F.CoordinateVertici[i]).squaredNorm()){
                            if((VerticiTraccia[1]-F.CoordinateVertici[i]).squaredNorm() <tol2){
                                //visto che i punti coincidono assegno solo l'altro
                                VerticiTracciaOrdinati={VerticiTraccia[0]};

                            }
                            else{

                                VerticiTracciaOrdinati={VerticiTraccia[1],VerticiTraccia[0]};



                            }
                        }
                        else{
                            if((VerticiTraccia[0]-F.CoordinateVertici[i]).squaredNorm() <tol2){
                                //visto che i punti coincidono assegno solo l'altro
                                VerticiTracciaOrdinati={VerticiTraccia[1]};
                            }
                            else{

                                VerticiTracciaOrdinati=VerticiTraccia;
                            }
                        }
                        //guardo in quale stavo già inserendo tutti i miei dati e continuo a farlo
                        if(SegnoPrec>0){
                            PuntiPositivi.push_back(F.CoordinateVertici[i]);
                            NuoviIndiciPositivi.push_back(F.IdVertici[i]);
                            for (unsigned int j=0; j<VerticiTracciaOrdinati.size(); j++){

                                PuntiPositivi.push_back(VerticiTracciaOrdinati[j]);
                                NuoviIndiciPositivi.push_back(RicercaIdVertice(FMadre, VerticiTracciaOrdinati[j], tol2));
                                PuntiNuovi++;
                            }
                        }
                        else{
                            PuntiNegativi.push_back(F.CoordinateVertici[i]);
                            NuoviIndiciNegativi.push_back(F.IdVertici[i]);
                            for (unsigned int j=0; j<VerticiTracciaOrdinati.size(); j++){

                                PuntiNegativi.push_back(VerticiTracciaOrdinati[j]);
                                NuoviIndiciPositivi.push_back(RicercaIdVertice(FMadre, VerticiTracciaOrdinati[j], tol2));
                                PuntiNuovi++;

                            }
                        }

                    }



                    //la traccia non è sul lato e non ha vertici coincidenti
                    else {
                        if(segno*SegnoPrec>0){
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
                        //se invece hanno segni diversi...
                        else {
                            PuntiNuovi++;
                            //essendo passanti ho già il punto di intersezione
                            //guardo quale sta sulla retta giusta
                            //aggiungo prima il vertice della traccia e poi se stesso
                            if((F.CoordinateVertici[i]-F.CoordinateVertici[0]).cross(F.CoordinateVertici[i]-VerticiTraccia[0]).squaredNorm()<tol2){
                                if (segno>0){
                                    //aggiungo il punto nuovo sia a una sia all'altra
                                    PuntiNegativi.push_back(VerticiTraccia[0]);
                                    NuoviIndiciNegativi.push_back(RicercaIdVertice(FMadre, VerticiTraccia[0], tol2));

                                    PuntiPositivi.push_back(VerticiTraccia[0]);
                                    NuoviIndiciPositivi.push_back(RicercaIdVertice(FMadre, VerticiTraccia[0], tol2));
                                    PuntiPositivi.push_back(F.CoordinateVertici[i]);
                                    NuoviIndiciPositivi.push_back(F.IdVertici[i]);
                                }
                                else{
                                    PuntiNegativi.push_back(VerticiTraccia[0]);
                                    NuoviIndiciNegativi.push_back(RicercaIdVertice(FMadre, VerticiTraccia[0], tol2));
                                    PuntiPositivi.push_back(VerticiTraccia[0]);
                                    NuoviIndiciPositivi.push_back(RicercaIdVertice(FMadre, VerticiTraccia[0], tol2));
                                    PuntiNegativi.push_back(F.CoordinateVertici[i]);
                                    NuoviIndiciNegativi.push_back(F.IdVertici[i]);
                                }

                            }
                            //scopro che era il punto 1 e non il punto 0, rifaccio tutto
                            else{
                                if (segno>0){
                                    //aggiungo il punto nuovo sia a una sia all'altra
                                    PuntiNegativi.push_back(VerticiTraccia[1]);
                                    NuoviIndiciNegativi.push_back(RicercaIdVertice(FMadre, VerticiTraccia[1], tol2));
                                    PuntiPositivi.push_back(VerticiTraccia[1]);
                                    NuoviIndiciPositivi.push_back(RicercaIdVertice(FMadre, VerticiTraccia[1], tol2));
                                    PuntiPositivi.push_back(F.CoordinateVertici[i]);
                                    NuoviIndiciPositivi.push_back(F.IdVertici[i]);

                                }
                                else{
                                    PuntiNegativi.push_back(VerticiTraccia[1]);
                                    NuoviIndiciNegativi.push_back(RicercaIdVertice(FMadre, VerticiTraccia[1], tol2));
                                    PuntiPositivi.push_back(VerticiTraccia[1]);
                                    NuoviIndiciPositivi.push_back(RicercaIdVertice(FMadre, VerticiTraccia[1], tol2));
                                    PuntiNegativi.push_back(F.CoordinateVertici[i]);
                                    NuoviIndiciNegativi.push_back(F.IdVertici[i]);

                                }

                            }

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



        //devo fare la funzione che mi divide le tracce e mi dice se sono di una frattura o dell'altra
        double segnoVerticeTraccia0;
        double segnoVerticeTraccia1;
        vector<unsigned int>TracceNegative;
        vector<unsigned int>TraccePositive;
        vector<unsigned int>TracceNegativeNopass;
        vector<unsigned int> TraccePositiveNopass;
        Vector3d NuovoVerticeTraccia;
        unsigned int IdPositivo;
        unsigned int IdNegativo;
        Vector3d DirezioneTraccia=VerticiTraccia[0]-VerticiTraccia[1];
        if(TracciaSulBordo){
            for (unsigned int j=1; j<F.TraccePass.size();j++){
                TraccePositive.push_back(F.TraccePass[j]);

            }
            for (unsigned int j=0; j<F.TracceNoPass.size();j++){
                TraccePositive.push_back(F.TracceNoPass[j]);

            }
        }
        else{
            if(!F.TraccePass.empty()){
                for (unsigned int i=1; i<F.TraccePass.size(); i++){
                    segnoVerticeTraccia0=DirezioneTraccia.cross(VerticiTraccia[0]-Tracce[F.TraccePass[i]].VerticiTraccia[0]).dot(F.vecNormale);
                    segnoVerticeTraccia1=DirezioneTraccia.cross(VerticiTraccia[0]-Tracce[F.TraccePass[i]].VerticiTraccia[1]).dot(F.vecNormale);

                    if(segnoVerticeTraccia0*segnoVerticeTraccia1<0 && abs(segnoVerticeTraccia0)>tol && abs(segnoVerticeTraccia1)>tol){
                        NuovoVerticeTraccia=IncontroTraRette(Tracce[F.TraccePass[i]].VerticiTraccia[0]-Tracce[F.TraccePass[i]].VerticiTraccia[1], Tracce[F.TraccePass[i]].VerticiTraccia[0],DirezioneTraccia , VerticiTraccia[0]);
                        if(segnoVerticeTraccia0>0){
                            IdPositivo=Tracce.size();
                            Tracce.push_back(Traccia({VerticiTraccia[0],NuovoVerticeTraccia}));
                            IdNegativo=IdPositivo+1;
                            Tracce.push_back(Traccia({NuovoVerticeTraccia,VerticiTraccia[1]}));

                            TraccePositive.push_back(IdPositivo);
                            TracceNegative.push_back(IdNegativo);


                            //inserisco un vertice nelle positive, l'altro nelle negative
                            //qua vado a inserire la traccia nuova.

                        }
                        else{
                            IdNegativo=Tracce.size();
                            Tracce.push_back(Traccia({VerticiTraccia[0],NuovoVerticeTraccia}));
                            IdPositivo=IdPositivo+1;
                            Tracce.push_back(Traccia({NuovoVerticeTraccia,VerticiTraccia[1]}));

                            TraccePositive.push_back(IdPositivo);
                            TracceNegative.push_back(IdNegativo);

                            //cambio inserimento, ora una da una parte ora dall'altra
                        }
                    }
                    else {
                        if(segnoVerticeTraccia0>0){
                            Tracce.push_back(Traccia({VerticiTraccia[0],VerticiTraccia[1]}));
                            TraccePositive.push_back(F.TraccePass[i]);

                        }
                        else{
                            Tracce.push_back(Traccia({VerticiTraccia[0],VerticiTraccia[1]}));
                            TracceNegative.push_back(F.TraccePass[i]);
                        }


                    }
                }
            }

            //rifaccio tutto per le non passanti (è un copia e incolla)
            //TracceNOpass

            if(!F.TracceNoPass.empty()){
                for (unsigned int i=1; i<F.TraccePass.size(); i++){
                    segnoVerticeTraccia0=DirezioneTraccia.cross(VerticiTraccia[0]-Tracce[F.TraccePass[i]].VerticiTraccia[0]).dot(F.vecNormale);
                    segnoVerticeTraccia1=DirezioneTraccia.cross(VerticiTraccia[0]-Tracce[F.TraccePass[i]].VerticiTraccia[1]).dot(F.vecNormale);

                    if(segnoVerticeTraccia0*segnoVerticeTraccia1<0 && abs(segnoVerticeTraccia0)>tol && abs(segnoVerticeTraccia1)>tol){
                        NuovoVerticeTraccia=IncontroTraRette(Tracce[F.TraccePass[i]].VerticiTraccia[0]-Tracce[F.TraccePass[i]].VerticiTraccia[1], Tracce[F.TraccePass[i]].VerticiTraccia[0],DirezioneTraccia , VerticiTraccia[0]);
                        if(segnoVerticeTraccia0>0){
                            IdPositivo=Tracce.size();
                            Tracce.push_back(Traccia({VerticiTraccia[0],NuovoVerticeTraccia}));
                            IdNegativo=IdPositivo+1;
                            Tracce.push_back(Traccia({NuovoVerticeTraccia,VerticiTraccia[1]}));

                            TraccePositiveNopass.push_back(IdPositivo);
                            TracceNegativeNopass.push_back(IdNegativo);


                            //inserisco un vertice nelle positive, l'altro nelle negative
                            //qua vado a inserire la traccia nuova.

                        }
                        else{
                            IdNegativo=Tracce.size();
                            Tracce.push_back(Traccia({VerticiTraccia[0],NuovoVerticeTraccia}));
                            IdPositivo=IdPositivo+1;
                            Tracce.push_back(Traccia({NuovoVerticeTraccia,VerticiTraccia[1]}));

                            TraccePositiveNopass.push_back(IdPositivo);
                            TracceNegativeNopass.push_back(IdNegativo);

                            //cambio inserimento, ora una da una parte ora dall'altra
                        }
                    }
                    else {
                        if(segnoVerticeTraccia0>0){
                            Tracce.push_back(Traccia({VerticiTraccia[0],VerticiTraccia[1]}));
                            TraccePositiveNopass.push_back(F.TracceNoPass[i]);

                        }
                        else{
                            Tracce.push_back(Traccia({VerticiTraccia[0],VerticiTraccia[1]}));
                            TracceNegativeNopass.push_back(F.TracceNoPass[i]);
                        }


                    }
                }
            }

        }

        //qua creo le fratture che mi servono
        if(TracciaSulBordo){
            //Figli.resize(1);
            Frattura F=Frattura(PuntiPositivi.size(),PuntiPositivi);
            F.TraccePass=TraccePositive;
            F.TracceNoPass=TraccePositiveNopass;
            F.IdVertici=NuoviIndiciPositivi;
            Figli.push_back(F);
        }
        else{
            Frattura F1=Frattura(PuntiPositivi.size(),PuntiPositivi);
            F1.TraccePass=TraccePositive;
            F1.TracceNoPass=TraccePositiveNopass;
            F1.IdVertici=NuoviIndiciPositivi;
            Figli.push_back(F1);

            Frattura F2=Frattura(PuntiNegativi.size(),PuntiNegativi);
            F2.TraccePass=TracceNegative;
            F2.TracceNoPass=TracceNegativeNopass;
            F2.IdVertici=NuoviIndiciNegativi;
            Figli.push_back(F1);
            Figli.push_back(F2);
            Figli.push_back(Frattura(PuntiNegativi.size(),PuntiNegativi));
        }


        return Figli;

    }

    unsigned int RicercaIdVertice(Frattura& FMadre, Vector3d PuntodaControllare, double tol2){

        for (unsigned int i=0; i<FMadre.SottoPoligoni.CoordinatesCell0Ds.size(); i++){
            if((FMadre.SottoPoligoni.CoordinatesCell0Ds[i]-PuntodaControllare).squaredNorm()<tol2){
                return i;

            }
        }
        int j=FMadre.SottoPoligoni.CoordinatesCell0Ds.size();
        FMadre.SottoPoligoni.IdCell0Ds.push_back(j);
        FMadre.SottoPoligoni.CoordinatesCell0Ds.push_back(PuntodaControllare);
        FMadre.SottoPoligoni.NumberofCell0Ds ++;
        return j;

    }






    //Fine codice Flavio

    vector<Frattura> calcoloSottoPoligoniNoPass(Frattura& F,double tol);

}
