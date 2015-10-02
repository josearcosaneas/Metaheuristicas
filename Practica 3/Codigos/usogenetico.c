
#include "clustering.h"
#include <iostream>
#include <stdlib.h>

using namespace std;

int main(int argc, char **argv){
	unsigned long semilla;
	if (argc != 2) {
		cout<<"\nFormato: usogenetico <nombre_fich_configuracion>\n\n";
		abort();
	}
	Input(argc, argv);
	
	cout<< "Introduzca la semilla: ";
	cin >> semilla;
	Seed = semilla;


	cout<< "\nALGORITMOS: \n";
	cout<<"	1. AGG	"<<endl;
	cout<<"	2. AGE	"<<endl;

	
	char algoritmo;
	cout<<"\nIntroduzca el numero de algoritmo a usar: "<<endl;
	cin>>algoritmo;

	if (algoritmo == '1'){
		AGG();
		liberarMemoriaAGG();
	}
	else if (algoritmo == '2'){
		AGE();
		liberarMemoriaAGE();
	}
	else{
		cout<<"\nError intentelo de nuevo...\n\n"<<endl;
	}
		
	return (0);
}
