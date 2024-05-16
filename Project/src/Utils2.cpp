#include "Utils.hpp"
#include "GeometryLibrary.hpp"
#include "Eigen/Eigen"
#include "Sort.hpp"
#include <iostream>
#include <sstream> // controlla se servono tutte
#include <fstream>
#include <string>
#include "Utils2.hpp"
#include <vector>
using namespace std;
namespace DFN{
vector<Frattura> calcoloSottoPoligoniPass(Frattura& F,double tol, double tol2,bool& TracciaSulBordo, int& contId, vector<Traccia> Tracce){
    vector<Frattura> Figli;
    vector<Vector3d>PuntiPositivi;//vettore in cui mettere i punti a destra
    vector<Vector3d>PuntiNegativi;//vettore in cui mettere i punti a sinistra
    int PuntiNuovi=0; //quanti nuovi vertici ho trovato
    vector<Vector3d> VerticiTraccia={ Tracce[F.TraccePass[0]].VerticiTraccia[0],Tracce[F.TraccePass[0]].VerticiTraccia[1]};//vettore con dentro i due vertici della traccia
    int SegnoPrec=1;
    double segno;
    for (int i=0; i<F.NumVertici; i++){
        if(PuntiNuovi<2){
            //se devo ancora trovarne faccio controlli altrimenti faccio un'assegnazione veloce
            segno=((VerticiTraccia[0]-VerticiTraccia[1]).cross(VerticiTraccia[0]-F.CoordinateVertici[i])).dot(F.vecNormale);
            if(i==0){
                //caso 1, il vertice coincide con il vertice traccia ma il lato non è compreso
                if((VerticiTraccia[0]-F.CoordinateVertici[i]).squaredNorm() <tol2 && abs(segno)>tol ){
                    //dico che ci sono vertici traccia che funzionano, rimane solo più l'altro punto
                    PuntiNuovi++;
                    PuntiPositivi.push_back(F.CoordinateVertici[i]);
                    PuntiNegativi.push_back(F.CoordinateVertici[i]);
                    SegnoPrec=(VerticiTraccia[0]-VerticiTraccia[1]).cross(VerticiTraccia[0]-F.CoordinateVertici[i+1]).dot(F.vecNormale);
                }
                else if ((VerticiTraccia[1]-F.CoordinateVertici[i]).squaredNorm() <tol2 && abs(segno)>tol){
                    PuntiNuovi++;
                    PuntiPositivi.push_back(F.CoordinateVertici[i]);
                    PuntiNegativi.push_back(F.CoordinateVertici[i]);
                    SegnoPrec=((VerticiTraccia[0]-VerticiTraccia[1]).cross(VerticiTraccia[0]-F.CoordinateVertici[i+1])).dot(F.vecNormale);
                }
                // caso 2, la retta della traccia appartiene al lato
                else if(abs(segno)<tol){
                    TracciaSulBordo=true;
                    vector<Vector3d>VerticiTracciaOrdinati;
                    //quale è il più vicino?

                    if((VerticiTraccia[1]-F.CoordinateVertici[i]).squaredNorm()<(VerticiTraccia[0]-F.CoordinateVertici[i]).squaredNorm()){
                        if((VerticiTraccia[1]-F.CoordinateVertici[i]).squaredNorm() <tol2){
                            //visto che i punti coincidono assegno solo l'altro
                            VerticiTracciaOrdinati={VerticiTraccia[0]};
                            contId++;
                        }
                        else{

                            VerticiTracciaOrdinati={VerticiTraccia[1],VerticiTraccia[0]};
                            contId++;

                            contId++;
                        }
                    }
                    else{
                        if((VerticiTraccia[0]-F.CoordinateVertici[i]).squaredNorm() <tol2){
                            //visto che i punti coincidono assegno solo l'altro
                            VerticiTracciaOrdinati={VerticiTraccia[1]};
                            contId++;
                        }
                        else{

                            VerticiTracciaOrdinati=VerticiTraccia;
                            contId++;

                            contId++;
                        }
                    }
                    //sta sulla retta con il successivo?

                    if((F.CoordinateVertici[i+1]-F.CoordinateVertici[i]).cross(VerticiTraccia[0]-F.CoordinateVertici[i]).squaredNorm()<tol2){
                        //salvo tutti nel positivo
                        PuntiPositivi.push_back(F.CoordinateVertici[i]);
                        for (int j=0; j<VerticiTracciaOrdinati.size(); j++){

                            PuntiPositivi.push_back(VerticiTracciaOrdinati[j]);
                            PuntiNuovi++;
                        }



                    }
                    else{
                        for (int j=0; j<VerticiTracciaOrdinati.size(); j++){

                            PuntiPositivi.push_back(VerticiTracciaOrdinati[j]);
                            PuntiNuovi++;
                        }


                        PuntiPositivi.push_back(F.CoordinateVertici[i]);
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
                    PuntiNegativi.push_back(F.CoordinateVertici[i]);
                    SegnoPrec=((VerticiTraccia[0]-VerticiTraccia[1]).cross(VerticiTraccia[0]-F.CoordinateVertici[i+1])).dot(F.vecNormale);
                }
                else if ((VerticiTraccia[1]-F.CoordinateVertici[i]).squaredNorm() <tol2 && abs(segno)>tol){
                    PuntiNuovi++;
                    PuntiPositivi.push_back(F.CoordinateVertici[i]);
                    PuntiNegativi.push_back(F.CoordinateVertici[i]);
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
                            contId++;
                        }
                        else{

                            VerticiTracciaOrdinati={VerticiTraccia[1],VerticiTraccia[0]};
                            contId++;

                            contId++;
                        }
                    }
                    else{
                        if((VerticiTraccia[0]-F.CoordinateVertici[i]).squaredNorm() <tol2){
                            //visto che i punti coincidono assegno solo l'altro
                            VerticiTracciaOrdinati={VerticiTraccia[1]};
                            contId++;
                        }
                        else{

                            VerticiTracciaOrdinati=VerticiTraccia;
                            contId++;

                            contId++;
                        }
                    }
                    //guardo in quale stavo già inserendo tutti i miei dati e continuo a farlo
                    if(SegnoPrec>0){
                        PuntiPositivi.push_back(F.CoordinateVertici[i]);
                        for (int j=0; j<VerticiTracciaOrdinati.size(); j++){

                            PuntiPositivi.push_back(VerticiTracciaOrdinati[j]);
                            PuntiNuovi++;
                        }
                    }
                    else{
                        PuntiNegativi.push_back(F.CoordinateVertici[i]);
                        for (int j=0; j<VerticiTracciaOrdinati.size(); j++){

                            PuntiNegativi.push_back(VerticiTracciaOrdinati[j]);
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
                        }
                        else{
                            PuntiNegativi.push_back(F.CoordinateVertici[i]);
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
                                contId++;
                                PuntiNegativi.push_back(VerticiTraccia[0]);
                                PuntiPositivi.push_back(VerticiTraccia[0]);
                                PuntiPositivi.push_back(F.CoordinateVertici[i]);
                            }
                            else{
                                contId++;
                                PuntiNegativi.push_back(VerticiTraccia[0]);
                                PuntiPositivi.push_back(VerticiTraccia[0]);
                                PuntiNegativi.push_back(F.CoordinateVertici[i]);
                            }

                        }
                        //scopro che era il punto 1 e non il punto 0, rifaccio tutto
                        else{
                            if (segno>0){
                                //aggiungo il punto nuovo sia a una sia all'altra
                                PuntiNegativi.push_back(VerticiTraccia[1]);
                                PuntiPositivi.push_back(VerticiTraccia[1]);
                                PuntiPositivi.push_back(F.CoordinateVertici[i]);

                                contId++;
                            }
                            else{
                                PuntiNegativi.push_back(VerticiTraccia[1]);
                                PuntiPositivi.push_back(VerticiTraccia[1]);
                                PuntiNegativi.push_back(F.CoordinateVertici[i]);

                                contId++;
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
                    PuntiNegativi.push_back(F.CoordinateVertici[i]);

                }
                else if ((VerticiTraccia[1]-F.CoordinateVertici[i]).squaredNorm() <tol2 && abs(segno)>tol){
                    PuntiNuovi++;
                    PuntiPositivi.push_back(F.CoordinateVertici[i]);
                    PuntiNegativi.push_back(F.CoordinateVertici[i]);

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
                            contId++;
                        }
                        else{

                            VerticiTracciaOrdinati={VerticiTraccia[1],VerticiTraccia[0]};
                            contId++;

                            contId++;
                        }
                    }
                    else{
                        if((VerticiTraccia[0]-F.CoordinateVertici[i]).squaredNorm() <tol2){
                            //visto che i punti coincidono assegno solo l'altro
                            VerticiTracciaOrdinati={VerticiTraccia[1]};
                            contId++;
                        }
                        else{

                            VerticiTracciaOrdinati=VerticiTraccia;
                            contId++;

                            contId++;
                        }
                    }
                    //guardo in quale stavo già inserendo tutti i miei dati e continuo a farlo
                    if(SegnoPrec>0){
                        PuntiPositivi.push_back(F.CoordinateVertici[i]);
                        for (int j=0; j<VerticiTracciaOrdinati.size(); j++){

                            PuntiPositivi.push_back(VerticiTracciaOrdinati[j]);
                            PuntiNuovi++;
                        }
                    }
                    else{
                        PuntiNegativi.push_back(F.CoordinateVertici[i]);
                        for (int j=0; j<VerticiTracciaOrdinati.size(); j++){

                            PuntiNegativi.push_back(VerticiTracciaOrdinati[j]);
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
                        }
                        else{
                            PuntiNegativi.push_back(F.CoordinateVertici[i]);
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
                                contId++;
                                PuntiNegativi.push_back(VerticiTraccia[0]);
                                PuntiPositivi.push_back(VerticiTraccia[0]);
                                PuntiPositivi.push_back(F.CoordinateVertici[i]);
                            }
                            else{
                                contId++;
                                PuntiNegativi.push_back(VerticiTraccia[0]);
                                PuntiPositivi.push_back(VerticiTraccia[0]);
                                PuntiNegativi.push_back(F.CoordinateVertici[i]);
                            }

                        }
                        //scopro che era il punto 1 e non il punto 0, rifaccio tutto
                        else{
                            if (segno>0){
                                //aggiungo il punto nuovo sia a una sia all'altra
                                PuntiNegativi.push_back(VerticiTraccia[1]);
                                PuntiPositivi.push_back(VerticiTraccia[1]);
                                PuntiPositivi.push_back(F.CoordinateVertici[i]);

                                contId++;
                            }
                            else{
                                PuntiNegativi.push_back(VerticiTraccia[1]);
                                PuntiPositivi.push_back(VerticiTraccia[1]);
                                PuntiNegativi.push_back(F.CoordinateVertici[i]);

                                contId++;
                            }

                        }

                    }
                }
            }


            }

        else{
            if(SegnoPrec>0){
                PuntiPositivi.push_back(F.CoordinateVertici[i]);
            }
            else{
                PuntiNegativi.push_back(F.CoordinateVertici[i]);
            }
        }
    }
    //devo fare la funzione che mi divide le tracce e mi dice se sono di una frattura o dell'altra
    double segnoVerticeTraccia1;
    double segnoVerticeTraccia2;
    vector<Traccia>TracceNegative;
    vector<Traccia>TraccePositive;
    Vector3d NuovoVerticeTraccia;
    Vector3d DirezioneTraccia=VerticiTraccia[0]-VerticiTraccia[1];
    if(!F.TraccePass.empty()){
    for (int i=1; i<F.TraccePass.size(); i++){
           segnoVerticeTraccia1=DirezioneTraccia.cross(VerticiTraccia[0]-Tracce[F.TraccePass[i]].VerticiTraccia[0]).dot(F.vecNormale);
           segnoVerticeTraccia2=DirezioneTraccia.cross(VerticiTraccia[0]-Tracce[F.TraccePass[i]].VerticiTraccia[1]).dot(F.vecNormale);
            if(abs(segnoVerticeTraccia1)<tol) {


}
            else if (abs(segnoVerticeTraccia2)<tol){
//altro inserimento

}
             else if(segnoVerticeTraccia1*segnoVerticeTraccia2<0){
               NuovoVerticeTraccia=IncontroTraRette(Tracce[F.TraccePass[i]].VerticiTraccia[0]-Tracce[F.TraccePass[i]].VerticiTraccia[1], Tracce[F.TraccePass[i]].VerticiTraccia[0],DirezioneTraccia , VerticiTraccia[0]);
               if(segnoVerticeTraccia1>0){
                   //anche qua nelle negative
                   //qua vado a inserire la traccia nuova. Forse anche qui potremmo volere un altro modo...

               }
               else{
                   //cambio inserimento, ora una da una parte ora dall'altra
               }
            }
               else {
                   //inserisco le tracce così come sono o da una parte o dall'altra
               }
        }
    }

    //rifaccio tutto per le non passanti (è un copia e incolla)


    //qua creo le fratture che mi servono
    if(TracciaSulBordo){
        //Figli.resize(1);
        Figli.push_back(Frattura(1,PuntiPositivi.size(),PuntiPositivi));
    }
    else{
        Figli.push_back(Frattura(1,PuntiPositivi.size(),PuntiPositivi));
        Figli.push_back(Frattura(2,PuntiNegativi.size(),PuntiNegativi));
    }


    return Figli;

}
}

