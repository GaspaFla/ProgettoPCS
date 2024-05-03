#pragma once
#include "GeometryLibrary.hpp"


using namespace std;

namespace DFN{
void Merge(vector<Trace>& v,const unsigned int& sx,const unsigned int& cx,const unsigned int& dx);
void MergeSort(vector<Trace>& v, const unsigned int& sx, const unsigned int& dx);
void MergeSort(vector<Trace>& v);
}
