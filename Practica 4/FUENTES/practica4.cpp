#include "clustering.h"
#include <iostream>
#include <cmath>
#include <stdlib.h>
#include <time.h>
#include <fstream>
#include "random_ppio.h"
#include <cstdlib>
#include <vector>
#include <list>

using namespace std;


struct hormiga{
	
	vector<int> recorrido;
	//El dato pair:
		//first -> número de elemento del conjunto de patrones.
		//second -> booleano (false si no escogido // true si escogido)
	vector<pair<int, bool> > candidatos;
	

};

/* ALGORITMO QUICKSORT.*/

int dividir(vector<pair<int, double> > &array, int inicio, int fin){
  int izq;
  int der;
  int ind;
  int indAux;
  double pivote;
  double temp;
 
  pivote = array[inicio].second;
  izq = inicio;
  der = fin;
 
  //Mientras no se crucen los índices
  while (izq < der){
    while (array[der].second > pivote){
      der--;
    }
 
    while ((izq < der) && (array[izq].second <= pivote)){
      izq++;
    }
 
    // Si todavia no se cruzan los indices seguimos intercambiando
    if(izq < der){
      temp= array[izq].second;
		indAux = array[izq].first;
      array[izq].second = array[der].second;
		array[izq].first = array[der].first;
      array[der].second = temp;
		array[der].first = indAux;
    }
  }
 
  //Los indices ya se han cruzado, ponemos el pivote en el lugar que le corresponde
  temp = array[der].second;
  indAux = array[der].first;
  array[der].second = array[inicio].second;
  array[der].first= array[inicio].first;
  array[inicio].second = temp;
  array[inicio].first = indAux;
 
  return der;
}
 
/*	Funcion recursiva para hacer el ordenamiento Quicksort	*/
void quicksort(vector<pair<int,double> > &array, int inicio, int fin)
{
  int pivote;
  if(inicio < fin){
    pivote = dividir(array, inicio, fin );
    quicksort( array, inicio, pivote - 1 );//ordeno la lista de los menores
    quicksort( array, pivote + 1, fin );//ordeno la lista de los mayores
  }
}

/*Cuando se ordena, el vectos de campos, apunta a los elementos de las listas de movimientos, vecinos y cambios.

*/
void ordenar(vector<pair<int, double> > &v){

	quicksort(v, 0, v.size() - 1 );

}

/* Distancia de un patrón a cualquier centroide del conjunto. 
	- Resultado -> tipo de dato double. 
*/
double distanciaPC(vector<double> dimPatron, vector<double> dimCentroide){
	
	double distancia=0.0;
	int dimensiones = dimPatron.size();
	for(int i=0; i < dimensiones; i++){
		distancia += (double)((1.0*dimPatron[i]-1.0*dimCentroide[i])*(1.0*dimPatron[i]-1.0*dimCentroide[i]));	
	}
	//return (sqrt(distancia));
	return distancia;

}

/* Al inicio, la hormiga sólo conoce el conjunto inicial de nodos a los que puede moverse.
	- Ninguno de los candidatos ha sido escogido.
*/
void inicializarHormiga(hormiga &h, vector<int> candidatos){

	pair<int,double> auxiliar;
	for(int i=0; i<candidatos.size(); i++){
		auxiliar.first = candidatos[i];
		auxiliar.second = false;
		h.candidatos.push_back(auxiliar);
	}

}

/* Añadir un nodo al conjunto de nodos solución de una hormiga y marcar su pertenencia. */
void aniadirNodo(hormiga &h, int posicion_nodo){
	
	h.recorrido.push_back(h.candidatos[posicion_nodo].first);
	h.candidatos[posicion_nodo].second = true;

}

/* ACS -> Actualizar los valores de feromona asociados al arco (nodo solución) que escoge la hormiga h en la iteración i.
	-t0 es el valor inicial de feromona.
	-deltha es el porcentaje que se evapora.
*/
void actualizacionLocalFeromona(vector<pair<int,double> > &feromonas, int t0, int deltha, int posicion){

	feromonas[posicion].second = ((1-deltha)*feromonas[posicion].second) + (deltha * t0);

}

/* Actualiza todos los componentes de feromona de los nodos solución de una hormiga. 
	-Aporta un 1/coste de la mejor solución.
*/
void actualizacionGlobalFeromona(hormiga h_mejor, vector<pair<int,double> > &feromonas, double costeMejor, double cons_evap){

	double aporte = 1/costeMejor;
	int nodo_solucion=0;
	while(nodo_solucion < h_mejor.recorrido.size()){
		for(int i=0; i < feromonas.size(); i++){
			if(h_mejor.recorrido[nodo_solucion] == feromonas[i].first){
				feromonas[i].second = (1-cons_evap)*feromonas[i].second+cons_evap*aporte;
				nodo_solucion += 1;
			}
		}
	}
	
}
		
/* Evapora feromona de los nodos pertenecientes a la mejor solución o de todos dependiendo de si estamos ejecutando ACS o MMAS {0 ACS -Evapora a todos.} // {1 MMAS -Evapora al mejor.}
*/
void quitarFeromona(vector<pair<int,double> >&feromonas, const double cons_evap, int modo, vector<int> nodos_solucion){

	if(modo == 0){
		
		for(int i=0; i < feromonas.size(); i++){
			feromonas[i].second = feromonas[i].second*(1-cons_evap);
		}
	}
	else{
		int pasos=0;
		while(pasos<nodos_solucion.size()){
			for(int i=0; i<feromonas.size(); i++){
				if(nodos_solucion[pasos] == feromonas[i].first){
					feromonas[i].second = feromonas[i].second*(1-cons_evap);
					pasos+=1;
				}
			}
		}
	}
}
	
/*Elegir uno de los candidatos para ser el primer nodo de la solución. */
void primerPaso(hormiga &h){
	
	int posicion_primer_nodo = Randint(0, h.candidatos.size()-1);
	h.recorrido.push_back(h.candidatos[posicion_primer_nodo].first);
	h.candidatos[posicion_primer_nodo].second = true;
	
}

/* Obtiene las valoraciones asociadas a los n-k nodos candidatos con respecto a los k ya seleccionados usando Kaufman */
vector<double> valoraciones(hormiga &h, vector<vector<double> > tabla_patrones/*, int iteracion*/){
	
	vector<double> ganancias;	
	ganancias.resize(h.candidatos.size());

	for(int i=0; i<ganancias.size(); i++){
		ganancias[i]=0.0;
	}

	
   double distancia = 0.0;
	double distancia_min=10E37;
	double valor = 0.0;

	//Para el primer patrón i no seleccionado.
	for(int i=0; i < h.candidatos.size(); i++){
		if(h.candidatos[i].second == true){
			ganancias[i] = -1.0;
		}
		else{
			ganancias[i] =0.0;
			//para cada patrón distinto de i y no seleccionado.
			for(int j=0; j < h.candidatos.size(); j++){
				if (h.candidatos[i].first != h.candidatos[j].first && h.candidatos[j].second != true){
					distancia_min=10E37;
					//Para cada centroide ya calculado: 
					//Hallar el más cercano.
					for(int k=0; k<h.recorrido.size(); k++){
						distancia = distanciaPC(tabla_patrones[h.candidatos[i].first], tabla_patrones[h.recorrido[k]]);
						if(distancia < distancia_min){
							distancia_min=distancia;
						}
					}
					valor = max((distancia_min - (distanciaPC(tabla_patrones[h.candidatos[j].first], tabla_patrones[h.candidatos[i].first]))),0.0);
					if(valor>0){
						ganancias[i] += valor;
					}
				}
			}
		}
	}
	
	return ganancias;

}

/* Devuelve el vector con el coste (Dado por Kaufman) multiplicado por la feromona asociada a cada nodo. */
vector<double> heuristica(vector<double> ganancias, vector<pair<int,double> > feromona, double alpha, double betha){
	
	vector<double> valoraciones;
	valoraciones.resize(ganancias.size());
	
	for(int i=0; i < valoraciones.size(); i++){
		valoraciones[i] = ((pow(feromona[i].second, alpha)) * (pow(ganancias[i], betha)));
	}

	return valoraciones;

}

/* Escoger el patrón de mejor ganancia del total. (Greedy) */
int escogerMejor(vector<double> valoraciones){

	double mejor = 0.0;
	int posicion = 0;
	for(int i=0; i < valoraciones.size(); i++){
		if(valoraciones[i] > mejor){
			mejor = valoraciones[i];
			posicion = i;
		}
	}

	return posicion; 	

}

/* Elegir un candidato aleatoriamente con elección por ruleta .*/
int escogerPorRuleta(vector<double> valoraciones){
	
	/* Sumar las valoraciones heurísticas de todos los elementos menos los ecogidos (aquellos con valoración negativa).*/
	double suma_total = 0.0;
	for(int i=0; i < valoraciones.size(); i++){
		if(valoraciones[i] > 0){
			suma_total += valoraciones[i];
		}
	}

	vector<pair<int, double> > probabilidades;
	pair<int,double> aux;
	for(int i=0; i < valoraciones.size(); i++){
		if(valoraciones[i]>0){
			aux.first = i;
			aux.second = valoraciones[i]/suma_total;
			probabilidades.push_back(aux);
		}	
	}
	
	vector<pair<double,double> > intervalos;
	pair<double,double> auxiliar;
	double suma_uno, suma_2;

	for(int i=0; i < probabilidades.size(); i++){
		if(i==0){
			auxiliar.first = 0;
			auxiliar.second = probabilidades[i].second;
			intervalos.push_back(auxiliar);
		}
		else{
			suma_uno = intervalos[i-1].second + probabilidades[i].second;
			auxiliar.first = intervalos[i-1].second;
			auxiliar.second = suma_uno;
			intervalos.push_back(auxiliar);
		}
	}

	double aleatorio = Rand();
	int posicion = 0;
	for(int i=0; i < intervalos.size(); i++){
		if(aleatorio >= intervalos[i].first && aleatorio < intervalos[i].second){
			posicion = i;
		}
	}

	posicion = probabilidades[posicion].first;

	return posicion;

}

/* En ACS se escogen patrones de dos formas dependiendo de un valor probabilístico.
	*/
int escogerCandidatoACS(vector<double> valoraciones, int q){
	
	double probabilidad = Rand();
	int candidato_escogido;
	
	if(probabilidad <= q){
		candidato_escogido = escogerMejor(valoraciones);
		
	}
	else{
		candidato_escogido = escogerPorRuleta(valoraciones);
	}

	return candidato_escogido;

}

/* Crear una solución a patir de los medoides.
	
*/
void establecerConfiguracion(hormiga &h, vector<vector<int> > &matriz_pertenencia, int num_patrones, int numero_clusters, vector<vector<double> > tabla_patrones){
	
	/* Crear un vector de patrones con los seleccionados. */
	vector<pair<int,double> > patrones;
	patrones.resize(num_patrones);
	for(int i=0; i < patrones.size(); i++){
		patrones[i].first = i;
		patrones[i].second = false;
	}

	/* Inicialización de la estructura para la matriz de pertenencia. */
	matriz_pertenencia.resize(num_patrones);
	for(int i=0; i < matriz_pertenencia.size(); i++){
		matriz_pertenencia[i].resize(numero_clusters);
	}

	for(int i=0; i < matriz_pertenencia.size(); i++){
		for(int j=0; j < matriz_pertenencia[i].size(); j++){
			matriz_pertenencia[i][j] = 0;
		}
	}

	for(int i=0; i < h.recorrido.size(); i++){
		matriz_pertenencia[h.recorrido[i]][i] = 1;
		patrones[h.recorrido[i]].second = true;
	}

	/* Asignar los patrones al cluster más cercano sin modificar los centroides durante el proceso. */
	int posicion_patron;
	for(int i=0; i < patrones.size(); i++){
		if(patrones[i].second != true){

			posicion_patron = patrones[i].first;
			double min = 5E37;
			int cluster_elegido=0;
			for(int j=0; j<numero_clusters; j++){
				double distancia = distanciaPC(tabla_patrones[posicion_patron], tabla_patrones[h.recorrido[j]]);	//Se calcula el cluster de distancia mínima.
				if(distancia<min){
					min = distancia;
					cluster_elegido=j;
				}
			}
			matriz_pertenencia[posicion_patron][cluster_elegido]=1;
			patrones[i].second = true;
		}
	}

}

/* Mejor de todas las hormigas.
	
*/
int mejorHormiga(vector<Clustering> soluciones){

	int posicion_mejor=0;
	double mejor_coste = 2e50;
	for(int i=0; i < soluciones.size(); i++){
		if(soluciones[i].costeSolucion() < mejor_coste){
			mejor_coste = soluciones[i].costeSolucion();
			posicion_mejor = i;
		}
	}
	
	return posicion_mejor;

}

/* Comprobar si una solución es mejor que otra, entendiendo por mejor que su valor es mejor. */
void comprobarMejorSolucion(Clustering &mejor, hormiga &mejor_hormiga, Clustering mejor_solucion_actual, hormiga mejor_colonia_actual){

	if(mejor.costeSolucion() > mejor_solucion_actual.costeSolucion()){
		mejor = mejor_solucion_actual;
		mejor_hormiga = mejor_colonia_actual;
	}

}	

/* Inicializar los valores del vector de feromonas a un valor v. */
void inicializarFeromonas(vector<double> &feromonas, double valor_inicial){

	if(valor_inicial <= 0){
		cout << "El valor inicial para el vector de feromonas es incorrecto. No puede ser menor o igual que cero." << endl;
		exit(1);
	}

	for(int i=0; i < feromonas.size(); i++){
		feromonas[i] = valor_inicial;
	}

}

//Cargar el fichero con los patrones y sus dimensiones.
void cargarEstructura(const char * nombre_archivo, vector<vector<double> > &tabla_patrones){
	
	ifstream archivo(nombre_archivo);
	int i=0, j=0;
	// Tamaño de las dimensiones para aumentar los índices i y j.
	int dimensiones = tabla_patrones[0].size();
	while (!archivo.eof()) {
		archivo >> tabla_patrones[i][j];
		j++;
		i += j/ dimensiones; 
		j= j % dimensiones; 

	}

	archivo.close();    

}

/* FUNCIONES PARA LECTURA DE ESTRUCTURAS. */
//Lectura del vector de candidatos.
void leerVectorCandidatos(vector<int> candidatos){

	cout << "El número de candidatos seleccionado es: " << candidatos.size() << endl;
	for(int i=0; i < candidatos.size(); i++){
		cout << candidatos[i] << ", ";
	}
	cout << endl;
}

//Lectura de la matriz de patrones.
template<typename tipoDato>
void leerMatrizPatrones(const vector<vector<tipoDato> > matriz){
	
	cout << endl;
	for(int i=0; i < matriz.size(); i++){
		cout << "Patrón número " << i << " con dimensiones -> "; 
		for(int j=0; j < matriz[i].size(); j++){
			cout << matriz[i][j] << " ";
		}
		cout << endl;
	}
	cout << endl;

}

//Lectura de una hormiga.
void mostrarHormiga(hormiga h){
	
	for(int i=0; i < h.recorrido.size(); i++){
		cout << h.recorrido[i] << ", ";
	}
	cout << endl;
	for(int i=0; i < h.candidatos.size(); i++){
		cout << h.candidatos[i].first << " [";
		if(h.candidatos[i].second == true){
			cout << "S";
		}
		else{
			cout << "NS";
		}
		cout << "]" << " ,";
	}
	cout << endl;
}

void mostrarColonia(vector<hormiga> colonia){

	for(int i=0; i < colonia.size(); i++){
		mostrarHormiga(colonia[i]);
	}

}

/* Se obtiene la lista de candidatos*/
void obtenerListaCandidatos(vector<vector<double> > tabla_patrones, vector<int> &candidatos,int patrones,int dimensiones,int clusters, int num_candidatos){

	vector<pair<int,bool> >lista_patrones;	//Lista que almacena todos los patrones.

	vector<pair<int,double> > ganancias;	//Vector que almacena la ganancia de cada patrón.
	ganancias.resize(patrones);

	for(int i=0; i<ganancias.size(); i++){
		ganancias[i].first=i;
		ganancias[i].second=0.0;
	}
	
	vector<double> centro;	//Vector que almacena las dimensiones (X0..Xn-1) de la media de todos los elementos del conjunto.
	centro.resize(dimensiones);

	for(int i=0; i<centro.size(); i++){	//inicializar centro a 0.0
		centro[i]=0.0;
	}

	pair<int, bool> auxiliar;
	for(int i=0; i<patrones; i++){	//La inicializamos con todos los patrones del 0 al n-1.
		auxiliar.first = i;
		auxiliar.second = false;
		lista_patrones.push_back(auxiliar);
	}

	//Recorremos todos los patrones y vamos acumulando sus dimensiones en el vector centro.
	for(int i=0; i < lista_patrones.size(); i++){
		for(int z=0; z<dimensiones; z++){
			centro[z]+=tabla_patrones[i][z];
		}
	}
	for(int j=0; j<dimensiones; j++){	//Hacemos la media.
		centro[j]=centro[j]/(double)lista_patrones.size();
	}
	
	/* Buscamos el patrón con menor distancia a la media de todos los patrones (El más céntrico). */
	double distancia_min=10E37;
	double distancia = 0.0;

	int mejor;
	//Recorremos todos los patrones en busca del mas cercano a la media.
	for(int i=0; i < lista_patrones.size(); i++){
		distancia = distanciaPC(tabla_patrones[i], centro)*distanciaPC(tabla_patrones[i], centro);
		if(distancia < distancia_min){
			mejor = i;
			distancia_min = distancia;
		}
	}

	//Introducir el patrón más céntrico en la lista de candidatos.
	candidatos.push_back(mejor);
	lista_patrones[mejor].second = true;

	/* Ahora comienza el bucle greedy en el que, en cada paso, se
       escoge el patron mas alejado de los centros ya determinados
       y con mas ejemplos alrededor como nuevo centro */
       
	double valor = 0.0;
	//Usando únicamente el medoide más céntrico del conjutno obtenemos las ganancias de los demás.
	for(int c=1; c<2; c++){
		//Para el primer patrón i no seleccionado.
		for(int i=0; i < lista_patrones.size(); i++){
			if(lista_patrones[i].second == true){
				ganancias[lista_patrones[i].first].second=-1.0;
			}
			else{
				ganancias[i].second=0.0;
				//para cada patrón distinto de i y no seleccionado.
				for(int j=0; j < lista_patrones.size(); j++){
					if (lista_patrones[i].first!=lista_patrones[j].first && lista_patrones[j].second != true){
						distancia_min=10E37;
						//Para cada centroide ya calculado: 
						//Hallar el más cercano.
						for(int k=0; k<c; k++){
							distancia = distanciaPC(tabla_patrones[lista_patrones[i].first], tabla_patrones[mejor]);
							if(distancia < distancia_min){
								distancia_min=distancia;
							}
						}
						valor = max((distancia_min - (distanciaPC(tabla_patrones[lista_patrones[j].first], tabla_patrones[lista_patrones[i].first]))),0.0);
						if(valor>0){
							ganancias[lista_patrones[i].first].second += valor;
						}
					}
				}
			}
		}
	}
	//Ordenar el vector con las ganancias obtenidas con respecto al más céntrico de los patrones y elegirlas para la lista de candidatos.
	cout << "G.S" << ganancias.size() << endl;
	ordenar(ganancias);
	for(int i=0; i < ganancias.size(); i++){
		cout << ganancias[i].first << " , " << ganancias[i].second << " ";
	}
	cout << endl;
	for(int i=0; i<num_candidatos-1; i++){
		candidatos.push_back(ganancias[ganancias.size()-(i+1)].first);
	}
	cout << "TAMCAN: " << candidatos.size() << endl;
	for(int i=0; i < candidatos.size(); i++){
		cout << candidatos[i] << endl;
	}
}

/* Crea los individuos de la colonia y elige, para cada uno de ellos, el nodo de partida. */
void configurarColonia(vector<hormiga> &colonia, vector<int> candidatos){

	for(int i=0; i < colonia.size(); i++){
		hormiga auxiliar;
		inicializarHormiga(auxiliar, candidatos);
		primerPaso(auxiliar);
		colonia[i]=auxiliar;
	}

}

void crearSolucionesAsociadas(vector<Clustering> &soluciones, int numero_soluciones, int num_patrones, int numero_dimensiones, int numero_clusters, vector<vector<double> > patrones, vector<vector<vector<int> > > matrices_pertenencia){
	
	for(int i=0; i < numero_soluciones; i++){
		Clustering auxiliar(num_patrones, numero_dimensiones, numero_clusters, patrones);
		auxiliar.iniciarConfiguracion(matrices_pertenencia[i]);
		soluciones[i] = auxiliar;		
	}

}

int reglaTransicionACS(hormiga &h, vector<vector<double> > tabla_patrones, double p, vector<pair<int,double> > &feromonas, double alpha, double betha){
		
		vector<double> val;
		val = valoraciones(h, tabla_patrones);	
		vector<double> x;
		x = heuristica(val, feromonas, alpha, betha);
		int candidato_elegido = escogerCandidatoACS(val, p);
		aniadirNodo(h, candidato_elegido);

		return candidato_elegido;

}

int reglaTransicionMMAS(hormiga &h, vector<vector<double> > tabla_patrones, vector<pair<int,double> > &feromonas, double alpha, double betha){
		
		vector<double> val;
		val = valoraciones(h, tabla_patrones);	
		vector<double> x;
		x = heuristica(val, feromonas, alpha, betha);
		int candidato_elegido = escogerPorRuleta(val);
		aniadirNodo(h, candidato_elegido);

		return candidato_elegido;

}

void copiarRecorrido(hormiga &h, vector<int> recorrido_aleatorio){
	
	for(int i=0; i < recorrido_aleatorio.size(); i++){
		h.recorrido.push_back(recorrido_aleatorio[i]);
	}
}

void truncarValoresFeromona(vector<pair<int,double> > &feromonas, double Gmax, double Gmin){

	for(int i=0; i < feromonas.size(); i++){
		if(feromonas[i].second > Gmax){
			feromonas[i].second = Gmax;
		}
		if(feromonas[i].second < Gmin){
			feromonas[i].second = Gmin;
		}
	}

}

void ACS(const char *archivo){

	/* Constantes del algoritmo. */
	int num_patrones, num_dim_patron, numero_clusters;
	unsigned long sem;
	int num_candidatos;
	int num_ants;
	double cons_evap;
	double alpha, betha;
	double deltha;
	double prob_eleccionACS;
	double t0 = 10e-6;
	int num_iter_LS;
	int eval = 0;
	int max_eval;
	/* Fin constantes. */

	/* Captura de valores por teclado. */
	cout << "Introduce un valor para la sem: "; 
	cin >> sem;
	Set_random(sem);
	cout << " Introduce los valores que se indiquen ";
	cout << "Patrones: ";
	cin >> num_patrones;
	cout << "Número de dimensiones de cada patrón: ";
	cin >> num_dim_patron;
	cout << "Número de clusters: ";
	cin >> numero_clusters;
	cout << "Número de hormigas de la colonia: " << endl;
	cin >> num_ants;
	cout << " Número de candidatos que se van a elegir de entre todos los elementos del problema: ";
	cin >> num_candidatos;

	if(num_candidatos <= 0 || num_candidatos > num_patrones){
		cout << "El número de candidatos no es válido. " << endl;
		exit(1);
	}
	
	cout << "Constante de evaporación: ";
	cin >> cons_evap;
	cout << "Alpha: ";
	cin >> alpha;
	cout << "Betha: ";
	cin >> betha;
	cout << "Deltha: ";
	cin >> deltha;
	cout << "Probabilidad de escoger el mejor candidato: ";
	cin >> prob_eleccionACS;
	cout << "Número de iteraciones para la búsqueda local: ";
	cin >> num_iter_LS;
	cout << "Introduce el número máximo de evaluaciones: "; 
	cin >> max_eval;
	/* Fin captura de datos. */

	/* Asignar memoria para las estructuras. */
	// TABLA DE PATRONES:
	
	vector<vector<double> > tabla_de_patrones;
	tabla_de_patrones.resize(num_patrones);
	// TAMAÑO DE COLUMNAS -> Para cada patrón i pertenenciente al conjunto X, sus dimensiones.
	for(int i=0; i < num_patrones; i++){
		tabla_de_patrones[i].resize(num_dim_patron);
	}
	//Cargar la estructura del archivo correspondiente.
	cargarEstructura(archivo, tabla_de_patrones);
	//Lectura de datos.
	leerMatrizPatrones(tabla_de_patrones);
	
	//LISTA DE CANDIDATOS.
	//Vector de enteros con los patrones candidatos.
	vector<int> candidatos;
	//Obtener la lista de candidatos a partir del total de patrones del conjunto usando Kaufmann.
	obtenerListaCandidatos(tabla_de_patrones, candidatos, num_patrones, num_dim_patron, numero_clusters, num_candidatos);
	//Lectura de la lista de candidatos.
	leerVectorCandidatos(candidatos);
	
	// FEROMONA
	// Vector con tantas posiciones como candidatos se consideren en el problema. Todos ellos inicializados a t0.
	vector<pair<int,double> >feromonas;
	pair<int,double> auxiliar;
	for(int i=0; i < num_candidatos; i++){
		auxiliar.first = candidatos[i];
		auxiliar.second = t0;
		feromonas.push_back(auxiliar);
	}

	// COLONIA DE HORMIGAS:
	// Vector con las h hormigas que forman la colonia.
	vector<hormiga> colonia;
	colonia.resize(num_ants);
	
	// Mejor hormiga.
	hormiga mejor_hormiga_global;
	
	// CONJUNTO DE SOLUCIONES FORMADAS POR CADA HORMIGA:
	// Vector con las s soluciones asociadas a cada hormiga.
	vector<Clustering> soluciones_hormigas;
	soluciones_hormigas.resize(num_ants);

	// Mejor solucion global.
	Clustering mejor_global(num_patrones, num_dim_patron, numero_clusters, tabla_de_patrones);
	
	// CONJUNTO DE CONFIGURACIONES:
	// Vector que contiene las matrices de pertenencia de cada configuración de hormigas.
	// -> Los k medoides en sus respectivos clusters.
	// -> Los n-k patrones asignados al cluster más cercano.
	vector<vector<vector<int> > > matrices_pertenencia;
	matrices_pertenencia.resize(num_ants);
		
	double t_inicial = clock();

	while(eval < max_eval){
	// INICIALIZAR LA COLONIA.
	// -> Creación de las hormigas que formarán parte de la colonia.
	// -> Elección de un candidato al azar como primer nodo.
	configurarColonia(colonia, candidatos);
	
	//Lectura de la colonia.
	//mostrarColonia(colonia);
	
	//Bucle para la regla de transición.
	for(int i=1; i < numero_clusters; i++){
		for(int j=0; j < colonia.size(); j++){
			int posicion = reglaTransicionACS(colonia[j], tabla_de_patrones, prob_eleccionACS, feromonas, alpha, betha);
			actualizacionLocalFeromona(feromonas, t0, deltha, posicion);
		}
	}
	
	for(int i=0; i < num_ants; i++){
		establecerConfiguracion(colonia[i], matrices_pertenencia[i], num_patrones, numero_clusters, tabla_de_patrones);
	}
	
	// Inicialización de las soluciones asociadas a cada hormiga.
	// Configuración inicial de los clusters.
	crearSolucionesAsociadas(soluciones_hormigas, num_ants, num_patrones, num_dim_patron, numero_clusters, tabla_de_patrones, matrices_pertenencia);

	for(int i=0; i < num_ants; i++){
		soluciones_hormigas[i].busquedaLocal(num_iter_LS, eval);
	}

	/* Búsqueda de la mejor hormiga (En base a su solución correspondiente). */
	int posicion_mejor = mejorHormiga(soluciones_hormigas);
	if(eval == 0){
		mejor_global = soluciones_hormigas[posicion_mejor];
		mejor_hormiga_global = colonia[posicion_mejor];
	}
	else{
		comprobarMejorSolucion(mejor_global, mejor_hormiga_global, soluciones_hormigas[posicion_mejor], colonia[posicion_mejor]);
	}
	
	/* Evaporación de feromona ACS. */
	
	quitarFeromona(feromonas, cons_evap, 1, mejor_hormiga_global.recorrido);
	actualizacionGlobalFeromona(mejor_hormiga_global, feromonas, mejor_global.costeSolucion(), cons_evap);

	}

	double t_final= clock()- t_inicial;
	/* Lectura de la solución.*/
	// Coste de la mejor configuración.
	// Mejor configuración.
	// Hormiga correspondiente a la mejor configuración.
	mejor_global.leerClusters();
	mostrarHormiga(mejor_hormiga_global);
	cout << "Tiempo Total: " << t_final/CLOCKS_PER_SEC << endl;
	cout << "Coste de la mejor solución encontrada: " << mejor_global.costeSolucion() << endl;

}

void MMAS(const char *archivo){

	/* Constantes del algoritmo. */
	int num_patrones, num_dim_patron, numero_clusters;
	unsigned long sem;
	int num_candidatos;
	int num_ants;
	double cons_evap;
	double alpha, betha;
	//double deltha;
	//double prob_eleccionACS;
	double Gmax;
	double Gmin;
	int num_iter_LS;
	int eval = 0;
	int max_eval;
	/* Fin constantes. */

	/* Captura de valores por teclado. */
	cout << "Introduce un valor para la sem: "; 
	cin >> sem;
	Set_random(sem);
	cout << "Introduce los valores que se indican: ";
	cout << "Patrones: ";
	cin >> num_patrones;
	cout << "Número de dimensiones de cada patrón: ";
	cin >> num_dim_patron;
	cout << "Número de clusters: ";
	cin >> numero_clusters;
	cout << "Número de hormigas de la colonia: " << endl;
	cin >> num_ants;
	cout << "Número de candidatos: ";
	cin >> num_candidatos;

	if(num_candidatos <= 0 || num_candidatos > num_patrones){
		cout << "El número de candidatos no es válido. " << endl;
		exit(1);
	}

	cout << "Constante de evaporación: ";
	cin >> cons_evap;
	cout << "Alpha: ";
	cin >> alpha;
	cout << "Betha: ";
	cin >> betha;
	cout << "Introduce el número de iteraciones para la búsqueda local: ";
	cin >> num_iter_LS;
	cout << "Introduce el número máximo de evaluaciones: "; 
	cin >> max_eval;
	/* Fin captura de datos. */

	/* Asignar memoria para las estructuras. */
	// TABLA DE PATRONES:
	
	vector<vector<double> > tabla_de_patrones;
	tabla_de_patrones.resize(num_patrones);
	for(int i=0; i < num_patrones; i++){
		tabla_de_patrones[i].resize(num_dim_patron);
	}
	//Cargar la estructura del archivo correspondiente.
	cargarEstructura(archivo, tabla_de_patrones);
	//Lectura de datos.
	leerMatrizPatrones(tabla_de_patrones);
	
	
	//Vector de enteros con los patrones candidatos.
	vector<int> candidatos;
	//Obtener la lista de candidatos a partir del total de patrones del conjunto usando Kaufmann.
	obtenerListaCandidatos(tabla_de_patrones, candidatos, num_patrones, num_dim_patron, numero_clusters, num_candidatos);
	//Lectura de la lista de candidatos.
	leerVectorCandidatos(candidatos);
	
	// FEROMONA
	
	vector<pair<int,double> >feromonas;
	pair<int,double> auxiliar;
	for(int i=0; i < num_candidatos; i++){
		auxiliar.first = candidatos[i];
		auxiliar.second = Gmax;
		feromonas.push_back(auxiliar);
	}

	// COLONIA DE HORMIGAS:
	
	vector<hormiga> colonia;
	colonia.resize(num_ants);
	
	// Vector para poder copiar el recorrido aleatorio de la primera solución en la mejor hormiga global.
	vector<int> recorrido_aleatorio;
	recorrido_aleatorio.resize(numero_clusters);

	// Mejor hormiga.
	hormiga mejor_hormiga_global;
	
	// CONJUNTO DE SOLUCIONES FORMADAS POR CADA HORMIGA:
	// Vector con las s soluciones asociadas a cada hormiga.
	vector<Clustering> soluciones_hormigas;
	soluciones_hormigas.resize(num_ants);

	// Solucion Inicial -> Aleatoria.
	// Con ella se darán valores a Gmax y Gmin.
	Clustering aleatorio(num_patrones, num_dim_patron, numero_clusters, tabla_de_patrones);
	aleatorio.solucionAleatoria(recorrido_aleatorio);

	// Se inicializan los valores de Gmax y Gmin como:
	// Gmax -> 1/ρ·Coste(Saleatoria)
	// Gmin -> Gmax/500
	Gmax = 1/(cons_evap*aleatorio.costeSolucion());
	Gmin = Gmax/500;

	// Mejor solucion global.
	Clustering mejor_global(num_patrones, num_dim_patron, numero_clusters, tabla_de_patrones);
	
	//Al principio la mejor solución global es la aleatoria que se crea al comienzo de la ejecución.
	mejor_global = aleatorio;
	
	// La mejor hormiga global también es la que tiene como recorrido los k primeros elementos de la solución aleatoria.
	copiarRecorrido(mejor_hormiga_global, recorrido_aleatorio);
	
	// CONJUNTO DE CONFIGURACIONES:
	// Vector que contiene las matrices de pertenencia de cada configuración de hormigas.
	
	vector<vector<vector<int> > > matrices_pertenencia;
	matrices_pertenencia.resize(num_ants);
			
	double t_inicial = clock();

	while(eval < max_eval){
	// INICIALIZAR LA COLONIA.
	
	configurarColonia(colonia, candidatos);
	
	
	mostrarColonia(colonia);
	
	//Bucle para la regla de transición.
	for(int i=1; i < numero_clusters; i++){
		for(int j=0; j < colonia.size(); j++){
			int posicion = reglaTransicionMMAS(colonia[j], tabla_de_patrones, feromonas, alpha, betha);
			//actualizacionLocalFeromona(feromonas, t0, deltha, posicion);
		}
	}
		cout << "4" << endl;
	for(int i=0; i < num_ants; i++){
		establecerConfiguracion(colonia[i], matrices_pertenencia[i], num_patrones, numero_clusters, tabla_de_patrones);
	}
	
	
	// Configuración inicial de los clusters.
	crearSolucionesAsociadas(soluciones_hormigas, num_ants, num_patrones, num_dim_patron, numero_clusters, tabla_de_patrones, matrices_pertenencia);
	cout << "5" << endl;
	for(int i=0; i < num_ants; i++){
		soluciones_hormigas[i].busquedaLocal(num_iter_LS, eval);
	}

	/* Búsqueda de la mejor hormiga (En base a su solución correspondiente). */
	int posicion_mejor = mejorHormiga(soluciones_hormigas);
	
	bool actualizar = false;
	if(mejor_global.costeSolucion() > soluciones_hormigas[posicion_mejor].costeSolucion()){
		actualizar = true;
	}
	cout << "Global: " << mejor_global.costeSolucion() << endl;
	comprobarMejorSolucion(mejor_global, mejor_hormiga_global, soluciones_hormigas[posicion_mejor], colonia[posicion_mejor]);
	
	if(actualizar ==true){
		Gmax = 1/(cons_evap * mejor_global.costeSolucion());
		Gmin = Gmax/500;
	}
	cout << "6" << endl;
	
	quitarFeromona(feromonas, cons_evap, 0, mejor_hormiga_global.recorrido);
	actualizacionGlobalFeromona(mejor_hormiga_global, feromonas, mejor_global.costeSolucion(), cons_evap);
	
	// Truncar los valores que excedan Gmax a Gmax y los que queden por debajo de Gmin a Gmin
	truncarValoresFeromona(feromonas, Gmax, Gmin);

	}

	double t_final= clock()- t_inicial;
	/* Lectura de la solución.*/
	
	mejor_global.leerClusters();
	mostrarHormiga(mejor_hormiga_global);
	cout << "Tiempo Total: " << t_final/CLOCKS_PER_SEC << endl;
	cout << "Coste de la mejor solución encontrada: " << mejor_global.costeSolucion() << endl;
}

int main(int argc, char **argv){

	const char * cadena = argv[1];
	char algoritmo_ACO;
	cout << "########### Algoritmos basados en colonias de hormigas (ACO)" << endl;
	cout << "Algoritmos: " << endl;
	cout << " 1 ACS -> Ant Colony Sistem. " << endl;
	cout << " 2 MMAS -> MaxMin Ant System. " << endl;
	cout << " Escoger el algoritmo a ejecutar. " << endl;
	cout << "     A - para Ant Colony System  " << endl;
	cout << "     M - para Max Min Ant Systeam  " << endl;
	cout << "Introducir los parámetros que se indican por pantalla." << endl;
	
	
	cout << "¿Qué algoritmo quieres ejecutar? " << endl;
	cout << " 1 ACS -> Ant Colony Sistem. " << endl;
	cout << " 2 MMAS -> MaxMin Ant System. " << endl;
	cout << " 3 Nada. " << endl;
	cin >> algoritmo_ACO;
	
	if(algoritmo_ACO == 'A'){
		ACS(cadena);
	}
	else if(algoritmo_ACO == 'M'){
		MMAS(cadena);
	}
	else{
		cout << "Error en la seleccion. " << endl;
	}	
	
}
