/* 
 * File:   lecturaarchivo.cpp
 * Author: Jose Arcos
 *
 * Created on 23 de marzo de 2013, 13:46
 */

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <math.h>
using namespace std;

/*
 * 
 */
class datos {

    vector <vector <float> > dat;
    
public:
    //Devuelve un puntero a los datos
    vector<vector <float> >* referencia(){
        return &dat;
    }
    bool read (string nombre, int columnas){
        ifstream f;
        f.open(nombre.c_str(), ifstream::in);
        if (f==0){
            cout << "Error a abrir el archivo: " << nombre << endl;
            return false;
        }
        float num;
        vector <float> aux;
        while (!f.eof()){
            aux.clear();
            for (int cont=0; cont < columnas; cont++){
                num= -10000000000;
                f >> num;
                aux.push_back(num);
            }
            if (num!=-10000000000)
                dat.push_back(aux);
        }
        return true;
    }
    
    float get (int fila, int columna){
        return dat[fila][columna];
    }
    
    int getfila(){
        return dat.size();
    }
    
    int itemssize(){
        return dat.size();
    }
 
    int getcolumna(){
        if (getfila()==0)
            return 0;
        return dat[0].size();
    }
    
    void getdatscreen(){
        for (int i=0; i < getfila(); i++){
            cout << i+1 << " --->";
            for (int j=0; j < getcolumna(); j++){
                cout <<dat[i][j] << "\t";
            }
            cout << endl;
        }
    }
    
};