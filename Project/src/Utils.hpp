#pragma once
#include <iostream>
#include <vector>
#include "GeometryLibrary.hpp"
using namespace std;


namespace DFN{
//Creo una funzione che legge tutto il file e salva le fratture in un vettore
bool importoFratture(const string& fileName, vector<Frattura>& Fratture, double tol);


// controllo che i due poligoni non siano vicini
bool ControlloCentromero(Frattura &F1, Frattura &F2);

bool SiIntersecano(Frattura &F1, Frattura &F2, array<Vector3d,4>&puntiFrattura, double tol);

bool stampaTracceFatture( vector<Frattura>& Fratture, vector<Traccia>& Tracce);
bool stampaTracce( vector<Traccia>& Tracce);

Vector3d CalcoloRetta(Frattura &F1, Frattura &F2);
Vector3d IncontroTraRette(Vector3d t1, Vector3d &v1,Vector3d &t2 , Vector3d &v2 );
Traccia CalcoloTracce(Frattura &F1, Frattura &F2, unsigned int IdTraccia, double tol, array<Vector3d,4>&puntiFrattura1, array<Vector3d,4>&puntiFrattura2);
array<unsigned int,2> EstremiTraccia(array<Vector3d,4>& PuntiIntersezione, Vector3d PuntoRetta, double tol, array<bool,2>& Tips);
void Progetto1(const string& fileName, double tol);
}


double setTol1D();
double setTol2D(const double tol1D);
bool testLengthEdges(vector<Vector3d>& CoordinatesVertices, double tol);

