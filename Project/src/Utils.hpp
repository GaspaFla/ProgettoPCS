#pragma once
#include <iostream>
#include <vector>
#include "GeometryLibrary.hpp"
using namespace std;


namespace DFN{
//Creo una funzione che legge tutto il file e salva le fratture in un vettore
bool importoFratture(const string& fileName, vector<Frattura>& Fratture, double tol);

 //controllo che i due piani non siano paralleli
bool ControlloNonParallelo(Vector3d  &n1, Vector3d &n2, double tol);

// controllo che i due poligoni non siano vicini
bool ControlloCentromero(Frattura &F1, Frattura &F2, double tol);


bool SiIntersecano(Frattura &F1, Frattura &F2, vector<Vector3d>&puntiFrattura);

Vector3d CalcoloRetta(Frattura &F1, Frattura &F2);
Vector3d IncontroTraRette(Vector3d t1, Vector3d &v1,Vector3d &t2 , Vector3d &v2 );
vector<Traccia> CalcoloTracce(Frattura &F1, Frattura &F2);
}


double setTol1D();
double setTol2D(const double tol1D);
bool testLengthEdges(vector<Vector3d>& CoordinatesVertices, double tol);

