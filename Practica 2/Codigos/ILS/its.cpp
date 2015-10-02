#include <cstdlib>
#include "its.h"
#include "BL.h"
#include <cstdlib>
#include <iostream>
#include <vector>
#include <math.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include "time.h"

using namespace std;

/*
 *
 */
ITS::ITS(int _veces, int _k, int _itera, vector < vector <float> > * _dat){
    veces=_veces-1;
    k=_k;
    dat=_dat;
    iteracionesbl=_itera;
    BL* aux;
    aux= new BL(k, dat);
    aux->ejecuta(iteracionesbl);
    J=aux->getJ();
    solucion=aux->getvectorsolucion();
    cout << (1.0/(float)_veces)*100.0 <<"%" << endl;
    delete aux;

}
void ITS::ejecutar(){
    BL* aux;
    aux= new BL(k, dat);
    float auxJ;
    for (int cont=1; cont<veces; cont++ ){
        aux->setvectorsolucion(mutar(solucion));
        aux->ejecuta(iteracionesbl);
        auxJ=aux->getJ();
        if (J>auxJ){
            J=auxJ;
            solucion=aux->getvectorsolucion();
        }
        cout << (float)(cont+1)/(float)veces*100.0<<"%" << endl;
    }
    cout << "J=" << J << endl;
}

vector <int> ITS::mutar(vector<int> sol){
    for (int cont=0; cont < sol.size(); cont++){
        if((rand()%100)>60){
            sol[cont]=(rand()%k);
        }
    }
    return sol;
}
