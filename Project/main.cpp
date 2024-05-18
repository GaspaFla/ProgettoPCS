#include <iostream>
#include "./src/GeometryLibrary.hpp"
#include "./src/Utils.hpp"
//#include"./src/Sort.hpp"
using namespace std;
using namespace DFN;
int main()
{

    string fileName = "./DFN/FR200_data.txt";

    double tol1D = setTol1D();
    Progetto1(fileName, tol1D);


    return 0;
}
