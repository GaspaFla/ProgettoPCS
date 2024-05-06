#pragma <once>
#include "../src/Utils.hpp"
#include"../src/GeometryLibrary.hpp"
#include <gtest/gtest.h>
using namespace DFN;
TEST(ControlloDelPuntoMedio, DueTriangoliDistanti)
{
    Vector3d x(0,1,0);
    Vector3d y(-1,0,0);
    Vector3d z(1,0,0);
    vector<Vector3d>Coord={x,y,z};
    Frattura F1=Frattura(0,3,Coord);
    int control=1;
    Vector3d x1(3,1,0);
    Vector3d y1(2,0,0);
    Vector3d z1(4,0,0);
    vector<Vector3d> Coord1={x1,y1,z1};
    Frattura F2=Frattura(0,3,Coord1);
    double tol=0.000000001;
    bool flag =ControlloCentromero(F1,F2, tol);
    if(flag){
        control=0;
    }

    EXPECT_EQ(1, control);



}

TEST(ControlloCentromero, Quadratimoltovicini)
{
//TERZO TEST, DUE QUADRATI CHE SI INTERSECANO POCO (ANCHE SE SONO COMPLANARI)
Vector3d x(0,0,-1);
Vector3d y(0,1,0);
Vector3d z(0,-1,0);
Vector3d k(0,0,1);
//QUADRATO CENTRO ZERO RAGGIO 1
vector<Vector3d>Coord={x,y,z,k};
Frattura F1=Frattura(0,4,Coord);

Vector3d x1(0,1.9,-1);
Vector3d y1(0,2.9,0);
Vector3d z1(0,0.9,0);
Vector3d k1(0,1.9,1);
vector<Vector3d> Coord1={x1,y1,z1,k1};
Frattura F2=Frattura(0,4,Coord1);
double tol=0.000000001;
bool flag=ControlloCentromero(F1,F2, 0.000000000001);
if(flag){
    cout<<"ehhi";
}
else{
    cout<<"a";
}
}

//TEST EstremiTraccia
TEST(EstremiTraccia, PuntiAllineati)
{
    Vector3d PuntoRetta(5,0,0) ;
    Vector3d P0(4,0,0);
    Vector3d P1(3,0,0);
    Vector3d P2(1,0,0);
    Vector3d P3(4,0,0);
    array<Vector3d,4> Punti ={P0,P1,P2,P3};
    double tol = 0.000000000000001;
    array<unsigned int,2> PuntiInterni = EstremiTraccia(Punti,PuntoRetta,tol);
    bool flag = false;
    cout<<PuntiInterni[0] << " " <<PuntiInterni[1]<<endl;
    if(PuntiInterni[0] == 0 && PuntiInterni[1] == 1){
        flag = true;
    }
    EXPECT_TRUE(flag);
}
