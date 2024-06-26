#pragma <once>
#include "../src/Utils.hpp"
#include"../src/GeometryLibrary.hpp"
#include <gtest/gtest.h>
#include"../src/Sort.hpp"
using namespace DFN;


TEST(TESTIMPORTOFRATTURE,FileAggiuntivo){
    double tol = 0.00000000000000001;
    vector<Frattura> Fratture;
    string fileName = "./DFN/FileTestImporto.txt";
    bool flag = importoFratture(fileName, Fratture, tol);
    EXPECT_TRUE(flag);
    EXPECT_EQ(Fratture[0].IdFrattura,0);
    EXPECT_EQ(Fratture[0].NumVertici,4);
    Vector3d x(0,0,-1);
    Vector3d y(0,1,0);
    Vector3d z(0,-1,0);
    Vector3d k(0,0,1);
    for(unsigned int i = 0;i<3;i++){
        EXPECT_DOUBLE_EQ(Fratture[0].CoordinateVertici[0][i],x[i]);
        EXPECT_DOUBLE_EQ(Fratture[0].CoordinateVertici[1][i],y[i]);
        EXPECT_DOUBLE_EQ(Fratture[0].CoordinateVertici[2][i],z[i]);
        EXPECT_DOUBLE_EQ(Fratture[0].CoordinateVertici[3][i],k[i]);
    }
}


TEST(TESTLUNGHEZZALATI,LunghezzaNonZero){
    Vector3d p1(4,0,-1);
    Vector3d p2(4,0,1);
    Vector3d p3(2,0,5);
    vector<Vector3d> CoordinateVertici = {p1,p2,p3};
    double tol = 0.00000000000000001;
    bool flag =  testLunghezzaLati( CoordinateVertici,  tol);
    EXPECT_TRUE(flag);
}
TEST(TESTLUNGHEZZALATI,LunghezzaZero){
    Vector3d p1(4,0,1);
    Vector3d p2(4,0,1);
    Vector3d p3(2,0,5);
    vector<Vector3d> CoordinateVertici = {p1,p2,p3};
    double tol = 0.00000000000000001;
    bool flag =  testLunghezzaLati( CoordinateVertici,  tol);
    EXPECT_FALSE(flag);
}


//TEST Centroide

TEST(TESTCENTROIDE, DueTriangoliDistanti)
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
    bool flag =ControlloCentroide(F1,F2);
    int control=1;
    if(flag){
        control=0;
    }

    EXPECT_EQ(1, control);
}

TEST(TESTCENTROIDE, Quadratimoltovicini)
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
    bool flag=ControlloCentroide(F1,F2);
    int control=1;
    if(flag){
        control=0;
    }

    EXPECT_EQ(0, control);
}


//TEST SiIntersecano

TEST(TESTINTERSEZIONE, QuadratiPerpendicolariPerOrigine)
{
    Vector3d x(0,1,-1);
    Vector3d y(0,1,1);
    Vector3d z(0,-1,1);
    Vector3d k(0,-1,-1);

    vector<Vector3d>Coord={x,y,z,k};
    Frattura F1=Frattura(0,4,Coord);
    Vector3d x1(1,0,-1);
    Vector3d y1(1,0,1);
    Vector3d z1(-1,0,1);
    Vector3d k1(-1,0,-1);
    vector<Vector3d> Coord1={x1,y1,z1,k1};
    Frattura F2=Frattura(0,4,Coord1);
    array<Vector3d,4> PuntiIntersecatiF1;
    array<Vector3d,4> PuntiIntersecatiF2;
    double tol = 0.0000000000000001;
    bool b1=false;
    bool b2=false;
    bool flag=SiIntersecano(F1,F2, PuntiIntersecatiF1,tol,b1);
    bool flag1=SiIntersecano(F2,F1, PuntiIntersecatiF2,tol,b2);

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

TEST(TESTINTERSEZIONE, QuadratiPerpendicolariNonPerOrigine)
{

    Vector3d x(0,6,-1);
    Vector3d y(0,6,1);
    Vector3d z(0,4,1);
    Vector3d k(0,4,-1);

    vector<Vector3d>Coord={x,y,z,k};
    Frattura F1=Frattura(0,4,Coord);
    Vector3d x1(1,5,-1);
    Vector3d y1(1,5,1);
    Vector3d z1(-1,5,1);
    Vector3d k1(-1,5,-1);
    vector<Vector3d> Coord1={x1,y1,z1,k1};
    Frattura F2=Frattura(0,4,Coord1);
    array<Vector3d,4> PuntiIntersecatiF1;
    array<Vector3d,4> PuntiIntersecatiF2;
    double tol = 0.0000000000000001;
    bool b1=false;
    bool b2=false;
    bool flag=SiIntersecano(F1,F2, PuntiIntersecatiF1,tol,b1);
    bool flag1=SiIntersecano(F2,F1, PuntiIntersecatiF2,tol,b2);




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

TEST(TESTINTERSEZIONE, QuadratiViciniCheNonSiIntersecano)
{
    Vector3d x(0,1,-1);
    Vector3d y(0,1,1);
    Vector3d z(0,-1,1);
    Vector3d k(0,-1,-1);

    vector<Vector3d>Coord={x,y,z,k};
    Frattura F1=Frattura(0,4,Coord);
    Vector3d x1(1,1,-1);
    Vector3d y1(1,1,1);
    Vector3d z1(0.1,-1,1);
    Vector3d k1(0.1,-1,-1);
    vector<Vector3d> Coord1={x1,y1,z1,k1};
    Frattura F2=Frattura(0,4,Coord1);
    array<Vector3d,4> PuntiIntersecatiF1;
    array<Vector3d,4> PuntiIntersecatiF2;
    double tol = 0.0000000000000001;
    bool b1=false;
    bool b2=false;
    bool flag=SiIntersecano(F1,F2, PuntiIntersecatiF1,tol,b1);
    bool flag1=SiIntersecano(F2,F1, PuntiIntersecatiF2,tol,b2);

    int control=1;
    if(flag && flag1){
        control=0;
    }

    EXPECT_EQ(1, control);


}

TEST(TESTINTERSEZIONE, QuadratiPerpendicolariPerOrigineNonPassante)
{

    Vector3d x(0,1,-1);
    Vector3d y(0,1,1);
    Vector3d z(0,-1,1);
    Vector3d k(0,-1,-1);

    vector<Vector3d>Coord={x,y,z,k};
    Frattura F1=Frattura(0,4,Coord);
    Vector3d x1(2,0,-3);
    Vector3d y1(2,0,3);
    Vector3d z1(-2,0,3);
    Vector3d k1(-2,0,-3);
    vector<Vector3d> Coord1={x1,y1,z1,k1};
    Frattura F2=Frattura(0,4,Coord1);
    array<Vector3d,4> PuntiIntersecatiF1;
    array<Vector3d,4> PuntiIntersecatiF2;
    double tol = 0.0000000000000001;
    bool b1=false;
    bool b2=false;
    bool flag=SiIntersecano(F1,F2, PuntiIntersecatiF1,tol,b1);
    bool flag1=SiIntersecano(F2,F1, PuntiIntersecatiF2,tol,b2);

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

TEST(TESTINTERSEZIONE,TriangoloSuPiano )
{
    Vector3d x(2,2,0);
    Vector3d y(-2,2,0);
    Vector3d z(-2,-2,0);
    Vector3d k(2,-2,-0);
    vector<Vector3d>Coord={x,y,z,k};
    Frattura F1=Frattura(0,4,Coord);
    Vector3d x1(1,0,0);
    Vector3d y1(0,0,3);
    Vector3d z1(-1,0,0);
    vector<Vector3d> Coord1={x1,y1,z1};
    Frattura F2=Frattura(0,3,Coord1);
    array<Vector3d,4> PuntiIntersecatiF1;
    array<Vector3d,4> PuntiIntersecatiF2;
    double tol = 0.0000000000000001;
    bool b1=false;
    bool b2=false;
    bool flag = SiIntersecano(F1,F2, PuntiIntersecatiF1,tol,b1);
    bool flag1=SiIntersecano(F2,F1, PuntiIntersecatiF2,tol,b2);
    EXPECT_TRUE(flag);
    EXPECT_TRUE(flag1);

    for(int i=0; i<3; i++){
        EXPECT_DOUBLE_EQ(PuntiIntersecatiF2[0][i],x1[i]);
        EXPECT_DOUBLE_EQ(PuntiIntersecatiF2[1][i],y1[i]);
        EXPECT_DOUBLE_EQ(PuntiIntersecatiF2[2][i],z1[i]);
        EXPECT_DOUBLE_EQ(PuntiIntersecatiF2[3][i],y1[i]);
    }

}

TEST(TESTINTERSEZIONE,ToccantePerEntrambi )
{
    Vector3d x(2,2,0);
    Vector3d y(-2,2,0);
    Vector3d z(-2,-2,0);
    Vector3d k(2,-2,-0);

    vector<Vector3d>Coord={x,y,z,k};
    Frattura F1=Frattura(0,4,Coord);
    Vector3d x1(2,2,0);
    Vector3d y1(-2,2,0);
    Vector3d z1(-2,2,3);
    Vector3d k1(2,2,3);
    vector<Vector3d> Coord1={x1,y1,z1,k1};
    Frattura F2=Frattura(0,4,Coord1);
    array<Vector3d,4> PuntiIntersecatiF1;
    array<Vector3d,4> PuntiIntersecatiF2;
    double tol = 0.0000000000000001;
    bool b1=false;
    bool b2=false;
    bool flag = SiIntersecano(F1,F2, PuntiIntersecatiF1,tol, b1);
    bool flag1=SiIntersecano(F2,F1, PuntiIntersecatiF2,tol,b2);
    EXPECT_TRUE(flag);
    EXPECT_TRUE(flag1);

    for(int i=0; i<3; i++){
        EXPECT_DOUBLE_EQ(PuntiIntersecatiF2[0][i],k1[i]);
        EXPECT_DOUBLE_EQ(PuntiIntersecatiF2[1][i],x1[i]);
        EXPECT_DOUBLE_EQ(PuntiIntersecatiF2[2][i],y1[i]);
        EXPECT_DOUBLE_EQ(PuntiIntersecatiF2[3][i],z1[i]);
    }

}

TEST(TESTINTERSEZIONE,ToccaSoloUnPunto )
{
    Vector3d x(2,2,0);
    Vector3d y(-2,2,0);
    Vector3d z(-2,-2,0);
    Vector3d k(2,-2,-0);
    //QUADRATO CENTRO ZERO RAGGIO 1
    vector<Vector3d>Coord={x,y,z,k};
    Frattura F1=Frattura(0,4,Coord);
    Vector3d x1(0,0,0);
    Vector3d y1(0,1,1);
    Vector3d z1(0,-1,1);
    vector<Vector3d> Coord1={x1,y1,z1};
    Frattura F2=Frattura(0,3,Coord1);
    array<Vector3d,4> PuntiIntersecatiF1;
    array<Vector3d,4> PuntiIntersecatiF2;
    double tol = 0.0000000000000001;
    bool b1=false;
    bool b2=false;
    bool flag = SiIntersecano(F1,F2, PuntiIntersecatiF1,tol, b1);
    bool flag1=SiIntersecano(F2,F1, PuntiIntersecatiF2,tol, b2);
    EXPECT_TRUE(flag);
    EXPECT_FALSE(flag1);

    for(int i=0; i<3; i++){
        EXPECT_DOUBLE_EQ(PuntiIntersecatiF1[0][i],y[i]);
        EXPECT_DOUBLE_EQ(PuntiIntersecatiF1[1][i],x[i]);
        EXPECT_DOUBLE_EQ(PuntiIntersecatiF1[2][i],k[i]);
        EXPECT_DOUBLE_EQ(PuntiIntersecatiF1[3][i],z[i]);
    }
    //permuto tutto
    Coord1={z1,x1,y1};
    F2=Frattura(0,3,Coord1);
    b1=false;
    b2=false;
    flag = SiIntersecano(F1,F2, PuntiIntersecatiF1,tol,b1);
    flag1=SiIntersecano(F2,F1, PuntiIntersecatiF2,tol,b2);
    EXPECT_TRUE(flag);
    EXPECT_FALSE(flag1);

    for(int i=0; i<3; i++){
        EXPECT_DOUBLE_EQ(PuntiIntersecatiF1[0][i],y[i]);
        EXPECT_DOUBLE_EQ(PuntiIntersecatiF1[1][i],x[i]);
        EXPECT_DOUBLE_EQ(PuntiIntersecatiF1[2][i],k[i]);
        EXPECT_DOUBLE_EQ(PuntiIntersecatiF1[3][i],z[i]);
    }
    // permuto di nuovo

    Coord1={y1,z1,x1};
    F2=Frattura(0,3,Coord1);
    b1=false;
    b2=false;
    flag = SiIntersecano(F1,F2, PuntiIntersecatiF1,tol,b1);
    flag1=SiIntersecano(F2,F1, PuntiIntersecatiF2,tol,b2);
    EXPECT_TRUE(flag);
    EXPECT_FALSE(flag1);

    for(int i=0; i<3; i++){
        EXPECT_DOUBLE_EQ(PuntiIntersecatiF1[0][i],y[i]);
        EXPECT_DOUBLE_EQ(PuntiIntersecatiF1[1][i],x[i]);
        EXPECT_DOUBLE_EQ(PuntiIntersecatiF1[2][i],k[i]);
        EXPECT_DOUBLE_EQ(PuntiIntersecatiF1[3][i],z[i]);
    }






}

TEST(TESTINTERSEZIONE,TriangoloAttraverso)
{
    Vector3d x(2,2,0);
    Vector3d y(-2,2,0);
    Vector3d z(-2,-2,0);
    Vector3d k(2,-2,-0);

    vector<Vector3d>Coord={x,y,z,k};
    Frattura F1=Frattura(0,4,Coord);
    Vector3d x1(0,1,-1);
    Vector3d y1(0,1,1);
    Vector3d z1(0,-2,0);
    vector<Vector3d> Coord1={x1,y1,z1};
    Frattura F2=Frattura(0,3,Coord1);
    array<Vector3d,4> PuntiIntersecatiF1;
    array<Vector3d,4> PuntiIntersecatiF2;
    double tol = 0.0000000000000001;
    bool b1=false;
    bool b2=false;
    bool flag = SiIntersecano(F1,F2, PuntiIntersecatiF1,tol,b1);
    bool flag1=SiIntersecano(F2,F1, PuntiIntersecatiF2,tol,b2);
    EXPECT_TRUE(flag);
    EXPECT_TRUE(flag1);

    for(int i=0; i<3; i++){
        EXPECT_DOUBLE_EQ(PuntiIntersecatiF2[0][i],y1[i]);
        EXPECT_DOUBLE_EQ(PuntiIntersecatiF2[1][i],x1[i]);
        EXPECT_DOUBLE_EQ(PuntiIntersecatiF2[2][i],z1[i]);
        EXPECT_DOUBLE_EQ(PuntiIntersecatiF2[3][i],y1[i]);
    }

}

TEST(TESTINTERSEZIONE,RomboAttraverso)
{
    Vector3d x(2,2,0);
    Vector3d y(-2,2,0);
    Vector3d z(-2,-2,0);
    Vector3d k(2,-2,-0);

    vector<Vector3d>Coord={x,y,z,k};
    Frattura F1=Frattura(0,4,Coord);
    Vector3d x1(0,0,-4);
    Vector3d y1(0,-4,0);
    Vector3d z1(0,0,4);
    Vector3d k1(0,4,0);
    vector<Vector3d> Coord1={x1,y1,z1,k1};
    Frattura F2=Frattura(0,4,Coord1);
    array<Vector3d,4> PuntiIntersecatiF1;
    array<Vector3d,4> PuntiIntersecatiF2;
    double tol = 0.0000000000000001;
    bool b1=false;
    bool b2=false;
    bool flag = SiIntersecano(F1,F2, PuntiIntersecatiF1,tol,b1);
    bool flag1=SiIntersecano(F2,F1, PuntiIntersecatiF2,tol,b2);
    EXPECT_TRUE(flag);
    EXPECT_TRUE(flag1);

    for(int i=0; i<3; i++){
        EXPECT_DOUBLE_EQ(PuntiIntersecatiF2[0][i],z1[i]);
        EXPECT_DOUBLE_EQ(PuntiIntersecatiF2[1][i],y1[i]);
        EXPECT_DOUBLE_EQ(PuntiIntersecatiF2[2][i],k1[i]);
        EXPECT_DOUBLE_EQ(PuntiIntersecatiF2[3][i],z1[i]);
    }

}


//TEST IncontroTraRette
TEST(TESTINCONTRORETTE,TracciaPoligono)
{
    Vector3d direzionedeiLati(2,-1,0);
    Vector3d VerticePoligono(1,2,3);
    Vector3d direzioneRetta(1,1,-1);
    Vector3d puntoRetta(0,1,2);
    Vector3d puntoIntersezione = IncontroTraRette( direzionedeiLati, VerticePoligono,direzioneRetta , puntoRetta );
    EXPECT_DOUBLE_EQ(puntoIntersezione[0],0.714285714285714);
    EXPECT_DOUBLE_EQ(puntoIntersezione[1],2.142857142857143);
    EXPECT_DOUBLE_EQ(puntoIntersezione[2], 3.000000000000000);
}


//TEST EstremiTraccia
TEST(TESTESTREMITRACCIA, PuntiAllineati)
{
    Vector3d PuntoRetta(5,0,0) ;
    double tol = 0.000000000000001;
    array<bool,2> Tips;
    double tol2=SetTolProdotto(tol);

    //Caso solo due punti coincidenti ==> tre punti distinti
    Vector3d P0(4,0,0);
    Vector3d P1(1,0,0);
    Vector3d P2(4,0,0);
    Vector3d P3(2,0,0);
    array<Vector3d,4> Punti ={P0,P1,P2,P3};
    bool fintaIntersezione = false;
    array<unsigned int,2> PuntiInterni = EstremiTraccia(Punti,tol,Tips,fintaIntersezione, tol2);
    bool flag1 = false;
    if(PuntiInterni[0] == 0 && PuntiInterni[1] == 3 && Tips[0] == true && Tips[1] == false){
        flag1 = true;
    }
    EXPECT_TRUE(flag1);

    //Caso tutti punti distinti
    //Sottocaso non passante per entrambi
    P0 = {1,0,0};
    P1 = {4,0,0};
    P2 = {2,0,0};
    P3 = {5,0,0};
    Punti ={P0,P1,P2,P3};
    PuntiInterni = EstremiTraccia(Punti,tol,Tips,fintaIntersezione, tol2);
    bool flag2 = false;
    if(PuntiInterni[0] == 1 && PuntiInterni[1] == 2 && Tips[0] == true && Tips[1] == true && fintaIntersezione == false){
        flag2 = true;
    }
    EXPECT_TRUE(flag2);

    //Sottocaso passante per uno e non passante per l'altro
    P0 = {2,0,0};
    P1 = {3,0,0};
    P2 = {1,0,0};
    P3 = {5,0,0};
    Punti ={P0,P1,P2,P3};
    PuntiInterni = EstremiTraccia(Punti,tol,Tips,fintaIntersezione,tol2);
    bool flag3 = false;
    if(PuntiInterni[0] == 0 && PuntiInterni[1] == 1 && Tips[0] == false && Tips[1] == true && fintaIntersezione == false){
        flag3 = true;
    }
    EXPECT_TRUE(flag3);


    //Caso finta intersezione
    P0 = {2,0,0};
    P1 = {3,0,0};
    P2 = {4,0,0};
    P3 = {5,0,0};
    Punti ={P0,P1,P2,P3};
    PuntiInterni = EstremiTraccia(Punti,tol,Tips,fintaIntersezione,tol2);
    bool flag4 = false;
    if(fintaIntersezione){
        flag4 = true;
    }
    EXPECT_TRUE(flag4);


}


//TEST CalcoloTracce
TEST(TESTCALCOLOTRACCE,NonPassEntrambe)
{   Vector3d p1(1,1,0);
    Vector3d p2(-1,1,0);
    Vector3d p3(-1,-1,0);
    Vector3d p4(1,-1,0);

    Vector3d p5(10,0,-1);
    Vector3d p6(10,0,1);
    Vector3d p7(0,0,0);

    vector<Vector3d> CoordinateV1 = {p1,p2,p3,p4};
    vector<Vector3d> CoordinateV2 = {p5,p6,p7};
    Frattura F1(0,4,CoordinateV1);
    Frattura F2(1,3,CoordinateV2);
    unsigned int IdTraccia = 0;
    array<Vector3d,4> puntiFrattura1;
    array<Vector3d,4> puntiFrattura2;
    double tol = 0.0000000000000001;
    bool b1=false;
    bool b2=false;
    double tol2=SetTolProdotto(tol);
    SiIntersecano(F1,F2,puntiFrattura1,tol,b1);
    SiIntersecano(F2,F1,puntiFrattura2,tol,b2);
    Traccia T;
    bool flag1 = CalcoloTracce(F1, F2, IdTraccia, tol, puntiFrattura1, puntiFrattura2, T, b1,b2, tol2);

    EXPECT_TRUE(flag1);

    Vector3d EstremoCorretto1(1,0,0);
    Vector3d EstremoCorretto2(0,0,0);

    EXPECT_TRUE(flag1);
    for(unsigned int i =0;i<3;i++){
        EXPECT_DOUBLE_EQ(T.VerticiTraccia[0][i],EstremoCorretto1[i]);
        EXPECT_DOUBLE_EQ(T.VerticiTraccia[1][i],EstremoCorretto2[i]);
    }
    EXPECT_TRUE(T.Tips[0]);
    EXPECT_TRUE(T.Tips[1]);
}

TEST(TESTCALCOLOTRACCE,FintaTraccia)
{   Vector3d p1(1,1,0);
    Vector3d p2(-1,1,0);
    Vector3d p3(-1,-1,0);
    Vector3d p4(1,-1,0);
    Vector3d p5(2,0,-1);
    Vector3d p6(4,0,-1);
    Vector3d p7(4,0,1);
    Vector3d p8(2,0,1);

    vector<Vector3d> CoordinateV1 = {p1,p2,p3,p4};
    vector<Vector3d> CoordinateV2 = {p5,p6,p7,p8};
    unsigned int NumVertici = 4;
    Frattura F1(0,NumVertici,CoordinateV1);
    Frattura F2(1,NumVertici,CoordinateV2);
    unsigned int IdTraccia = 0;
    array<Vector3d,4> puntiFrattura1;
    array<Vector3d,4> puntiFrattura2;
    double tol = 0.0000000000001;
    double tol2=SetTolProdotto(tol);
    bool b1=false;
    bool b2=false;
    SiIntersecano(F1,F2,puntiFrattura1,tol,b1);
    SiIntersecano(F2,F1,puntiFrattura2,tol,b2);

    Traccia T;
    bool flag1 = CalcoloTracce(F1, F2, IdTraccia, tol, puntiFrattura1, puntiFrattura2, T,b1,b2, tol2);

    EXPECT_FALSE(flag1);
}

TEST(TESTCALCOLOTRACCE,PassantePerEntrambe){
    Vector3d p1(1,1,0);
    Vector3d p2(-1,1,0);
    Vector3d p3(-1,-1,0);
    Vector3d p4(1,-1,0);

    Vector3d p5(0,1,2);
    Vector3d p6(0,1,-2);
    Vector3d p7(0,-1,-2);
    Vector3d p8(0,-1,2);

    vector<Vector3d> CoordinateV1 = {p1,p2,p3,p4};
    vector<Vector3d> CoordinateV2 = {p5,p6,p7,p8};
    unsigned int NumVertici = 4;
    Frattura F1(0,NumVertici,CoordinateV1);
    Frattura F2(1,NumVertici,CoordinateV2);
    unsigned int IdTraccia = 0;
    array<Vector3d,4> puntiFrattura1;
    array<Vector3d,4> puntiFrattura2;
    double tol = 0.0000000000001;
    double tol2=SetTolProdotto(tol);
    bool b1=false;
    bool b2=false;
    SiIntersecano(F1,F2,puntiFrattura1,tol,b1);
    SiIntersecano(F2,F1,puntiFrattura2,tol,b2);

    Traccia T;
    bool flag1 = CalcoloTracce(F1, F2, IdTraccia, tol, puntiFrattura1, puntiFrattura2, T,b1,b2, tol2);


    Vector3d EstremoCorretto1(0,1,0);
    Vector3d EstremoCorretto2(0,-1,0);

    EXPECT_TRUE(flag1);
    for(unsigned int i =0;i<3;i++){
        EXPECT_DOUBLE_EQ(T.VerticiTraccia[0][i],EstremoCorretto1[i]);
        EXPECT_DOUBLE_EQ(T.VerticiTraccia[1][i],EstremoCorretto2[i]);
    }

    EXPECT_FALSE(T.Tips[0]);
    EXPECT_FALSE(T.Tips[1]);

}

TEST(TESTCALCOLOTRACCE,TracciaSulLato)
{
    Vector3d x(2,2,0);
    Vector3d y(-2,2,0);
    Vector3d z(-2,-2,0);
    Vector3d k(2,-2,-0);
    vector<Vector3d>CoordinateV1={x,y,z,k};
    Frattura F1=Frattura(0,4,CoordinateV1);
    Vector3d x1(2,2,0);
    Vector3d y1(-2,2,0);
    Vector3d z1(-2,2,3);
    Vector3d k1(2,2,3);
    vector<Vector3d> CoordinateV2={x1,y1,z1,k1};
    Frattura F2=Frattura(0,4,CoordinateV2);

    unsigned int IdTraccia = 0;
    array<Vector3d,4> puntiFrattura1;
    array<Vector3d,4> puntiFrattura2;
    double tol = 0.0000000000000001;
    double tol2=SetTolProdotto(tol);
    bool b1=false;
    bool b2=false;
    SiIntersecano(F1,F2,puntiFrattura1,tol,b1);
    SiIntersecano(F2,F1,puntiFrattura2,tol,b2);
    Traccia T;
    bool flag1 = CalcoloTracce(F1, F2, IdTraccia, tol, puntiFrattura1, puntiFrattura2, T, b1,b2, tol2);

    EXPECT_TRUE(flag1);
    for(unsigned int i =0;i<3;i++){
        EXPECT_DOUBLE_EQ(T.VerticiTraccia[0][i],x[i]);
        EXPECT_DOUBLE_EQ(T.VerticiTraccia[1][i],y[i]);
    }
    EXPECT_FALSE(T.Tips[0]);
    EXPECT_FALSE(T.Tips[0]);

}

TEST(TESTCALCOLOTRACCE,RomboAttraverso)
{
    Vector3d x(2,2,0);
    Vector3d y(-2,2,0);
    Vector3d z(-2,-2,0);
    Vector3d k(2,-2,-0);
    vector<Vector3d>Coord={x,y,z,k};
    Frattura F1=Frattura(0,4,Coord);
    //ROMBO
    Vector3d x1(0,0,-4);
    Vector3d y1(0,4,0);
    Vector3d z1(0,0,4);
    Vector3d k1(0,-4,0);
    vector<Vector3d> Coord1={x1,y1,z1,k1};
    Frattura F2=Frattura(0,4,Coord1);

    unsigned int IdTraccia = 0;
    array<Vector3d,4> puntiFrattura1;
    array<Vector3d,4> puntiFrattura2;
    double tol = 0.0000000000000001;
    double tol2=SetTolProdotto(tol);
    bool b1=false;
    bool b2=false;
    SiIntersecano(F1,F2,puntiFrattura1,tol,b1);
    SiIntersecano(F2,F1,puntiFrattura2,tol,b2);
    Traccia T;
    bool flag1 = CalcoloTracce(F1, F2, IdTraccia, tol, puntiFrattura1, puntiFrattura2, T, b1,b2, tol2);

    Vector3d EstremoCorretto1(0,2,0);
    Vector3d EstremoCorretto2(0,-2,0);

    EXPECT_TRUE(flag1);
    for(unsigned int i =0;i<3;i++){
        EXPECT_DOUBLE_EQ(T.VerticiTraccia[0][i],EstremoCorretto1[i]);
        EXPECT_DOUBLE_EQ(T.VerticiTraccia[1][i],EstremoCorretto2[i]);
    }
    EXPECT_FALSE(T.Tips[0]);
    EXPECT_TRUE(T.Tips[1]);

}

TEST(TESTCALCOLOTRACCE,TriangoloToccante)
{
    Vector3d x(2,2,0);
    Vector3d y(-2,2,0);
    Vector3d z(-2,-2,0);
    Vector3d k(2,-2,0);
    vector<Vector3d>Coord={x,y,z,k};
    Frattura F1=Frattura(0,4,Coord);
    //TRIANGOLO
    Vector3d x1(2,0,0);
    Vector3d y1(2,0,3);
    Vector3d z1(0,2,0);
    vector<Vector3d> Coord1={x1,y1,z1};
    Frattura F2=Frattura(0,3,Coord1);

    unsigned int IdTraccia = 0;
    array<Vector3d,4> puntiFrattura1;
    array<Vector3d,4> puntiFrattura2;
    double tol = 0.0000000000000001;
    double tol2=SetTolProdotto(tol);
    bool b1=false;
    bool b2=false;
    SiIntersecano(F1,F2,puntiFrattura1,tol,b1);
    SiIntersecano(F2,F1,puntiFrattura2,tol,b2);
    Traccia T;
    bool flag1 = CalcoloTracce(F1, F2, IdTraccia, tol, puntiFrattura1, puntiFrattura2, T, b1,b2, tol2);

    Vector3d EstremoCorretto1(0,2,0);
    Vector3d EstremoCorretto2(2,0,0);

    EXPECT_TRUE(flag1);
    for(unsigned int i =0;i<3;i++){
        EXPECT_DOUBLE_EQ(T.VerticiTraccia[0][i],EstremoCorretto1[i]);
        EXPECT_DOUBLE_EQ(T.VerticiTraccia[1][i],EstremoCorretto2[i]);
    }
    EXPECT_FALSE(T.Tips[0]);
    EXPECT_FALSE(T.Tips[1]);

}

TEST(TESTCALCOLOTRACCE,RettangoloAttraversoToccante)
{
    Vector3d x(2,2,0);
    Vector3d y(-2,2,0);
    Vector3d z(-2,-2,0);
    Vector3d k(2,-2,-0);
    vector<Vector3d>Coord={x,y,z,k};
    Frattura F1=Frattura(0,4,Coord);
    Vector3d x1(2,2,-3);
    Vector3d y1(-2,2,-3);
    Vector3d z1(-2,2,3);
    Vector3d k1(2,2,3);
    vector<Vector3d> Coord1={x1,y1,z1,k1};
    Frattura F2=Frattura(0,4,Coord1);


    unsigned int IdTraccia = 0;
    array<Vector3d,4> puntiFrattura1;
    array<Vector3d,4> puntiFrattura2;
    double tol = 0.0000000000000001;
    double tol2=SetTolProdotto(tol);
    bool b1=false;
    bool b2=false;
    SiIntersecano(F1,F2,puntiFrattura1,tol,b1);
    SiIntersecano(F2,F1,puntiFrattura2,tol,b2);
    Traccia T;
    bool flag1 = CalcoloTracce(F1, F2, IdTraccia, tol, puntiFrattura1, puntiFrattura2, T, b1,b2, tol2);


    EXPECT_TRUE(flag1);
    for(unsigned int i =0;i<3;i++){
        EXPECT_DOUBLE_EQ(T.VerticiTraccia[0][i],x[i]);
        EXPECT_DOUBLE_EQ(T.VerticiTraccia[1][i],y[i]);
    }
    EXPECT_FALSE(T.Tips[0]);
    EXPECT_FALSE(T.Tips[1]);

}

TEST(TESTCALCOLOTRACCE,PassantePerUno){
    Vector3d p1(1,1,0);
    Vector3d p2(-1,1,0);
    Vector3d p3(-1,-1,0);
    Vector3d p4(1,-1,0);

    Vector3d p5(0,2,2);
    Vector3d p6(0,2,-2);
    Vector3d p7(0,-2,-2);
    Vector3d p8(0,-2,2);

    vector<Vector3d> CoordinateV1 = {p1,p2,p3,p4};
    vector<Vector3d> CoordinateV2 = {p5,p6,p7,p8};
    unsigned int NumVertici = 4;
    Frattura F1(0,NumVertici,CoordinateV1);
    Frattura F2(1,NumVertici,CoordinateV2);
    unsigned int IdTraccia = 0;
    array<Vector3d,4> puntiFrattura1;
    array<Vector3d,4> puntiFrattura2;
    double tol = 0.0000000000001;
    double tol2=SetTolProdotto(tol);
    bool b1=false;
    bool b2=false;
    SiIntersecano(F1,F2,puntiFrattura1,tol,b1);
    SiIntersecano(F2,F1,puntiFrattura2,tol,b2);

    Traccia T;
    bool flag1 = CalcoloTracce(F1, F2, IdTraccia, tol, puntiFrattura1, puntiFrattura2, T,b1,b2, tol2);

    EXPECT_TRUE(flag1);
    EXPECT_FALSE(T.Tips[0]);
    EXPECT_TRUE(T.Tips[1]);
    Vector3d EstremoCorretto1(0,1,0);
    Vector3d EstremoCorretto2(0,-1,0);

    EXPECT_TRUE(flag1);
    for(unsigned int i =0;i<3;i++){
        EXPECT_DOUBLE_EQ(T.VerticiTraccia[0][i],EstremoCorretto1[i]);
        EXPECT_DOUBLE_EQ(T.VerticiTraccia[1][i],EstremoCorretto2[i]);
    }

}

//TEST MergeSort
TEST(TESTMERGESORT,VettoriDiversi){
    Vector3d x(0,0,1);
    Vector3d y(0,0,5);
    Vector3d x1(3,0,0);
    Vector3d y1(3.5,0,0);
    Vector3d x2(2,0,0);
    Vector3d y2(3,0,0);
    array<unsigned int,2> F={1,1};
    array<bool,2> T={true, true};
    array<Vector3d,2>z={x,y};
    array<Vector3d,2>z1={x1,y1};
    array<Vector3d,2>z2={x2,y2};

    vector<Traccia>t;
    t.resize(3);
    t[0]=Traccia(0,z, F,T);
    t[1]=Traccia(1,z1,F,T);
    t[2]=Traccia(2,z2,F,T);
    vector<unsigned int> vId={1,2,0};
    MergeSort(t,vId);
    EXPECT_EQ(vId[0],0);
    EXPECT_EQ(vId[1],2);
    EXPECT_EQ(vId[2],1);
}

TEST(TESTMERGESORT,VettoriConUgualLunghezza){
    Vector3d x(0,0,1);
    Vector3d y(0,0,5);
    Vector3d x1(3,0,0);
    Vector3d y1(4,0,0);
    Vector3d x2(2,0,0);
    Vector3d y2(3,0,0);

    array<unsigned int,2> F={1,1};
    array<bool,2> T={true, true};
    array<Vector3d,2>z={x,y};
    array<Vector3d,2>z1={x1,y1};
    array<Vector3d,2>z2={x2,y2};

    vector<Traccia>t;
    t.resize(3);
    t[0]=Traccia(0,z, F,T);
    t[1]=Traccia(1,z1,F,T);
    t[2]=Traccia(2,z2,F,T);

    vector<unsigned int> vId={1,2,0};

    MergeSort(t,vId);
    EXPECT_EQ(vId[0],0);
    EXPECT_EQ(vId[1],1);
    EXPECT_EQ(vId[2],2);
}
