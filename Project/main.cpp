#include <iostream>
#include "./src/GeometryLibrary.hpp"
#include "./src/Utils.hpp"
using namespace std;
using namespace DFN;
int main()
{ /*vector<Vector3d> Coord;
    Vector3d x(0,0,1);
    Vector3d y(0,0,5);
    Vector3d z(1,0,0);
    Coord={x,y,z};
    Frattura F=Frattura(0,3,Coord);
    vector<Vector3d> Coord1;
    Vector3d x1(4,0,5);
    Vector3d y1(4,0,14);
    Vector3d z1(50,0,0);
    Coord1={x1,y1,z1};
    Frattura F2=Frattura(0,3,Coord1);
    bool flag=ControlloCentromero(F,F2, 0.000000000001);
    if(flag){
        cout<<"ehhi";
    }
    else{
        cout<<"a";
    }

    unsigned int Id=1;
    array<Vector3d,2> V;
    array<unsigned int,2> num;
    array<bool,2> fla;
    V[0]=x;
    V[1]=y;
    Traccia T1=Traccia(Id,V,num,fla);

     Id=2;
    V[0]=x1;
    V[1]=y1;
    Traccia T2=Traccia(Id,V,num,fla);

    Id=3;
    V[0]=z1;
    V[1]=z;
    Traccia T3=Traccia(Id,V,num,fla);

    vector<Traccia> t= {T3,T2,T1};

    cout<<endl<<t[0].IdTraccia;
    cout<<endl<<t[1].IdTraccia;
    cout<<endl<<t[2].IdTraccia;


    //anche il mergesort funziona, evviva
 
 vector<Frattura> Fractures;

    vector<Frattura> Fratture;

    string fileName = "./DFN/FR3_data.txt";

    double tol1D = setTol1D();
    importoFratture(fileName,Fratture,tol1D);
    for(auto f : Fratture){
        cout<<"Id "<< f.IdFrattura << " NumVertici " << f.NumVertici<< endl;
        for (unsigned int i = 0; i< f.NumVertici; i++){
            cout << "Vertice " <<i << " :" <<endl;
            cout  << f.CoordinateVertici[i] << endl;
        }
    }
    //PRIMO TEST, SONO DISTANTI STESSO TRIANGOLO DI RAGGIO MINORE DI 1
/*
    //SECONDO TEST, DUE QUADRATI CON DISTANZA MINIMA 0.1 CON RAGGI
    Vector3d x(-1,0,0);
    Vector3d y(0,1,0);
    Vector3d z(0,-1,0);
    Vector3d k(1,0,0);
    //QUADRATO CENTRO ZERO RAGGIO 1
    vector<Vector3d>Coord={x,y,z,k};
    Frattura F1=Frattura(0,4,Coord);

    Vector3d x1(-1,2.1,0);
    Vector3d y1(0,3.1,0);
    Vector3d z1(0,1.1,0);
    Vector3d k1(1,2.1,0);
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
    */

    Vector3d x(0,1,-1);
    Vector3d y(0,1,1);
    Vector3d k(0,-1,-1);
    Vector3d z(0,-1,1);
    //QUADRATO CENTRO ZERO RAGGIO 1
    vector<Vector3d>Coord={x,y,z,k};
    Frattura F1=Frattura(0,4,Coord);
    Vector3d x1(1,0,-1);
    Vector3d y1(1,0,1);
    Vector3d k1(-1,0,-1);
    Vector3d z1(-1,0,1);
    vector<Vector3d> Coord1={x1,y1,z1,k1};
    Frattura F2=Frattura(0,4,Coord1);
    vector<Vector3d> PuntiIntersecatiF1;
    vector<Vector3d> PuntiIntersecatiF2;
    PuntiIntersecatiF1.resize(4);
    PuntiIntersecatiF2.resize(4);
    bool flag=SiIntersecano(F1,F2, PuntiIntersecatiF1);
     cout<<"escodalIntersecano"<<endl;
    bool flag1=SiIntersecano(F2,F1, PuntiIntersecatiF2);
     for (int i=0; i<4; i++){
       // cout<<PuntiIntersecatiF1[i];
       //cout<<PuntiIntersecatiF2[i];
    }
    /*Vector3d b=CalcoloRetta(F1,F2);

    cout<<"la retta ha euqazione"<<b<<endl;
    int control=1;
    if(flag){
        control=0;
    }
    int control1=1;
    if(flag1){
        control1=0;
    }
    cout<<control<<endl<<control1;

    Vector3d dir1;
    dir1<<0,1,0;
    Vector3d dir2;
    dir2<<1,0,0;
    Vector3d c=IncontroTraRette(dir1, x,dir2 , x1 );
    cout<<c<<endl;
*/


    vector<Traccia> w=CalcoloTracce(F1,F2);






 return 0;
}
