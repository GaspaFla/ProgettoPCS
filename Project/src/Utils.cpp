
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
        cerr << "Errore : file non trovato";
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



bool ControlloCentromero(Frattura &F1, Frattura &F2){ //AGGIUSTA

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
        double segnoVertice=-F2.termineNotoPiano+F2.vecNormale.dot(F1.CoordinateVertici[i]);//sostituisco il punto nel piano e trovo il segno del vertice

        if(segnoVertice*segnoVerticeprec<0){

            risultato=true;//ho intersezione
            puntiFrattura[2*cont]=F1.CoordinateVertici[i];//vertice poligono che serve, uso cont che vale 0 e poi 1
            puntiFrattura[(2*cont)+1]=F1.CoordinateVertici[i-1];//vertice poligono che serve
            cont=cont+1;
            //sto salvando per due
        }
        segnoVerticeprec=segnoVertice;
        i++;
    }
    if (cont==1){
        puntiFrattura[2*cont]=F1.CoordinateVertici[F1.NumVertici-1];
        puntiFrattura[(2*cont)+1]=F1.CoordinateVertici[0];
        //cout<<"ho usato il comando"<<endl;
        }
    return risultato;
}
Traccia CalcoloTracce(Frattura &F1, Frattura &F2, unsigned int IdTraccia, double tol, vector<Vector3d>&puntiFrattura1, vector<Vector3d>&puntiFrattura2){

        //ora calcolo la retta di intersezione tra i piani
        Vector3d DirettriceDellaRettaDiIntersezione= (F1.vecNormale).cross(F2.vecNormale);
        Matrix3d VettoriDelPiano;
        for (int i=0; i<4; i++){
           /* cout<<"La prima Frattura ha come vertici passanti"<<endl;
            cout<<puntiFrattura1[i]<<endl;
            cout<<"La seconda Frattura ha come vertici passanti"<<endl;
            cout<<puntiFrattura2[i]<<endl;
*/
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
        PuntiIntersezione[i+2]=IncontroTraRette(puntiFrattura2[2*i]-puntiFrattura2[2*i+1],puntiFrattura2[2*i],DirettriceDellaRettaDiIntersezione,PuntodellaRettadiIntersezione);
        //salvo il punto della prima frattura nel posto 0, il punto della seconda frattura nel posto 2 e così via...
        //cout<<PuntiIntersezione[i]<<endl<<"I punti in cui si intersecano la prima Frattura"<<endl;
        //cout<<PuntiIntersezione[i+2]<<endl<<"I punti in cui si intersecano la seconda Frattura"<<endl;
        }
        array<unsigned int,2> PuntiInterni;
        array<bool,2> Tips;
        //CONTROLLO se i punti coincidono ovvero se la traccia è PASSANTE per ENTRAMBI

        if(((PuntiIntersezione[0]-PuntiIntersezione[2]).squaredNorm() <tol*tol || (PuntiIntersezione[0]-PuntiIntersezione[3]).squaredNorm()<tol*tol) && ((PuntiIntersezione[1]-PuntiIntersezione[2]).squaredNorm() <tol*tol || (PuntiIntersezione[1]-PuntiIntersezione[3]).squaredNorm()<tol*tol)){
            PuntiInterni = {0,1};
            Tips={false, false};
        }
        //FINE CONTROLLO
        else{
        PuntiInterni=EstremiTraccia(PuntiIntersezione, PuntodellaRettadiIntersezione,tol,Tips);
        }
        array<unsigned int, 2> IdFratture={F1.IdFrattura,F2.IdFrattura};
        if(Tips[0]){
            F1.TraccePass.push_back(IdTraccia);
        }
        else{
            F1.TracceNoPass.push_back(IdTraccia);
        }
        if(Tips[1]){
            F2.TraccePass.push_back(IdTraccia);
        }
        else{
            F2.TracceNoPass.push_back(IdTraccia);
        }

        Traccia t=Traccia(IdTraccia,{PuntiIntersezione[PuntiInterni[0]],PuntiIntersezione[PuntiInterni[1]]},IdFratture,Tips);
        return t ;
    }









Vector3d IncontroTraRette(Vector3d direzionedeiLati, Vector3d &VerticePoligono,Vector3d &direzioneretta , Vector3d &puntointersezione ){
    Vector3d P=puntointersezione-VerticePoligono;
    MatrixXd M(3, 2);
    M.col(0)=direzionedeiLati;
    M.col(1)=direzioneretta;
    Vector2d alfa=M.householderQr().solve(P);
    Vector3d PuntoDiFrattura=alfa[0]*direzionedeiLati+VerticePoligono;
    return(PuntoDiFrattura);
}

Vector3d CalcoloRetta(Frattura &F1, Frattura &F2){
    Vector3d t= F1.vecNormale.cross(F2.vecNormale);
    Matrix3d M;
    M.row(0)=F1.vecNormale;
    M.row(1)=F2.vecNormale;
    M.row(2)=t;
    Vector3d d ={F1.termineNotoPiano, F2.termineNotoPiano, 0};
    Vector3d v=M.partialPivLu().solve(d);

    return v;
}


array<unsigned int,2> EstremiTraccia(array<Vector3d,4>& PuntiIntersezione, Vector3d PuntoRetta, double tol, array<bool,2>& Tips){//controlla che il punto sulla retta non coincide con gli altri
    //So che i punti sono tutti allineati e che 0 1 sono del primo poligono e 2 3 del secondo poligono
    //Devo cercare i due punti centrali perchè saranno gli estremi della traccia
    array<unsigned int,2> PuntiInterni;
    unsigned int cont = 0;

    //Solo uno vertice coincide ==>PASSANTE per uno e NONPASSANTE per l'altro
    if((PuntiIntersezione[0]-PuntiIntersezione[2]).squaredNorm() <tol*tol ){
        PuntiInterni[0] = 0;
        if((PuntiIntersezione[0]-PuntiIntersezione[1]).squaredNorm() < (PuntiIntersezione[0]-PuntiIntersezione[3]).squaredNorm()){
            PuntiInterni[1]= 1;
            Tips = {true,false};
        }
        else {
            PuntiInterni[1] = 3;
            Tips = {false,true};

        }
    }
    else if( (PuntiIntersezione[0]-PuntiIntersezione[3]).squaredNorm()<tol*tol){
        PuntiInterni[0] = 0;
        if((PuntiIntersezione[0]-PuntiIntersezione[1]).squaredNorm() < (PuntiIntersezione[0]-PuntiIntersezione[2]).squaredNorm()){
            PuntiInterni[1]= 1;
            Tips = {true,false};
        }
        else{
            PuntiInterni[1] = 2;
            Tips = {false,true};
        }

    }
    else if((PuntiIntersezione[1]-PuntiIntersezione[2]).squaredNorm() <tol*tol){
        PuntiInterni[0] = 1;
        if((PuntiIntersezione[1]-PuntiIntersezione[0]).squaredNorm() < (PuntiIntersezione[1]-PuntiIntersezione[3]).squaredNorm()){
            PuntiInterni[1]= 0;
            Tips = {true,false};
        }
        else{
            PuntiInterni[1] = 3;
            Tips = {false,true};
        }
    }
    else if( (PuntiIntersezione[1]-PuntiIntersezione[3]).squaredNorm()<tol*tol){
        PuntiInterni[0] = 1;
        if((PuntiIntersezione[1]-PuntiIntersezione[0]).squaredNorm() < (PuntiIntersezione[1]-PuntiIntersezione[2]).squaredNorm()){
            PuntiInterni[1]= 0;
            Tips = {true,false};
        }
        else {
            PuntiInterni[1] = 2;
            Tips = {false,true};
        }

    }
    else{

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
        if(PuntiInterni[0] == 0 && PuntiInterni[1] == 1){
            Tips = {true,false};
        }
        else if(PuntiInterni[0] == 2 && PuntiInterni[1] == 3){
            Tips = {false,true};
        }
        else {
            Tips = {false,false};
        }

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
}


void Progetto1(const string& fileName, double tol){
    vector<Frattura> Fratture;
    vector<Traccia> Tracce;
    unsigned int IdTraccia=0;
    if (importoFratture(fileName, Fratture, tol)){
        for (int i = 0; i < Fratture.size()-1; i++) {
            for (int j = i+1; j < Fratture.size(); j++) {
                //mettere l'if che siano paralleli
                if(ControlloCentromero(Fratture[i], Fratture[j])){
                    vector<Vector3d> puntiFrattura1;
                    vector<Vector3d> puntiFrattura2;
                    puntiFrattura1.resize(4);
                    puntiFrattura2.resize(4);
                    //array di 4 cambia
                    if(SiIntersecano(Fratture[i],Fratture[j],puntiFrattura1) && SiIntersecano(Fratture[j],Fratture[i],puntiFrattura2)){
                    Traccia TracceAggiuntive=CalcoloTracce(Fratture[i], Fratture[j], IdTraccia, tol, puntiFrattura1, puntiFrattura2);
                    IdTraccia ++;
                    Tracce.push_back(TracceAggiuntive);
                    }
                }

            }


        }

        stampaTracce(Tracce);

    }


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

