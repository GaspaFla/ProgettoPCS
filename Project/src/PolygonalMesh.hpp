#pragma once
#include <Eigen/Eigen>
#include <vector>

using namespace std;
using namespace Eigen;


namespace LibreriaMesh{
struct MeshPoligonale{
    //Cell0D
    unsigned int NumeroCell0D = 0; //numero di nodi
    vector<unsigned int> IdCell0D = {};
    vector<Vector3d> CoordinateCell0D ;  // Uso i vettori di Eigen perchè voglio fare operazioni matematiche

    //Cell1D
    unsigned int NumeroCell1D = 0; //numero di lati
    vector<unsigned int> IdCell1D = {};
    vector<array<unsigned int,2>> VerticiCell1D = {};  // Uso gli array di STL perchè so a priori la dimensione


    //Cell2D
    unsigned int NumeroCell2D = 0;
    vector<unsigned int> IdCell2D = {};
    // Non so a priori quanti vertici e lati ha un poligono, quindi non posso usare gli array ==> uso i vettori
    // Faccio una lista di vettori che contengono gli id dei vertici o lati
    vector<vector<unsigned int>> VerticiCell2D  = {};
    vector<vector<unsigned int>> LatiCell2D = {};

    //Triangolazione per esportare su Paraview
    vector<vector<vector<unsigned int>>> Triangolazione();

    void GedimInterface(vector<vector<unsigned int>>& triangoli,
                                        VectorXi& materiali);


};

}
