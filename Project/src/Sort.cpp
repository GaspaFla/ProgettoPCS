#include "Utils.hpp"
#include "GeometryLibrary.hpp"
#include "Utils.hpp"
#include "GeometryLibrary.hpp"
#include "Eigen/Eigen"
#include <iostream>
#include <sstream> // controlla se servono tutte
#include <fstream>
using namespace std;

namespace DFN{

void Merge(vector<Traccia>& v,vector<unsigned int>& vId,
           const unsigned int& sx,
           const unsigned int& cx,
           const unsigned int& dx){

    unsigned int i = sx;
    unsigned int j = cx + 1;

    //vector<Traccia> b;
    vector<unsigned int> b;
    b.reserve(dx - sx + 1);

    while( i <= cx && j <= dx)
    {
        if (v[vId[i]].lunghezza >= v[vId[j]].lunghezza)
            b.push_back(vId[i++]);
        else
            b.push_back(vId[j++]);
    }

    if (i <= cx)
        b.insert(b.end(), vId.begin() + i, vId.begin() + cx + 1);
    if ( j <= dx)
        b.insert(b.end(), vId.begin() + j, vId.begin() + dx + 1);

    copy(b.begin(), b.end(), vId.begin() + sx);

}

void MergeSort(vector<Traccia>& v,vector<unsigned int>& vId,
               const unsigned int& sx,
               const unsigned int& dx){

    if (sx < dx)
    {
        unsigned int cx = (sx + dx) / 2;
        MergeSort(v,vId, sx, cx);
        MergeSort(v,vId, cx + 1, dx);

        Merge(v,vId, sx, cx, dx);
    }

}
void MergeSort(vector<Traccia>& v,vector<unsigned int>& vId){
    MergeSort(v,vId, 0, vId.size()-1);
}
}
