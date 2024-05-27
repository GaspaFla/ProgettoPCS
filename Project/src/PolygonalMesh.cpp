#include "Utils.hpp"
#include "GeometryLibrary.hpp"
#include "PolygonalMesh.hpp"
#include "Eigen/Eigen"
#include "Sort.hpp"
#include <iostream>
#include <sstream> // controlla se servono tutte
#include <fstream>
#include <string>
using namespace std;
using namespace Eigen;

namespace LibreriaMesh{
void MeshPoligonale::GedimInterface(vector<vector<unsigned int>>& triangoli,
                                  VectorXi& materials)
    {
        const unsigned int numPolygons = VerticiCell2D.size();
        vector<vector<vector<unsigned int>>> triangleList = Triangolazione();

        unsigned int numTotalTriangles = 0;
        for(unsigned int p = 0; p < numPolygons; p++)
            numTotalTriangles += triangleList[p].size();

        triangoli.reserve(numTotalTriangles);
        materials = VectorXi::Zero(numTotalTriangles);

        unsigned int count = 0;
        for(unsigned int p = 0; p < numPolygons; p++)
        {
            for(unsigned int t = 0; t < triangleList[p].size(); t++)
            {
                triangoli.push_back(triangleList[p][t]);
                materials(count) = p;
                count++;
            }
        }
    }
}
