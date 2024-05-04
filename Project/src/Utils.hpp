#pragma once
#include <iostream>
#include <vector>
#include "GeometryLibrary.hpp"
using namespace std;


namespace DFN{
//Creo una funzione che legge tutto il file e salva le fratture in un vettore
bool importFracture(const string& fileName, vector<Fracture>& Fractures, double tol);

 //controllo che i due piani non siano paralleli
bool ControlloNonParallelo(Vector3d  &n1, Vector3d &n2, double tol);

// controllo che i due poligoni non siano vicini
bool ControlloCentromero(Fracture &F1, Fracture &F2, double tol);


bool SiIntersecano(Vector4d &piano, vector<Vector3d>&punto);

void CalcoloRetta(Fracture &F1, Fracture &F2);
void IncontroTraRette(Vector3d t1, Vector3d v1,Vector3d t2 , Vector3d v2 );


}

double setTol1D();
double setTol2D(const double tol1D);
bool testLengthEdges(vector<Vector3d>& CoordinatesVertices, double tol);
