#pragma once
#include <iostream>
#include <vector>
#include "GeometryLibrary.hpp"
#include "Utils.hpp"
using namespace std;



namespace DFN{
    void Taglia(Frattura F,double tol);
    void calcoloSottoPoligoniPass(Frattura F,double tol);
    void calcoloSottoPoligoniNoPass(Frattura F,double tol);
    void converteInCelle(Frattura F);//Deve prendere un oggetto di poligonaleMesh
}
