#pragma once
#include <Eigen/Eigen>
#include <vector>
#include "PolygonalMesh.hpp"

using namespace std;
using namespace Eigen;
using namespace LibreriaMesh;

namespace DFN{ //Scelgo come namespace DFN perchè così ho tutto quello che riguarda fratture/tracce sotto lo stesso namespace
struct Traccia{
    unsigned int IdTraccia;
    array<Vector3d,2> VerticiTraccia;
    array<unsigned int,2> FrattureTraccia; //memorizzo id delle due fratture che fanno parte della traccia
    array<bool,2> Tips; // memorizzo False se è passante e True se non è passante
    double lunghezza;
    Traccia() = default;
    Traccia(unsigned int IdT, array<Vector3d,2> VerticiT, array<unsigned int,2> FrattureT,array<bool,2> T){//Costruttore
        IdTraccia = IdT;
        VerticiTraccia = VerticiT;
        FrattureTraccia = FrattureT;
        Tips = T;
        lunghezza = (VerticiT[0]-VerticiT[1]).norm();//Non salvo la lunghezza al quadrato perchè devo stamparla
                                                     //La memorizza come attributo per poter fare il calcolo una volta sola
    }
    Traccia(array<Vector3d,2> VerticiT){
         VerticiTraccia = VerticiT;
    }

};
struct Frattura{
    unsigned int IdFrattura;
    unsigned int NumVertici; // num vertici per ogni frattura
    vector<Vector3d> CoordinateVertici ;  //Uso i vettori di Eigen perchè voglio fare operazioni matematiche
    vector<unsigned int> TracceNoPass; //Creo un vettore degli id delle tracce così posso accedere al vettore di tutte le tracce che saranno ordinate pe id
    vector<unsigned int> TraccePass;
    Vector3d vecNormale;
    double termineNotoPiano;
    MeshPoligonale SottoPoligoni; //Ogni Frattura ha la mesh generata dalle tracce
    vector<unsigned int> IdVertici;//Associo ai vertici degli id che andranno poi a far parte della mesh ==> sono inizializzati prima di effettuare i tagli

    Frattura(unsigned int IdF, unsigned int NumV,  vector<Vector3d> CoordinateV ){//Costruttore
        IdFrattura = IdF;
        NumVertici = NumV;
        CoordinateVertici = CoordinateV;
        vecNormale = (CoordinateVertici[0]-CoordinateVertici[1]).cross(CoordinateVertici[1]-CoordinateVertici[2]);
        termineNotoPiano = vecNormale.dot(CoordinateV[0]);
        //Salvo direttamente la normale e il termine noto per poter risalire all'equazione del piano
        //in questo modo devo calcolarli una volta sola
    }

    Frattura( unsigned int NumV,  vector<Vector3d> CoordinateV ){//Costruttore
        NumVertici = NumV;
        CoordinateVertici = CoordinateV;
        vecNormale = (CoordinateVertici[0]-CoordinateVertici[1]).cross(CoordinateVertici[1]-CoordinateVertici[2]);
        termineNotoPiano = vecNormale.dot(CoordinateV[0]);
    }
    Frattura(unsigned int IdF){
        IdFrattura = IdF;
    }


};

}
