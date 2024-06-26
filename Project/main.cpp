#include "./src/GeometryLibrary.hpp"
#include "./src/Utils.hpp"
#include "./src/Utils2.hpp"
#include<chrono>
#include<iostream>

using namespace std;
using namespace DFN;
int main()
{

    string fileName = "./DFN/FR3_data.txt";

    double tol1D =setTol1D();
    double tol2=SetTolProdotto(tol1D);
    vector<Frattura> Fratture;
    vector<Traccia> Tracce;
    bool parte1 = Progetto1(fileName,Fratture,Tracce, tol1D,tol2);
    if(parte1){
        Progetto2(Fratture, Tracce, tol1D, tol2);
    }

    return 0;
}
