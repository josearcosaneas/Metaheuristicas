/* 
 * File:   clustering.cpp
 * Author: Jose Arcos
 * Created on 26 de marzo de 2013, 16:26
 */

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
class cluster {
    //Vector que controla los centroides
    vector < vector <float> > centroides;
    //Matriz de los centroides y los datos
    //bool **matriz;
    //Puntero a los datos
    vector <vector <float> >* dat;
    //n=numero de centroides, d=numero de datos que existen
    int n, d, dim;
    vector <int> vectorsolucion;
    float tiempoejecucion;
    
public:
    
    /*Crea la base de datos:
     *que se van a utilizar de tal forma que:
     * centroides es un vector de puntos centroides 
     * la matriz es para controlar a que datos le corresponde los centroides
     * un puntero a los datos reales
     * dos enteros para controlar cuantos centroides se quieren y la cantidad de datos
     *Para utilizarlo le introducimos la cantidad de centroides que se quieren y un puntero
     * a los datos, que seran del tipo vector < vector > >
    */
    cluster(int c,  vector < vector< float > > * _dat){
        clock_t  t1, t2;
        t1= clock();
        dat=_dat;
        //Cantidad de datos:
        d=dat->size();
        //Dimension de los datos:
        dim=dat->at(0).size();
        //Cantidad de centroides:
        n=c;
        vector <float> aux;
        //Auxiliar para hacer la dimensión de los centroides = 0
        aux.resize(dim,0);
        //Se inicializan los centroides al auxiliar
        centroides.resize(n,aux);
        //Inicializo el vector solución aleatoriamente.
        srand(time(NULL));
        for (int cont=0; cont < d ; cont++){
            int numero = rand()%c;
            vectorsolucion.push_back(numero);
        }
        t2=clock();
        tiempoejecucion=(t2-t1)/CLOCKS_PER_SEC;
    }
    
    /*
     Calcula la distancia euclidea de un dato respecto a un centroide
     */
    float distancia (int indexdat ,int indexcent){
        float sum=0;
        for (int cont=0; cont < dim; cont++){
            float aux = dat->at(indexdat).at(cont) - centroides[indexcent][cont];
            sum=sum+(aux*aux);
        }
        return sum;
    }
    
    /*
     *Calcula la distancia minima de un dato, respecto a todos los centroides
     * y devuelve un entero, que es el centroide más cercano.
     */
    int minimo (int indexdat){
        //cout << "Comprobando el minimo de: " << indexdat << endl;
        int cent=0;
        float valor=distancia(indexdat, 0);
        for (int cont=0; cont < centroides.size(); cont++ ){
            float aux= distancia(indexdat,cont);
            if (aux<valor){
                valor=aux;
                cent=cont;
            }
        }
        return cent;
    }
    
    
    //Devuelve false si no se ha actualizado ningun dato
    bool actualizarmatriz(){
        //Hacemos la matriz de incidencia con los valores correspondientes al
        //vector solución
        bool matriz[n][d];
        //En la matriz [n] se refiere a los centroides y [d] a los indices de los datos
        for (int i=0; i < n; i++){
            for (int j=0; j < d; j++){
                if (vectorsolucion[j]==i){
                    matriz[i][j]=true;
                }
                else{
                    matriz[i][j]=false;
                }
            }
        }
        //Calculamos el valor de los centroides
        for (int i=0; i<n; i++){
            int elem=0;
            vector <float> aux;
            aux.resize(dim,0);
            for (int j=0; j<d; j++){
                if (matriz[i][j]==true){
                    for (int cont=0; cont<dim; cont++){
                        aux[cont]=aux[cont]+dat->at(j)[cont];
                    }
                    elem++;
                }
            }
            if (elem!=0){
                for (int cont=0; cont<dim; cont++){
                    centroides[i][cont]=aux[cont]/elem;
                }
            }
        }
        //Buscamos si existe una distancia menor. Si es asi, el vector de soluciones se modifica, y el valor del 
        //booleano se modifica a true.
        bool modificacion;
        for (int j=0; j < d; j++ ){
            int nuevocentroide=minimo(j);
            if (nuevocentroide!=vectorsolucion[j]){
                vectorsolucion[j]=nuevocentroide;
                modificacion=true;
            }
        }
        return modificacion;
    }
    
    void ejecuta(int veces){
        int eva=0;
        bool modificacion=true;
        clock_t t1=clock();
        //La evaluacion de los datos terminará si se repite el ciclo n veces o no hay modificacion.
        while(eva<=veces||modificacion){
            modificacion=false;
            cout << "Porcentaje:" << (float)eva/(float)veces*100.0 << "%"<<endl;
            modificacion=actualizarmatriz();
            eva++;
        }
        clock_t t2= clock();
        tiempoejecucion+=(t2-t1)/CLOCKS_PER_SEC;
        cout << "J: " << valordej() << endl;
        cout << "Tiempo en ejecucion: " << tiempoejecucion << " segundos." << endl;
        //Menu de informacion
        cout << "/********************************************************/"<<endl;
        int opcion=-1;
        while (opcion!=0){
                cout << "Que tipo de información quiere que se muestre:"<< endl;
                cout << "1 - Resumen"<<endl;
                cout << "2 - Extendida" << endl;
                cout << "3 - Informacion de los clusster" << endl;
                cout << "4 - Salir" << endl;
                cout << "Opcion: ";
                cin >> opcion;
                switch(opcion){
                    case 1: mostrarinformacion();opcion=-1;break;
                    case 2: mostrarinformacionextendida();opcion=-1;break;
                    case 3: mostrarinformacionclusster(); opcion=-1;break;
                    case 4: opcion=0;break;
                    default: opcion=-1;
            }
        }
    }
    
    //Muestra el valor del vector solución
    void mostrarinformacion(){
        cout << "\n\nCentroide respecto los datos: "<<endl<<"Dato\tCentroide"<<endl;
        for (int i=0; i < vectorsolucion.size(); i++){
                cout << i << ": " << vectorsolucion[i]<< endl;
        }
        cout << endl;
    }
    //Muestra cuantos datos estan relacionados respecto al centroide
    void mostrarinformacionclusster (){
        vector <int> aux;
        aux.resize(n,0);
        for (int cont=0; cont < vectorsolucion.size(); cont++){
            aux[vectorsolucion[cont]]++;
        }
        for (int cont=0; cont < aux.size(); cont++){
            cout << "Clusster " << cont << " tiene " << aux[cont] << " elementos." << endl;
        }
        cout << "Existen " << d << " elementos" << endl;
        cout << "Distancia J= " << valordej() << endl;
    }
    //Muestra la información del valor de los centroides, y cuantos datos existen en cada centroide
    void mostrarinformacionextendida(){
        cout << "Solucion de datos:"<<endl;
        for (int cont=0; cont < n; cont++){
            cout << "Centroide " << cont << ":"<< endl;
            for (int i=0; i<dim ; i++){
                cout << centroides[cont][i] << " ";
            }
            cout << endl;
        }
        cout << "\n\nCentroide respecto los datos: "<<endl;
        for (int i=0; i < vectorsolucion.size(); i++){
                cout << i << ": " << vectorsolucion[i]<< endl;
        }
        cout << endl;
    }
    
    float valordej (){
        float sum=0;
        for (int cont=0; cont < d; cont++){
            sum+=distancia(cont,vectorsolucion[cont]);
        }
        return sum;
    }
    
};
