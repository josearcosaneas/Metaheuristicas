
#include <cstdlib>
#include "BL.h"
#include <vector>
using namespace std;

class BMP {
    vector <vector <float> >* dat;
    int generar;
    int k;
    float J;
    int iteraciones;
    clock_t tiempoejecucion;
public:
    BMP(int _k,  vector < vector< float > > * _dat, int _iteraciones, int _generar){
        generar=_generar;
        k=_k;
        dat=_dat;
        iteraciones=_iteraciones;
        tiempoejecucion=0;
    }

    BMP (int _k,  vector < vector< float > > * _dat, int _iteraciones){
        generar=25;
        k=_k;
        dat=_dat;
        iteraciones=_iteraciones;
        tiempoejecucion=0;
    }
    float ejecutar (){
        BL *bl;
        bl=new BL(k,dat);
        cout << "0%" <<endl;
        bl->ejecuta(iteraciones);
        tiempoejecucion=bl->gettiempoejecucion();
        J=bl->getJ();
        delete bl;
        for (int cont=1; cont < generar; cont++){
            cout << (float)cont/(float)generar*100.0 <<"%  ------>>> Tiempo estimado total: " << (int)((100-(float)cont/(float)generar*100.0)*bl->getTime())/60 << " min" <<  endl;
            bl=new BL(k,dat);
            bl->ejecuta(iteraciones);
            float auxJ=bl->getJ();
            tiempoejecucion+=bl->gettiempoejecucion();
            delete bl;
            if (auxJ<J){
                J=auxJ;
            }
        }
        cout << "Tiempo: " << (float)tiempoejecucion/(float)CLOCKS_PER_SEC << " seg." << endl;
        return J;
    }

};

