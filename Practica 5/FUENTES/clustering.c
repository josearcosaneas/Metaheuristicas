#include "extern.h"
#include "timer.h"
#include <iostream>
#include <vector>		

using namespace std;

#define num_evaluaciones 20000
#define tam_poblacion 10
#define prob_AGG 0.7
#define prob_mutacion 0.01

double distanciaPatronCentroide(double *x, double *y) {
	int i;
 	double suma = 0.0;
 
	for (i=0;i<n_variables;i++)
  		suma += (x[i] - y[i])*(x[i] - y[i]);
 
	return (sqrt(suma));
 	return (suma);
}

void recalculoCentroides(int cluster1, int cluster2, int i) {
	int l;
	for (l = 0; l < n_variables; l++) {
		Centros[cluster1][l] = ((Tam[cluster1] * Centros[cluster1][l]) - tabla[i][l]) / (Tam[cluster1] - 1);
	 	Centros[cluster2][l] = ((Tam[cluster2] * Centros[cluster2][l]) + tabla[i][l]) / (Tam[cluster2] + 1);
	 }
}

void aumentarTamanio(int cluster) {
	Tam[cluster]++;
}

void disminuirTamanio(int cluster) {
	Tam[cluster]--;
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

void calculoInicialCentroides(int cromosoma){ 
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
		recalculoCentroidesIni(cluster, New[cromosoma].Gene[i]);
		Conf[New[cromosoma].Gene[i]] = cluster;
		Tam[cluster]++;		
	}
			
}

void recalculoCentroides(int cluster, int patron) {
	int l;
	for (l = 0; l < n_variables; l++) {
		//Centros[cluster1][l] = ((Tam[cluster1] * Centros[cluster1][l]) - tabla[i][l]) / (Tam[cluster1] - 1);
	 	Centros[cluster][l] = ((Tam[cluster] * Centros[cluster][l]) + tabla[patron][l]) / (Tam[cluster] + 1);
	 }
}
void recalculoCentroidesIni(int cluster, int patron) {
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

int usado(int num, int* Hijo) {
	int i, usado = 0;
	for (i = 0; i < n_datos && !usado; i++) {
		if (Hijo[i] == num)
			usado = 1;
	}	
	return usado;
}
double objetivoActual() {
	int i;
	double suma = 0.0;
	 
	for (i=0;i<n_datos;i++)
	  	suma += distanciaPatronCentroide(tabla[i],Centros[Conf[i]]);

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

void AM_10()
{
	int eval;
	ESTRUCTURA *temp;
	int cont =0;
	double mejor_objetivo = 10E38;

	Inicializar();
	start_timers(); 

	while(eval < num_evaluaciones){

		temp = Old;
		Old = New;
		New = temp; 

		SeleccionAGG();
		OX();
		MutacionAGG();
		if ( (cont%10 == 0) and (cont > 0) ) {
			for (int i = 0; i < tam_poblacion; i++) {
				New[i].objetivo = BL(i,eval);
				New[i].cambio = 0;				
			}
		}
		else {
			for (int i = 0; i < tam_poblacion; i++) {
				if (New[i].cambio != 0) {				
					evaluacion(i);
					eval++;
				}
			}
		}
		ReemplazarAGG();
		for (int i = 0; i < tam_poblacion; i++) {
			if (mejor_objetivo > New[i].objetivo){
				//cout<< New[i].objetivo <<endl;
				mejor_objetivo = New[i].objetivo;
			}
		}
		cont++;
		//cout <<cont <<endl;
	}
	double tiempo = elapsed_time();
	cout<<"El mejor resultado es: "<< mejor_objetivo<<endl;
	cout<<"El tiempo: "<<tiempo<<endl;
}

void AM_1()
{
	int eval;
	ESTRUCTURA *temp;
	int cont =0;
	double mejor_objetivo = 10E38;

	Inicializar();
	start_timers(); 

	while(eval < num_evaluaciones){

		temp = Old;
		Old = New;
		New = temp; 

		SeleccionAGG();
		OX();
		MutacionAGG();
		if ( (cont%10 == 0) and (cont > 0) ) {
			for(int i=0; i < tam_poblacion; i++){
				double random = Randfloat(0,1);
				if (random <= 0.1){
					New[i].objetivo = BL(i,eval);
					New[i].cambio = 0;
				}	
			}
		//	int i = Randint(0,tam_poblacion-1);
		//	New[i].objetivo = BL(i,eval);
		//	New[i].cambio = 0;				
			
		}
		else {
			for (int i = 0; i < tam_poblacion; i++) {
				if (New[i].cambio != 0) {				
					evaluacion(i);
					eval++;
				}
			}
		}
		ReemplazarAGG();
		for (int i = 0; i < tam_poblacion; i++) {
			if (mejor_objetivo > New[i].objetivo){
				cout<< New[i].objetivo <<endl;
				mejor_objetivo = New[i].objetivo;
			}
		}
		cont++;
		//cout <<cont <<endl;
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
			New[i].cambio = Old[ind1].cambio;
		}
		else{
			for (int j = 0; j <n_datos;j++)
				New[i].Gene[j] = Old[ind2].Gene[j];
			New[i].objetivo = Old[ind2].objetivo;
			New[i].cambio = Old[ind2].cambio;			
		}
	}
}

void ReemplazarAGG(){

	int i, j, found, mejor = 0, peor = 0;
	double mejor_Sol = Old[0].objetivo;
	double peor_Sol = New[0].objetivo;
	
	
	for (i=0, found=0; i<tam_poblacion && (!found); i++)
	  	for (j=0, found=1; (j < n_datos) && (found); j++)
	   		found = (New[i].Gene[j] == Old[mejor].Gene[j]);
	
	if (!found) {
		for (i = 0; i < tam_poblacion; i++) {
			if (peor_Sol < New[i].objetivo) {
				peor_Sol = New[i].objetivo;
				peor = i;
			}
		}
		
		for (i = 0; i < n_datos; i++)
			New[peor].Gene[i] = Old[mejor].Gene[i];
			
		New[peor].objetivo = Old[mejor].objetivo;
		New[peor].cambio = Old[mejor].cambio;
		
	}


}

void OX()
{
	int posicionA, posicionB;
	int HijoA[n_datos];
	int HijoB[n_datos];
	int poblacion_cruce = tam_poblacion*prob_AGG;
	int j, k ,l;

	for (int i = 0; i < poblacion_cruce * 2; i = i + 2) {
		posicionA = Randint(0, n_datos-2);
		posicionB = Randint(posicionA+1, n_datos-1);
		
		for ( j = 0; j < n_datos; j++) {
			HijoA[j] = -1;
			HijoB[j] = -1;
		}
		
		for ( j = posicionA; j <= posicionB; j++) {
			HijoA[j] = New[i].Gene[j];
			HijoB[j] = New[i+1].Gene[j];
		}
		
		for(k = 0, l = 0, j = posicionB+1; j < n_datos; j++) {
		
			while(k >= posicionA && k <= posicionB)
				k++;
				
			while(l >= posicionA && l <= posicionB)
				l++;
				
			if(usado(New[i+1].Gene[j],HijoA) == 0) {
				HijoA[k] = New[i+1].Gene[j];
				k++;				
			}
			
			if(usado(New[i].Gene[j],HijoB) == 0) {
				HijoB[l] = New[i].Gene[j];
				l++;
			}
		}
		
		for( j = 0; j <= posicionB; j++) {
		
			while(k >= posicionA && k <= posicionB)
				k++;
				
			while(l >= posicionA && l <= posicionB)
				l++;
				
			if(usado(New[i+1].Gene[j],HijoA) == 0) {
				HijoA[k] = New[i+1].Gene[j];
				k++;				
			}
			
			if(usado(New[i].Gene[j],HijoB) == 0) {
				HijoB[l] = New[i].Gene[j];
				l++;
			}
		}
		
		for (int j = 0; j < n_datos; j++) {
			New[i].Gene[j] = HijoA[j];
			New[i+1].Gene[j] = HijoB[j];
		}
		
		New[i].cambio = 1;
		New[i+1].cambio = 1;
	}

	
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


double BL(int cromosoma, int &evaluaciones){

	calculoInicialCentroides(cromosoma);	
	
	int i, j, k;
	int cluster, cambio = 1;
	double min, A, B, coste, objetivo_vecino;
	
	double mejor_objetivo = objetivoActual();
	evaluaciones++;

	for (i = 0; i < n_datos; i++) {

		A = (distanciaPatronCentroide(tabla[i],Centros[Conf[i]])*distanciaPatronCentroide(tabla[i],Centros[Conf[i]]));
				
		min = 10e30;
		cluster = i;
				
		for (k = 0; k < k_inicial; k++) {
			if (k != Conf[i]) {
				B = (distanciaPatronCentroide(tabla[i],Centros[k])*distanciaPatronCentroide(tabla[i],Centros[k]));
				if (B < min) {
					cluster = k;
					min = B;
				}
			}	
		}
				
		coste = min - A;

		if (Tam[Conf[i]] > 1) {
			if ((cluster != Conf[i]) && (coste < 0)) {
					
				recalculoCentroides(Conf[i], cluster, i);
				disminuirTamanio(Conf[i]);
				aumentarTamanio(cluster);
				//objetivo_vecino = objetivo_vecino + factorizacionObjetivo(Conf[i], cluster, i);
						
				Conf[i] = cluster;

				// se que al no tener factorizada la función objetivo no debería llamarla desde
				// aqui, pero lo hago así para hacer el mismo número de evaluaciones que el resto
				// de personas que si la tienen factorizada y que así los resultados sean similares...
				objetivo_vecino = objetivoActual();
				evaluaciones++;
					
				if (mejor_objetivo > objetivo_vecino) {
					mejor_objetivo = objetivo_vecino;
					//cambio = 1;
				}
			}
		}	
	}	
	
	return mejor_objetivo;
}
