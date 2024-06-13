
#include <vector>
#include "GeometryLibrary.hpp"
#include "Utils.hpp"
using namespace std;



namespace DFN{
    void Taglia(Frattura& F,Frattura & FMadre,vector<Traccia>& Tracce, double tol,double tol2);
    vector<Frattura> calcoloSottoPoligoniPass(Frattura& F,double tol, double tol2,bool& TracciaSulBordo, vector<Traccia>& Tracce, Frattura& FMadre);
    vector<Frattura> calcoloSottoPoligoniNoPass(Frattura& F,double tol, double tol2, vector<Traccia>& Tracce, Frattura& FMadre, bool& BassaTolleranza);
    void converteInCelle(Frattura& F,Frattura& FMadre);
    unsigned int RicercaIdVertice(Frattura& FMadre, Vector3d PuntodaControllare, double tol2);
    bool stampaMesh(vector<Frattura>& Fratture);
    void Progetto2(vector<Frattura>& Fratture,vector<Traccia>& Tracce, double tol,double tol2);
    void esportaMesh(Frattura& F);
}
