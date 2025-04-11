#include <TH1F.h>
#include <TNtuple.h>
#include <TPaveText.h>
#include <TF1.h>
#include <TGraphErrors.h>
#include <TROOT.h>
#include <TCanvas.h>
#include <iostream>
#include <fstream>
#include <string>
#include <string.h> 
#include <cstring>
#include <stdlib.h>
#include <time.h>
#include "TPolyMarker.h"
#include "TVirtualPad.h"
#include "TList.h"


using namespace std;

int contador_lineas(char nombre_archivo[34]){ //cuenta las lineas de dato del archivo
	int count=0, chequeo=0;
	string comentario;
	ifstream inFile;
	inFile.open(nombre_archivo);
	if(inFile.fail()){
		cerr << "Error al abrir el archivo" << endl;
		exit(1);
	}
	while(!inFile.eof()){
		inFile >> comentario;
		if(chequeo == 1) count++;
		if(comentario == "comment"){chequeo=1;}
	}
	inFile.close();
	count=(count-1)/3;
	cout << "contador datos vale: " << count << endl;
	return count;
} // lee el archivo y obtiene las lineas de datos que contiene este.

int contador_sobras(){ //cuenta las lineas de dato del archivo
	int count=0;
	int a,b,c;
	ifstream inFile;
	inFile.open("sobras_rayos_radon.txt");
	if(inFile.fail()){
		cerr << "Error al abrir el archivo" << endl;
		exit(1);
	}
	while(!inFile.eof()){
		inFile >>a>>b>>c;
		count++;
	}
	inFile.close();
	cout << "contador sobras vale: " << count << endl;
	return count;
} //entrega las lineas de datos que contiene el archivo sobras

int grafico_rayos_radon(){
	Int_t chequeo=0, count=0, count2=0, segundos=0,i=0, aux_prom=0;
	Int_t suma_rayos=0, suma_radon=0, num_rayos=0, num_radon=0, p=0,sumae=0;
	Float_t raiz;
	string comentario;
	char nombre_archivo[34];
	ifstream archivo_nombre;
	archivo_nombre.open("nombres.dat"); //lee los datos que se encuentran en nombres.dat
	
	while(!archivo_nombre.eof()){ //va leeyendo de forma secuencial los datos que se encuentran en nombres.dat

		Float_t temporal[1000];
		Float_t contador_radon[10000000];
		Float_t contador_rayos[10000000];
		Float_t acumulado_rayos[100000], acumulado_radon[100000];
		Float_t ev[10000000];
		Float_t evento[100000];
		suma_rayos=0;
		suma_radon=0;
		sumae=0;
		aux_prom=0;
		num_rayos=0;
		num_radon=0;
		p=0;
		count=0;
		count2=0;
		segundos=0;
		i=0;
		chequeo=0;
		ifstream inFile;
		ifstream sobras;
		archivo_nombre>>nombre_archivo; //obtenemos el nombre del archivo a leer
		count=contador_lineas(nombre_archivo);
		count2=contador_sobras();
		Int_t x[50000000], y[50000000], z[50000000];
		inFile.open(nombre_archivo); //abrimos el archivo a leer
		while(!inFile.eof()){ //leemos el archivo 
			if(chequeo == 1){
			inFile >> x[i] >>y[i]>>z[i];
				if(i>0){
					if(x[i-1]==x[i]){}
					else{segundos++;}
				}
			i++;
			}
			else{
			inFile >> comentario;
			if(comentario == "comment"){chequeo=1;}
			}
		}
		inFile.close();
		chequeo=0;
        	cout<<"Los segundos son: "<<segundos<<endl;
		sobras.open("sobras_rayos_radon.txt");
		for(Int_t q=0;q<count2;q++){sobras>>contador_rayos[q]>>contador_radon[q]>>ev[p];}
		p=count2+1;
		sobras.close();
		for(Int_t q=1;q<count;q++){ //se obtienen los datos por segundo del archivo actual
				if(x[q-1]==x[q]){
					if(y[q-1]>60 && z[q-1]>60){					
						raiz = y[q-1]*z[q-1];
						raiz = sqrt(raiz); //obtenemos el error estadisticos de la forma poisson 
						if(raiz<80){ num_rayos++;}
						else if(raiz>100){ num_radon++;} //obtenemos el rate bajo ciertas caracteristicas
						else{}
					}
				}
				else{
					if(y[q-1]>60 && z[q-1]>60){					
						raiz = y[q-1]*z[q-1];
						raiz = sqrt(raiz);
						if(raiz<80){ num_rayos++;}
						else if(raiz>100){ num_radon++;}
						else{}
					}
						contador_rayos[p]=num_rayos; //contador_rayos es un registro donde cada valor de su arreglo corresponde al rate de ocho horas acumulado
						contador_radon[p]=num_radon;
						num_rayos=0;
						num_radon=0;
						if(x[q]==1367319420||x[q]==1368503940||x[q]==1369644636||x[q]==1370500800||x[q]==1370586480||x[q]==1371677460||x[q]==1376562360||x[q]==1377859680||x[q]==1378110609||x[q]==1379941320||x[q]==1380033404||x[q]==1381324740||x[q]==1385702340||x[q]==1385705100||x[q]==1386299940){ev[p]=1;} //colocamos los eventos teluricos percibidos en la zona
						else{ev[p]=0;}
						p++;
				}
		}
		
		p=0;
		for(Int_t q=0;q<(segundos+count2);q++){ //obtenemos el rate de cada ocho horas
				if(aux_prom<28800){
					aux_prom++;
					suma_rayos += contador_rayos[q];
					suma_radon += contador_radon[q];
					sumae += ev[q];
				}
				else{
					acumulado_rayos[p]=suma_rayos;
					acumulado_radon[p]=suma_radon;
					evento[p]=sumae;
					p++;
					aux_prom=0;
					suma_rayos=0;
					suma_radon=0;
					sumae=0;				
				}
		} //fin for
//nota: el aux restante al final del for nos indicara cuantos datos quedan sobrantes en el archivo
		ofstream fe("sobras_rayos_radon.txt");
		for(Int_t q=(28800*p+1);q<(segundos+count2);q++){fe<<contador_rayos[q]<<" "<<contador_radon[q]<<" "<<ev[q]<<endl;}
		fe.close(); //guardamos las sobras reescribiendo

		ofstream fa("datos_menores80.txt",ios_base::app);
		for(Int_t q=0;q<(segundos+count2)/28800;q++){fa<<acumulado_rayos[q]<<" "<<evento[q]<<endl;}
		fa.close(); //guardamos los datos que se obtienen en acumulados de rayos

		ofstream fi("datos_mayores100.txt",ios_base::app);
		for(Int_t q=0;q<(segundos+count2)/28800;q++){fi<<acumulado_radon[q]<<" "<<evento[q]<<endl;}
		fi.close(); //guardamos los datos que se obtienen en acumulados de radon
	}

	return 0;
}

