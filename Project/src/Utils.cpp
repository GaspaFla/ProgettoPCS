
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

bool SiIntersecano(Frattura &F1, Frattura &F2, vector<Vector3d>&puntiFrattura){

    bool risultato=false;//non si intersecano se non funziona
    int cont=0; //quante volte ho avuto intersezione retta poligono
    double segnoVerticeprec=0;//sgn del vertice predente
    int i=0;//contatore
    while (i<F1.NumVertici && cont<2){ //avanzo fino a che non guardo tutti o trovo i due lati
        double segnoVertice=F2.termineNotoPiano+F2.vecNormale.dot(F1.CoordinateVertici[i]);//sostituisco il punto nel piano e trovo il segno del vertice

        if(segnoVertice*segnoVerticeprec<0){

            risultato=true;//ho intersezione
            puntiFrattura[2*cont]=F1.CoordinateVertici[i];//vertice poligono che serve, uso cont che vale 0 e poi 1
            cout<<F1.CoordinateVertici[i]<<"questo è la coordinata del vertice numero"<<i<<endl;
            puntiFrattura[(2*cont)+1]=F1.CoordinateVertici[i-1];//vertice poligono che serve
            cout<<F1.CoordinateVertici[i-1]<<"questo è la coordinata del vertice numero"<<i-1<<endl;
            cont=cont+1;
        }
        segnoVerticeprec=segnoVertice;
        i++;
    }
    if (cont==1){
        puntiFrattura[2*cont]=F1.CoordinateVertici[F1.NumVertici-1];
        puntiFrattura[(2*cont)+1]=F1.CoordinateVertici[0];
        }
    return risultato;
}
vector<Traccia> CalcoloTracce(Frattura &F1, Frattura &F2){
    vector<Vector3d> puntiFrattura1;
    vector<Vector3d> puntiFrattura2;
    puntiFrattura1.resize(4);
     puntiFrattura2.resize(4);
    if(SiIntersecano(F1,F2,puntiFrattura1) && SiIntersecano(F2,F1,puntiFrattura2)){
        //ora calcolo la retta di intersezione tra i piani
        cout<<"1"<<endl;
        Vector3d DirettriceDellaRettaDiIntersezione= (F1.vecNormale).cross(F2.vecNormale);
        Matrix3d VettoriDelPiano;
        for (int i=0; i<4; i++){
            cout<<puntiFrattura1[i]<<endl;
            cout<<puntiFrattura2[i]<<endl;
        }

        //riempo le righe della matrice
        VettoriDelPiano.row(0)=F1.vecNormale;
        VettoriDelPiano.row(1)=F2.vecNormale;
        VettoriDelPiano.row(2)=DirettriceDellaRettaDiIntersezione;
        Vector3d TerminiNoti ={F1.termineNotoPiano, F2.termineNotoPiano, 0};

        //calcolo il punto della Retta di Intersezione
        Vector3d PuntodellaRettadiIntersezione=VettoriDelPiano.partialPivLu().solve(TerminiNoti);

        //calcolo il punto di intersezione per le rette del primo poligono
        array<Vector3d,4> PuntiIntersezione;
        for(unsigned int i=0; i<2; i++){
        PuntiIntersezione[i]=IncontroTraRette(puntiFrattura1[2*i]-puntiFrattura1[2*i+1],puntiFrattura1[2*i],DirettriceDellaRettaDiIntersezione,PuntodellaRettadiIntersezione);
        PuntiIntersezione[2*i]=IncontroTraRette(puntiFrattura1[2*i]-puntiFrattura1[2*i+1],puntiFrattura1[2*i],DirettriceDellaRettaDiIntersezione,PuntodellaRettadiIntersezione);
        cout<<PuntiIntersezione[i]<<"I punti in cui si intersecano la prima Frattura"<<endl;
        cout<<PuntiIntersezione[2*i]<<"I punti in cui si intersecano la seconda Frattura"<<endl;
        }
        vector <Traccia> t;
        return t;
    }



}
Vector3d IncontroTraRette(Vector3d t1, Vector3d &v1,Vector3d &t2 , Vector3d &v2 ){
    Vector3d P=v2-v1;
    MatrixXd M(3, 2);
    M.col(0)=t1;
    M.col(1)=-t2;
    Vector2d v=M.householderQr().solve(P);
    Vector3d x=v[0]*t1+v1;
    return(x);
}

Vector3d CalcoloRetta(Frattura &F1, Frattura &F2){
    Vector3d t= F1.vecNormale.cross(F2.vecNormale);
    cout<<t<<endl;
    Matrix3d M;
    M.row(0)=F1.vecNormale;
    M.row(1)=F2.vecNormale;
    M.row(2)=t;
    Vector3d d ={F1.termineNotoPiano, F2.termineNotoPiano, 0};
    Vector3d v=M.partialPivLu().solve(d);

    return v;
}


//FINE CODICE FLAVIO

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

