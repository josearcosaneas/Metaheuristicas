#include "define.h"


extern TTABLA tabla;
extern ESTRUCTURA *Old, *New;
extern TipoConfiguracion Conf, Tam, HijoA, HijoB;

extern TipoCentros Centros;
extern int n_datos, n_variables, k_inicial, k_actual;
    
extern float semilla;

extern unsigned long Seed;

extern char fich_datos[100];

extern void Input (int, char**);
extern void liberarMemoriaAGE();//->
extern void liberarMemoriaAGG();

extern double distanciaPatronCentroide(double *x, double *y);//->
extern int clusterMasCercano(double *patron);//->
extern void recalculoCentroides(int cluster, int patron);//->
extern void calculoInicialCentroides(int cromosoma);
extern void recalculoCentroides(int cluster, int patron);
extern void recalculoCentroidesIni(int cluster, int patron);


extern void Inicializar();//->
//-------------------
extern void AGG();
extern void AGE();
//--------------------
extern double J(int cromosoma) ;//Funcion objetivo->
extern void evaluacion(int cromosoma);
extern void evaluacionAGE(int cromosoma);

extern void SeleccionAGG();//-->
extern void ReemplazarAGG();//->
extern void OX();//-->
extern void MutacionAGG();



extern double BL(int cromosoma,int& eval);
extern void AM();
