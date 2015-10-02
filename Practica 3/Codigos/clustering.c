#include "extern.h"
#include "timer.h"
#include <iostream>
#include <vector>		
# include <algorithm>
using namespace std;

#define num_evaluaciones 20000
#define tam_poblacion 50
#define prob_AGG 0.7
#define prob_AGE 1
#define prob_mutacion 0.01

double distanciaPatronCentroide(double *x, double *y) {
	int i;
 	double suma = 0.0;
 
	for (i=0;i<n_variables;i++)
  		suma += (x[i] - y[i])*(x[i] - y[i]);
 
	return (sqrt(suma));
 	return (suma);
}

int clusterMasCercano(double *patron) {
	double dist_minima, dist_act;
	int k, cluster;
 
 	dist_minima=10E38;
 	for (k=0;k<k_inicial;k++) {
   		dist_act=distanciaPatronCentroide(patron,Centros[k]);
   		if (dist_act<dist_minima) {
     		cluster=k;
     		dist_minima=dist_act;
     	}
   	}
 	return cluster;
}

void recalculoCentroides(int cluster, int patron) {
	int l;
	for (l = 0; l < n_variables; l++) {
		//Centros[cluster1][l] = ((Tam[cluster1] * Centros[cluster1][l]) - tabla[i][l]) / (Tam[cluster1] - 1);
	 	Centros[cluster][l] = ((Tam[cluster] * Centros[cluster][l]) + tabla[patron][l]) / (Tam[cluster] + 1);
	 }
}

double J(int cromosoma) {
	int i;
	double suma = 0.0;
	 
	for (i=0;i<n_datos;i++)
	  	suma += distanciaPatronCentroide(tabla[New[cromosoma].Gene[i]],Centros[Conf[New[cromosoma].Gene[i]]]);

	return (suma*suma/(double)n_datos);	
}

void evaluacion(int cromosoma) {
	
	int i, j;
	
	for (i = 0; i < k_inicial; i++)
		Tam[i] = 0;
	for (i = 0; i < k_inicial; i++) {
		for (j = 0; j < n_variables; j++) {
			Centros[i][j] = tabla[New[cromosoma].Gene[i]][j];
		}
		Conf[New[cromosoma].Gene[i]] = i;
		Tam[i]++;
	}
	
	for (i = k_inicial; i < n_datos; i++) {
		int cluster = clusterMasCercano(tabla[New[cromosoma].Gene[i]]);
		recalculoCentroides(cluster, New[cromosoma].Gene[i]);
		Conf[New[cromosoma].Gene[i]] = cluster;
		Tam[cluster]++;			
	}
	
	New[cromosoma].objetivo = J(cromosoma);
	New[cromosoma].cambio = 0;
	
}
void evaluacionAGE(int cromosoma) {
	
	int i, j;
	
	for (i = 0; i < k_inicial; i++)
		Tam[i] = 0;
	for (i = 0; i < k_inicial; i++) {
		for (j = 0; j < n_variables; j++) {
			Centros[i][j] = tabla[Old[cromosoma].Gene[i]][j];
		}
		Conf[Old[cromosoma].Gene[i]] = i;
		Tam[i]++;
	}
	
	for (i = k_inicial; i < n_datos; i++) {
		int cluster = clusterMasCercano(tabla[Old[cromosoma].Gene[i]]);
		recalculoCentroides(cluster, Old[cromosoma].Gene[i]);
		Conf[Old[cromosoma].Gene[i]] = cluster;
		Tam[cluster]++;			
	}
	
	Old[cromosoma].objetivo = J(cromosoma);
	Old[cromosoma].cambio = 0;
	
}
void liberarMemoriaAGG() {
	
	for (int i=0;i<n_datos;i++)
  		free (tabla[i]);
  		
  	free (tabla);

  	free (Conf);
	free (HijoA);
	free (HijoB);
 
	for (int i=0;i<k_inicial;i++)
  		free (Centros[i]);
  		
 	free (Centros);
 
	free(Tam);
	for (int i=0; i < tam_poblacion; i++) {
	   	free (New[i].Gene);
	   	free (Old[i].Gene);
	} 
	free (New);
	free (Old);	
}
void liberarMemoriaAGE() {
	
	for (int i=0;i<n_datos;i++)
  		free (tabla[i]);
  		
  	free (tabla);

  	free (Conf);
	free (HijoA);
	free (HijoB);
 
	for (int i=0;i<k_inicial;i++)
  		free (Centros[i]);
  		
 	free (Centros);
 
	free(Tam);
	for (int i=0; i < tam_poblacion; i++) {
	   	//free (New[i].Gene);
	   	free (Old[i].Gene);
	} 
	//free (New);
	free (Old);	
}
void Inicializar(){

	for(int i = 0 ; i < tam_poblacion; i++){
		for(int j = 0; j < n_datos; j++){
			New[i].Gene[j] = j;
		}
		for (int j = 0; j < n_datos;j++){
			int aleatorio = Randint(0,n_datos-1);
			int aux = New[i].Gene[j];
			New[i].Gene[j] = New[i].Gene[aleatorio];
			New[i].Gene[aleatorio] = aux; 

		}
		evaluacion(i);
	}
}
void AGG()
{
	int eval=0;
	ESTRUCTURA *temp;
	Inicializar();
	double mejor_objetivo = 10E38;

	start_timers(); 

	while(eval < num_evaluaciones){
		
		temp = Old;
		Old = New;
		New = temp; 

		SeleccionAGG();
		OX();
		MutacionAGG();
		ReemplazarAGG();

		for (int i = 0; i < tam_poblacion; i++) {
			if (New[i].cambio != 0) {
				evaluacion(i);
				eval++;
			}
		}

		for (int i = 0; i < tam_poblacion; i++) {
			if (mejor_objetivo > New[i].objetivo)
				mejor_objetivo = New[i].objetivo;
		}cout<<"Iteración: "<<eval<<endl;
	}
	double tiempo = elapsed_time();
	cout<<"El mejor resultado es: "<< mejor_objetivo<<endl;
	cout<<"El tiempo: "<<tiempo<<endl;
}
void AGE()
{
	int eval=0;
	ESTRUCTURA *temp;
	Inicializar();
	double mejor_objetivo = 10E38;

	Old = New;
	start_timers(); 

	while(eval < num_evaluaciones){

		SeleccionAGE();
		OXAGE();
		MutacionAGE();
		ReemplazarAGE();
	cout<<"Tamanio de la poblacion: "<< tam_poblacion<<endl;
		for (int i = 0; i < tam_poblacion; i++) {
			if (Old[i].cambio != 0) {
				evaluacionAGE(i);
				eval++;
	
			}
		}cout<<"iteracion: "  <<endl;

		for (int i = 0; i < tam_poblacion; i++) {
			if (mejor_objetivo > Old[i].objetivo)
				mejor_objetivo = Old[i].objetivo;
		}
	}
	double tiempo = elapsed_time();
	cout<<"El mejor resultado es: "<< mejor_objetivo<<endl;
	cout<<"El tiempo: "<<tiempo<<endl;
}

void SeleccionAGG(){

	for(int i = 0; i < tam_poblacion;i++){
		int ind1 = Randint(0,tam_poblacion-1);
		int ind2 = Randint(0,tam_poblacion-1);
		if (Old[ind1].objetivo < Old[ind2].objetivo){
			for (int j = 0; j <n_datos;j++)
				New[i].Gene[j] = Old[ind1].Gene[j];
			New[i].objetivo = Old[ind1].objetivo;
			New[i].cambio = 0;
		}
		else{
			for (int j = 0; j <n_datos;j++)
				New[i].Gene[j] = Old[ind2].Gene[j];
			New[i].objetivo = Old[ind2].objetivo;
			New[i].cambio = 0;			
		}
	}
}
void SeleccionAGE(){

	for(int i = 0; i < 2;i++){
		int ind1 = Randint(0,tam_poblacion-1);
		int ind2 = Randint(0,tam_poblacion-1);
		if (Old[ind1].objetivo < Old[ind2].objetivo){
			for (int j = 0; j <n_datos;j++)
				New[i].Gene[j] = Old[ind1].Gene[j];
			New[i].objetivo = Old[ind1].objetivo;
			New[i].cambio = 0;
		}
		else{
			for (int j = 0; j <n_datos;j++)
				New[i].Gene[j] = Old[ind2].Gene[j];
			New[i].objetivo = Old[ind2].objetivo;
			New[i].cambio = 0;			
		}
	}
}

void ReemplazarAGG(){

	int i, j, found, mejor = 0, peor = 0;
	double mejor_Sol = Old[0].objetivo;
	double peor_Sol = New[0].objetivo;
	

	//Seleccionamos la mejor solucion de nuestra poblacion
	for (i = 0; i < tam_poblacion; i++) {
		if (mejor_Sol > Old[i].objetivo) {
			mejor_Sol = Old[i].objetivo;
			mejor = i;
		}
	}
	
	//Si la poblacion no es la misma 
	for (i=0, found=0; i<tam_poblacion && (!found); i++)
	  	for (j=0, found=1; (j < n_variables) && (found); j++)
	   		found = (New[i].Gene[j] == Old[mejor].Gene[j]);
	
	//Miramos si es la peor de todas y plamantenemos
	if (!found) {
		for (i = 0; i < tam_poblacion; i++) {
			if (peor_Sol < New[i].objetivo) {
				peor_Sol = New[i].objetivo;
				peor = i;
			}
		}
		
		for (i = 0; i < n_variables; i++){
			New[peor].Gene[i] = Old[mejor].Gene[i];
			}
		New[peor].objetivo = Old[mejor].objetivo;
		
	}


}
void ReemplazarAGE(){
	int found, mejor = 0, peor = 0;
	double mejor_Sol = New[0].objetivo;
	double peor_Sol1 = Old[0].objetivo;
	double peor_Sol2 = Old[0].objetivo;
	int peor1 = 0;
	int peor2 = 0;

	//Seleccionamos las dos peores soluciones de nuestra poblacion
	for (int i = 0; i < tam_poblacion; i++) {
		if (peor_Sol1 < Old[i].objetivo) {
			peor_Sol1 = Old[i].objetivo;
			peor1 = i;
		}
	}	
	for (int i = 0; i < tam_poblacion; i++) {
		if ((peor_Sol2 < Old[i].objetivo) and (peor_Sol1 != Old[i].objetivo)) {
			peor_Sol2 = Old[i].objetivo;
			peor2 = i;
		}
	}	
	//cout<<peor1 <<" "<<peor2<<endl
	vector<double> menor;

	menor.push_back(peor_Sol2);
	menor.push_back(peor_Sol1);
	menor.push_back(New[0].objetivo);
	menor.push_back(New[1].objetivo);

	sort(menor.begin(),menor.end());

	cout<< peor_Sol1<<endl;
	cout<< peor_Sol2<<endl;
	cout<<New[0].objetivo<<endl;
	cout<<New[1].objetivo<<endl<<endl<<endl;


	for(int i = 0; i < 2; i++){
		int peor;
		if( i == 0){
			peor = peor1;
		}else if(i == 1){
			peor = peor2;
		}
		if (menor[i] == New[0].objetivo){
			for (int j = 0; j < n_datos; j++)
				Old[peor].Gene[j] = New[0].Gene[j];
			Old[peor].objetivo = New[0].objetivo;
			Old[peor].cambio = New[0].cambio;
		}
		else if (menor[i] == New[1].objetivo){
			for (int j = 0; j < n_datos; j++)
				Old[peor].Gene[j] = New[1].Gene[j];
			Old[peor].objetivo = New[1].objetivo;
			Old[peor].cambio = New[1].cambio;
		}
	}
	cout<<"////////////////////////"<<endl;
	cout<<Old[peor1].objetivo<<endl;
	cout<<Old[peor2].objetivo<<endl;
	cout<<"////////////////////////"<<endl;

	
}
void OX()
{
	int poblacion_cruce = tam_poblacion*prob_AGG;
	int xpoint1, xpoint2,madre,padre;
	int Hijo1[n_datos];
	int Hijo2[n_datos];
	vector<int> auxmadre1;
	vector<int> auxpadre1;
	vector<int> auxmadre2;
	vector<int> auxpadre2; 

	for(int i = 0 ; i < poblacion_cruce;i +=2 ){
		madre = i;
		padre = i+1;

		xpoint1=Randint (0,n_datos-1);
		if (xpoint1!=n_datos-1)
			xpoint2=Randint (xpoint1+1,n_datos-1);
		else
			xpoint2=n_datos-1;

		//Primer hijo y Segundo hijo
		for(int j = xpoint1 ; j < xpoint2;j++){ 
			Hijo1[j] = New[madre].Gene[j];
			Hijo2[j] = New[padre].Gene[j];
			auxmadre1.push_back(New[madre].Gene[j]);
			auxpadre2.push_back(New[padre].Gene[j]);
		}

		for(int j = 0; j < n_datos;j++){
			bool pertenece = false;
			//Primer hijo
			for(int k = 0; k < auxmadre1.size();k++){
				if(New[padre].Gene[j] == auxmadre1[k]){
					pertenece = true;
				}
				if(pertenece == true){
					k = auxmadre1.size();
				}
			}
			if(pertenece == false){
				auxpadre1.push_back(New[padre].Gene[j]);
			}
			pertenece = false;
			//Segundo hijo
			for(int k = 0; k < auxpadre2.size();k++){
				if(New[madre].Gene[j] == auxpadre2[k]){
					pertenece = true;
				}
				if(pertenece == true){
					k = auxpadre2.size();
				}
			}
			if(pertenece == false){
				auxmadre2.push_back(New[madre].Gene[j]);
			}

		}

		int x = xpoint2;
		for(int j = 0; j < auxpadre1.size();j++){
			Hijo1[x] = auxpadre1[j];
			x++;
			x = x%n_datos;	
		}

		x = xpoint2;
		for(int j = 0; j < auxmadre2.size();j++){
			Hijo2[x] = auxmadre2[j];
			x++;
			x = x%n_datos;	
		}

		for (int j = 0; j < n_datos; j++) {
			New[madre].Gene[j] = Hijo1[j];
			New[padre].Gene[j] = Hijo2[j];}
		New[madre].cambio = 1;
		New[padre].cambio = 1;


		auxpadre1.clear();
		auxmadre1.clear();
		auxpadre2.clear();
		auxmadre2.clear();		


	}
	
}
void OXAGE(){
	int poblacion_cruce = tam_poblacion*0.7;
	int xpoint1, xpoint2,madre,padre;
	int Hijo1[n_datos];
	int Hijo2[n_datos];
	vector<int> auxmadre1;
	vector<int> auxpadre1;
	vector<int> auxmadre2;
	vector<int> auxpadre2; 

	madre = 0;
	padre = 1;

	xpoint1=Randint (0,n_datos-1);
	if (xpoint1!=n_datos-1)
		xpoint2=Randint (xpoint1+1,n_datos-1);
	else
		xpoint2=n_datos-1;

	//Primer hijo y Segundo hijo
	for(int j = xpoint1 ; j < xpoint2;j++){ 
		Hijo1[j] = New[madre].Gene[j];
		Hijo2[j] = New[padre].Gene[j];
		auxmadre1.push_back(New[madre].Gene[j]);
		auxpadre2.push_back(New[padre].Gene[j]);
	}

	for(int j = 0; j < n_datos;j++){
		bool pertenece = false;
		//Primer hijo
		for(int k = 0; k < auxmadre1.size();k++){
			if(New[padre].Gene[j] == auxmadre1[k]){
				pertenece = true;
			}
			if(pertenece == true){
				k = auxmadre1.size();
			}
		}
		if(pertenece == false){
			auxpadre1.push_back(New[padre].Gene[j]);
		}
		pertenece = false;
		//Segundo hijo
		for(int k = 0; k < auxpadre2.size();k++){
			if(New[madre].Gene[j] == auxpadre2[k]){
				pertenece = true;
			}
			if(pertenece == true){
				k = auxpadre2.size();
			}
		}
		if(pertenece == false){
			auxmadre2.push_back(New[madre].Gene[j]);
		}

	}

	int x = xpoint2;
	for(int j = 0; j < auxpadre1.size();j++){
		Hijo1[x] = auxpadre1[j];
		x++;
		x = x%n_datos;	
	}

	x = xpoint2;
	for(int j = 0; j < auxmadre2.size();j++){
		Hijo2[x] = auxmadre2[j];
		x++;
		x = x%n_datos;	
	}

	for (int j = 0; j < n_datos; j++) {
		New[madre].Gene[j] = Hijo1[j];
		New[padre].Gene[j] = Hijo2[j];
	}
	New[madre].cambio = 1;
	New[padre].cambio = 1;


	//cout<< auxmadre.size()<< " "<<auxpadre.size()<<endl;
	auxpadre1.clear();
	auxmadre1.clear();
	auxpadre2.clear();
	auxmadre2.clear();		

}

void MutacionAGG(){

	int mutaciones = tam_poblacion*n_datos*prob_mutacion;
	int cromosoma,gen_nuevo,gen_viejo;

	for (int i = 0; i < mutaciones; i++) {		
		cromosoma = Randint(0, tam_poblacion-1);
		gen_viejo = Randint(0, n_datos-1);	
		gen_nuevo = Randint(0, n_datos-1);
		
		int temp;
		
		temp = New[cromosoma].Gene[gen_viejo];
		New[cromosoma].Gene[gen_viejo] = New[cromosoma].Gene[gen_nuevo];
		New[cromosoma].Gene[gen_nuevo] = temp;
		
		New[cromosoma].cambio = 1;

	}
}
void MutacionAGE(){

	int mutaciones = tam_poblacion*n_datos*prob_mutacion;
	int cromosoma,gen_nuevo,gen_viejo;

	for (int i = 0; i < mutaciones; i++) {		
		cromosoma = Randint(0, 2-1);
		gen_viejo = Randint(0, n_datos-1);	
		gen_nuevo = Randint(0, n_datos-1);
		
		int temp;
		//cout<< cromosoma <<" ";
		temp = New[cromosoma].Gene[gen_viejo];
		New[cromosoma].Gene[gen_viejo] = New[cromosoma].Gene[gen_nuevo];
		New[cromosoma].Gene[gen_nuevo] = temp;
		
		New[cromosoma].cambio = 1;
	}
}
