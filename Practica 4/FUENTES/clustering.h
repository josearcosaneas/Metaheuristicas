#ifndef __CLUSTERING
#define __CLUSTERING

#include <iostream>
#include <fstream>
#include "random_ppio.h"
#include <cstdlib>
#include <vector>
#include <list>

using namespace std;

class Clustering{

	struct campos{

		double valor;
		int indice;

	};

	private:

		int patrones;
		int dimensiones;
		int num_clusters;
		vector<vector<double> > tabla_patrones;
		vector<list<int> > clusters;
		vector<vector<double> > centroides;
		double coste;

	public:

		Clustering();
		
		Clustering(int patrones, int dimensiones, int num_clusters, vector<vector<double> > tab_patrones);
		
		~Clustering();

		int num_patrones();
	
		int num_dimensiones();

		double costeSolucion();

		void patron_pos(int posicion);

		void leerEstructura();

		void iniciarConfiguracion(vector<vector<int> >matriz_pertenencia);

		void solucionAleatoria(vector<int> &recorrido_aleatorio);

		void leerClusters();

		void leerCentroides();

		void calculoCentroides();
		
		bool yaUsado(vector<int> usados, list<int>::iterator it);

		void incluirUsados(vector<int> &usados, list<int>::iterator it);
		
		double distanciaPatronCentroide(vector<double> dimPatron, vector<double> dimCentroide);

		double sumDistanciasCluster(int numCluster);

		double funcionSolucion();

		double funcionSolucion(vector<list<int> > estructura);

		void moverPatron(int clusterOrigen, int clusterDestino, list<int>::iterator &elemento);

		void recalcularCentroides(int clusterOrigen, int clusterDestino, list<int>::iterator posPatron);

		double factorizacion(int clusterOrigen, int clusterDestino, list<int>::iterator posPatron);

		void busquedaLocal(int numero_iteraciones, int &evaluaciones);
		
    	Clustering & operator= (const Clustering original);

};

#endif


