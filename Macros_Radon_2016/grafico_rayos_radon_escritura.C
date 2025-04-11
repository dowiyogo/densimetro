#include <iostream>
#include <fstream>
#include <string>
#include <string.h> 
#include <cstring>
#include <stdlib.h>
#include <time.h>
#include <TGraphErrors.h>
#include <TGraph.h>
#include <TCanvas.h>
#include <TLatex.h>

using namespace std;

int grafico_rayos_radon_escritura(){

	Float_t rate_radon[797], rate_rayos[797];
	ifstream inrayos;
	inrayos.open("datos_menores80.txt");
	Float_t temporal[797];
	Int_t i=0;
	Float_t dias_rayos[40], dias_radon[40];
	Float_t err_rayos[40], err_radon[40];
	Float_t eventos1[797], eventos2[797];
	Float_t sumad=0;
	Float_t sumade=0;
	Int_t p=0;
	Float_t temporal2[247];
	Float_t err[40];
	Int_t grado3c=0;
	Int_t grado4c=0;
	while(!inrayos.eof()){
		inrayos >> rate_rayos[i] >>eventos1[i];
		if(eventos1[i] == 110000){ eventos1[i] = 300000;} //ponemos el valor de los eventos teluricos a valores que se puedan observar en el grafico superpuesto a los valores del rate
		else if(eventos1[i] == 115000){ eventos1[i] = 400000;}
		else eventos1[i] = -1;
		i++;
	}
	inrayos.close(); //lectura del archivo prom/desv
	Int_t j=0;
	ifstream inradon;
	inradon.open("datos_mayores100.txt");
	while(!inradon.eof()){
		inradon>>rate_radon[j]>>eventos2[j];
		if(eventos2[j] == 110000){ eventos2[j] = 650000;grado3c++;}
		else if(eventos2[j] == 115000){ eventos2[j] = 700000;grado4c++;}
		else eventos2[j] = -1;
		j++;
	}
	std::cout<<"el numero de grado IV es="<<grado4c<<std::endl;
	std::cout<<"el numero de grado III es="<<grado3c<<std::endl;
	const Int_t grado3=grado3c;
	const Int_t grado4=grado4c;
	inradon.close(); //lectura del archivo de datos
	for(Int_t q=0;q<797;q++){ temporal[q]=q*8+1;} //generamos un arreglo que representara el tiempo en puntos de cada ocho horas
	Int_t promedio_dias=0;
	p=0;
	for(Int_t q=0;q<797;q++){
		if(promedio_dias==19){ //acumulamos los eventos de 8 horas a 160 horas (8*20=160)
				dias_rayos[p]=sumad+rate_rayos[q];
				err_rayos[p]=(Float_t)sqrt((Double_t)dias_rayos[p]);
				//cout << err[p] << endl;
				promedio_dias=0;
				p++;
				sumad=0;
				}
		else{
			promedio_dias++;
			sumad += rate_rayos[q];	
			}
	}		
	
	Int_t promedio_eventos=0;
	p=0;
	for(Int_t q=0;q<797;q++){
		if(promedio_eventos==19){ //acumulamos los eventos a 160 horas, promedio_eventos sirve para obtener los eventos de estas horas, no corresponde a una variable promedio. 
				dias_radon[p]=sumade+rate_radon[q];
				err_radon[p]=(Float_t)sqrt((Double_t)dias_radon[p]);
				promedio_eventos=0;
				p++;
				sumade=0;
				}
		else{
			promedio_eventos++;
			sumade +=rate_radon[q];	
			}
	}

	for(Int_t q=0;q<40;q++){ temporal2[q]=q*160+1;} //generamos una variable que representa el tiempo pero cada 160 horas. 
	TCanvas *prueba=new TCanvas("prueba","Datos prueba",300,10,700,500); //genera una imagen con los datos que satisfacen el sqrt(PMT1*PMT2) < 80

//prueba->Divide(1,2);
//prueba->cd(1);
	prueba->SetGrid();
	TGraphErrors* gr4=new TGraphErrors(40,temporal2,dias_rayos,0,err_rayos);
	gr4->RemovePoint(39);
	gr4->SetTitle("");

	gr4->SetMarkerStyle(20);
	gr4->SetMarkerSize(0.8);
	gr4->SetMarkerColor(1);
	gr4->SetLineStyle(1);
	gr4->SetMaximum(600e3);
	gr4->SetMinimum(0);
	gr4->GetXaxis()->SetTitle("Elapsed Time [h]");
	gr4->GetYaxis()->SetTitle("Number of Events when #sqrt{PMT_{1}#bullet PMT_{2}} < 80");
	
	

	TGraph *gr5= new TGraph(740,temporal,eventos1);
	TLatex* latex4[grado4];
	TLatex* latex3[grado3];
	Int_t k=0;
	Int_t cuenta3=0;
	Int_t cuenta4=0;
	while(k<740){
	if(eventos1[k]==300000){
        latex3[cuenta3]= new TLatex(temporal[k],eventos1[k],"#color[2]{III}");latex3[cuenta3]->SetTextSize(0.03);
	gr5->GetListOfFunctions()->Add(latex3[cuenta3]);cuenta3++;}
	else if(eventos1[k]==400000){
		if((cuenta4==0)||(cuenta4==4)){latex4[cuenta4]= new TLatex(temporal[k],eventos1[k],"");latex4[cuenta4]->SetTextSize(0.03);
						gr5->GetListOfFunctions()->Add(latex4[cuenta4]);cuenta4++;}
		else {latex4[cuenta4]= new TLatex(temporal[k],eventos1[k],"#color[2]{IV}");latex4[cuenta4]->SetTextSize(0.03);
		gr5->GetListOfFunctions()->Add(latex4[cuenta4]);cuenta4++;}
		}
	k++;	
	}
//	TLatex Tl; Tl.SetTextFont(43); Tl.SetTextSize(20); Tl.SetTextAngle(90);
//	Tl.DrawLatex(5000., 800000., "#sqrt{pmt_{1}#bullet pmt_{2}}");

	gr5->SetMarkerStyle(29);
	gr5->SetMarkerSize(0.5);
	gr5->SetMarkerColor(2);
	gr5->SetMaximum(600e3);
	gr5->SetMinimum(0);
	gr4->Draw("AP");
	gr5->Draw("P");
	prueba->Update();


	TCanvas *MyC=new TCanvas("MyC","Datos Anuales",300,10,700,500); //genera una imagen con los datos que satisfacen el sqrt(PMT1*PMT2) > 100
	MyC->SetGrid();
//prueba->cd(2);
	TGraphErrors* gr3=new TGraphErrors(40,temporal2,dias_radon,0,err_radon);
	gr3->RemovePoint(39);
	

	gr3->SetTitle("");

	gr3->SetMarkerStyle(20);
	gr3->SetMarkerColor(1);
	gr3->SetLineStyle(1);
	gr3->SetMarkerSize(0.8);
	gr3->SetMaximum(800e3);
	gr3->SetMinimum(0);
	gr3->GetXaxis()->SetTitle("Elapsed Time [h]");
	gr3->GetYaxis()->SetTitle("Number of Events when #sqrt{PMT_{1}#bullet PMT_{2}} > 100");
	TGraph *gr6= new TGraph(740,temporal,eventos2);
	TLatex* latex4_[grado4];
	TLatex* latex3_[grado3];
	Int_t k_=0;
	Int_t cuenta3_=0;
	Int_t cuenta4_=0;
	while(k_<740){
	if(eventos2[k_]==650000){
	latex3_[cuenta3_]= new TLatex(temporal[k_],eventos2[k_],"#color[2]{III}");latex3_[cuenta3_]->SetTextSize(0.03);
	gr6->GetListOfFunctions()->Add(latex3_[cuenta3_]);cuenta3_++;}
	else if(eventos2[k_]==700000){
			if((cuenta4_==0)||(cuenta4_==4)){latex4_[cuenta4_]= new TLatex(temporal[k],eventos2[k_],"");latex4_[cuenta4_]->SetTextSize(0.03);
			gr6->GetListOfFunctions()->Add(latex4_[cuenta4_]);cuenta4_++;}
			else {latex4_[cuenta4_]= new TLatex(temporal[k_],eventos2[k_],"#color[2]{IV}");latex4_[cuenta4_]->SetTextSize(0.03);
			gr6->GetListOfFunctions()->Add(latex4_[cuenta4_]);cuenta4_++;}
			}
	k_++;	
	}

	gr6->SetMarkerStyle(29);
	gr6->SetMarkerSize(0.5);
	gr6->SetMarkerColor(2);
	gr6->SetMaximum(800e3);
	gr6->SetMinimum(0);
	gr3->Draw("AP");
	gr6->Draw("P");
//prueba->Update();

	MyC->Update();
//se obtienen el numero de datos promedio que obtiene le maquina cada ocho horas y su desviacion estandar
	return 0;
}
