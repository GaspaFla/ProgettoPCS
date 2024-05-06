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
    Vector3d x1(3,1,0);
    Vector3d y1(2,0,0);
    Vector3d z1(4,0,0);
    vector<Vector3d> Coord1={x1,y1,z1};
    Frattura F2=Frattura(0,3,Coord1);
    double tol=0.000000001;
    bool flag =ControlloCentromero(F1,F2);
    int control=1;
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

double tol = 0.000000001;
bool flag=ControlloCentromero(F1,F2);
int control=1;
if(flag){
    control=0;
}

EXPECT_EQ(0, control);
}


TEST(ControlloIntersezionePiano, QuadratiPerpendicolariPerOrigine)
{
    //TERZO TEST, DUE QUADRATI CHE SI INTERSECANO POCO (ANCHE SE SONO COMPLANARI)
    Vector3d x(0,1,-1);
    Vector3d y(0,1,1);
    Vector3d z(0,-1,1);
    Vector3d k(0,-1,-1);
    //QUADRATO CENTRO ZERO RAGGIO 1
    vector<Vector3d>Coord={x,y,z,k};
    Frattura F1=Frattura(0,4,Coord);
    Vector3d x1(1,0,-1);
    Vector3d y1(1,0,1);
    Vector3d z1(-1,0,1);
    Vector3d k1(-1,0,-1);
    vector<Vector3d> Coord1={x1,y1,z1,k1};
    Frattura F2=Frattura(0,4,Coord1);
    vector<Vector3d> PuntiIntersecatiF1;
    vector<Vector3d> PuntiIntersecatiF2;
    PuntiIntersecatiF1.resize(4);
    PuntiIntersecatiF2.resize(4);
    bool flag=SiIntersecano(F1,F2, PuntiIntersecatiF1);
    bool flag1=SiIntersecano(F2,F1, PuntiIntersecatiF2);
    Vector3d b=CalcoloRetta(F1,F2);

    int control=1;
    if(flag){
        control=0;
    }
    int control1=1;
    if(flag1){
        control1=0;
    }
    EXPECT_EQ(0, control1);
    EXPECT_EQ(0, control);
    for(int i=0; i<3; i++){
    EXPECT_DOUBLE_EQ(PuntiIntersecatiF1[0][i],z[i]);
    EXPECT_DOUBLE_EQ(PuntiIntersecatiF2[0][i],z1[i]);
    EXPECT_DOUBLE_EQ(PuntiIntersecatiF1[2][i],k[i]);
    EXPECT_DOUBLE_EQ(PuntiIntersecatiF2[2][i],k1[i]);
    }

}

TEST(ControlloIntersezionePiano2, QuadratiPerpendicolariNonPerOrigine)
{
    //TERZO TEST, DUE QUADRATI CHE SI INTERSECANO POCO (ANCHE SE SONO COMPLANARI)
    Vector3d x(0,6,-1);
    Vector3d y(0,6,1);
    Vector3d z(0,4,1);
    Vector3d k(0,4,-1);
    //QUADRATO CENTRO ZERO RAGGIO 1
    vector<Vector3d>Coord={x,y,z,k};
    Frattura F1=Frattura(0,4,Coord);
    Vector3d x1(1,5,-1);
    Vector3d y1(1,5,1);
    Vector3d z1(-1,5,1);
    Vector3d k1(-1,5,-1);
    vector<Vector3d> Coord1={x1,y1,z1,k1};
    Frattura F2=Frattura(0,4,Coord1);
    vector<Vector3d> PuntiIntersecatiF1;
    vector<Vector3d> PuntiIntersecatiF2;
    PuntiIntersecatiF1.resize(4);
    PuntiIntersecatiF2.resize(4);
    bool flag=SiIntersecano(F1,F2, PuntiIntersecatiF1);
    bool flag1=SiIntersecano(F2,F1, PuntiIntersecatiF2);
    Vector3d b=CalcoloRetta(F1,F2);



    int control=1;
    if(flag){
        control=0;
    }
    int control1=1;
    if(flag1){
        control1=0;
    }
    EXPECT_EQ(0, control1);
    EXPECT_EQ(0, control);
        for(int i=0; i<3; i++){
            EXPECT_DOUBLE_EQ(PuntiIntersecatiF1[0][i],z[i]);
            EXPECT_DOUBLE_EQ(PuntiIntersecatiF2[0][i],z1[i]);
            EXPECT_DOUBLE_EQ(PuntiIntersecatiF1[2][i],k[i]);
            EXPECT_DOUBLE_EQ(PuntiIntersecatiF2[2][i],k1[i]);
    }
}

TEST(ControlloIntersezionePiano3, QuadratiViciniCheNonSiIntersecano)
{
    //TERZO TEST, DUE QUADRATI CHE SI INTERSECANO POCO (ANCHE SE SONO COMPLANARI)
    Vector3d x(0,1,-1);
    Vector3d y(0,1,1);
    Vector3d z(0,-1,1);
    Vector3d k(0,-1,-1);
    //QUADRATO CENTRO ZERO RAGGIO 1
    vector<Vector3d>Coord={x,y,z,k};
    Frattura F1=Frattura(0,4,Coord);
    Vector3d x1(1,1,-1);
    Vector3d y1(1,1,1);
    Vector3d z1(0.1,-1,1);
    Vector3d k1(0.1,-1,-1);
    vector<Vector3d> Coord1={x1,y1,z1,k1};
    Frattura F2=Frattura(0,4,Coord1);
    vector<Vector3d> PuntiIntersecatiF1;
    vector<Vector3d> PuntiIntersecatiF2;
    PuntiIntersecatiF1.resize(4);
    PuntiIntersecatiF2.resize(4);
    bool flag=SiIntersecano(F1,F2, PuntiIntersecatiF1);
    bool flag1=SiIntersecano(F2,F1, PuntiIntersecatiF2);

    int control=1;
    if(flag && flag1){
        control=0;
    }

    EXPECT_EQ(1, control);


}


TEST(ControlloIntersezionePiano4, QuadratiPerpendicolariPerOrigineNonPassante)
{
    //TERZO TEST, DUE QUADRATI CHE SI INTERSECANO POCO (ANCHE SE SONO COMPLANARI)
    Vector3d x(0,1,-1);
    Vector3d y(0,1,1);
    Vector3d z(0,-1,1);
    Vector3d k(0,-1,-1);
    //QUADRATO CENTRO ZERO RAGGIO 1
    vector<Vector3d>Coord={x,y,z,k};
    Frattura F1=Frattura(0,4,Coord);
    Vector3d x1(2,0,-3);
    Vector3d y1(2,0,3);
    Vector3d z1(-2,0,3);
    Vector3d k1(-2,0,-3);
    vector<Vector3d> Coord1={x1,y1,z1,k1};
    Frattura F2=Frattura(0,4,Coord1);
    vector<Vector3d> PuntiIntersecatiF1;
    vector<Vector3d> PuntiIntersecatiF2;
    PuntiIntersecatiF1.resize(4);
    PuntiIntersecatiF2.resize(4);
    bool flag=SiIntersecano(F1,F2, PuntiIntersecatiF1);
    bool flag1=SiIntersecano(F2,F1, PuntiIntersecatiF2);
    Vector3d b=CalcoloRetta(F1,F2);



    int control=1;
    if(flag){
        control=0;
    }
    int control1=1;
    if(flag1){
        control1=0;
    }
    EXPECT_EQ(0, control1);
    EXPECT_EQ(0, control);
    for(int i=0; i<3; i++){
        EXPECT_DOUBLE_EQ(PuntiIntersecatiF1[0][i],z[i]);
        EXPECT_DOUBLE_EQ(PuntiIntersecatiF2[0][i],z1[i]);
        EXPECT_DOUBLE_EQ(PuntiIntersecatiF1[2][i],k[i]);
        EXPECT_DOUBLE_EQ(PuntiIntersecatiF2[2][i],k1[i]);
    }

}

//TEST EstremiTraccia
TEST(EstremiTraccia, PuntiAllineati)
{
    Vector3d PuntoRetta(5,0,0) ;
    double tol = 0.000000000000001;
    array<bool,2> Tips;

    //Caso solo due punti coincidenti
    Vector3d P0(4,0,0);
    Vector3d P1(1,0,0);
    Vector3d P2(4,0,0);
    Vector3d P3(2,0,0);
    array<Vector3d,4> Punti ={P0,P1,P2,P3};
    array<unsigned int,2> PuntiInterni = EstremiTraccia(Punti,PuntoRetta,tol,Tips);
    bool flag1 = false;
    if(PuntiInterni[0] == 0 && PuntiInterni[1] == 3 && Tips[0] == true && Tips[1] == false){
        flag1 = true;
    }
    EXPECT_TRUE(flag1);

    //Caso tutti punti distinti
    //Sottocaso non passante per entrambi
    P0 = {1,0,0};
    P1 = {2,0,0};
    P2 = {4,0,0};
    P3 = {5,0,0};
    Punti ={P0,P1,P2,P3};
    PuntiInterni = EstremiTraccia(Punti,PuntoRetta,tol,Tips);
    bool flag2 = false;
    if(PuntiInterni[0] == 1 && PuntiInterni[1] == 2 && Tips[0] == true && Tips[1] == true){
        flag2 = true;
    }
    EXPECT_TRUE(flag2);

    //Sottocaso passante per uno e non passante per l' altro
    P0 = {2,0,0};
    P1 = {3,0,0};
    P2 = {1,0,0};
    P3 = {5,0,0};
    Punti ={P0,P1,P2,P3};
    PuntiInterni = EstremiTraccia(Punti,PuntoRetta,tol,Tips);
    bool flag3 = false;
    if(PuntiInterni[0] == 0 && PuntiInterni[1] == 1 && Tips[0] == false && Tips[1] == true){
        flag3 = true;
    }
    EXPECT_TRUE(flag3);
}

