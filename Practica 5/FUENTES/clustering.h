
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


double distanciaPatronCentroide(double *x, double *y);//->
int clusterMasCercano(double *patron);//->
void recalculoCentroides(int cluster, int patron);//->
void calculoInicialCentroides(int cromosoma);
void recalculoCentroides(int cluster, int patron);
void recalculoCentroidesIni(int cluster, int patron);


void Inicializar();//->

void AGG();

double J(int cromosoma) ;//Funcion objetivo->
void evaluacion(int cromosoma);

void SeleccionAGG();//-->
void ReemplazarAGG();//->
void OX();//-->
void MutacionAGG();//-->

double BL(int cromosoma, int& eval);
void AM_10();
void AM_1();

