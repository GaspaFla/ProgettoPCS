#include <iostream>
#include "./src/GeometryLibrary.hpp"
#include "./src/Utils.hpp"
using namespace std;
using namespace DFN;
int main()
{  /* vector<Vector3d> Coord;
    Vector3d x(0,0,1);
    Vector3d y(0,0,5);
    Vector3d z(1,0,0);
    Coord={x,y,z};
    Fracture F=Fracture(0,3,Coord);
    vector<Vector3d> Coord1;
    Vector3d x1(4,0,5);
    Vector3d y1(4,0,14);
    Vector3d z1(50,0,0);
    Coord1={x1,y1,z1};
    Fracture F2=Fracture(0,3,Coord1);
    bool flag=ControlloCentromero(F,F2, 0.000000000001);
    if(flag){
        cout<<"ehhi";
    }
    else{
        cout<<"a";
    }

    unsigned int Id=1;
    array<Vector3d,2> V;
    V[0]=x;
    V[1]=y;
    Trace T1=Trace(Id,V);

     Id=2;
    V[0]=x1;
    V[1]=y1;
    Trace T2=Trace(Id,V);

    Id=3;
    V[0]=z1;
    V[1]=z;
    Trace T3=Trace(Id,V);

    vector<Trace> t= {T3,T2,T1};
    SortLibrary::MergeSort(t);
    cout<<endl<<t[0].IdTrace;
    cout<<endl<<t[1].IdTrace;
    cout<<endl<<t[2].IdTrace;


    //anche il mergesort funziona, evviva
 
 vector<Fracture> Fractures;
    string fileName = "./DFN/FR3_data.txt";

    importFracture(fileName,Fractures);
    for(auto f : Fractures){
        cout<<"Id "<< f.IdFracture << " NumVertici " << f.NumVertices<< endl;
        for (unsigned int i = 0; i< f.NumVertices; i++){
            cout << "Vertice " <<i << " :" <<endl;
            cout  << f.CoordinatesVertices[i] << endl;
        }

    //PRIMO TEST, SONO DISTANTI STESSO TRIANGOLO DI RAGGIO MINORE DI 1
    Vector3d x(0,1,0);
    Vector3d y(-1,0,0);
    Vector3d z(1,0,0);
    vector<Vector3d>Coord={x,y,z};
    Fracture F1=Fracture(0,3,Coord);

    Vector3d x1(3,1,0);
    Vector3d y1(2,0,0);
    Vector3d z1(4,0,0);
    vector<Vector3d> Coord1={x1,y1,z1};
    Fracture F2=Fracture(0,3,Coord1);
    double tol=0.000000001;
    bool flag=ControlloCentromero(F1,F2, 0.000000000001);
    if(flag){
        cout<<"ehhi";
    }
    else{
        cout<<"a";
    }

    //SECONDO TEST, DUE QUADRATI CON DISTANZA MINIMA 0.1 CON RAGGI
    Vector3d x(-1,0,0);
    Vector3d y(0,1,0);
    Vector3d z(0,-1,0);
    Vector3d k(1,0,0);
    //QUADRATO CENTRO ZERO RAGGIO 1
    vector<Vector3d>Coord={x,y,z,k};
    Fracture F1=Fracture(0,4,Coord);

    Vector3d x1(-1,2.1,0);
    Vector3d y1(0,3.1,0);
    Vector3d z1(0,1.1,0);
    Vector3d k1(1,2.1,0);
    vector<Vector3d> Coord1={x1,y1,z1,k1};
    Fracture F2=Fracture(0,4,Coord1);
    double tol=0.000000001;
    bool flag=ControlloCentromero(F1,F2, 0.000000000001);
    if(flag){
        cout<<"ehhi";
    }
    else{
        cout<<"a";
    }
    */
    //TERZO TEST, DUE QUADRATI CHE SI INTERSECANO POCO (ANCHE SE SONO COMPLANARI)
    Vector3d x(0,0,-1);
    Vector3d y(0,1,0);
    Vector3d z(0,-1,0);
    Vector3d k(0,0,1);
    //QUADRATO CENTRO ZERO RAGGIO 1
    vector<Vector3d>Coord={x,y,z,k};
    Fracture F1=Fracture(0,4,Coord);

    Vector3d x1(0,1.9,-1);
    Vector3d y1(0,2.9,0);
    Vector3d z1(0,0.9,0);
    Vector3d k1(0,1.9,1);
    vector<Vector3d> Coord1={x1,y1,z1,k1};
    Fracture F2=Fracture(0,4,Coord1);
    double tol=0.000000001;
    bool flag=ControlloCentromero(F1,F2, 0.000000000001);
    if(flag){
        cout<<"ehhi";
    }
    else{
        cout<<"a";
    }








 return 0;
}
