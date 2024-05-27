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

bool SiIntersecano(Frattura &F1, Frattura &F2, array<Vector3d,4>&puntiFrattura,double tol, bool& LatoAppartiene);

Vector3d IncontroTraRette(Vector3d& t1, Vector3d &v1,Vector3d &t2 , Vector3d &v2 );

bool CalcoloTracce(Frattura &F1, Frattura &F2, unsigned int IdTraccia, double tol, array<Vector3d,4>&puntiFrattura1, array<Vector3d,4>&puntiFrattura2,Traccia&T, bool& LatoAppartiene1, bool& LatoAppartiene2,  double tol2);

array<unsigned int,2> EstremiTraccia(array<Vector3d,4>& PuntiIntersezione,  double tol, array<bool,2>& Tips, bool& fintaIntersezione,  double tol2);

void Progetto1(const string& fileName,vector<Frattura>& Fratture,vector<Traccia>& Tracce, double tol,double tol2);

bool stampaTracceFratture( vector<Frattura>& Fratture, vector<Traccia>& Tracce);
bool stampaTracce( vector<Traccia>& Tracce);
}


double setTol1D();
double SetTolProdotto(const double tol1D);
bool testLunghezzaLati(vector<Vector3d>& CoordinatesVertices, double tol);
