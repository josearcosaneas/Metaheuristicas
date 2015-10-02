
#ifndef BL_H
#define	BL_H
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


class BL {
public:
    BL(int c,  vector < vector< float > > * _dat);
    void ejecuta(int veces);
    float getJ();
    float getTime();
    float calcularJ(vector <int> &vs);
    float gettiempoejecucion();
    vector <vector <float > > calcularcentroides (vector <int> &vs);
private:
    //Vector que controla los centroides
    vector < vector <float> > centroides;
    //Puntero a los datos
    vector <vector <float> >* dat;
    //n=numero de centroides, d=numero de datos que existen
    int k, d, dim;
    vector <int> vectorsolucion;
    float tiempoejecucion;
    float J;
};

#endif	/* BL_H */

