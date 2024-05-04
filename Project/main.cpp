#include "./src/GeometryLibrary.hpp"
#include "./src/Utils.hpp"
#include <iostream>
#include<vector>

using namespace std;
using namespace DFN;

int main()
{
    vector<Frattura> Fratture;
    string fileName = "./DFN/FR3_data.txt";

    double tol1D = setTol1D();
    importoFratture(fileName,Fratture,tol1D);
    for(auto f : Fratture){
        cout<<"Id "<< f.IdFrattura << " NumVertici " << f.NumVertici<< endl;
        for (unsigned int i = 0; i< f.NumVertici; i++){
            cout << "Vertice " <<i << " :" <<endl;
            cout  << f.CoordinateVertici[i] << endl;
        }
    }
  return 0;
}
