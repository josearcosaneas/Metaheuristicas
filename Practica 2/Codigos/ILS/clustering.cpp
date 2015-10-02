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
    //Puntero a los datos
    vector <vector <float> >* dat;
    //n=numero de centroides, d=numero de datos que existen
    int k, d, dim;
    vector <int> vectorsolucion;
    float tiempoejecucion;
    float J;

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

    float calcularj (){
        float sum=0;
        for (int cont=0; cont < d; cont++){
            sum+=distancia(cont,vectorsolucion[cont]);
        }
        return sum;
    }

    float calcularj (vector<int>& vect){
        float sum=0;
        for (int cont=0; cont < vect.size(); cont++){
            sum+=distancia(cont,vect[cont]);
        }
        return sum;
    }


    void calcularcentroides(){
        cout << "Entra... "<<endl;
        vector <float> aux;
        aux.resize(dim,0);
        centroides.resize(k,aux);
        aux.resize(k,0);
        for (int i=0; i<d; i++){
            for (int j=0; j<dim; j++){
                centroides[vectorsolucion[i]][j]=centroides[vectorsolucion[i]][j]+dat->at(vectorsolucion[i])[j];
                cout << centroides[vectorsolucion[i]][j] << endl;
            }
            aux[vectorsolucion[i]]=aux[vectorsolucion[i]]+1;
        }
        cout << "Ha salido..." << endl;
        for (int i=0; i < k; i++){
            for (int j=0; j < dim; j++)
                centroides[i][j]=centroides[i][j]/aux[i];
        }
    }

    void ejecuta (int veces){
        int cluster_actual;
        J=calcularj();
        bool mejora=true;
        int eval=0;
        while (eval<veces && mejora){
            mejora=false;
            for (int i=0; i <= d; i++){
                cluster_actual=vectorsolucion[i];
                //cout << cluster_actual << "\t";
                float Jp= 3.4028235e38f;
                for (int j=0; j<=k; j++){
                    vector <int> vectorsolucionp(vectorsolucion);
                    //cout << vectorsolucion.size() << "----" << vectorsolucionp.size() << endl;
                    //vectorsolucionp.resize(vectorsolucion);
                    //vectorsolucionp=vectorsolucion;
                    if (j!=cluster_actual){
                        //cout << j << "-//-" << cluster_actual << endl;
                        vectorsolucionp[i]=j;
                        cout << "Listado del vector solucion:" << endl;
                        for (int cont=0; cont < vectorsolucionp.size(); cont++)
                            cout << vectorsolucion[cont]<<","<<vectorsolucionp[cont]<<endl;
                        float Jpc=calcularj(vectorsolucionp);
                        cout << "---->" << Jpc << " < ?" << Jp << endl;
                        if (Jpc<Jp){
                            Jp=Jpc;
                            cluster_actual=j;
                        }
                    }
                    cout << "ppppp"<< endl;
                    cout << Jp << "...." << J<<endl;
                    if (Jp<J){
                        cout << "o, o... puede haber un problemas... vamos a ver..."<<endl;
                        mejora=true;
                        vectorsolucion[i]=cluster_actual;
                        J=Jp;
                        calcularcentroides();
                    }
                }
            }
            cout << "Porcentaje: " << eval/veces*100.0 << "%" << endl;
            eval++;
        }

    }
};
