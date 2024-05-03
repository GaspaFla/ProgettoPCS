#include "./src/GeometryLibrary.hpp"
#include "./src/Utils.hpp"
#include <iostream>
#include<vector>

using namespace std;
using namespace DFN;

int main()
{
    vector<Fracture> Fractures;
    string fileName = "./DFN/FR3_data.txt";

    importFracture(fileName,Fractures);
    for(auto f : Fractures){
        cout<<"Id "<< f.IdFracture << " NumVertici " << f.NumVertices<< endl;
        for (unsigned int i = 0; i< f.NumVertices; i++){
            cout << "Vertice " <<i << " :" <<endl;
            cout  << f.CoordinatesVertices[i] << endl;
        }
    }
  return 0;
}
