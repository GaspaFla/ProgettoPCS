#include <iostream>
#include "GeometryLibrary.hpp"
#include "Utils.hpp"
#include <sstream> // controlla se servono tutte
#include <fstream>
#include <Eigen/Eigen>


using namespace std;

namespace DFN{
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




bool ControlloNonParallelo(Vector3d  &n1, Vector3d &n2, double tol){
    if ((n1.cross(n2)).norm()<tol){//DECIDI SE USARE NORMA AL QUADRATO  E CAMBIARE TOLLERANZA
        cout<<n1(2)<<endl;
        //controllo se i due vettori sono perpendivolati tra le componenti del vettore direttore
        return false;
    }
    return true;
}

bool ControlloCentromero(Fracture &F1, Fracture &F2, double tol){ //AGGIUSTA

    Vector3d vecmediaF1= Vector3d::Zero(3);
    for (unsigned int i=0; i<F1.NumVertices; i++){
        vecmediaF1=vecmediaF1+F1.CoordinatesVertices[i];
    }
    //**fai la media scemo
    Vector3d vecmediaF2= Vector3d::Zero(3);
    for (unsigned int i=0; i<F2.NumVertices; i++){
        vecmediaF2=vecmediaF2+F2.CoordinatesVertices[i];
    }

    vecmediaF1 = vecmediaF1/3; //?
    vecmediaF2 = vecmediaF2/3;

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
    if ((vecmediaF1-vecmediaF2).squaredNorm()<raggioquadro1+raggioquadro2){
        return true;
    }
    //confronto la distanza tra i centroidi con la somma dei raggi delle palle
    //faccio tutto al quadrato

    return false;
}

//Introduco delle tolleranze per poter fare i controlli e i confronti
//Uso una funzione perchè viene chiamata una sola volta all'inizio del progremma ==> una volta settata resta quella per coerenza
double setTol1D(){
    double tolDefault = 10 *  numeric_limits<double>::epsilon();
    double tolInput;
    cout << "Inserire tolleranza 1D";
    cin >> tolInput;
    return max(tolDefault,tolInput);
}

double setTol2D(const double tol1D){
    double tolDefault = 10 *  numeric_limits<double>::epsilon();
    double tolInput;
    cout << "Inserire tolleranza 2D";
    cin >> tolInput;
    double tol2D = pow(tol1D,2)*sqrt(3)/4; // NON SI USA pow perchè costa tanto la chiamata a funzione ==> SOSTITUISCI CON IL PRODOTTO
    double tol = max(tol2D,tolDefault);
    return max(tol,tolInput);
}



}
