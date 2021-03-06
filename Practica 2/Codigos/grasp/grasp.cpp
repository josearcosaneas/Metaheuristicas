#include <vector>
#include <list>
#include "random.h"
#include <time.h>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <cassert>
#include <iostream>
using namespace std;

double diferencia (vector<double>  &A,vector<double> &B){
    assert(A.size()==B.size());
    double suma = 0;
    for (int i = 0; i < A.size();i++){
        suma += (A[i]-B[i])*(A[i]-B[i]);
       }
    return suma;
}

double J (vector<list<int> > &C,vector<vector<double> > &datos, vector<vector<double> > &z){

    double suma = 0;

    for (int i = 0; i < C.size(); i++){
        for (std::list<int>::const_iterator it = C[i].begin();it != C[i].end(); ++it)
            suma+=diferencia(datos[*it],z[i]);
    }

    return suma;
}

void BL(int k,vector<list<int> > &C, vector<vector<double> > &datos,
                      vector<vector<double> >&z){
    bool fin = false;
    vector<int> m(k,0);//número de elementos de cada cluster

    //Calculo de centroides
    for (int i = 0; i < k; i++){ //Para cada cluster
        for (std::list<int>::iterator it = C[i].begin();it != C[i].end(); ++it){
            for (int t = 0; t < z[i].size();t++)
                z[i][t]+=datos[*it][t];
        }

        for (int t = 0; t < z[i].size();t++){
            z[i][t]/=C[i].size();
            m[i] = C[i].size();
        }
    }
    cout << "Solución inicial: " << (C,datos,z) << endl;

    double A, B, JP;
    long double minB=100000000000000;
    int q, evaluaciones=0, max_evaluaciones = 10000;
    std::list<int>::iterator it;
    do{
        fin = true;
        for (int i = 0; i < k; i++){
            it=C[i].begin();
            while(it != C[i].end()){
                A = m[i]*diferencia(datos[*it], z[i])/(m[i]-1);
                for (int j = 0; j < k; j++){
                    if (i != j){
                        evaluaciones++;
                        B = m[j]*diferencia(datos[*it], z[j])/(m[j]+1);
                        if (B < minB){
                            minB = B;
                            q=j;
                        }
                    }
                }
                JP = minB - A;
                if (JP < 0){
                    for (int t = 0; t < z[i].size();t++){
                        z[i][t]= (m[i]*z[i][t] - datos[*it][t]) /(m[i]-1);
                        z[q][t]= (m[q]*z[q][t] + datos[*it][t]) /(m[q]+1);
                    }
                    C[q].push_back(*it);
                    it = C[i].erase(it);
                    m[i]--;
                    m[q]++;
                    fin = false;
                }else ++it;
                minB=100000000000000;
            }
        }
    }while (!fin && evaluaciones <= max_evaluaciones);
}

vector<list<int> > generaSolInicial(int n_elem, int k, int Seed){
    vector<list<int> > C(k,list<int>());
    //Generar solución aleatoria
    for (int i = 0; i < n_elem; i++)
        C[Randint(0,k-1)].push_back(i);
    for (int i = 0; i < k; i++)
        assert(!C[i].empty());
return C;
}



int Seed;

inline bool busca(vector<int> &A, int b){
    for(int i=0; i<A.size(); i++)
        if(A[i]==b) return true;
    return false;
}

// Centroide más cercano a un patrón p
inline int CMC(int p, vector<vector<double> > &datos, vector<vector<double> > &z){
    double dm=diferencia(datos[p],z[0]), d;
    int c=0;
    for(int i=0; i<z.size(); i++){
        d=diferencia(z[i],datos[p]);
        if(d<dm){
            dm=d;
            c=i;
        }
    }
    return c;
}

void solGreedy(int k,vector<list<int> > &C, vector<vector<double> > &datos,
                      vector<vector<double> >&z){
    C= vector<list<int> > (k,list<int>());
    z= vector<vector<double> > (k,vector<double>(datos[0].size(),0));
    vector<double> media(datos[0].size(),0); //Punto media
    vector<int> lrc, s;

    for(int i=0; i<datos.size(); i++)
        for(int j=0; j<datos[i].size(); j++)
            media[j]+=datos[i][j];
    for(int i=0; i<media.size(); i++)
        media[i]/=datos.size();
    double d,dm=diferencia(datos[0],media);
    //Buscamos el patrón más centrico (el primer centroide)
    int sel;
    for(int i=0; i<datos.size(); i++){
        d=diferencia(datos[i],media);
        if(d<dm){
            dm=d;
            sel=i;
        }
    }
    z[0]=datos[sel];
    s.push_back(sel);

    vector<double> g(datos.size(),0); //Ganancias
    double dist_act, valor, umbral, dist_minima, max_ganancia, min_ganancia;

 
    for (int k0=1; k0<k; k0++){
	cout << "Iteracion k: " << k0 << endl ;
        for (int i=0; i<datos.size(); i++) {
		cout << "iteracion i " << i << endl ;
            if (busca(s,i))
            	g[i]=-1.0; //Si ya ha sido seleccionado (no candidato)
            else{
                g[i]=0.0;
                for (int j=0; j<datos.size(); j++) {
			cout << "iteracion j " << j << endl ;
                    if (!busca(s,j) && i!=j){
                    /* Se calcula la distancia del patron j al
                       centro mas cercano de los ya determinados */
                        dist_minima=10E38;
                        for (int k2=0;k2<k0;k2++){
				cout << "iteracion k2 " << k2 << endl ;
                            dist_act=diferencia(datos[j],z[k2]);
                            if (dist_act<dist_minima)
                                dist_minima=dist_act;
                        }
                        valor=dist_minima - diferencia(datos[j],datos[i]);
                        if (valor>0)
                        	g[i] += valor;
                    }
		}
            }
	}

	cout << "Ganancias calculadas en " << k0 << endl << flush;
        max_ganancia=-1.0;
        min_ganancia=10E38;
        for (int i=0; i<datos.size(); i++)
            if (g[i]!=-1){ //Si es candidato
                if (g[i]>max_ganancia) max_ganancia=g[i];
                if(g[i]<min_ganancia) min_ganancia=g[i];
            }

        umbral=max_ganancia-0.3*(max_ganancia-min_ganancia);
        cout << "Umbral: " << umbral << endl;
        for (int i=0; i<datos.size(); i++)
            if (g[i]!=-1 && g[i]>umbral) lrc.push_back(i);

        int seed=Randint(0,lrc.size()-1);
        z[k0]=datos[lrc[seed]];
        s.push_back(lrc[seed]);
        cout << "LRC Size: " << lrc.size() << endl;
        lrc.clear();
    }

    /* Una vez que se conocen los k centros, se genera una
       configuracion inicial asociando cada patron al cluster
       mas cercano */
    for (int i=0; i<datos.size(); i++)
        C[CMC(i,datos,z)].push_back(i);
    for(int i=0; i<k; i++){
        assert(!z[i].empty());
        assert(!C[i].empty());
    }
}

int main(int argc, char ** argv){
    vector<vector<double> > z, best_z, datos; // centroides
    int k;
    int MAX_INIT=5;
    double jota;
    double MIN_J;

    assert(argc>2);
    Seed = atoi(argv[2]);
    if (strcmp(argv[1],"yeast.txt")==0){
        k=10;
        datos = vector<vector<double> > (1484,vector<double> (8,0));
        z = vector<vector<double> > (k,vector<double> (8,0));
    }else if (strcmp(argv[1],"wdbc.txt")==0){
        k=2;
        datos = vector<vector<double> > (569,vector<double> (31,0));
        z = vector<vector<double> > (k,vector<double> (31,0));
    }else if (strcmp(argv[1],"Aggregation.txt")==0){
        k=7;
        datos = vector<vector<double> > (788,vector<double> (2,0));
        z = vector<vector<double> > (k,vector<double> (2,0));
    }

    cout << argv[1] << " con semilla " << argv[2] << endl;
    vector<list<int> > best_sol(k,list<int>());
    vector<list<int> > sol_act(k,list<int>());

    clock_t start_time;
    //Lectura de fichero
    ifstream fi;
    fi.open(argv[1]);
    assert(fi);
    for (int i=0; i < datos.size(); i++)
        for (int j = 0; j < datos[i].size(); j++)
            fi >> datos[i][j];
    fi.close();

    start_time = clock();
    for(int i=0; i<MAX_INIT; i++){
        cout << "BL en solución " << i+1 << "." << endl;
        solGreedy(k,sol_act,datos,z);
        BL(k,sol_act,datos,z);
        jota=J(sol_act,datos,z);
        if(i==0) MIN_J=jota;
        if(jota<MIN_J)
            best_sol=sol_act;
            best_z=z;
            MIN_J=jota;
    }
    //cout << "Evaluaciones: " << ev << endl;
    cout << "Mejor solucion: " <<  MIN_J << endl;
    cout << "Tiempo GRASP: " << (float)(clock()- start_time) / CLOCKS_PER_SEC << endl << endl;
}

