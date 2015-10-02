#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cmath>
#include "clustering.h"
#include <stdlib.h>
#include <time.h>
#include <algorithm>

using namespace std;

/*Constructor sin parámetros: Crea un objeto de tipo clustering vacío.*/
Clustering::Clustering(){

	patrones=0;
	dimensiones=0;
	num_clusters=0;

}
/*	Constructor con parámetros: Crea un objeto de tipo clustering con sus estructuras inicializadas
	pero vacías.
*/
Clustering::Clustering(int pat_cluster, int dim_cluster, int num_clust, vector<vector<double> > tab_patrones){
	
	patrones = pat_cluster;
	dimensiones = dim_cluster;
	num_clusters = num_clust;

	tabla_patrones.resize(patrones);

	for(int f=0; f<patrones; f++){
		tabla_patrones[f].resize(dimensiones);
	}

	for(int i=0; i<patrones; i++){
		for(int j=0; j<dimensiones; j++){
			tabla_patrones[i][j]=tab_patrones[i][j];
		}
	}


	centroides.resize(num_clusters);
  
	for(int f=0; f<num_clusters; f++){
		centroides[f].resize(dimensiones);
	}

	for(int i=0; i<num_clusters; i++){
		for(int j=0; j<dimensiones;j++){
			centroides[i][j]=0;
		}
	}

	coste = 0.0;
	clusters.resize(num_clust);
}

/*	Destructor: Libera memoria reservada.	*/
Clustering::~Clustering(){


}
/* Devuelve el número de patrones. */
int Clustering::num_patrones(){

	return patrones;

}
/*	Devuelve el número de dimensiones que tiene cada patrón.	*/
int Clustering::num_dimensiones(){

	return dimensiones;

}
/*	Devuelve la posición de un patrón en la tabla de patrones que se rellena a partir de un archivo inicial.	*/
void Clustering::patron_pos(int posicion){

	if(posicion <= 0 || posicion > patrones){
		cout << "La posicion introducida no se encuentra entre los limites de la tabla de patrones." << endl;
		exit(1);
	}
	cout << "Patron numero " << posicion << " con dimensiones: ";
	for(int i=0; i<dimensiones; i++){
		cout << tabla_patrones[posicion-1][i] << " ";
	}
	cout << endl;
}

double Clustering::costeSolucion(){

	return coste;

}
/* Crear la configuración de los clusters. */
void Clustering::iniciarConfiguracion(vector<vector<int> >matriz_pertenencia){

	for(int i=0; i < matriz_pertenencia.size(); i++){
		for(int j=0; j < matriz_pertenencia[i].size(); j++){
			if(matriz_pertenencia[i][j] == 1){
				clusters[j].push_back(i);
			}
		}
	}
	for(int i=0; i < clusters.size(); i++){
		clusters[i].sort();
	}

	calculoCentroides();

}

void Clustering::solucionAleatoria(vector<int> &recorrido_aleatorio){

	vector<int> elementos;
	elementos.resize(patrones);

	for(int i=0; i<patrones; i++){
		elementos[i]=i;
	}

	int numeroBarajes = Randint(0,20);
	int patronElegido;
	int indice=0;
	/* Mezclar el vector. */
	for(int i=0; i<numeroBarajes; i++){
		random_shuffle(elementos.begin(), elementos.end());
	}

	int cluster_asignado;
	
	for(int i=0; i<patrones; i++){
		cluster_asignado = Randint(0, num_clusters-1);
		clusters[cluster_asignado].push_back(elementos[i]);
	}
	
	for(int i=0; i<clusters.size(); i++){
		clusters[i].sort();
	}
	calculoCentroides();
	coste = funcionSolucion();

	recorrido_aleatorio.resize(num_clusters);
	for(int i=0; i < num_clusters; i++){
		recorrido_aleatorio[i] = elementos[i];
	}
		
}
/*	Lee la tabla de patrones con la que cuenta el problema.	*/
void Clustering::leerEstructura(){
	for(int i=0; i<patrones; i++){
		for(int j=0; j<dimensiones; j++){
			cout << tabla_patrones[i][j] << " "; 
		}
		cout << endl;
	}
}

/*	Lee la configuración de clusters de un objeto clustering.	*/
void Clustering::leerClusters(){

	cout << "La muestra se compone de " << num_clusters << " con " << patrones << " patrones y " << dimensiones << " dimensiones por cada patron." << endl;
	
	for(int i=0; i<clusters.size(); i++){
		cout << "Cluster numero " << i << " contiene " << clusters[i].size() << " patrones: " << endl;
		list<int>::iterator it = clusters[i].begin();
		for(it; it!=clusters[i].end(); it++){
			cout << *it << " ";
		}
		cout << endl;
	}
	cout << "Lectura finalizada. " << endl;
}
/*	Para cada cluster, cancula su elemento representativo (CENTROIDE) que es centro de todos los elementos que lo forman.	*/
void Clustering::calculoCentroides(){

	int cont;
	for(int i=0; i<clusters.size(); i++){
		list<int>::iterator it = clusters[i].begin();
		cont=0;
		for(it; it!=clusters[i].end(); it++){
			for(int z=0; z<dimensiones; z++){
				centroides[i][z]+=tabla_patrones[*it][z];
				cont++;
			}
		}
		for(int j=0; j<dimensiones; j++){
			centroides[i][j]=centroides[i][j]/clusters[i].size();
		}
		
	}

}	
/*	Lee la matriz de centroides para cada cluster.	*/
void Clustering::leerCentroides(){

	cout << "Los centroides para los clusters son: " << endl;
	for(int i=0; i<num_clusters; i++){
		cout << "Para el cluster " << i << ": " << endl;
		for(int j=0; j<dimensiones; j++){
			cout << centroides[i][j] << " ";
		}
		cout << endl;
	}

}

/*	Calcula la distancia de un patrón a su cluster.	*/
double Clustering::distanciaPatronCentroide(vector<double> dimPatron, vector<double> dimCentroide){
	
	double distancia=0.0;
	
	for(int i=0; i<dimensiones; i++){
		distancia += (double)((1.0*dimPatron[i]-1.0*dimCentroide[i])*(1.0*dimPatron[i]-1.0*dimCentroide[i]));	
	}
		
	return distancia;

}
/*	Suma la distancia de todos los elementos de un cluster a su centroide y devuelve el valor acumulado.	*/
double Clustering::sumDistanciasCluster(int numCluster){

	double distCluster=0.0;
	list<int>::iterator it = clusters[numCluster].begin();
	for(it; it!=clusters[numCluster].end(); it++){
		distCluster += distanciaPatronCentroide(tabla_patrones[*it], centroides[numCluster]);
	}

	return distCluster;
}
/*	Devuelve el coste de la configuración actual	*/
double Clustering::funcionSolucion(){
	
	double solucion=0.0;
	for(int i=0; i<clusters.size(); i++){
			solucion += sumDistanciasCluster(i);
	}
	
	return solucion;
}
/*	Mueve un patrón del cluster i al cluster j	*/
void Clustering::moverPatron(int clusterOrigen, int clusterDestino, list<int>::iterator &elemento){
	
	double valor = *elemento;
	clusters[clusterDestino].push_back(*elemento);
	elemento = clusters[clusterOrigen].erase(elemento);
	clusters[clusterDestino].sort();

}
/*	Factorización: Recalcula los centroides eliminando la contribución del elemento cambiado y añadiendo esa contribución al nuevo cluster.*/
void Clustering::recalcularCentroides(int clusterOrigen, int clusterDestino, list<int>::iterator posPatron){

	for(int i=0; i<dimensiones; i++){

		centroides[clusterOrigen][i]= (((1.0*clusters[clusterOrigen].size())*centroides[clusterOrigen][i])-tabla_patrones[*posPatron][i])/(clusters[clusterOrigen].size()-1.0);
		centroides[clusterDestino][i]=(((1.0*clusters[clusterDestino].size())*centroides[clusterDestino][i])+tabla_patrones[*posPatron][i])/(clusters[clusterDestino].size()+1.0);

	}
}

double Clustering::factorizacion(int clusterOrigen, int clusterDestino, list<int>::iterator posPatron){
	
	double fact=0;
	//Eliminar contribución al cluster origen. 
	double x1 = (1.0*clusters[clusterOrigen].size())/(1.0*clusters[clusterOrigen].size()-1.0);
	double x2 = distanciaPatronCentroide(tabla_patrones[*posPatron],centroides[clusterOrigen]);
	//Eliminar contribución al cluster destino.
	double y1 = (1.0*clusters[clusterDestino].size())/(1.0*clusters[clusterDestino].size()+1.0);
	double y2 = distanciaPatronCentroide(tabla_patrones[*posPatron],centroides[clusterDestino]);

	fact = (y1*y2)-(x1*x2);
	return fact;

}
/*	Evita que un mismo elemento sea cambiado de cluster en la misma iteración.	*/
bool Clustering::yaUsado(vector<int> usados, list<int>::iterator it){

	bool usado=false;
	for(int i=0; i<usados.size(); i++){
		if(*it==usados[i])
			usado=true;
	}
	return usado;

}
/*	Introduce un valor en el vector de usados	*/
void Clustering::incluirUsados(vector<int> &usados, list<int>::iterator it){

	usados.push_back(*it);

}


/* ---------------------------------------Búsqueda Local-----------------------*/

/*	Algoritmo de búsqueda local: 
	Operador de vecino: Mueve los patrones de su cluster al cluster cuyo centroide tiene distancia mínima.
	Buenos resultados pero atascos en óptimos locales.
*/
void Clustering::busquedaLocal(int numero_iteraciones, int &evaluaciones){

	vector<int> usados;
	
	double mejorSolucion = funcionSolucion();
	evaluaciones++;
	for(int iteraciones=0; iteraciones < numero_iteraciones; iteraciones++){
		
		usados.clear();
		for(int i=0; i<num_clusters;i++){
			list<int>::iterator it = clusters[i].begin();
			for(it; it!=clusters[i].end(); it++){
				double x1 = (1.0*clusters[i].size())/(1.0*clusters[i].size()-1.0);
				double x2 = distanciaPatronCentroide(tabla_patrones[*it],centroides[i]);
				double a = x1*x2;
				if(!yaUsado(usados,it)){
					int min=i;
					double aux = 2e50;
					for(int j=0; j<num_clusters; j++){
						if(j!=i){
							double y1 = (1.0*clusters[j].size())/(1.0*clusters[j].size()+1.0);
							double y2 = distanciaPatronCentroide(tabla_patrones[*it],centroides[j]);
							double B=y1*y2;

							if (B < aux){
								aux = B;
								min=j;
							}	
						}						
					}
					double minB = aux; 
					double coste= minB-a;
					if(clusters[i].size()>2){
						if((min!=i) && coste<0){

							recalcularCentroides(i,min,it);
							moverPatron(i,min,it);
							double solVecino = funcionSolucion();
							evaluaciones++;
							if(mejorSolucion>solVecino){
	
								incluirUsados(usados,it);
								mejorSolucion=solVecino;
								
							}
						}
					}
 				}
			}
		}
	}
	
	
	coste = funcionSolucion();
	cout << "La solucion es: " << funcionSolucion() << endl;


}
/*	Operador igual.	*/
Clustering & Clustering::operator=(const Clustering original){

	if(this->patrones==0 && this->dimensiones==0 && this->num_clusters==0){
		this->patrones = original.patrones;
		this->dimensiones = original.dimensiones;
		this->num_clusters = original.num_clusters;

		clusters.resize(original.clusters.size());
		for(int i=0; i<clusters.size(); i++){
			clusters[i]=original.clusters[i];
		}

		tabla_patrones.resize(this->patrones);
		for(int f=0; f<patrones; f++){
			tabla_patrones[f].resize(dimensiones);
		}

		for(int i=0; i<this->patrones; i++){
			for(int j=0; j<this->dimensiones; j++){
				tabla_patrones[i][j]=original.tabla_patrones[i][j];
			}
		}

		centroides.resize(num_clusters);
		for(int f=0; f<centroides.size(); f++){
			centroides[f].resize(dimensiones);
		}

		for(int i=0; i<this->num_clusters; i++){
			for(int j=0; j<this->dimensiones;j++){
				centroides[i][j]=original.centroides[i][j];
			}
		}

		coste = original.coste;

	}
	else{

		for(int i=0; i<clusters.size(); i++){
			clusters[i]=original.clusters[i];
		}

		for(int i=0; i<this->patrones; i++){
			for(int j=0; j<this->dimensiones; j++){
				tabla_patrones[i][j]=original.tabla_patrones[i][j];
			}
		}

		for(int i=0; i<this->num_clusters; i++){
			for(int j=0; j<this->dimensiones;j++){
				centroides[i][j]=original.centroides[i][j];
			}
		}

		coste = original.coste;
	}
	
	return *this;
	
}
