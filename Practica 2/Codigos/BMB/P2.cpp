#include <cstdlib>
#include "BMP.cpp"
#include <iostream>
#include "lecturaarchivo.cpp"

using namespace std;

/*
 *
 */


int main(int argc, char** argv) {
    while (true){
        cout << "/***********************************/" << endl;
        datos dat;
        string nombrearchivo;
        int ejecuciones=10000, opcion=0, cluss=7, columnas=2;
        while (opcion==0){
            cout << "Archivos:" <<endl;
            cout << "1 - Aggregation.txt"<<endl;
            cout << "2 - wdbc.txt"<<endl;
            cout << "3 - yeast.txt"<<endl;
            cout << "4 - Cambiar el numero de iteraciones, el actual es: " << ejecuciones << endl;
            cout << "5 - Cambiar el numero de clusster, el actual es: " << cluss << endl;
            cout << "6 - Otro archivo" << endl;
            cout << "Opcion: ";
            cin >> opcion;
            switch(opcion){
                case 1: nombrearchivo="Aggregation.txt"; columnas=2; cluss=7;break;
                case 2: nombrearchivo="wdbc.txt"; columnas=31; cluss=2;break;
                case 3: nombrearchivo="yeast.txt"; columnas= 8; cluss=10 ;break;
                case 4:
                    cout << "Nuevo valor de iteraciones: ";
                    cin >> ejecuciones;
                    while (ejecuciones < 0){
                        cout << "Nuevo valor de iteraciones: ";
                        cin >> ejecuciones;
                    }
                    opcion=0; break;
                case 5:
                    cout << "Nuevo valor de clusster: ";
                    cin >> cluss;
                    while (cluss < 0){
                        cout << "Nuevo valor de clusster: ";
                        cin >> cluss;
                    }
                    opcion=0;break;
                case 6:
                    cout << "Introduce el nombre del archivo: ";
                    cin >> nombrearchivo;
                    cout << "Columnas: ";
                    cin >> columnas;
                    break;
                default: opcion=0;
            }
        }
        if  (dat.read(nombrearchivo.c_str(),columnas)){
                BMP *bmp;
                bmp=new BMP(cluss,dat.referencia(),ejecuciones);
                cout << "J= "<<bmp->ejecutar()<<endl;
        }
        else{
            cout <<"No se ha podido abrir el archivo. Compruebe el nombre." << endl;
        }
    }
}
