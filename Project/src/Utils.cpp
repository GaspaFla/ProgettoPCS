
#include "Utils.hpp"
#include "GeometryLibrary.hpp"
#include "Eigen/Eigen"
#include <iostream>
#include <sstream> // controlla se servono tutte
#include <fstream>
#include <string>
using namespace std;

namespace DFN{
bool importoFratture(const string& fileName, vector<Frattura>& Fratture,double tol){
    // Passo un vettore vuoto che vado a riempire con le fratture se la lettura del file va a buon fine
    ifstream file(fileName);
    if(file.fail()){
        cout << "Errore : file non trovato";
        return false;
    }
    // Sto assumendo che il formato del file è corretto ==> VALUTA SE AGGIUNGERE CONTROLLI
    string testa; // La prima riga va saltata
    getline(file,testa);
    string line;
    getline(file,line);
    unsigned int NumFratture = 0;
    stringstream convert(line);
    convert >> NumFratture;
    Fratture.reserve(NumFratture); //?
    //Ora letto tutte le righe e mi salvo le fratture
    for(unsigned int i = 0; i< NumFratture; i++){
        char c;
        unsigned int FratturaId;
        unsigned int NumVertici;
        getline(file,line);//Una riga va saltata
        getline(file,line);
        stringstream convert(line);
        convert>> FratturaId >> c >>NumVertici;
        getline(file,line);
        vector<Vector3d> CoordinateVertici ;
        CoordinateVertici.resize(NumVertici);   //se metti reserve esplode
        for(unsigned int j = 0; j<3; j++){
            getline(file,line);
            stringstream convert(line);
            convert >>CoordinateVertici[0][j];
            for(unsigned int k = 1; k<NumVertici; k++){
                convert>>c>>CoordinateVertici[k][j];
            }
        }
        if(testLengthEdges(CoordinateVertici,tol)){//Decidi se tenere il controllo, costo molto elevato
            Frattura F(FratturaId,NumVertici,CoordinateVertici);
            Fratture.push_back(F);

        }
    }

    file.close();
    return true;
}



bool ControlloCentromero(Frattura &F1, Frattura &F2, double tol){ //AGGIUSTA

    Vector3d vecmediaF1= Vector3d::Zero(3);
    for (unsigned int i=0; i<F1.NumVertici; i++){
        vecmediaF1=vecmediaF1+F1.CoordinateVertici[i];
    }

    Vector3d vecmediaF2= Vector3d::Zero(3);
    for (unsigned int i=0; i<F2.NumVertici; i++){
        vecmediaF2=vecmediaF2+F2.CoordinateVertici[i];
    }

    vecmediaF1 = vecmediaF1/F1.NumVertici;
    vecmediaF2 = vecmediaF2/F2.NumVertici;

    //calcolo le medie dei centri (sono vettori)--se funziona posso ottimizzare mettendoli in un ciclo for
    double raggioquadro1=0;
    double raggioquadro2=0;
    for (unsigned int i=0; i<F1.NumVertici; i++){
        raggioquadro1=max(raggioquadro1,(F1.CoordinateVertici[i]-vecmediaF1).squaredNorm());
    }
    for (unsigned int i=0; i<F2.NumVertici; i++){
        raggioquadro2=max(raggioquadro2,(F2.CoordinateVertici[i]-vecmediaF2).squaredNorm());
    }
    //confronto le distanze dal centro e scelgo quella maggiore, in modo tale da definire una palla in cui sia contenuto il poligono
    if ((vecmediaF1-vecmediaF2).squaredNorm()<(raggioquadro1+raggioquadro2 + 2*raggioquadro1*raggioquadro2)){ //uso il quadrato della somma dei lati perchè costa meno il doppio prodotto che fare la radice
        return true;
    }
    //confronto la distanza tra i centroidi con la somma dei raggi delle palle
    //faccio tutto al quadrato

    return false;
}


//INIZIO CODICE FLAVIO

bool SiIntersecano(Vector3d &normale, vector<Vector3d>&punto, Vector3d &puntodelpiano){
    double d = normale.dot(puntodelpiano);//calcolo la mia costante nella formula ax+by+cz+d=0
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
void CalcoloRetta(Frattura &F1, Frattura &F2){
    Vector3d t= F1.vecNormale.cross(F2.vecNormale);
    Matrix3d M;
    M.row(0)=F1.vecNormale;
    M.row(1)=F2.vecNormale;
    M.row(2)=t;
    Vector3d d ={F1.termineNotoPiano, F2.termineNotoPiano, 0};
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

//FINE CODICE FLAVIO


array<unsigned int,2> EstremiTraccia(array<Vector3d,4>& PuntiIntersezione, Vector3d PuntoRetta){//controlla che il punto sulla retta non coincide con gli altri
    //So che i punti sono tutti allineati e che 0 1 sono del primo poligono e 2 3 del secondo poligono
    //Devo cercare i due punti centrali perchè saranno gli estremi della traccia
    array<unsigned int,2> PuntiInterni;
    unsigned int cont = 0;
    for(unsigned int i = 0;i<3; i++){//aggiusta indici
        Vector3d u = PuntoRetta-PuntiIntersezione[i];
        unsigned int numPositivi = 0;
        unsigned int numNegativi = 0;
        for(unsigned int j = 0;j<4; j++){//aggiusta indici
            if(j!=i){
                Vector3d v = PuntiIntersezione[j]-PuntiIntersezione[i];
                if(u.dot(v)>0)
                    numPositivi++;
                else numNegativi++;
            }
        }
        if(numNegativi == 2 || numPositivi == 2){
            PuntiInterni[cont] = i;
            cont++;
        }
    }
    if(cont == 1){//Risparmio di fare un ciclo del for
        PuntiInterni[1] = 3;
    }
    return PuntiInterni;
}

bool stampaTracce( vector<Traccia> Tracce){
    ofstream file("stampaTracce.txt");
    if(file.fail()){
        cout << "Errore";
        return false;
    }
    file<<"Number of Traces" <<endl;
    file<<Tracce.size()<<endl;
    file<<"TraceId; FractureId1; FractureId2; X1; Y1; Z1; X2; Y2; Z2"<<endl;
    for(auto t : Tracce){
        file<<t.IdTraccia<<"; "<<t.FrattureTraccia[0]<<"; "<<t.FrattureTraccia[1]<<"; "<<
            t.VerticiTraccia[0][0]<<"; "<<t.VerticiTraccia[0][1]<<"; "<<t.VerticiTraccia[0][2]<<"; "<<
            t.VerticiTraccia[1][0]<<"; "<<t.VerticiTraccia[1][1]<<"; "<<t.VerticiTraccia[1][2]<<endl;
    }

    file.close();
    return true;
}

//Completare quando è pronto MergeSort
bool stampaTracceFatture( vector<Frattura> Fratture, vector<Traccia> Tracce){
    ofstream file("stampaTracceFatture.txt");
    if(file.fail()){
        cout << "Errore";
        return false;
    }

    for(auto f : Fratture){
        file<<"FractureId; NumTraces "<<endl;
        file<<"TraceId; Tips; Length" <<endl;
        // Chiamare MergeSort
        for(unsigned int i = 0; i<f.TraccePass.size();i++){
            //Stampa
        }
        for(unsigned int i = 0; i<f.TracceNoPass.size();i++){
            //Stampa
        }

    }

    file.close();
    return true;
}
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
    double tol2D = tol1D*tol1D*sqrt(3)/4;
    double tol = max(tol2D,tolDefault);
    return max(tol,tolInput);
}


bool testLengthEdges(vector<Vector3d>& CoordinateVertici, double tol){
    for(unsigned int i = 0;i<(CoordinateVertici.size()-1);i++){
        for(unsigned int j = i+1;j<CoordinateVertici.size();j++){
            if((CoordinateVertici[i]-CoordinateVertici[j]).norm() < tol){
                cout<<"Errore: due vertici della frattura coincidono"<<endl;
                return false;
            }
        }
    }
    return true;
}

