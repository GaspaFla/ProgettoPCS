#pragma once
#include "GeometryLibrary.hpp"

using namespace std;

namespace DFN{
bool ControlloNonParallelo(Fracture &F1, Fracture &F2, double tol); //controllo che i due piani non siano paralleli
bool ControlloCentromero(Fracture &F1, Fracture &F2, double tol);// controllo che i due poligoni non siano vicini
Vector4d CalcoloPiano(Vector3d &normale, Vector3d &punto);
bool SiIntersecano(Vector4d &piano, vector<Vector3d>&punto);
bool importFracture(const string& fileName, vector<Fracture>& Fractures);
void CalcoloRetta(Fracture &F1, Fracture &F2);
void IncontroTraRette(Vector3d t1, Vector3d v1,Vector3d t2 , Vector3d v2 );

}
using namespace DFN;
namespace SortLibrary{
void Merge(vector<Trace>& v,const unsigned int& sx,const unsigned int& cx,const unsigned int& dx);
void MergeSort(vector<Trace>& v, const unsigned int& sx, const unsigned int& dx);
void MergeSort(vector<Trace>& v);
}
