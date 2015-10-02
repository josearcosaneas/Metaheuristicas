
#include "define.h"

TTABLA tabla;
ESTRUCTURA *Old, *New;
TipoConfiguracion Conf, Tam, HijoA, HijoB;
TipoCentros Centros;

int n_datos, n_variables, k_inicial;

float semilla;

unsigned long Seed;

int num_cruces, num_mutaciones;

char fich_datos[100];

void Input(int,char**);//->
void liberarMemoriaAGG();//->
void liberarMemoriaAGE();//->

double distanciaPatronCentroide(double *x, double *y);//->
int clusterMasCercano(double *patron);//->
void recalculoCentroides(int cluster, int patron);//->

void Inicializar();//->

void AGG();
void AGE();

double J(int cromosoma) ;//Funcion objetivo->
void evaluacion(int cromosoma);
void evaluacionAGE(int cromosoma);

void SeleccionAGG();//-->
void ReemplazarAGG();//->
void SeleccionAGE();//-->
void ReemplazarAGE();//->
void OX();//-->
void OXAGE();//-->
void MutacionAGG();//-->
void MutacionAGE();//-->
