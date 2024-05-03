#include "./src/GeometryLibrary.hpp"
#include "./src/Utils.hpp"
#include <iostream>
#include<vector>

using namespace std;
using namespace DFN;

int main()
{
    vector<Fracture> Fractures;
    string fileName = "C:/Users/rosit/OneDrive/Desktop/Programmazione e Calcolo/Progetto_PCS_2024/Progetto_PCS_2024/Project/DFN/FR3_data.txt";
    importFracture(fileName,Fractures);
    for(auto f : Fractures){
        cout<< f.IdFracture << " " << f.NumVertices<< endl;
        for (unsigned int i = 0; i< f.NumVertices; i++){
            cout << f.CoordinatesVertices[i] << endl;
        }
    }
  return 0;
}
