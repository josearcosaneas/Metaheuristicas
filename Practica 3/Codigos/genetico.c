
#include "clustering.h"
#include <iostream>

using namespace std;

int main(int argc, char **argv){

	if (argc != 2) {
		cout<<"\nFormato: main <nombre_fich_configuracion>\n\n";
		abort();
	}
	Input(argc, argv);
	
	cout<< "\nALGORITMOS INCLUIDOS: \n";
	cout<<"	1. AGG		-------->	 A"<<endl;
	cout<<"	2. AGE		-------->	 E"<<endl;

	
	char algoritmo;
	cout<<"\nIntroduzca el numero de algoritmo a usar: "<<endl;
	cin>>algoritmo;

	if (algoritmo == 'A'){
		AGG();
	}
	else if (algoritmo == 'E'){
		AGE();
	}
	else{
		cout<<"\nAlgoritmo no registrado. El programa finaliza...\n\n"<<endl;
	}
	liberarMemoria();

	return (0);
}