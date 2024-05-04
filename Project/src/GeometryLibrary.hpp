/* In questo file mettiamo le strutture delle fratture e delle tracce
  Frattura -> ID NumVertici VettoredeiVertici  Tracce

 Traccia -> ID Vertici IdFratture Lunghezza
 */

#pragma once
#include <Eigen/Eigen>
#include <vector>

using namespace std;
using namespace Eigen;

namespace DFN{ //Scelgo come namespace DFN perchè così ho tutto quello che riguarda fratture/tracce sotto lo stesso namespace
struct Trace{
    unsigned int IdTrace;
    array<Vector3d,2> VerticesTrace;
    array<unsigned int,2> FractureTrace; //memorizzo id delle due fratture che fanno parte della traccia
    array<bool,2> Tips; // memorizzo True se è passante e False se non è passante
    double length;
    Trace(unsigned int IdT, array<Vector3d,2> VerticesT, array<unsigned int,2> FractureT,array<bool,2> T){//Costruttore
        IdTrace = IdT;
        VerticesTrace = VerticesT;
        FractureTrace = FractureT;
        Tips = T;
        length = (VerticesT[0]-VerticesT[1]).squaredNorm();//Salvo la lunghezza al quadrato per evitare di fare radici
    }
};
struct Fracture{

    unsigned int IdFracture;
    unsigned int NumVertices; // num vertici per ogni frattura
    vector<Vector3d> CoordinatesVertices ;  //Uso i vettori di Eigen perchè voglio fare operazioni matematiche
    vector<unsigned int> TraceNoPass; //Creo un vettore degli id delle tracce così posso accedere al vettore di tutte le tracce che saranno ordinate pe id
    vector<unsigned int> TracePass;
    double d;
    Vector3d vecNormal;
    Fracture(unsigned int IdF, unsigned int NumV,  vector<Vector3d> CoordinatesV ){//Costruttore
        IdFracture = IdF;
        NumVertices = NumV;
        CoordinatesVertices = CoordinatesV;
        vecNormal = (CoordinatesVertices[0]-CoordinatesVertices[1]).cross(CoordinatesVertices[1]-CoordinatesVertices[2]);
        //Salvo direttamente la normale che mi servirà per dei conti in seguito
    }

};

}
