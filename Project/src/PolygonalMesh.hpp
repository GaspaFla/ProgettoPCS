#pragma once
#include <Eigen/Eigen>
#include <iostream>
#include <vector>

using namespace std;
using namespace Eigen;


namespace MeshLibrary{
struct MeshPoligonale{
    //Cell0D
    unsigned int NumberofCell0Ds = 0; //numero di nodi
    vector<unsigned int> IdCell0Ds = {};
    vector<Vector3d> CoordinatesCell0Ds ;  // Uso i vettori di Eigen perchè voglio fare operazioni matematiche

    //Cell1D
    unsigned int NumberofCell1Ds = 0; //numero di lati
    vector<unsigned int> IdCell1Ds = {};
    vector<array<unsigned int,2>> VerticesCell1Ds = {};  // Uso gli array di STL perchè so a priori la dimensione


    //Cell2D
    unsigned int NumberofCell2Ds = 0;
    vector<unsigned int> IdCell2Ds = {};
    // Non so a priori quanti vertici e lati ha un poligono, quindi non posso usare gli array
    // Uso vettori
    // Faccio una lista di vettori che contengono gli id dei vertici o lati
    vector<vector<unsigned int>> VerticesCell2Ds  = {};
    vector<vector<unsigned int>> EdgesCell2Ds = {};



};
}
