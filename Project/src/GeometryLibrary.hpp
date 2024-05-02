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
    array<unsigned int,2> FracturTrace; //memorizzo id delle due fratture che fanno parte della traccia
    array<bool,2> Tips; // memorizzo True se è passante e False se non è passante
};
struct Fracture{

    unsigned int IdFracture;
    unsigned int NumVertices; // num vertici per ogni frattura
    vector<Vector3d> CoordinatesVertices ;  //Uso i vettori di Eigen perchè voglio fare operazioni matematiche
    vector<Trace > TraceNoPass;
    vector<Trace> TracePass;
    Fracture(unsigned int Id,unsigned int NumV,vector<Vector3d> CoordinatesV){
        IdFracture=Id;
        NumVertices=NumV;
        CoordinatesVertices=CoordinatesV;
    }

    //map<unsigned int,list<unsigned int>> VerticesMarker;


};

}
