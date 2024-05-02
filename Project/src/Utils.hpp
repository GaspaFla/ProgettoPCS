#pragma once
#include "GeometryLibrary.hpp"

using namespace std;

namespace DFN{
bool ControlloNonParallelo(Fracture F1, Fracture F2, double tol); //controllo che i due piani non siano paralleli
bool ControlloCentromero(Fracture F1, Fracture F2, double tol);// controllo che i due poligoni non siano vicini




}
