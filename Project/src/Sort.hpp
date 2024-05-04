#pragma once
#include "GeometryLibrary.hpp"


using namespace std;

namespace DFN{
void Merge(vector<Traccia>& v,const unsigned int& sx,const unsigned int& cx,const unsigned int& dx);
void MergeSort(vector<Traccia>& v, const unsigned int& sx, const unsigned int& dx);
void MergeSort(vector<Traccia>& v);
}
