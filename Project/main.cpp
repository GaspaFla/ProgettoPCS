#include <iostream>
#include "./src/GeometryLibrary.hpp"
#include "./src/Utils.hpp"
using namespace std;
using namespace DFN;
int main()
{   vector<Vector3d> Coord;
    Vector3d x(0,0,1);
    Vector3d y(0,0,0);
    Vector3d z(1,0,0);
    Coord={x,y,z};
    Fracture F=Fracture(0,3,Coord);
    vector<Vector3d> Coord1;
    Vector3d x1(4,0,5);
    Vector3d y1(4,0,4);
    Vector3d z1(5,0,4);
    Coord1={x1,y1,z1};
    Fracture F2=Fracture(0,3,Coord1);
    bool flag=ControlloCentromero(F,F2, 0.000000000001);
    if(flag){
        cout<<"ehhi";
    }
    else{
        cout<<"a";
    }





  return 0;
}
