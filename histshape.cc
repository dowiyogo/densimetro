#include <iostream>
#include <fstream>
#include <string>
#include <TH1F.h>
#include <math.h>

Double_t histshape_2(char *file_list){

  ifstream distance_list(file_list);

  char distance_file[300];
	TSpectrum *distancePMT1 = new TSpectrum(10);
	TSpectrum *distancePMT2 = new TSpectrum(10);

	TNtuple *distance = new TNtuple("distance","Distance QDC data","PMT1:PMT2");

while(distance_list>>distance_file){
		distance->ReadFile(distance_file);
}
  distance_list.close();

new TCanvas("c1","PMT1 Charge Distribution");

	distance->Draw("PMT1>>histo1(1200,0,1200)"); //Dibujo la columna PMT1 del TNtuple como un Histograma
	TH1F* histo1 = (TH1F *)gROOT->FindObject("histo1"); // Busco el objeto y asigno un puntero para el histograma recien dibujado
 	histo1->GetXaxis()->SetTitle("QDC Channel");
	histo1->GetYaxis()->SetTitle("events");

new TCanvas("c2","PMT2 Charge Distribution");	
	distance->Draw("PMT2>>histo2(1200,0,1200)"); //Dibujo la columna PMT1 del TNtuple como un Histograma
	TH1F* histo2 = (TH1F *)gROOT->FindObject("histo2"); // Busco el objeto y asigno un puntero para el histograma recien dibujado
 	histo2->GetXaxis()->SetTitle("QDC Channel");
	histo2->GetYaxis()->SetTitle("events");

	
TCanvas *canvas1 = new TCanvas("canvas1","Histogramas");
	canvas1->Divide(2,1);
	canvas1->cd(1);

	distance->Draw("PMT1>>histo3(1200,0,1200)"); //Dibujo la columna PMT1 del TNtuple como un Histograma
	TH1F* histo3 = (TH1F *)gROOT->FindObject("histo3"); // Busco el objeto y asigno un puntero para el histograma recien dibujado
	Int_t npeaks = distancePMT1->Search(histo3,3,"",0.03); // Al objeto de clase TSpectrum le hago buscar peaks en el histograma apuntado por el puntero h 
	Double_t distance_pmt1=histo3->Integral(781,881);
 	histo3->GetXaxis()->SetTitle("QDC Channel");
	histo3->GetYaxis()->SetTitle("events");
	histo3->GetXaxis()->SetTitleOffset(1);
	histo3->GetYaxis()->SetTitleOffset(1.6);

	//Finding peaks and fitting gaussians
	Float_t *peaks = distancePMT1->GetPositionX(); // Obtengo las posiciones de los peaks, guardadas en un arreglo de mayor a menor en amplitud
	Float_t *peaksY= distancePMT1->GetPositionY();
	Double_t sigma = 100; // establezco un sigma para el fitting

	if(npeaks > 1) 
		std::cout<<"Se encontraron los peaks del PMT1, donde peaks[1] es "<<peaks[1]<<"y peaks[2] es "<<peaks[2]<<std::endl;
	else
	{
		std::cout<<"Se encontraron menos de 2 peaks"<<std::endl;
		return 1;
	}

	TF1 *func = new TF1("func","gaus(0) + pol1(3)",peaks[1]-sigma,peaks[1]+sigma); //hago un fit de la gausiana, con el primer parametro de nombre 0
	func->SetLineColor(kRed);
	func->SetParameters(peaksY[1],peaks[1],sigma/2,50,-0.1); //parametros iniciales
	func->SetParLimits(0,0,3000);
	func->SetParLimits(1,peaks[1]-sigma,peaks[1]+sigma);
	func->SetParLimits(2,0,2*sigma);
	func->SetParLimits(3,0,500); //constante polinomio b
	func->SetParLimits(4,-1,0); //pendiente m

	histo3->Fit("func","RB+"); //  El "+" agrega la función a la lista de funciones del histograma y el "B" es para considerar los limites.
	Double_t sig = func->GetParameter(2); // asigno a sig el valor del sigma encontrado por el fitting

	canvas1->cd(2);

	distance->Draw("PMT2>>histo4(1200,0,1200)",Form("(PMT1>%f-0.5*%f)&&(PMT1<%f+0.5*%f)",peaks[1],sig,peaks[1],sig)); // establezco la condicion para generar el segundo histograma
	TH1F* histo4 = (TH1F *)gROOT->FindObject("histo4"); // Asigno el puntero al histograma dibujado
	Int_t inf_limit=(Int_t)(ceil(peaks[1]-0.5*sig));
	Int_t sup_limit=(Int_t)(floor(peaks[1]+0.5*sig));
	Double_t events_pmt1_peak=histo1->Integral(inf_limit,sup_limit);
	npeaks = distancePMT2->Search(histo4,3,"",0.03); // Al objeto de clase TSpectrum le hago buscar peaks en el histograma apuntado por el puntero h 
	Float_t *peaks2 = distancePMT2->GetPositionX();
	Int_t inf_limit_ped=(Int_t)(ceil(1));
	Int_t sup_limit_ped=(Int_t)(floor(peaks2[0]+10));
	Double_t pedestal_pmt2=histo4->Integral(inf_limit_ped,sup_limit_ped);
	Double_t entries=histo4->GetEntries();
 	histo4->GetXaxis()->SetTitle("QDC Channel");
	histo4->GetYaxis()->SetTitle("events");
	histo4->GetXaxis()->SetTitleOffset(1);
	histo4->GetYaxis()->SetTitleOffset(1.6);


	Double_t distance_pmt2=histo4->Integral(681,781);

new TCanvas("c3","PMT2 Charge Distribution with Limits");	
	distance->Draw("PMT2>>histo5(100,681,781)",Form("((PMT1>%f-0.5*%f)&&(PMT1<%f+0.5*%f))&&((PMT2>681)&&(PMT2<781))",peaks[1],sig,peaks[1],sig));
	TH1F* histo5 = (TH1F *)gROOT->FindObject("histo5"); // Asigno el puntero al histograma dibujado
 	histo5->GetXaxis()->SetTitle("QDC Channel");
	histo5->GetYaxis()->SetTitle("events");

std::cout<<"*****************************events on Histograms*******************************"<<std::endl;
	printf("Counts on Fitted Peak of PMT1 %.0f\n",events_pmt1_peak);
	printf("Counts of Histogram of PMT2 data when PMT1 was on peak %.0f\n",entries);
	printf("Pedestal Events on PMT2 when PMT1 was on peak: %.0f\n",pedestal_pmt2);
std::cout<<"***************************************************************************"<<std::endl;
std::cout<<"*****************************AIR PMT1 & PMT2*******************************"<<std::endl;
	printf("Probability of events on peak PMT1: %.5f\n",events_pmt1_peak/(histo1->GetEntries()));
//	printf("Counts PMT1 780-880 %.0f\n",distance_pmt1);
	printf("probability of events in photopeak pmt2: %.8f\n",(float)(((float)distance_pmt2)/((float)(entries))));	
	printf("Probability of having peak in PMT1 AND PMT2: %.8f\n",(float)(((float)distance_pmt2)/((float)(entries)))*events_pmt1_peak/(histo1->GetEntries()));
	printf("Probability of having events on PMT2 if PMT1 was on peak: %.8f\n",((entries-pedestal_pmt2)/(events_pmt1_peak)));

	printf("Counts PMT2 680-780 %.0f\n",distance_pmt2);
std::cout<<"***************************************************************************"<<std::endl;

return ((entries-pedestal_pmt2)/(events_pmt1_peak));
}
