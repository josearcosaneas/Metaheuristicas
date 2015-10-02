
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

BL::BL(int c,  vector < vector< float > > * _dat){
        clock_t  t1, t2;
        t1= clock();
        dat=_dat;
        //Cantidad de datos:
        d=dat->size();
        //Dimension de los datos:
        dim=dat->at(0).size();
        //Cantidad de centroides:
        k=c;
        vector <float> aux;
        //Auxiliar para hacer la dimensión de los centroides = 0
        aux.resize(dim,0);
        //Se inicializan los centroides al auxiliar
        centroides.resize(k,aux);
        //Inicializo el vector solución aleatoriamente.
        srand(time(NULL));
        for (int cont=0; cont < d ; cont++){
            int numero = rand()%c;
            vectorsolucion.push_back(numero);
        }
        t2=clock();
        tiempoejecucion=(t2-t1);
}

float BL::getJ(){
    return J;
}

float BL::getTime(){
    return tiempoejecucion/CLOCKS_PER_SEC;
}
vector < vector < float> > BL::calcularcentroides(vector < int > &vs){
    vector < vector <float> > cent;
    vector < float> aux;
    aux.resize(dim ,0.0);
    cent.resize(k,aux);
    int cont[k];
    for (int i=0; i < k; i++){
        cont[i]=0;
    }
    for (int i=0; i < vs.size() ; i++){
        for (int j=0; j < dim; j++){
            cent[vs[i]][j]=cent[vs[i]][j]+dat->at(i)[j];
        }
        cont[vs[i]]++;

    }
    for (int i=0; i< k; i++){
        for (int j=0; j<dim; j++){
            if (cont[i]!=0){
                cent[i][j]=cent[i][j]/cont[i];
            }
        }
    }
    return cent;
}

float BL::calcularJ(vector < int > &vs){
    float suma=0.0;
    vector< vector <float > > centroidesaux= calcularcentroides(vs);
    for (int i=0; i < vs.size(); i++){
        for (int j=0; j < dim; j++){
                float aux= centroidesaux[vs[i]][j] - dat->at(i)[j];
                suma=suma+(aux*aux);
        }
    }
    return suma;
}

float BL::gettiempoejecucion(){
    return tiempoejecucion;
}

void BL::ejecuta(int veces){
    cout << "Cantidad de datos: " << d << "\nDimension: " << dim << "\nPorcentaje: 0%" <<endl;
    clock_t t1,t2,t3, t4;
    t1=clock();
    J=calcularJ(vectorsolucion);
    centroides=calcularcentroides(vectorsolucion);
    int eva=0;
    bool mejora=true;
    bool mens=true;
    while (eva<veces && mejora){
        t3=clock();
        mejora=false;
        for (int i=0; i < d; i++){
            vector <int> vectorsolucionp;
            vectorsolucionp=vectorsolucion;
            int cluster_actual=vectorsolucion[i];
            int m_cluster=cluster_actual;
            vector < vector < float> > centroidesp(centroides);
            float mJp=3.4028235e38f;
            for (int j=0; j<k; j++){
                if (j!=cluster_actual){
                    vectorsolucionp[i]=j;
                    centroidesp=calcularcentroides(vectorsolucionp);
                    float Jp=calcularJ(vectorsolucionp);
                    if (Jp<mJp){
                        mJp=Jp;
                        m_cluster=j;

                    }
                    eva++;
                }
            }
            if (mJp<J){
                mejora=true;
                vectorsolucion[i]=m_cluster;
                J=mJp;
                centroides=calcularcentroides(vectorsolucion);
            }
            t4=clock();
            float porc=(float)eva/(float)veces*100.0;
            if (porc<100)
                cout << "Porcentaje: " << porc<< "% ---- tiempo estimado: "<< ((t4-t3)/CLOCKS_PER_SEC)*(100.0-((float)eva/(float)veces)*100.0) << " seg"  << endl;
            else if (mens){
                cout << "Porcentaje: 100"<< "% ---- tiempo estimado: 0 seg"  << endl;
                mens=false;
            }

        }

        }
    t2=clock();
    tiempoejecucion+=t2-t1;
    cout << "J= " << getJ() << endl << "Tiempo:" << getTime() << "s." << endl;
}
