#include "Utils.hpp"
#include "GeometryLibrary.hpp"
#include "Eigen/Eigen"
#include "Sort.hpp"
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
        if(testLunghezzaLati(CoordinateVertici,tol)){//Decidi se tenere il controllo, costo molto elevato
            Frattura F(FratturaId,NumVertici,CoordinateVertici);
            Fratture.push_back(F);

        }
        else{

            Frattura F(-1);//Va bene dato che non è unsigned)
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
    if ((vecmediaF1-vecmediaF2).squaredNorm()<(raggioquadro1+raggioquadro2 + 2*sqrt(raggioquadro1)*sqrt(raggioquadro2))){ //uso il quadrato della somma dei lati perchè costa meno il doppio prodotto che fare la radice
        return true;
    }
    //confronto la distanza tra i centroidi con la somma dei raggi delle palle
    //faccio tutto al quadrato

    return false;
}


//INIZIO CODICE FLAVIO

bool SiIntersecano(Frattura &F1, Frattura &F2, array<Vector3d,4>&puntiFrattura, double tol, bool& LatoAppartiene){
    bool risultato=false;//non si intersecano se non funziona
    int cont=0; //quante volte ho avuto intersezione retta poligono
    bool primo=true;//sgn del vertice predente
    int i=0;//contatore
    double segnoVerticeprec;
    while (i<F1.NumVertici && cont<2){ //avanzo fino a che non guardo tutti o trovo i due lati
        double segnoVertice=-F2.termineNotoPiano+F2.vecNormale.dot(F1.CoordinateVertici[i]);//sostituisco il punto nel piano e trovo il segno del vertice
        if(primo){//primo punto

            segnoVerticeprec = segnoVertice;
            primo=false;
            if(abs(segnoVertice)<tol){//il primo punto sta sul piano
                double segnoVerticePrec=-F2.termineNotoPiano+F2.vecNormale.dot(F1.CoordinateVertici[F1.NumVertici-1]);
                double segnoVerticeSucc=-F2.termineNotoPiano+F2.vecNormale.dot(F1.CoordinateVertici[i+1]);
                risultato = true; //non so se va qua?
                if(abs(segnoVerticePrec)<tol){//sta sul piano anche l'ultimo
                    puntiFrattura[2*cont]=F1.CoordinateVertici[i];
                    puntiFrattura[(2*cont)+1]=F1.CoordinateVertici[i+1];
                    cont++;
                    puntiFrattura[2*cont]=F1.CoordinateVertici[F1.NumVertici-1];
                    puntiFrattura[(2*cont)+1]=F1.CoordinateVertici[F1.NumVertici-2];
                    cont++;
                    LatoAppartiene=true;

                }
                else if(abs(segnoVerticeSucc)<tol){//sta sul piano anche il secondo
                    puntiFrattura[2*cont]=F1.CoordinateVertici[F1.NumVertici-1];
                    puntiFrattura[(2*cont)+1]=F1.CoordinateVertici[i];
                    cont++;
                    puntiFrattura[2*cont]=F1.CoordinateVertici[i+1];
                    puntiFrattura[(2*cont)+1]=F1.CoordinateVertici[i+2];
                    cont++;
                    LatoAppartiene=true;
                }
                else if(segnoVerticeSucc*segnoVerticePrec>0){//tocca un solo punto
                    return false;
                }
                else{//Se sono discordi vuol dire che passa attraverso
                    //Salvo il punto 0 ed 1, ma al prossimo giro non vado a controllare 1 perchè so già che è diverso da 0 e come si comporta con il precedente
                    //incremento i di 2, una volta nell'else e una volta nell'if del primo
                    puntiFrattura[2*cont]=F1.CoordinateVertici[i+1]; //Salvo 0 ed 1 come lato che interseca
                    puntiFrattura[(2*cont)+1]=F1.CoordinateVertici[i];
                    segnoVerticeprec=segnoVerticeSucc; //Dato che non andrò a studiare 1 ma direttamente 2 il segnoprecedente sarà quello di 1 e non di 0
                    cont++;
                    i++;
                }
            }
            i++;
        }
        else if(i!=(F1.NumVertici-1)){//Se non è il primo nè l'ultimo
            if(abs(segnoVertice)<tol){//un punto diverso dal primo è sul piano ==> controllo il successivo perchè il precedente già l'ho controllato
                // per non avere problemi basta iterare su tutti i vertici escluso l'ultimo in modo da poter fare i+1


                double segnoVerticeSucc=-F2.termineNotoPiano+F2.vecNormale.dot(F1.CoordinateVertici[i+1]);
                //sto salvando la retta con il precedente due
                if(abs(segnoVerticeSucc)<tol){//guardo se anche il successivo sta sul piano
                    risultato = true;
                    if(i==F1.NumVertici-2){//se ci sta ma è l'ultimo (i ferma a quello prima) combino l'ultimo con il primo
                        puntiFrattura[2*cont]=F1.CoordinateVertici[0];
                        puntiFrattura[(2*cont)+1]=F1.CoordinateVertici[i+1];
                        cont++;
                        puntiFrattura[2*cont]=F1.CoordinateVertici[i];
                        puntiFrattura[(2*cont)+1]=F1.CoordinateVertici[i-1];
                        cont++;
                        LatoAppartiene=true;
                    }
                    else{//altrimenti combino il prossimo con quello dopo
                        puntiFrattura[2*cont]=F1.CoordinateVertici[i+2];//vertice poligono che serve, uso cont che vale 0 e poi 1
                        puntiFrattura[(2*cont)+1]=F1.CoordinateVertici[i+1];//vertice poligono che serve
                        cont++;
                        puntiFrattura[2*cont]=F1.CoordinateVertici[i];//vertice poligono che serve, uso cont che vale 0 e poi 1
                        puntiFrattura[(2*cont)+1]=F1.CoordinateVertici[i-1];
                        cont++;
                        LatoAppartiene=true;
                    }

                }
                else if(segnoVerticeSucc*segnoVerticeprec>0) {//se il secondo non è nullo allora guardo come è la situazione tra quello prima e quello dopo
                    //caso in cui abbiamo che l'intersezione è solo un punto
                    return false;

                }
                else{
                    puntiFrattura[2*cont]=F1.CoordinateVertici[i+1];
                    puntiFrattura[(2*cont)+1]=F1.CoordinateVertici[i];
                    segnoVertice=segnoVerticeSucc;
                    //ho cambiato questo perchè cambia il mio valore poi dopo
                    //Dato che non andrò a studiare 1 ma direttamente 2 il segnoprecedente sarà quello di 1 e non di 0
                    cont++;
                    i++;
                }
            }
            else if(segnoVertice*segnoVerticeprec<0){//se non è nullo controllo con il segno del precedente==> se sono discordi ho intersezione
                risultato=true;//ho intersezione
                puntiFrattura[2*cont]=F1.CoordinateVertici[i];//vertice poligono che serve, uso cont che vale 0 e poi 1
                puntiFrattura[(2*cont)+1]=F1.CoordinateVertici[i-1];//vertice poligono che serve
                cont++;

            }
            segnoVerticeprec=segnoVertice;
            i++;//mando avanti il ciclo
        }
        else{//ultimo vertice

            if(abs(segnoVertice)<tol){//Se è sul piano non devo controllare se precedente e successivo sono sul piano
                double segnoVerticeSucc=-F2.termineNotoPiano+F2.vecNormale.dot(F1.CoordinateVertici[0]);
                if(segnoVerticeSucc*segnoVerticeprec<0){
                    risultato=true;//ho intersezione
                    puntiFrattura[2*cont]=F1.CoordinateVertici[i];//vertice poligono che serve, uso cont che vale 0 e poi 1
                    puntiFrattura[(2*cont)+1]=F1.CoordinateVertici[i-1];//vertice poligono che serve
                    cont++;
                }
                else{
                    return false;
                }
            }
            else if(segnoVertice*segnoVerticeprec<0){
                risultato=true;//ho intersezione
                puntiFrattura[2*cont]=F1.CoordinateVertici[i];//vertice poligono che serve, uso cont che vale 0 e poi 1
                puntiFrattura[(2*cont)+1]=F1.CoordinateVertici[i-1];//vertice poligono che serve
                cont++;
            }
            i++;
        }

    }
    if (cont==1){
        puntiFrattura[2*cont]=F1.CoordinateVertici[F1.NumVertici-1];
        puntiFrattura[(2*cont)+1]=F1.CoordinateVertici[0];

    }
    return risultato;
}
bool CalcoloTracce(Frattura &F1, Frattura &F2, unsigned int IdTraccia, double tol, array<Vector3d,4>&puntiFrattura1, array<Vector3d,4>&puntiFrattura2,Traccia& T, bool LatoAppartiene1, bool LatoAppartiene2,  double tol2){

    //ora calcolo la retta di intersezione tra i piani
    Vector3d DirettriceDellaRettaDiIntersezione= (F1.vecNormale).cross(F2.vecNormale);
    Matrix3d VettoriDelPiano;

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

    }
    array<unsigned int,2> PuntiInterni;
    array<bool,2> Tips;
    bool fintaIntersezione = false;

    //Controllo se i punti coincidono ovvero se la traccia è PASSANTE per ENTRAMBI

    if(((PuntiIntersezione[0]-PuntiIntersezione[2]).squaredNorm() <tol2 || (PuntiIntersezione[0]-PuntiIntersezione[3]).squaredNorm()<tol2) && ((PuntiIntersezione[1]-PuntiIntersezione[2]).squaredNorm() <tol2 || (PuntiIntersezione[1]-PuntiIntersezione[3]).squaredNorm()<tol2)){
        PuntiInterni = {0,1};
        Tips={false, false};

    }
    else{
        PuntiInterni=EstremiTraccia(PuntiIntersezione, tol,Tips,fintaIntersezione, tol2);
    }
    if(LatoAppartiene1){//Allora  è passante
        Tips[0]=false;
    }
    if(LatoAppartiene2){
        Tips[1]=false;
    }
    if(!fintaIntersezione){
        array<unsigned int, 2> IdFratture={F1.IdFrattura,F2.IdFrattura};
        if(Tips[0]){
            F1.TracceNoPass.push_back(IdTraccia);
        }
        else{
            F1.TraccePass.push_back(IdTraccia);
        }
        if(Tips[1]){
            F2.TracceNoPass.push_back(IdTraccia);
        }
        else{
            F2.TraccePass.push_back(IdTraccia);
        }

        T =Traccia(IdTraccia,{PuntiIntersezione[PuntiInterni[0]],PuntiIntersezione[PuntiInterni[1]]},IdFratture,Tips);
    }
    // Se fintaIntersezione = true non è andato a buon fine quindi CalcoloTracce deve tornare false
    // Viceversa se l'intersezione è vera e quindi fintaIntersezione = false, trova la traccia e ritorna ture
    return (!fintaIntersezione);
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



array<unsigned int,2> EstremiTraccia(array<Vector3d,4>& PuntiIntersezione, double tol, array<bool,2>& Tips,bool& fintaIntersezione,  double tol2){//controlla che il punto sulla retta non coincide con gli altri
    //So che i punti sono tutti allineati e che 0 1 sono del primo poligono e 2 3 del secondo poligono
    //Devo cercare i due punti centrali perchè saranno gli estremi della traccia
    //Passo fintaIntersezione = false e se vedo che non si intersecano metto true
    array<unsigned int,2> PuntiInterni;
    unsigned int cont = 0;


    //Solo uno vertice coincide ==>PASSANTE per uno e NONPASSANTE per l'altro
    if((PuntiIntersezione[0]-PuntiIntersezione[2]).squaredNorm() <tol2 ){
        PuntiInterni[0] = 0;
        if((PuntiIntersezione[0]-PuntiIntersezione[1]).dot(PuntiIntersezione[0]-PuntiIntersezione[3])<0){//Sono da punti opposti quindi è una finta intersezione
            fintaIntersezione = true;
        }
        else if((PuntiIntersezione[0]-PuntiIntersezione[1]).squaredNorm() < (PuntiIntersezione[0]-PuntiIntersezione[3]).squaredNorm()){
            PuntiInterni[1]= 1;
            Tips = {false,true};
        }
        else {
            PuntiInterni[1] = 3;
            Tips = {true,false};

        }
    }
    else if( (PuntiIntersezione[0]-PuntiIntersezione[3]).squaredNorm()<tol2){
        PuntiInterni[0] = 0;
        if((PuntiIntersezione[0]-PuntiIntersezione[1]).dot(PuntiIntersezione[0]-PuntiIntersezione[2])<0){//Sono da punti opposti quindi è una finta intersezione
            fintaIntersezione = true;
        }
        else if((PuntiIntersezione[0]-PuntiIntersezione[1]).squaredNorm() < (PuntiIntersezione[0]-PuntiIntersezione[2]).squaredNorm()){
            PuntiInterni[1]= 1;
            Tips = {false,true};
        }
        else{
            PuntiInterni[1] = 2;
            Tips = {true,false};
        }

    }
    else if((PuntiIntersezione[1]-PuntiIntersezione[2]).squaredNorm() <tol2){
        PuntiInterni[0] = 1;
        if((PuntiIntersezione[1]-PuntiIntersezione[0]).dot(PuntiIntersezione[1]-PuntiIntersezione[3])<0){//Sono da punti opposti quindi è una finta intersezione
            fintaIntersezione = true;
        }
        else if((PuntiIntersezione[1]-PuntiIntersezione[0]).squaredNorm() < (PuntiIntersezione[1]-PuntiIntersezione[3]).squaredNorm()){
            PuntiInterni[1]= 0;
            Tips = {false,true};
        }
        else{
            PuntiInterni[1] = 3;
            Tips = {true,false};
        }
    }
    else if( (PuntiIntersezione[1]-PuntiIntersezione[3]).squaredNorm()<tol2){
        PuntiInterni[0] = 1;
        if((PuntiIntersezione[1]-PuntiIntersezione[0]).dot(PuntiIntersezione[1]-PuntiIntersezione[2])<0){//Sono da punti opposti quindi è una finta intersezione
            fintaIntersezione = true;
        }
        else if((PuntiIntersezione[1]-PuntiIntersezione[0]).squaredNorm() < (PuntiIntersezione[1]-PuntiIntersezione[2]).squaredNorm()){
            PuntiInterni[1]= 0;
            Tips = {false,true};
        }
        else {
            PuntiInterni[1] = 2;
            Tips = {true,false};
        }

    }
    else{//tutti distinti
        Vector3d punto = PuntiIntersezione[0] + PuntiIntersezione[1] +  PuntiIntersezione[2] +  PuntiIntersezione[3];

        for(unsigned int i = 0;i<3; i++){
            Vector3d u = punto-PuntiIntersezione[i];
            unsigned int numPositivi = 0;
            unsigned int numNegativi = 0;
            for(unsigned int j = 0;j<4; j++){
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
            Tips = {false,true};
        }
        else if(PuntiInterni[0] == 2 && PuntiInterni[1] == 3){
            Tips = {true,false};
        }
        else {
            if((PuntiIntersezione[PuntiInterni[0]]-PuntiIntersezione[2]).dot(PuntiIntersezione[PuntiInterni[0]]-PuntiIntersezione[3])>0){
                //Se entra nell'if vuol dire che è una finta intersezione
                fintaIntersezione = true;
            }
            else{
                Tips = {true,true};
            }
        }

    }
    return PuntiInterni;
}

bool stampaTracce( vector<Traccia>& Tracce){
    ofstream file("stampaTracce.txt");
    if(file.fail()){
        cout << "Errore";
        return false;
    }
    file<<"# Number of Traces" <<endl;
    file<<Tracce.size()<<endl;
    for(auto t : Tracce){
        file<<"# TraceId; FractureId1; FractureId2; X1; Y1; Z1; X2; Y2; Z2"<<endl;
        file<<t.IdTraccia<<"; "<<t.FrattureTraccia[0]<<"; "<<t.FrattureTraccia[1]<<"; "<<
            t.VerticiTraccia[0][0]<<"; "<<t.VerticiTraccia[0][1]<<"; "<<t.VerticiTraccia[0][2]<<"; "<<
            t.VerticiTraccia[1][0]<<"; "<<t.VerticiTraccia[1][1]<<"; "<<t.VerticiTraccia[1][2]<<endl;
    }

    file.close();
    return true;
}

//Completare quando è pronto MergeSort
bool stampaTracceFratture( vector<Frattura>& Fratture, vector<Traccia>& Tracce){
    ofstream file("stampaTracceFatture.txt");
    if(file.fail()){
        cout << "Errore";
        return false;
    }
    for(auto f : Fratture){
        if(f.IdFrattura!=-1){
            file<<"# FractureId; NumTraces "<<endl;
            bool flag = true;
            file<<f.IdFrattura<<"; "<<(f.TracceNoPass.size() + f.TraccePass.size())<<endl;

            if(!f.TraccePass.empty()){
                MergeSort(Tracce,f.TraccePass);
                if(flag){
                    file<<"# TraceId; Tips; Length" <<endl;
                    flag = false;
                }
                for(unsigned int i = 0; i<f.TraccePass.size();i++){//Passanti = false
                    file<<f.TraccePass[i]<<"; false; "<< Tracce[f.TraccePass[i]].lunghezza<<endl;
                }
            }
            if(!f.TracceNoPass.empty()){

                MergeSort(Tracce,f.TracceNoPass);
                if(flag){
                    file<<"# TraceId; Tips; Length" <<endl;
                    flag = false;
                }
                for(unsigned int i = 0; i<f.TracceNoPass.size();i++){
                    file<<f.TracceNoPass[i]<<"; true; "<< Tracce[f.TracceNoPass[i]].lunghezza<<endl;
                }
            }
        }
    }

    file.close();
    return true;
}


void Progetto1(const string& fileName,vector<Frattura>& Fratture,vector<Traccia>& Tracce, double tol,double tol2){
    unsigned int IdTraccia=0;



    if (importoFratture(fileName, Fratture, tol)){//Controllo che le fratture sia importate correttamente
        //Tracce.reserve(Fratture.size()*Fratture.size());//CAMBIARE ==> sto supponendo tutti intersecano tutti
        for (int i = 0; i < Fratture.size()-1; i++) {
            if(Fratture[i].IdFrattura!=-1){
                for (int j = i+1; j < Fratture.size(); j++) {
                    if(Fratture[j].IdFrattura!=-1){
                    //Controllo se i piani sono paralleli ==> se sono // non possono intersecarsi
                        if ((Fratture[i].vecNormale.cross(Fratture[j].vecNormale)).squaredNorm()>tol2){//Voglio che sia maggiore della tolleranza per non essere paralleli
                            //Controllo se le loro sfere approssimanti si intersecano
                            if(ControlloCentromero(Fratture[i], Fratture[j])){
                                // a questo punto potrebbero intersecarsi
                                array<Vector3d,4> puntiFrattura1;
                                array<Vector3d,4> puntiFrattura2;
                                bool LatoAppartiene1=false;
                                bool LatoAppartiene2=false;
                                if(SiIntersecano(Fratture[i],Fratture[j],puntiFrattura1,tol, LatoAppartiene1) && SiIntersecano(Fratture[j],Fratture[i],puntiFrattura2,tol, LatoAppartiene2)){
                                    Traccia TracciaAggiuntiva;
                                    bool tracciaTrovata = CalcoloTracce(Fratture[i], Fratture[j], IdTraccia, tol, puntiFrattura1, puntiFrattura2, TracciaAggiuntiva, LatoAppartiene1, LatoAppartiene2,tol2);
                                    //CalcoloTracce
                                    if(tracciaTrovata){
                                        IdTraccia ++;
                                        Tracce.push_back(TracciaAggiuntiva);
                                    }

                                }
                            }
                        }
                    }
                }
            }

        }

        stampaTracce(Tracce);
        stampaTracceFratture(Fratture,Tracce);

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


double SetTolProdotto(const double tol1D){
    double tolDefault = 10 *  numeric_limits<double>::epsilon();
    double tolProd = tol1D*tol1D;
    double tol = max(tolProd,tolDefault);
    return tol;
}
bool testLunghezzaLati(vector<Vector3d>& CoordinateVertici, double tol){
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
