#include "Utils.hpp"
#include "GeometryLibrary.hpp"
#include "Eigen/Eigen"
#include <iostream>
using namespace std;

namespace DFN{
bool ControlloNonParallelo(Fracture F1, Fracture F2, double tol){
Vector3d v1=(F1.CoordinatesVertices[0]-F1.CoordinatesVertices[1]).cross(F1.CoordinatesVertices[1]-F1.CoordinatesVertices[2]);
Vector3d v2=(F2.CoordinatesVertices[0]-F2.CoordinatesVertices[1]).cross(F2.CoordinatesVertices[1]-F2.CoordinatesVertices[2]);
//presi due punti per ogni poligono trovo un vettore che giace sul piano. Dopodichè ne calcolo la direttrice del piano
if ((v1.cross(v2)).norm()<tol){
    cout<<v1(2)<<endl;
    //controllo se c'è un coefficiente comune tra le componenti del vettore direttore
    return false;
}
return true;




}
bool ControlloCentromero(Fracture F1, Fracture F2, double tol){

    Vector3d vecmediaF1= Vector3d::Zero(3);
    for (int i=0; i<F1.NumVertices; i++){
        vecmediaF1=vecmediaF1+F1.CoordinatesVertices[i];
    }
    Vector3d vecmediaF2= Vector3d::Zero(3);
    for (int i=0; i<F2.NumVertices; i++){
        vecmediaF2=vecmediaF2+F2.CoordinatesVertices[i];
    }
    //calcolo le medie dei centri (sono vettori)--se funziona posso ottimizzare mettendoli in un ciclo for
    double raggioquadro1=0;
    double raggioquadro2=0;
    for (int i=0; i<F1.NumVertices; i++){
    raggioquadro1=max(raggioquadro1,(F1.CoordinatesVertices[i]-vecmediaF1).squaredNorm());
    }
    for (int i=0; i<F2.NumVertices; i++){
        raggioquadro2=max(raggioquadro2,(F2.CoordinatesVertices[i]-vecmediaF2).squaredNorm());
    }
    //confronto le distanze dal centro e scelgo quella maggiore, in modo tale da definire una palla in cui sia contenuto il poligono
    if ((vecmediaF1-vecmediaF2).squaredNorm()<raggioquadro1+raggioquadro2){
        return true;
    }
    //confronto la distanza tra i centroidi con la somma dei raggi delle palle
    //faccio tutto al quadrato

    return false;

}




}
