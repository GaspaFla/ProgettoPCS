#include "Utils.hpp"
#include "GeometryLibrary.hpp"
#include "Eigen/Eigen"
#include <iostream>
#include <sstream> // controlla se servono tutte
#include <fstream>
#include <string>
using namespace std;

namespace DFN{
bool ControlloNonParallelo(Fracture &F1, Fracture &F2, double tol){
Vector3d v1=(F1.CoordinatesVertices[0]-F1.CoordinatesVertices[1]).cross(F1.CoordinatesVertices[1]-F1.CoordinatesVertices[2]);
Vector3d v2=(F2.CoordinatesVertices[0]-F2.CoordinatesVertices[1]).cross(F2.CoordinatesVertices[1]-F2.CoordinatesVertices[2]);
//presi due punti per ogni poligono trovo un vettore che giace sul piano. Dopodichè ne calcolo la direttrice del piano
//mettici la normale al piano, passo la normale e aggiungo come attributo
if ((v1.cross(v2)).norm()<tol){//DECIDI SE USARE NORMA AL QUADRATO  E CAMBIARE TOLLERANZA
    cout<<v1(2)<<endl;
    //controllo se i due vettori sono perpendivolati tra le componenti del vettore direttore
    return false;
}
return true;




}
bool ControlloCentromero(Fracture &F1, Fracture &F2, double tol){

    Vector3d vecmediaF1= Vector3d::Zero(3);
    for (unsigned int i=0; i<F1.NumVertices; i++){
        vecmediaF1=vecmediaF1+F1.CoordinatesVertices[i];
    }
    vecmediaF1=vecmediaF1/F1.NumVertices;
     Vector3d vecmediaF2= Vector3d::Zero(3);
    for (unsigned int i=0; i<F2.NumVertices; i++){
        vecmediaF2=vecmediaF2+F2.CoordinatesVertices[i];
    }
    vecmediaF2=vecmediaF2/F2.NumVertices;

    //calcolo le medie dei centri (sono vettori)--se funziona posso ottimizzare mettendoli in un ciclo for
    double raggioquadro1=0;
    double raggioquadro2=0;
    for (unsigned int i=0; i<F1.NumVertices; i++){
    raggioquadro1=max(raggioquadro1,(F1.CoordinatesVertices[i]-vecmediaF1).squaredNorm());
    }
    for (unsigned int i=0; i<F2.NumVertices; i++){
        raggioquadro2=max(raggioquadro2,(F2.CoordinatesVertices[i]-vecmediaF2).squaredNorm());
    }
    //confronto le distanze dal centro e scelgo quella maggiore, in modo tale da definire una palla in cui sia contenuto il poligono
    if ((vecmediaF1-vecmediaF2).squaredNorm()<(sqrt(raggioquadro1)+sqrt(raggioquadro2))*(sqrt(raggioquadro1)+sqrt(raggioquadro2))){
        return true;
    }
    //confronto la distanza tra i centroidi con la somma dei raggi delle palle
    //faccio tutto al quadrato

    return false;
}

Vector4d CalcoloPiano(Vector3d &normale, Vector3d &punto){
    double d=normale.dot(punto);
    Vector4d piano;
    piano<<normale[0],normale[1],normale[2],d;
    return piano;
}

bool SiIntersecano(Vector3d &normale, vector<Vector3d>&punto, Vector3d &puntodelpiano){
    double d=normale.dot(puntodelpiano);//calcolo la mia costante nella formula ax+by+cz+d=0
    bool rs=false;//non si intersecano se non funziona
    int cont=0; //quante volte ho avuto intersezione retta poligono
    int soprasottoprec=0;//sgn del vertice predente
    int i=0;//contatore
    while (i<punto.size() || cont<2){ //avanzo fino a che non guardo tutti o trovo i due lati
    double soprasotto=d+normale.dot(punto[i]);//sostituisco il punto nel piano e trovo il segno del vertice
    if(soprasotto*soprasottoprec<0){
            cont++;
            rs=true;

     //*qua dentro posso salvare quali lati devo mettere
        }
    soprasottoprec=soprasotto;
    i++;
    }
    return rs;
     }
void CalcoloRetta(Fracture &F1, Fracture &F2){
    Vector3d t= F1.normale.cross(F2.normale);
    Matrix3d M;
    M.row(0)=F1.normale;
    M.row(1)=F2.normale;
    M.row(2)=t;
    Vector3d d={F1.d, F2.d, 0};
    Vector3d v=M.partialPivLu().solve(d);
    }
void IncontroTraRette(Vector3d &t1, Vector3d &v1,Vector3d t2 , Vector3d &v2 ){
    Vector3d P=v2-v1;
    MatrixXd M(3, 2);
    M.col(0)=t1;
    M.col(1)=-t2;
    Vector2d v=M.partialPivLu().solve(P);

    Vector3d x=v[0]*t1+v1;
}

bool importFracture(const string& fileName, vector<Fracture>& Fractures){
    // Passo un vettore vuoto che vado a riempire con le fratture se la lettura del file va a buon fine
    ifstream file(fileName);
    if(file.fail()){
        cout << "Error : file not found";
        return false;
    }
    // Sto assumendo che il formato del file è corretto ==> VALUTA SE AGGIUNGERE CONTROLLI
    string header; // La prima riga va saltata
    getline(file,header);
    string line;
    getline(file,line);
    unsigned int NumFractures = 0;
    stringstream convert(line);
    convert >> NumFractures;
    Fractures.reserve(NumFractures); //?
    //Ora letto tutte le righe e mi salvo le fratture
    for(unsigned int i = 0; i< NumFractures; i++){
        char c;
        unsigned int FractureId;
        unsigned int NumVertices;
        getline(file,line);//Una riga va saltata
        getline(file,line);
        stringstream convert(line);
        string s;
        convert>> FractureId >> c >>NumVertices;
        getline(file,line);
        vector<Vector3d> CoordinatesVertices ;
        CoordinatesVertices.resize(NumVertices);   //se metti reserve esplode
        for(unsigned int j = 0; j<3; j++){
            getline(file,line);
            stringstream convert(line);
            convert >>CoordinatesVertices[0][j];
            for(unsigned int k = 1; k<NumVertices; k++){
                convert>>c>>CoordinatesVertices[k][j];
                }
        }
        Fracture F(FractureId,NumVertices,CoordinatesVertices);
        Fractures.push_back(F);


    }

    file.close();
    return true;
}

}

#include <vector>
using namespace DFN;
namespace SortLibrary {//*metti DFN

void Merge(vector<Trace>& v,
           const unsigned int& sx,
           const unsigned int& cx,
           const unsigned int& dx){

    unsigned int i = sx;
    unsigned int j = cx + 1;

    vector<Trace> b;
    b.reserve(dx - sx + 1);

    while( i <= cx && j <= dx)
    {
        if ((v[i].VerticesTrace[0]-v[i].VerticesTrace[1]).squaredNorm() <= (v[j].VerticesTrace[0]-v[j].VerticesTrace[1]).squaredNorm() )
            b.push_back(v[i++]);
        else
            b.push_back(v[j++]);
    }

    if (i <= cx)
        b.insert(b.end(), v.begin() + i, v.begin() + cx + 1);
    if ( j <= dx)
        b.insert(b.end(), v.begin() + j, v.begin() + dx + 1);

    copy(b.begin(), b.end(), v.begin() + sx);

}

void MergeSort(vector<Trace>& v,
               const unsigned int& sx,
               const unsigned int& dx){

    if (sx < dx)
    {
        unsigned int cx = (sx + dx) / 2;
        MergeSort(v, sx, cx);
        MergeSort(v, cx + 1, dx);

        Merge(v, sx, cx, dx);
    }

}
void MergeSort(vector<Trace>& v){
    MergeSort(v, 0, v.size()-1);
}
}
