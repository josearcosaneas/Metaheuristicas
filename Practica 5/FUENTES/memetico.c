
#include "clustering.h"
#include <iostream>
#include <stdlib.h>
using namespace std;

int main(int argc, char **argv){
	unsigned long semilla;
	if (argc != 2) {
		cout<<"\nFormato: main <nombre_fich_configuracion>\n\n";
		abort();
	}
	Input(argc, argv);
	
	cout<< "Introduzca la semilla: ";
	cin >> semilla;
	Seed = semilla;


	cout<< "\nALGORITMOS INCLUIDOS: \n";
	cout<<"	1. AM_(10,0.1)	-------->	 A"<<endl;
	cout<<"	1. AM_(10,1.0)	-------->	 B"<<endl;
	//cout<<"	2. AGE		-------->	 E"<<endl;

	
	char algoritmo;
	cout<<"\nIntroduzca el numero de algoritmo a usar: "<<endl;
	cin>>algoritmo;

	if (algoritmo == 'A'){
		AM_1();
		liberarMemoriaAGG();
	}
	else if (algoritmo == 'B'){
		AM_10();
		liberarMemoriaAGG();
	}


	else{
		cout<<"\nAlgoritmo no registrado. El programa finaliza...\n\n"<<endl;
	}
		
	return (0);
}
