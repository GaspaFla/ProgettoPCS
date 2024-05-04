#pragma <once>
#include "../src/Utils.hpp"
#include"../src/GeometryLibrary.hpp"
#include <gtest/gtest.h>
using namespace DFN;
TEST(ControlloDelPuntoMedio, A)
{
    Vector3d x(0,1,0);
    Vector3d y(-1,0,0);
    Vector3d z(1,0,0);
    vector<Vector3d>Coord={x,y,z};
    Fracture F1=Fracture(0,3,Coord);
    int control=1;
    Vector3d x1(3,1,0);
    Vector3d y1(2,0,0);
    Vector3d z1(4,0,0);
    vector<Vector3d> Coord1={x1,y1,z1};
    Fracture F2=Fracture(0,3,Coord1);
    double tol=0.000000001;
    bool flag =ControlloCentromero(F1,F2, tol);
    if(flag){
        control=0;
    }

     EXPECT_EQ(1, control);



}



