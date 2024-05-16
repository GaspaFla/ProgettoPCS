#pragma <once>
#include "../src/Utils.hpp"
#include"../src/GeometryLibrary.hpp"
#include "../src/Utils2.hpp"
#include"../src/PolygonalMesh.hpp"
#include <gtest/gtest.h>
#include"../src/Sort.hpp"
using namespace DFN;

//TEST ConverteInCelle
TEST(converteInCelle,LatiNonEsistenti){
    //Creo la frattura madre
    Vector3d x(2,2,0);
    Vector3d y(-2,2,0);
    Vector3d z(-2,-2,0);
    Vector3d k(2,-2,0);
    //QUADRATO CENTRO ZERO RAGGIO 1
    vector<Vector3d>Coord={x,y,z,k};
    Frattura FMadre=Frattura(0,4,Coord);
    //Setto gli id dei vertici  e aggiungo elementi alla mesh
    for(unsigned int i = 0;i<4;i++){
        unsigned int IdCell0D = FMadre.SottoPoligoni.NumberofCell0Ds;//inizializzato a 0
        FMadre.SottoPoligoni.IdCell0Ds.push_back(IdCell0D);
        FMadre.IdVertici.push_back(IdCell0D);
        FMadre.SottoPoligoni.CoordinatesCell0Ds.push_back(FMadre.CoordinateVertici[i]);
        FMadre.SottoPoligoni.NumberofCell0Ds ++;
    }
    //Creo frattura figlia
    Vector3d w(0,2,0);
    Vector3d h(0,-2,0);
    vector<Vector3d>Coord2={x,w,h,k};
    Frattura F =Frattura(4,Coord2);
    F.IdVertici.push_back(0);
    //Aggiungo i nuovi e li aggiungo anche alla mesh della madre
    F.IdVertici.push_back(4);
    FMadre.SottoPoligoni.IdCell0Ds.push_back(4);
    FMadre.SottoPoligoni.CoordinatesCell0Ds.push_back(w);
    FMadre.SottoPoligoni.NumberofCell0Ds ++;

    F.IdVertici.push_back(5);
    FMadre.SottoPoligoni.IdCell0Ds.push_back(5);
    FMadre.SottoPoligoni.CoordinatesCell0Ds.push_back(h);
    FMadre.SottoPoligoni.NumberofCell0Ds ++;
    F.IdVertici.push_back(3);

    converteInCelle(F,FMadre);
    //Controllo sono state salvate correttamente
    //Cell1D
    EXPECT_EQ(FMadre.SottoPoligoni.NumberofCell1Ds,4);
    for(unsigned int i = 0;i<4;i++){
        EXPECT_EQ(FMadre.SottoPoligoni.IdCell1Ds[i],i);
        if(i!=4){
            EXPECT_EQ(FMadre.SottoPoligoni.VerticesCell1Ds[i][0],F.IdVertici[i]);
            EXPECT_EQ(FMadre.SottoPoligoni.VerticesCell1Ds[i][1],F.IdVertici[i+1]);
        }
        else{
            EXPECT_EQ(FMadre.SottoPoligoni.VerticesCell1Ds[i][0],F.IdVertici[i]);
            EXPECT_EQ(FMadre.SottoPoligoni.VerticesCell1Ds[i][1],F.IdVertici[0]);
        }
    }


    //Cell2D
    EXPECT_EQ(FMadre.SottoPoligoni.IdCell2Ds[0],0);
    EXPECT_EQ(FMadre.SottoPoligoni.NumberofCell2Ds,1);
    EXPECT_EQ(FMadre.SottoPoligoni.VerticesCell2Ds[0][0],0);
    EXPECT_EQ(FMadre.SottoPoligoni.VerticesCell2Ds[0][1],4);
    EXPECT_EQ(FMadre.SottoPoligoni.VerticesCell2Ds[0][2],5);
    EXPECT_EQ(FMadre.SottoPoligoni.VerticesCell2Ds[0][3],3);

    EXPECT_EQ(FMadre.SottoPoligoni.EdgesCell2Ds[0][0],0);
    EXPECT_EQ(FMadre.SottoPoligoni.EdgesCell2Ds[0][1],1);
    EXPECT_EQ(FMadre.SottoPoligoni.EdgesCell2Ds[0][2],2);
    EXPECT_EQ(FMadre.SottoPoligoni.EdgesCell2Ds[0][3],3);


}

TEST(calcoloSottoPoligoniPass,UnaTracciaPassante){
    //Creo la frattura madre
    Vector3d x(2,2,0);
    Vector3d y(-2,2,0);
    Vector3d z(-2,-2,0);
    Vector3d k(2,-2,0);
    //QUADRATO CENTRO ZERO RAGGIO 1
    vector<Vector3d>Coord={x,y,z,k};
    Frattura FMadre=Frattura(0,4,Coord);
    //Setto gli id dei vertici  e aggiungo elementi alla mesh
    for(unsigned int i = 0;i<4;i++){
        unsigned int IdCell0D = FMadre.SottoPoligoni.NumberofCell0Ds;//inizializzato a 0
        FMadre.SottoPoligoni.IdCell0Ds.push_back(IdCell0D);
        FMadre.IdVertici.push_back(IdCell0D);
        FMadre.SottoPoligoni.CoordinatesCell0Ds.push_back(FMadre.CoordinateVertici[i]);
        FMadre.SottoPoligoni.NumberofCell0Ds ++;
    }
    //Faccio finta esiste un'altra frattura con id=2
    Vector3d w(0,2,0);
    Vector3d h(0,-2,0);
    array<Vector3d,2> VerticiT = {w,h};
    array<unsigned int,2> FrattureT = {0,1};
    array<bool,2> Tips = {false,false};
    Traccia T = Traccia(0,VerticiT,FrattureT,Tips);
    FMadre.TraccePass.push_back(0);
    vector<Traccia> Tracce ;
    Tracce.push_back(T);
    double tol = 0.0000000000000001;
    double tol2 = SetTolProdotto(tol);
    bool TracciaSulBordo = false;
    vector<Frattura> FrattureFiglie = calcoloSottoPoligoniPass(FMadre,tol,tol2,TracciaSulBordo,Tracce,FMadre);
    //Controllo il numero dei vertici
    EXPECT_EQ(FrattureFiglie[0].NumVertici,4);
    EXPECT_EQ(FrattureFiglie[1].NumVertici,4);
    //Controllo se i vertici sono giusti

        for(unsigned int i = 0;i<3;i++){
        EXPECT_DOUBLE_EQ(FrattureFiglie[0].CoordinateVertici[0][i],x[i]);
        EXPECT_DOUBLE_EQ(FrattureFiglie[0].CoordinateVertici[1][i],w[i]);
        EXPECT_DOUBLE_EQ(FrattureFiglie[0].CoordinateVertici[2][i],h[i]);
        EXPECT_DOUBLE_EQ(FrattureFiglie[0].CoordinateVertici[3][i],k[i]);

        EXPECT_DOUBLE_EQ(FrattureFiglie[1].CoordinateVertici[0][i],w[i]);
        EXPECT_DOUBLE_EQ(FrattureFiglie[1].CoordinateVertici[1][i],y[i]);
        EXPECT_DOUBLE_EQ(FrattureFiglie[1].CoordinateVertici[2][i],z[i]);
        EXPECT_DOUBLE_EQ(FrattureFiglie[1].CoordinateVertici[3][i],h[i]);
        }


    //Controllo se gli id dei vertici sono giusti
        EXPECT_EQ(FrattureFiglie[0].IdVertici[0],0);
        EXPECT_EQ(FrattureFiglie[0].IdVertici[1],4);
        EXPECT_EQ(FrattureFiglie[0].IdVertici[2],5);
        EXPECT_EQ(FrattureFiglie[0].IdVertici[3],3);

        EXPECT_EQ(FrattureFiglie[1].IdVertici[0],4);
        EXPECT_EQ(FrattureFiglie[1].IdVertici[1],1);
        EXPECT_EQ(FrattureFiglie[1].IdVertici[2],2);
        EXPECT_EQ(FrattureFiglie[1].IdVertici[3],5);
    //Controllo che la madre ha i nuovi vertici nella mesh
        EXPECT_EQ(FMadre.SottoPoligoni.IdCell0Ds[4],4);
        EXPECT_EQ(FMadre.SottoPoligoni.IdCell0Ds[5],5);
        for(unsigned int i = 0;i<3;i++){
            EXPECT_EQ(FMadre.SottoPoligoni.CoordinatesCell0Ds[4][i],w[i]);
            EXPECT_EQ(FMadre.SottoPoligoni.CoordinatesCell0Ds[5][i],h[i]);
        }

}
