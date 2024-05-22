#pragma <once>
#include "../src/Utils.hpp"
#include"../src/GeometryLibrary.hpp"
#include "../src/Utils2.hpp"
#include"../src/PolygonalMesh.hpp"
#include <gtest/gtest.h>
#include"../src/Sort.hpp"
using namespace DFN;

//TEST ConverteInCelle
TEST(converteInCelle,LatiEsistenti){
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
    Frattura F1 =Frattura(4,Coord2);
    F1.IdVertici.push_back(0);
    //Aggiungo i nuovi e li aggiungo anche alla mesh della madre
    F1.IdVertici.push_back(4);
    FMadre.SottoPoligoni.IdCell0Ds.push_back(4);
    FMadre.SottoPoligoni.CoordinatesCell0Ds.push_back(w);
    FMadre.SottoPoligoni.NumberofCell0Ds ++;

    F1.IdVertici.push_back(5);
    FMadre.SottoPoligoni.IdCell0Ds.push_back(5);
    FMadre.SottoPoligoni.CoordinatesCell0Ds.push_back(h);
    FMadre.SottoPoligoni.NumberofCell0Ds ++;
    F1.IdVertici.push_back(3);

    converteInCelle(F1,FMadre);
    //Controllo sono state salvate correttamente
    //Cell1D
    EXPECT_EQ(FMadre.SottoPoligoni.NumberofCell1Ds,4);
    for(unsigned int i = 0;i<4;i++){
        EXPECT_EQ(FMadre.SottoPoligoni.IdCell1Ds[i],i);
        if(i!=4){
            EXPECT_EQ(FMadre.SottoPoligoni.VerticesCell1Ds[i][0],F1.IdVertici[i]);
            EXPECT_EQ(FMadre.SottoPoligoni.VerticesCell1Ds[i][1],F1.IdVertici[i+1]);
        }
        else{
            EXPECT_EQ(FMadre.SottoPoligoni.VerticesCell1Ds[i][0],F1.IdVertici[i]);
            EXPECT_EQ(FMadre.SottoPoligoni.VerticesCell1Ds[i][1],F1.IdVertici[0]);
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


    //Creo un'altra frattura figlia
    vector<Vector3d>Coord3={w,y,z,h};
    Frattura F2 =Frattura(4,Coord2);
    F2.IdVertici.push_back(4);
    F2.IdVertici.push_back(1);
    F2.IdVertici.push_back(2);
    F2.IdVertici.push_back(5);
    //Non devo aggiungere le cell0D alla madre perchè fatto prima
    converteInCelle(F2,FMadre);


    //Controllo sono state salvate correttamente per la seconda frattura
    //Cell1D
    EXPECT_EQ(FMadre.SottoPoligoni.NumberofCell1Ds,7);



    for(unsigned int i = 4;i<7;i++){
        EXPECT_EQ(FMadre.SottoPoligoni.IdCell1Ds[i],i);
    }

    EXPECT_EQ(FMadre.SottoPoligoni.VerticesCell1Ds[4][0],F2.IdVertici[0]);
    EXPECT_EQ(FMadre.SottoPoligoni.VerticesCell1Ds[4][1],F2.IdVertici[1]);

    EXPECT_EQ(FMadre.SottoPoligoni.VerticesCell1Ds[5][0],F2.IdVertici[1]);
    EXPECT_EQ(FMadre.SottoPoligoni.VerticesCell1Ds[5][1],F2.IdVertici[2]);

    EXPECT_EQ(FMadre.SottoPoligoni.VerticesCell1Ds[6][0],F2.IdVertici[2]);
    EXPECT_EQ(FMadre.SottoPoligoni.VerticesCell1Ds[6][1],F2.IdVertici[3]);

    //Cell2D
    EXPECT_EQ(FMadre.SottoPoligoni.IdCell2Ds[1],1);
    EXPECT_EQ(FMadre.SottoPoligoni.NumberofCell2Ds,2);
    EXPECT_EQ(FMadre.SottoPoligoni.VerticesCell2Ds[1][0],4);
    EXPECT_EQ(FMadre.SottoPoligoni.VerticesCell2Ds[1][1],1);
    EXPECT_EQ(FMadre.SottoPoligoni.VerticesCell2Ds[1][2],2);
    EXPECT_EQ(FMadre.SottoPoligoni.VerticesCell2Ds[1][3],5);

    EXPECT_EQ(FMadre.SottoPoligoni.EdgesCell2Ds[1][0],4);
    EXPECT_EQ(FMadre.SottoPoligoni.EdgesCell2Ds[1][1],5);
    EXPECT_EQ(FMadre.SottoPoligoni.EdgesCell2Ds[1][2],6);
    EXPECT_EQ(FMadre.SottoPoligoni.EdgesCell2Ds[1][3],1);

}



TEST(calcoloSottoPoligoniPass,UnaTracciaPassanteVerticale){
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



TEST(calcoloSottoPoligoniPass,TracciaPassanteOrizzontale){
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
    Vector3d w(2,0,0);
    Vector3d h(-2,0,0);
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
        EXPECT_DOUBLE_EQ(FrattureFiglie[1].CoordinateVertici[0][i],x[i]);
        EXPECT_DOUBLE_EQ(FrattureFiglie[1].CoordinateVertici[1][i],y[i]);
        EXPECT_DOUBLE_EQ(FrattureFiglie[1].CoordinateVertici[2][i],h[i]);
        EXPECT_DOUBLE_EQ(FrattureFiglie[1].CoordinateVertici[3][i],w[i]);

        EXPECT_DOUBLE_EQ(FrattureFiglie[0].CoordinateVertici[0][i],h[i]);
        EXPECT_DOUBLE_EQ(FrattureFiglie[0].CoordinateVertici[1][i],z[i]);
        EXPECT_DOUBLE_EQ(FrattureFiglie[0].CoordinateVertici[2][i],k[i]);
        EXPECT_DOUBLE_EQ(FrattureFiglie[0].CoordinateVertici[3][i],w[i]);
    }


    //Controllo se gli id dei vertici sono giusti
    EXPECT_EQ(FrattureFiglie[1].IdVertici[0],0);
    EXPECT_EQ(FrattureFiglie[1].IdVertici[1],1);
    EXPECT_EQ(FrattureFiglie[1].IdVertici[2],4);
    EXPECT_EQ(FrattureFiglie[1].IdVertici[3],5);

    EXPECT_EQ(FrattureFiglie[0].IdVertici[0],4);
    EXPECT_EQ(FrattureFiglie[0].IdVertici[1],2);
    EXPECT_EQ(FrattureFiglie[0].IdVertici[2],3);
    EXPECT_EQ(FrattureFiglie[0].IdVertici[3],5);
    //Controllo che la madre ha i nuovi vertici nella mesh
    EXPECT_EQ(FMadre.SottoPoligoni.IdCell0Ds[4],4);
    EXPECT_EQ(FMadre.SottoPoligoni.IdCell0Ds[5],5);
    for(unsigned int i = 0;i<3;i++){
        EXPECT_EQ(FMadre.SottoPoligoni.CoordinatesCell0Ds[4][i],h[i]);
        EXPECT_EQ(FMadre.SottoPoligoni.CoordinatesCell0Ds[5][i],w[i]);
    }

}




TEST(calcoloSottoPoligoniPass,TracciaPassanteLatiOppostiObliqua){
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
    Vector3d w(2,-1,0);
    Vector3d h(-2,1,0);
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
        EXPECT_DOUBLE_EQ(FrattureFiglie[0].CoordinateVertici[0][i],h[i]);
        EXPECT_DOUBLE_EQ(FrattureFiglie[0].CoordinateVertici[1][i],z[i]);
        EXPECT_DOUBLE_EQ(FrattureFiglie[0].CoordinateVertici[2][i],k[i]);
        EXPECT_DOUBLE_EQ(FrattureFiglie[0].CoordinateVertici[3][i],w[i]);

        EXPECT_DOUBLE_EQ(FrattureFiglie[1].CoordinateVertici[0][i],x[i]);
        EXPECT_DOUBLE_EQ(FrattureFiglie[1].CoordinateVertici[1][i],y[i]);
        EXPECT_DOUBLE_EQ(FrattureFiglie[1].CoordinateVertici[2][i],h[i]);
        EXPECT_DOUBLE_EQ(FrattureFiglie[1].CoordinateVertici[3][i],w[i]);


    }


    //Controllo se gli id dei vertici sono giusti

    EXPECT_EQ(FrattureFiglie[0].IdVertici[0],4);
    EXPECT_EQ(FrattureFiglie[0].IdVertici[1],2);
    EXPECT_EQ(FrattureFiglie[0].IdVertici[2],3);
    EXPECT_EQ(FrattureFiglie[0].IdVertici[3],5);

    EXPECT_EQ(FrattureFiglie[1].IdVertici[0],0);
    EXPECT_EQ(FrattureFiglie[1].IdVertici[1],1);
    EXPECT_EQ(FrattureFiglie[1].IdVertici[2],4);
    EXPECT_EQ(FrattureFiglie[1].IdVertici[3],5);

    //Controllo che la madre ha i nuovi vertici nella mesh
    EXPECT_EQ(FMadre.SottoPoligoni.IdCell0Ds[4],4);
    EXPECT_EQ(FMadre.SottoPoligoni.IdCell0Ds[5],5);
    for(unsigned int i = 0;i<3;i++){
        EXPECT_EQ(FMadre.SottoPoligoni.CoordinatesCell0Ds[4][i],h[i]);
        EXPECT_EQ(FMadre.SottoPoligoni.CoordinatesCell0Ds[5][i],w[i]);
    }

}


TEST(calcoloSottoPoligoniPass,TracciaPassanteObliquaUnVerticeCoincidente){
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
    Vector3d w(2,2,0);
    Vector3d h(-2,1,0);
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
    EXPECT_EQ(FrattureFiglie[1].NumVertici,3);
    EXPECT_EQ(FrattureFiglie[0].NumVertici,4);
    //Controllo se i vertici sono giusti

    for(unsigned int i = 0;i<3;i++){
        EXPECT_DOUBLE_EQ(FrattureFiglie[0].CoordinateVertici[0][i],x[i]);
        EXPECT_DOUBLE_EQ(FrattureFiglie[0].CoordinateVertici[1][i],h[i]);
        EXPECT_DOUBLE_EQ(FrattureFiglie[0].CoordinateVertici[2][i],z[i]);
        EXPECT_DOUBLE_EQ(FrattureFiglie[0].CoordinateVertici[3][i],k[i]);

        EXPECT_DOUBLE_EQ(FrattureFiglie[1].CoordinateVertici[0][i],x[i]);
        EXPECT_DOUBLE_EQ(FrattureFiglie[1].CoordinateVertici[1][i],y[i]);
        EXPECT_DOUBLE_EQ(FrattureFiglie[1].CoordinateVertici[2][i],h[i]);


    }


    //Controllo se gli id dei vertici sono giusti
    EXPECT_EQ(FrattureFiglie[0].IdVertici[0],0);
    EXPECT_EQ(FrattureFiglie[0].IdVertici[1],4);
    EXPECT_EQ(FrattureFiglie[0].IdVertici[2],2);
    EXPECT_EQ(FrattureFiglie[0].IdVertici[3],3);

    EXPECT_EQ(FrattureFiglie[1].IdVertici[0],0);
    EXPECT_EQ(FrattureFiglie[1].IdVertici[1],1);
    EXPECT_EQ(FrattureFiglie[1].IdVertici[2],4);

    ;
    //Controllo che la madre ha i nuovi vertici nella mesh
    EXPECT_EQ(FMadre.SottoPoligoni.IdCell0Ds[4],4);
    for(unsigned int i = 0;i<3;i++){
        EXPECT_EQ(FMadre.SottoPoligoni.CoordinatesCell0Ds[4][i],h[i]);
    }

}


TEST(calcoloSottoPoligoniPass,TracciaPassanteObliquaEntrambiVerticiCoincidenti){
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
    Vector3d w(-2,2,0);
    Vector3d h(2,-2,0);
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
    EXPECT_EQ(FrattureFiglie[0].NumVertici,3);
    EXPECT_EQ(FrattureFiglie[1].NumVertici,3);
    //Controllo se i vertici sono giusti

    for(unsigned int i = 0;i<3;i++){
        EXPECT_DOUBLE_EQ(FrattureFiglie[1].CoordinateVertici[0][i],y[i]);
        EXPECT_DOUBLE_EQ(FrattureFiglie[1].CoordinateVertici[1][i],z[i]);
        EXPECT_DOUBLE_EQ(FrattureFiglie[1].CoordinateVertici[2][i],k[i]);


        EXPECT_DOUBLE_EQ(FrattureFiglie[0].CoordinateVertici[0][i],x[i]);
        EXPECT_DOUBLE_EQ(FrattureFiglie[0].CoordinateVertici[1][i],y[i]);
        EXPECT_DOUBLE_EQ(FrattureFiglie[0].CoordinateVertici[2][i],k[i]);


    }


    //Controllo se gli id dei vertici sono giusti
    EXPECT_EQ(FrattureFiglie[1].IdVertici[0],1);
    EXPECT_EQ(FrattureFiglie[1].IdVertici[1],2);
    EXPECT_EQ(FrattureFiglie[1].IdVertici[2],3);


    EXPECT_EQ(FrattureFiglie[0].IdVertici[0],0);
    EXPECT_EQ(FrattureFiglie[0].IdVertici[1],1);
    EXPECT_EQ(FrattureFiglie[0].IdVertici[2],3);

    ;
    //Controllo che la madre ha i nuovi vertici nella mesh
    EXPECT_EQ(FMadre.SottoPoligoni.NumberofCell0Ds,4);//Non devono essere state aggiunte nuove cell0D

}



TEST(calcoloSottoPoligoniPass,TracciaPassanteLatiAdiacentiObliqua){
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
    Vector3d w(2,1,0);
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
    EXPECT_EQ(FrattureFiglie[1].NumVertici,5);
    EXPECT_EQ(FrattureFiglie[0].NumVertici,3);
    //Controllo se i vertici sono giusti

    for(unsigned int i = 0;i<3;i++){
        EXPECT_DOUBLE_EQ(FrattureFiglie[1].CoordinateVertici[0][i],x[i]);
        EXPECT_DOUBLE_EQ(FrattureFiglie[1].CoordinateVertici[1][i],y[i]);
        EXPECT_DOUBLE_EQ(FrattureFiglie[1].CoordinateVertici[2][i],z[i]);
        EXPECT_DOUBLE_EQ(FrattureFiglie[1].CoordinateVertici[3][i],h[i]);
        EXPECT_DOUBLE_EQ(FrattureFiglie[1].CoordinateVertici[4][i],w[i]);

        EXPECT_DOUBLE_EQ(FrattureFiglie[0].CoordinateVertici[0][i],h[i]);
        EXPECT_DOUBLE_EQ(FrattureFiglie[0].CoordinateVertici[1][i],k[i]);
        EXPECT_DOUBLE_EQ(FrattureFiglie[0].CoordinateVertici[2][i],w[i]);



    }


    //Controllo se gli id dei vertici sono giusti

    EXPECT_EQ(FrattureFiglie[1].IdVertici[0],0);
    EXPECT_EQ(FrattureFiglie[1].IdVertici[1],1);
    EXPECT_EQ(FrattureFiglie[1].IdVertici[2],2);
    EXPECT_EQ(FrattureFiglie[1].IdVertici[3],4);
    EXPECT_EQ(FrattureFiglie[1].IdVertici[4],5);


    EXPECT_EQ(FrattureFiglie[0].IdVertici[0],4);
    EXPECT_EQ(FrattureFiglie[0].IdVertici[1],3);
    EXPECT_EQ(FrattureFiglie[0].IdVertici[2],5);


    //Controllo che la madre ha i nuovi vertici nella mesh
    EXPECT_EQ(FMadre.SottoPoligoni.IdCell0Ds[4],4);
    EXPECT_EQ(FMadre.SottoPoligoni.IdCell0Ds[5],5);
    for(unsigned int i = 0;i<3;i++){
        EXPECT_EQ(FMadre.SottoPoligoni.CoordinatesCell0Ds[4][i],h[i]);
        EXPECT_EQ(FMadre.SottoPoligoni.CoordinatesCell0Ds[5][i],w[i]);
    }

}

TEST(calcoloSottoPoligoniPass,TracciaPassantePerLatoToccante){
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
    Vector3d w(-2,1,0);
    Vector3d h(-2,-1,0);
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
    EXPECT_EQ(FrattureFiglie[0].NumVertici,6);
    //Controllo se i vertici sono giusti

    for(unsigned int i = 0;i<3;i++){
        EXPECT_DOUBLE_EQ(FrattureFiglie[0].CoordinateVertici[0][i],x[i]);
        EXPECT_DOUBLE_EQ(FrattureFiglie[0].CoordinateVertici[1][i],y[i]);
        EXPECT_DOUBLE_EQ(FrattureFiglie[0].CoordinateVertici[2][i],w[i]);
        EXPECT_DOUBLE_EQ(FrattureFiglie[0].CoordinateVertici[3][i],h[i]);
        EXPECT_DOUBLE_EQ(FrattureFiglie[0].CoordinateVertici[4][i],z[i]);
        EXPECT_DOUBLE_EQ(FrattureFiglie[0].CoordinateVertici[5][i],k[i]);
    }


    //Controllo se gli id dei vertici sono giusti
    EXPECT_EQ(FrattureFiglie[0].IdVertici[0],0);
    EXPECT_EQ(FrattureFiglie[0].IdVertici[1],1);
    EXPECT_EQ(FrattureFiglie[0].IdVertici[2],4);
    EXPECT_EQ(FrattureFiglie[0].IdVertici[3],5);
    EXPECT_EQ(FrattureFiglie[0].IdVertici[4],2);
    EXPECT_EQ(FrattureFiglie[0].IdVertici[5],3);
    //Controllo che la madre ha i nuovi vertici nella mesh
    EXPECT_EQ(FMadre.SottoPoligoni.IdCell0Ds[4],4);
    EXPECT_EQ(FMadre.SottoPoligoni.IdCell0Ds[5],5);
    for(unsigned int i = 0;i<3;i++){
        EXPECT_EQ(FMadre.SottoPoligoni.CoordinatesCell0Ds[4][i],w[i]);
        EXPECT_EQ(FMadre.SottoPoligoni.CoordinatesCell0Ds[5][i],h[i]);
    }

}

TEST(calcoloSottoPoligoniPass,TracciaPassantePerLatoToccanteUnVerticeCoincidente){
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
    Vector3d w(-2,2,0);
    Vector3d h(-2,-1,0);
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
    EXPECT_EQ(FrattureFiglie[0].NumVertici,5);
    //Controllo se i vertici sono giusti

    for(unsigned int i = 0;i<3;i++){
        EXPECT_DOUBLE_EQ(FrattureFiglie[0].CoordinateVertici[0][i],x[i]);
        EXPECT_DOUBLE_EQ(FrattureFiglie[0].CoordinateVertici[1][i],y[i]);
        EXPECT_DOUBLE_EQ(FrattureFiglie[0].CoordinateVertici[2][i],h[i]);
        EXPECT_DOUBLE_EQ(FrattureFiglie[0].CoordinateVertici[3][i],z[i]);
        EXPECT_DOUBLE_EQ(FrattureFiglie[0].CoordinateVertici[4][i],k[i]);

    }


    //Controllo se gli id dei vertici sono giusti
    EXPECT_EQ(FrattureFiglie[0].IdVertici[0],0);
    EXPECT_EQ(FrattureFiglie[0].IdVertici[1],1);
    EXPECT_EQ(FrattureFiglie[0].IdVertici[2],4);
    EXPECT_EQ(FrattureFiglie[0].IdVertici[3],2);
    EXPECT_EQ(FrattureFiglie[0].IdVertici[4],3);

    //Controllo che la madre ha i nuovi vertici nella mesh
    EXPECT_EQ(FMadre.SottoPoligoni.IdCell0Ds[4],4);

    for(unsigned int i = 0;i<3;i++){
        EXPECT_EQ(FMadre.SottoPoligoni.CoordinatesCell0Ds[4][i],h[i]);

    }

}

TEST(calcoloSottoPoligoniPass,TracciaPassantePerLatoToccanteEntrambiVerticiCoincidenti){
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
    Vector3d w(-2,2,0);
    Vector3d h(-2,-2,0);
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
    //Controllo se i vertici sono giusti

    for(unsigned int i = 0;i<3;i++){
        EXPECT_DOUBLE_EQ(FrattureFiglie[0].CoordinateVertici[0][i],x[i]);
        EXPECT_DOUBLE_EQ(FrattureFiglie[0].CoordinateVertici[1][i],y[i]);
        EXPECT_DOUBLE_EQ(FrattureFiglie[0].CoordinateVertici[2][i],z[i]);
        EXPECT_DOUBLE_EQ(FrattureFiglie[0].CoordinateVertici[3][i],k[i]);

    }


    //Controllo se gli id dei vertici sono giusti
    EXPECT_EQ(FrattureFiglie[0].IdVertici[0],0);
    EXPECT_EQ(FrattureFiglie[0].IdVertici[1],1);
    EXPECT_EQ(FrattureFiglie[0].IdVertici[2],2);
    EXPECT_EQ(FrattureFiglie[0].IdVertici[3],3);

    //Controllo che la madre ha i nuovi vertici nella mesh
    EXPECT_EQ(FMadre.SottoPoligoni.NumberofCell0Ds,4);

}


TEST(calcoloSottoPoligoniNoPass,TracciaNonPassanteVerticale){
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
    Vector3d w(0,1,0);
    Vector3d h(0,-1,0);
    Vector3d w1(0,2,0);
    Vector3d h1(0,-2,0);
    array<Vector3d,2> VerticiT = {w,h};
    array<unsigned int,2> FrattureT = {0,1};
    array<bool,2> Tips = {false,false};
    Traccia T = Traccia(0,VerticiT,FrattureT,Tips);
    FMadre.TracceNoPass.push_back(0);
    vector<Traccia> Tracce ;
    Tracce.push_back(T);
    double tol = 0.0000000000000001;
    double tol2 = SetTolProdotto(tol);
    vector<Frattura> FrattureFiglie = calcoloSottoPoligoniNoPass(FMadre,tol,tol2,Tracce,FMadre);
    //Controllo il numero dei vertici
    EXPECT_EQ(FrattureFiglie[0].NumVertici,4);
    EXPECT_EQ(FrattureFiglie[1].NumVertici,4);
    //Controllo se i vertici sono giusti

    for(unsigned int i = 0;i<3;i++){
        EXPECT_DOUBLE_EQ(FrattureFiglie[0].CoordinateVertici[0][i],x[i]);
        EXPECT_DOUBLE_EQ(FrattureFiglie[0].CoordinateVertici[1][i],w1[i]);
        EXPECT_DOUBLE_EQ(FrattureFiglie[0].CoordinateVertici[2][i],h1[i]);
        EXPECT_DOUBLE_EQ(FrattureFiglie[0].CoordinateVertici[3][i],k[i]);

        EXPECT_DOUBLE_EQ(FrattureFiglie[1].CoordinateVertici[0][i],w1[i]);
        EXPECT_DOUBLE_EQ(FrattureFiglie[1].CoordinateVertici[1][i],y[i]);
        EXPECT_DOUBLE_EQ(FrattureFiglie[1].CoordinateVertici[2][i],z[i]);
        EXPECT_DOUBLE_EQ(FrattureFiglie[1].CoordinateVertici[3][i],h1[i]);
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
        EXPECT_EQ(FMadre.SottoPoligoni.CoordinatesCell0Ds[4][i],w1[i]);
        EXPECT_EQ(FMadre.SottoPoligoni.CoordinatesCell0Ds[5][i],h1[i]);
    }

}
TEST(calcoloSottoPoligoniNoPass,TracciaNonPassanteOrizzontale){
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
    Vector3d w1(1,0,0);
    Vector3d h1(-1,0,0);
    Vector3d w(2,0,0);
    Vector3d h(-2,0,0);
    array<Vector3d,2> VerticiT = {w1,h1};
    array<unsigned int,2> FrattureT = {0,1};
    array<bool,2> Tips = {false,false};
    Traccia T = Traccia(0,VerticiT,FrattureT,Tips);
    FMadre.TracceNoPass.push_back(0);
    vector<Traccia> Tracce ;
    Tracce.push_back(T);
    double tol = 0.0000000000000001;
    double tol2 = SetTolProdotto(tol);
    vector<Frattura> FrattureFiglie = calcoloSottoPoligoniNoPass(FMadre,tol,tol2,Tracce,FMadre);
    //Controllo il numero dei vertici
    EXPECT_EQ(FrattureFiglie[0].NumVertici,4);
    EXPECT_EQ(FrattureFiglie[1].NumVertici,4);
    //Controllo se i vertici sono giusti

    for(unsigned int i = 0;i<3;i++){
        EXPECT_DOUBLE_EQ(FrattureFiglie[1].CoordinateVertici[0][i],x[i]);
        EXPECT_DOUBLE_EQ(FrattureFiglie[1].CoordinateVertici[1][i],y[i]);
        EXPECT_DOUBLE_EQ(FrattureFiglie[1].CoordinateVertici[2][i],h[i]);
        EXPECT_DOUBLE_EQ(FrattureFiglie[1].CoordinateVertici[3][i],w[i]);

        EXPECT_DOUBLE_EQ(FrattureFiglie[0].CoordinateVertici[0][i],h[i]);
        EXPECT_DOUBLE_EQ(FrattureFiglie[0].CoordinateVertici[1][i],z[i]);
        EXPECT_DOUBLE_EQ(FrattureFiglie[0].CoordinateVertici[2][i],k[i]);
        EXPECT_DOUBLE_EQ(FrattureFiglie[0].CoordinateVertici[3][i],w[i]);
    }


    //Controllo se gli id dei vertici sono giusti
    EXPECT_EQ(FrattureFiglie[1].IdVertici[0],0);
    EXPECT_EQ(FrattureFiglie[1].IdVertici[1],1);
    EXPECT_EQ(FrattureFiglie[1].IdVertici[2],4);
    EXPECT_EQ(FrattureFiglie[1].IdVertici[3],5);

    EXPECT_EQ(FrattureFiglie[0].IdVertici[0],4);
    EXPECT_EQ(FrattureFiglie[0].IdVertici[1],2);
    EXPECT_EQ(FrattureFiglie[0].IdVertici[2],3);
    EXPECT_EQ(FrattureFiglie[0].IdVertici[3],5);
    //Controllo che la madre ha i nuovi vertici nella mesh
    EXPECT_EQ(FMadre.SottoPoligoni.IdCell0Ds[4],4);
    EXPECT_EQ(FMadre.SottoPoligoni.IdCell0Ds[5],5);
    for(unsigned int i = 0;i<3;i++){
        EXPECT_EQ(FMadre.SottoPoligoni.CoordinatesCell0Ds[4][i],h[i]);
        EXPECT_EQ(FMadre.SottoPoligoni.CoordinatesCell0Ds[5][i],w[i]);
    }

}

TEST(calcoloSottoPoligoniNoPass,TracciaNonPassanteObliquaUnVerticeCoincidente){
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
    Vector3d w(2,2,0);
    Vector3d h(-2,1,0);
    Vector3d h1(0,1.5,0);
    array<Vector3d,2> VerticiT = {w,h1};
    array<unsigned int,2> FrattureT = {0,1};
    array<bool,2> Tips = {false,false};
    Traccia T = Traccia(0,VerticiT,FrattureT,Tips);
    FMadre.TracceNoPass.push_back(0);
    vector<Traccia> Tracce ;
    Tracce.push_back(T);
    double tol = 0.0000000000000001;
    double tol2 = SetTolProdotto(tol);
    vector<Frattura> FrattureFiglie = calcoloSottoPoligoniNoPass(FMadre,tol,tol2,Tracce,FMadre);
    //Controllo il numero dei vertici
    EXPECT_EQ(FrattureFiglie[1].NumVertici,3);
    EXPECT_EQ(FrattureFiglie[0].NumVertici,4);
    //Controllo se i vertici sono giusti

    for(unsigned int i = 0;i<3;i++){
        EXPECT_DOUBLE_EQ(FrattureFiglie[0].CoordinateVertici[0][i],x[i]);
        EXPECT_DOUBLE_EQ(FrattureFiglie[0].CoordinateVertici[1][i],h[i]);
        EXPECT_DOUBLE_EQ(FrattureFiglie[0].CoordinateVertici[2][i],z[i]);
        EXPECT_DOUBLE_EQ(FrattureFiglie[0].CoordinateVertici[3][i],k[i]);

        EXPECT_DOUBLE_EQ(FrattureFiglie[1].CoordinateVertici[0][i],x[i]);
        EXPECT_DOUBLE_EQ(FrattureFiglie[1].CoordinateVertici[1][i],y[i]);
        EXPECT_DOUBLE_EQ(FrattureFiglie[1].CoordinateVertici[2][i],h[i]);


    }


    //Controllo se gli id dei vertici sono giusti
    EXPECT_EQ(FrattureFiglie[0].IdVertici[0],0);
    EXPECT_EQ(FrattureFiglie[0].IdVertici[1],4);
    EXPECT_EQ(FrattureFiglie[0].IdVertici[2],2);
    EXPECT_EQ(FrattureFiglie[0].IdVertici[3],3);

    EXPECT_EQ(FrattureFiglie[1].IdVertici[0],0);
    EXPECT_EQ(FrattureFiglie[1].IdVertici[1],1);
    EXPECT_EQ(FrattureFiglie[1].IdVertici[2],4);

    ;
    //Controllo che la madre ha i nuovi vertici nella mesh
    EXPECT_EQ(FMadre.SottoPoligoni.IdCell0Ds[4],4);
    for(unsigned int i = 0;i<3;i++){
        EXPECT_EQ(FMadre.SottoPoligoni.CoordinatesCell0Ds[4][i],h[i]);
    }

}

TEST(calcoloSottoPoligoniNoPass,TracciaNonPassanteObliquaConEstesaVerticiCoincidenti){
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
    Vector3d w(-2,2,0);
    Vector3d h(2,-2,0);
    Vector3d w1(-1,1,0);
    Vector3d h1(1,-1,0);
    array<Vector3d,2> VerticiT = {w,h};
    array<unsigned int,2> FrattureT = {0,1};
    array<bool,2> Tips = {false,false};
    Traccia T = Traccia(0,VerticiT,FrattureT,Tips);
    FMadre.TracceNoPass.push_back(0);
    vector<Traccia> Tracce ;
    Tracce.push_back(T);
    double tol = 0.0000000000000001;
    double tol2 = SetTolProdotto(tol);
    vector<Frattura> FrattureFiglie = calcoloSottoPoligoniNoPass(FMadre,tol,tol2,Tracce,FMadre);
    //Controllo il numero dei vertici
    EXPECT_EQ(FrattureFiglie[0].NumVertici,3);
    EXPECT_EQ(FrattureFiglie[1].NumVertici,3);
    //Controllo se i vertici sono giusti

    for(unsigned int i = 0;i<3;i++){
        EXPECT_DOUBLE_EQ(FrattureFiglie[1].CoordinateVertici[0][i],y[i]);
        EXPECT_DOUBLE_EQ(FrattureFiglie[1].CoordinateVertici[1][i],z[i]);
        EXPECT_DOUBLE_EQ(FrattureFiglie[1].CoordinateVertici[2][i],k[i]);


        EXPECT_DOUBLE_EQ(FrattureFiglie[0].CoordinateVertici[0][i],x[i]);
        EXPECT_DOUBLE_EQ(FrattureFiglie[0].CoordinateVertici[1][i],y[i]);
        EXPECT_DOUBLE_EQ(FrattureFiglie[0].CoordinateVertici[2][i],k[i]);


    }


    //Controllo se gli id dei vertici sono giusti
    EXPECT_EQ(FrattureFiglie[1].IdVertici[0],1);
    EXPECT_EQ(FrattureFiglie[1].IdVertici[1],2);
    EXPECT_EQ(FrattureFiglie[1].IdVertici[2],3);


    EXPECT_EQ(FrattureFiglie[0].IdVertici[0],0);
    EXPECT_EQ(FrattureFiglie[0].IdVertici[1],1);
    EXPECT_EQ(FrattureFiglie[0].IdVertici[2],3);

    ;
    //Controllo che la madre ha i nuovi vertici nella mesh
    EXPECT_EQ(FMadre.SottoPoligoni.NumberofCell0Ds,4);//Non devono essere state aggiunte nuove cell0D

}


TEST(calcoloSottoPoligoniNoPass,TracciaNonPassanteLatiAdiacentiObliqua){
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
    Vector3d w(2,1,0);
    Vector3d h(0,-2,0);
    Vector3d h1(1,-0.5,0);
    array<Vector3d,2> VerticiT = {w,h1};
    array<unsigned int,2> FrattureT = {0,1};
    array<bool,2> Tips = {false,false};
    Traccia T = Traccia(0,VerticiT,FrattureT,Tips);
    FMadre.TracceNoPass.push_back(0);
    vector<Traccia> Tracce ;
    Tracce.push_back(T);
    double tol = 0.0000000000000001;
    double tol2 = SetTolProdotto(tol);
    vector<Frattura> FrattureFiglie = calcoloSottoPoligoniNoPass(FMadre,tol,tol2,Tracce,FMadre);
    //Controllo il numero dei vertici
    EXPECT_EQ(FrattureFiglie[1].NumVertici,5);
    EXPECT_EQ(FrattureFiglie[0].NumVertici,3);
    //Controllo se i vertici sono giusti

    for(unsigned int i = 0;i<3;i++){
        EXPECT_DOUBLE_EQ(FrattureFiglie[1].CoordinateVertici[0][i],x[i]);
        EXPECT_DOUBLE_EQ(FrattureFiglie[1].CoordinateVertici[1][i],y[i]);
        EXPECT_DOUBLE_EQ(FrattureFiglie[1].CoordinateVertici[2][i],z[i]);
        EXPECT_DOUBLE_EQ(FrattureFiglie[1].CoordinateVertici[3][i],h[i]);
        EXPECT_DOUBLE_EQ(FrattureFiglie[1].CoordinateVertici[4][i],w[i]);

        EXPECT_DOUBLE_EQ(FrattureFiglie[0].CoordinateVertici[0][i],h[i]);
        EXPECT_DOUBLE_EQ(FrattureFiglie[0].CoordinateVertici[1][i],k[i]);
        EXPECT_DOUBLE_EQ(FrattureFiglie[0].CoordinateVertici[2][i],w[i]);



    }


    //Controllo se gli id dei vertici sono giusti

    EXPECT_EQ(FrattureFiglie[1].IdVertici[0],0);
    EXPECT_EQ(FrattureFiglie[1].IdVertici[1],1);
    EXPECT_EQ(FrattureFiglie[1].IdVertici[2],2);
    EXPECT_EQ(FrattureFiglie[1].IdVertici[3],4);
    EXPECT_EQ(FrattureFiglie[1].IdVertici[4],5);


    EXPECT_EQ(FrattureFiglie[0].IdVertici[0],4);
    EXPECT_EQ(FrattureFiglie[0].IdVertici[1],3);
    EXPECT_EQ(FrattureFiglie[0].IdVertici[2],5);


    //Controllo che la madre ha i nuovi vertici nella mesh
    EXPECT_EQ(FMadre.SottoPoligoni.IdCell0Ds[4],4);
    EXPECT_EQ(FMadre.SottoPoligoni.IdCell0Ds[5],5);
    for(unsigned int i = 0;i<3;i++){
        EXPECT_EQ(FMadre.SottoPoligoni.CoordinatesCell0Ds[4][i],h[i]);
        EXPECT_EQ(FMadre.SottoPoligoni.CoordinatesCell0Ds[5][i],w[i]);
    }

}

TEST(calcoloSottoPoligoniPassTracce,DueTraccePassantineIncidenti){
    //Creo la frattura madre
    Vector3d x(2,2,0);
    Vector3d y(-2,2,0);
    Vector3d z(-2,-2,0);
    Vector3d k(2,-2,0);
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
    Vector3d wo(2,0,0);
    Vector3d ho(-2,0,0);
    Vector3d o(0,0,0);
    array<Vector3d,2> VerticiT = {w,h};
    array<Vector3d,2> VerticiTo = {wo,ho};
    array<unsigned int,2> FrattureT = {0,1};
    array<bool,2> Tips = {false,false};
    Traccia T = Traccia(0,VerticiT,FrattureT,Tips);
    Traccia To = Traccia(1,VerticiTo,FrattureT,Tips);
    FMadre.TraccePass.push_back(0);
    FMadre.TraccePass.push_back(1);
    vector<Traccia> Tracce ;
    Tracce.push_back(T);
    Tracce.push_back(To);
    double tol = 0.0000000000000001;
    double tol2 = SetTolProdotto(tol);
    bool TracciaSulBordo = false;
    vector<Frattura> FrattureFiglie = calcoloSottoPoligoniPass(FMadre,tol,tol2,TracciaSulBordo,Tracce,FMadre);
    EXPECT_DOUBLE_EQ(FrattureFiglie[0].TraccePass[0],2);
    EXPECT_DOUBLE_EQ(FrattureFiglie[1].TraccePass[0],3);
    for (int i=0; i<3; i++){
        EXPECT_DOUBLE_EQ(Tracce[2].VerticiTraccia[0][i],wo[i]);
        EXPECT_DOUBLE_EQ(Tracce[2].VerticiTraccia[1][i],o[i]);
        EXPECT_DOUBLE_EQ(Tracce[3].VerticiTraccia[0][i],o[i]);
        EXPECT_DOUBLE_EQ(Tracce[3].VerticiTraccia[1][i],ho[i]);
    }
}

TEST(calcoloSottoPoligoniPassTracce,DueTraccePassantineSeparate){
    //Creo la frattura madre
    Vector3d x(2,2,0);
    Vector3d y(-2,2,0);
    Vector3d z(-2,-2,0);
    Vector3d k(2,-2,0);
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
    Vector3d wo(1,2,0);
    Vector3d ho(1,-2,0);
    array<Vector3d,2> VerticiT = {w,h};
    array<Vector3d,2> VerticiTo = {wo,ho};
    array<unsigned int,2> FrattureT = {0,1};
    array<bool,2> Tips = {false,false};
    Traccia T = Traccia(0,VerticiT,FrattureT,Tips);
    Traccia To = Traccia(1,VerticiTo,FrattureT,Tips);
    FMadre.TraccePass.push_back(0);
    FMadre.TraccePass.push_back(1);
    vector<Traccia> Tracce ;
    Tracce.push_back(T);
    Tracce.push_back(To);
    double tol = 0.0000000000000001;
    double tol2 = SetTolProdotto(tol);
    bool TracciaSulBordo = false;
    vector<Frattura> FrattureFiglie = calcoloSottoPoligoniPass(FMadre,tol,tol2,TracciaSulBordo,Tracce,FMadre);
    EXPECT_DOUBLE_EQ(FrattureFiglie[0].TraccePass[0],1);
    for (int i=0; i<3; i++){
        EXPECT_DOUBLE_EQ(Tracce[1].VerticiTraccia[0][i],wo[i]);
        EXPECT_DOUBLE_EQ(Tracce[1].VerticiTraccia[1][i],ho[i]);

    }
}

TEST(CalcoloSottoPoligono,TreFratture){
    Vector3d x(2,2,0);
    Vector3d y(-2,2,0);
    Vector3d z(-2,-2,0);
    Vector3d k(2,-2,0);
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
    //Faccio finta esiste un'altra frattura con id=1
    Vector3d w1(-2,-1,0);
    Vector3d h1(2,-1,0);
    Vector3d w2(0,2,0);
    Vector3d h2(-2,0,0);
    Vector3d w3(-1.5,1.5,0);
    Vector3d h3(1.5,-1.5,0);
    array<Vector3d,2> VerticiT1 = {w1,h1};
    array<Vector3d,2> VerticiT2 = {w2,h2};
    array<Vector3d,2> VerticiT3 = {w3,h3};
    array<unsigned int,2> FrattureT = {0,1};
    array<bool,2> Tips = {false,false};
    array<bool,2> Tips2= {true,false};
    Traccia T1 = Traccia(0,VerticiT1,FrattureT,Tips);
    Traccia T2 = Traccia(1,VerticiT2,FrattureT,Tips);
    Traccia T3 = Traccia(2,VerticiT3,FrattureT,Tips2);
    FMadre.TraccePass.push_back(0);
    FMadre.TraccePass.push_back(1);
    FMadre.TracceNoPass.push_back(2);
    //Ho passato già i vettori ordinati
    vector<Traccia> Tracce ;
    Tracce.push_back(T1);
    Tracce.push_back(T2);
    Tracce.push_back(T3);
    double tol = 0.0000000000000001;
    double tol2 = SetTolProdotto(tol);
    bool TracciaSulBordo = false;
    vector<Frattura> FrattureFiglie = calcoloSottoPoligoniPass(FMadre,tol,tol2,TracciaSulBordo,Tracce,FMadre);
    //Controllo che nelle fratture figlie sono calcolate bene le nuove tracce
    EXPECT_EQ(FrattureFiglie[0].TraccePass[0],1);
    EXPECT_EQ(FrattureFiglie[0].TracceNoPass[0],3);
    EXPECT_EQ(FrattureFiglie[1].TracceNoPass[0],4);
    EXPECT_EQ(Tracce.size(),5);
    Vector3d NuovoEstremo(1,-1,0);
    for (int i=0; i<3; i++){
        EXPECT_DOUBLE_EQ(Tracce[1].VerticiTraccia[0][i],w2[i]);
        EXPECT_DOUBLE_EQ(Tracce[1].VerticiTraccia[1][i],h2[i]);
        EXPECT_DOUBLE_EQ(Tracce[3].VerticiTraccia[0][i],w3[i]);
        EXPECT_DOUBLE_EQ(Tracce[3].VerticiTraccia[1][i],NuovoEstremo[i]);
        EXPECT_DOUBLE_EQ(Tracce[4].VerticiTraccia[0][i],NuovoEstremo[i]);
        EXPECT_DOUBLE_EQ(Tracce[4].VerticiTraccia[1][i],h3[i]);
    }
    //Ora calcolo i sottopoligoni della prima figlia
    vector<Frattura> FrattureFiglie1 = calcoloSottoPoligoniPass(FrattureFiglie[0],tol,tol2,TracciaSulBordo,Tracce,FMadre);
    //Controllo che nelle fratture figlie sono calcolate bene le nuove tracce
    EXPECT_EQ(Tracce.size(),7);
    EXPECT_EQ(FrattureFiglie1[0].TracceNoPass[0],6);
    EXPECT_EQ(FrattureFiglie1[1].TracceNoPass[0],5);
    /*
    Vector3d NuovoEstremo(1,-1,0);
    for (int i=0; i<3; i++){
        EXPECT_DOUBLE_EQ(Tracce[1].VerticiTraccia[0][i],w2[i]);
        EXPECT_DOUBLE_EQ(Tracce[1].VerticiTraccia[1][i],h2[i]);
        EXPECT_DOUBLE_EQ(Tracce[3].VerticiTraccia[0][i],w3[i]);
        EXPECT_DOUBLE_EQ(Tracce[3].VerticiTraccia[1][i],NuovoEstremo[i]);
        EXPECT_DOUBLE_EQ(Tracce[4].VerticiTraccia[0][i],NuovoEstremo[i]);
        EXPECT_DOUBLE_EQ(Tracce[4].VerticiTraccia[1][i],h3[i]);
    }*/
}

//TEST Taglia
TEST(Taglia,DueTraccePassantiIncidenti){
    Vector3d x(2,2,0);
    Vector3d y(-2,2,0);
    Vector3d z(-2,-2,0);
    Vector3d k(2,-2,0);
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
    //Faccio finta esiste un'altra frattura con id=1
    Vector3d w1(0,2,0);
    Vector3d h1(0,-2,0);
    Vector3d w2(2,0,0);
    Vector3d h2(-2,0,0);
    array<Vector3d,2> VerticiT1 = {w1,h1};
    array<Vector3d,2> VerticiT2 = {w2,h2};
    array<unsigned int,2> FrattureT = {0,1};
    array<bool,2> Tips = {false,false};
    Traccia T1 = Traccia(0,VerticiT1,FrattureT,Tips);
    Traccia T2 = Traccia(1,VerticiT2,FrattureT,Tips);
    FMadre.TraccePass.push_back(0);
    FMadre.TraccePass.push_back(1);
    vector<Traccia> Tracce ;
    Tracce.push_back(T1);
    Tracce.push_back(T2);
    double tol = 0.0000000000000001;
    double tol2 = SetTolProdotto(tol);
    bool TracciaSulBordo = false;
    Taglia(FMadre,FMadre,Tracce,tol,tol2);
    EXPECT_EQ(FMadre.SottoPoligoni.NumberofCell0Ds,9);
    EXPECT_EQ(FMadre.SottoPoligoni.NumberofCell1Ds,12);
     EXPECT_EQ(FMadre.SottoPoligoni.NumberofCell2Ds,4);

}




TEST(Taglia,TreTracce){
    Vector3d x(2,2,0);
    Vector3d y(-2,2,0);
    Vector3d z(-2,-2,0);
    Vector3d k(2,-2,0);
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
    //Faccio finta esiste un'altra frattura con id=1
    Vector3d w1(-2,-1,0);
    Vector3d h1(2,-1,0);
    Vector3d w2(0,2,0);
    Vector3d h2(-2,0,0);
    Vector3d w3(-1.5,1.5,0);
    Vector3d h3(1.5,-1.5,0);
    array<Vector3d,2> VerticiT1 = {w1,h1};
    array<Vector3d,2> VerticiT2 = {w2,h2};
    array<Vector3d,2> VerticiT3 = {w3,h3};
    array<unsigned int,2> FrattureT = {0,1};
    array<bool,2> Tips = {false,false};
    array<bool,2> Tips2= {true,false};
    Traccia T1 = Traccia(0,VerticiT1,FrattureT,Tips);
    Traccia T2 = Traccia(1,VerticiT2,FrattureT,Tips);
    Traccia T3 = Traccia(2,VerticiT3,FrattureT,Tips2);
    FMadre.TraccePass.push_back(0);
    FMadre.TraccePass.push_back(1);
    FMadre.TracceNoPass.push_back(2);
    //Ho passato già i vettori ordinati
    vector<Traccia> Tracce ;
    Tracce.push_back(T1);
    Tracce.push_back(T2);
    Tracce.push_back(T3);
    double tol = 0.0000000000000001;
    double tol2 = SetTolProdotto(tol);
    bool TracciaSulBordo = false;
    cout<<"ARRIVA QUI"<<endl;
    Taglia(FMadre,FMadre,Tracce,tol,tol2);
    EXPECT_EQ(FMadre.SottoPoligoni.NumberofCell0Ds,10);
    EXPECT_EQ(FMadre.SottoPoligoni.NumberofCell1Ds,15);
    EXPECT_EQ(FMadre.SottoPoligoni.NumberofCell2Ds,6);

}
