
#ifndef ITS_H
#define	ITS_H
#include "BL.h"
#include <vector>
#include <iostream>
using namespace std;

class ITS{
    int veces;
    float J;
    vector <int> solucion;
    vector < vector <float> > *dat;
    int k, iteracionesbl;
public:
    ITS(int _veces, int _k, int _itera, vector < vector <float> > * _dat);
    void ejecutar();
    vector <int> mutar (vector <int> sol);
};


#endif	/* ITS_H */
