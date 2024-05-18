/* In questo file mettiamo le strutture delle fratture e delle tracce
  Frattura -> ID NumVertici VettoredeiVertici  Tracce

 Traccia -> ID Vertici IdFratture Lunghezza
 */

#pragma once
#include <Eigen/Eigen>
#include <vector>
#include "PolygonalMesh.hpp"

using namespace std;
using namespace Eigen;
using namespace MeshLibrary;

namespace DFN{ //Scelgo come namespace DFN perchè così ho tutto quello che riguarda fratture/tracce sotto lo stesso namespace
struct Traccia{
    unsigned int IdTraccia;
    array<Vector3d,2> VerticiTraccia;
    array<unsigned int,2> FrattureTraccia; //memorizzo id delle due fratture che fanno parte della traccia
    array<bool,2> Tips; // memorizzo True se è passante e False se non è passante
    double lunghezza;
    Traccia() = default;
    Traccia(unsigned int IdT, array<Vector3d,2> VerticiT, array<unsigned int,2> FrattureT,array<bool,2> T){//Costruttore
        IdTraccia = IdT;
        VerticiTraccia = VerticiT;
        FrattureTraccia = FrattureT;
        Tips = T;
        lunghezza = (VerticiT[0]-VerticiT[1]).norm();//Salvo la lunghezza al quadrato per evitare di fare radici
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
    MeshPoligonale SottoPoligoni;
    vector<unsigned int> IdVertici;



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

    void aggiungiCell0DMesh(unsigned int IdCell0D,Vector3d coordinateCell0D){
        SottoPoligoni.IdCell0Ds.push_back(IdCell0D);
        SottoPoligoni.CoordinatesCell0Ds.push_back(coordinateCell0D);
        SottoPoligoni.NumberofCell0Ds ++;
    }


    //Prima di chiamare la ricorsiva sulla madre aggiungo i primi vertici alla mesh
    //Aggiungo anche gli id dei vertici della madre
    //Per i figli aggiungo dopo aver creato la frattura
};


//Crea un vettore che contiene le coordinate di tutti i vertici che andranno a creare la mesh ==> non vuoto solo per la madre
//Per ogni figlio e anche per la madre salvo un vettore che contiene solo gli id dei figli, così trovo le coordinate tramite il vettore della madre

}
