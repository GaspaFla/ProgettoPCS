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

    double tol1D = 0.0000000000000000000000001;
    double tol2=SetTolProdotto(tol1D);
    vector<Frattura> Fratture;
    vector<Traccia> Tracce;
    chrono::steady_clock::time_point t_begin = std::chrono::steady_clock::now();
    bool parte1 = Progetto1(fileName,Fratture,Tracce, tol1D,tol2);
    if(parte1){
        Progetto2(Fratture, Tracce, tol1D, tol2);
    }
    chrono::steady_clock::time_point t_end = std::chrono::steady_clock::now();
    double duration=std::chrono::duration_cast<std::chrono::milliseconds>(t_end-t_begin).count();
    cout<<"Tempo impiegato: "<<duration<<endl;

    return 0;
}
