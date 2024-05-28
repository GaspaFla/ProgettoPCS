#include "PolygonalMesh.hpp"
#include "Eigen/Eigen"


using namespace std;
using namespace Eigen;

namespace LibreriaMesh{

vector<vector<vector<unsigned int>>> MeshPoligonale::Triangolazione()
{
    const unsigned int numPoligoni = NumeroCell2D;
    vector<vector<vector<unsigned int>>> listaTriangoli(numPoligoni);

    for(unsigned int p = 0; p < numPoligoni; p++)
    {
        const unsigned int numVerticiPoligono = VerticiCell2D[p].size();

        for (unsigned int v = 0; v < numVerticiPoligono; v++)
        {
            const unsigned int nextVertex = VerticiCell2D[p][(v + 1) % numVerticiPoligono];
            const unsigned int nextNextVertex = VerticiCell2D[p][(v + 2) % numVerticiPoligono];

            if ((v + 2) % numVerticiPoligono == 0)
                break;

            vector<unsigned int> verticiTriangolo = {VerticiCell2D[p][0], nextVertex, nextNextVertex};

            listaTriangoli[p].push_back(verticiTriangolo);
        }
    }
    return listaTriangoli;
}


void MeshPoligonale::GedimInterface(vector<vector<unsigned int>>& triangoli,
                                  VectorXi& materiali)
    {
        const unsigned int numPolygons = VerticiCell2D.size();
        vector<vector<vector<unsigned int>>> triangleList = Triangolazione();

        unsigned int numTotalTriangles = 0;
        for(unsigned int p = 0; p < numPolygons; p++)
            numTotalTriangles += triangleList[p].size();

        triangoli.reserve(numTotalTriangles);
        materiali = VectorXi::Zero(numTotalTriangles);

        unsigned int count = 0;
        for(unsigned int p = 0; p < numPolygons; p++)
        {
            for(unsigned int t = 0; t < triangleList[p].size(); t++)
            {
                triangoli.push_back(triangleList[p][t]);
                materiali(count) = p;
                count++;
            }
        }
    }
}
