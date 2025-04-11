#include <iostream>
#include <fstream>
#include <string>
#include <TH1F.h>
#include <math.h>
TFile * rootfile;
Double_t histshape(char *file_list){
  rootfile = new TFile("shape.root","update");
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

new TCanvas("c2","PMT2 Charge Distribution");	
	distance->Draw("PMT2>>histo2(1200,0,1200)"); //Dibujo la columna PMT1 del TNtuple como un Histograma
	TH1F* histo2 = (TH1F *)gROOT->FindObject("histo2"); // Busco el objeto y asigno un puntero para el histograma recien dibujado
	
TCanvas *canvas1 = new TCanvas("canvas1","Histogramas");
	canvas1->Divide(2,1);
	canvas1->cd(1);

	distance->Draw("PMT1>>histo3(1200,0,1200)"); //Dibujo la columna PMT1 del TNtuple como un Histograma
	TH1F* histo3 = (TH1F *)gROOT->FindObject("histo3"); // Busco el objeto y asigno un puntero para el histograma recien dibujado
	Int_t npeaks = distancePMT1->Search(histo3,3,"",0.03); // Al objeto de clase TSpectrum le hago buscar peaks en el histograma apuntado por el puntero h 
	Double_t distance_pmt1=histo3->Integral(781,881);


	//Finding peaks and fitting gaussians
	Float_t *peaks = distancePMT1->GetPositionX(); // Obtengo las posiciones de los peaks, guardadas en un arreglo de mayor a menor en amplitud
	Float_t *peaksY= distancePMT1->GetPositionY();
	Double_t sigma = 100; // establezco un sigma para el fitting,unas 4 veces menor al RMS de todo el histograma

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



	Double_t distance_pmt2=histo4->Integral(681,781);

new TCanvas("c3","PMT2 Charge Distribution with Limits");	
	distance->Draw("PMT2>>histo5(100,681,781)",Form("((PMT1>%f-0.5*%f)&&(PMT1<%f+0.5*%f))&&((PMT2>681)&&(PMT2<781))",peaks[1],sig,peaks[1],sig));
	TH1F* histo5 = (TH1F *)gROOT->FindObject("histo5"); // Asigno el puntero al histograma dibujado
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
	histo1->Write(Form("histo1_%s",file_list));
	histo2->Write(Form("histo2_%s",file_list));
	histo3->Write(Form("histo3_%s",file_list));
	histo4->Write(Form("histo4_%s",file_list));
	histo5->Write(Form("histo5_%s",file_list));
	rootfile->Close();
return ((entries-pedestal_pmt2)/(events_pmt1_peak));
}
const int n=17;
int plot(){  
	rootfile = new TFile("shape.root","recreate");
	rootfile->Close();
	std::ostringstream nombre;
	Int_t d=0;
	Double_t ped[n];
for (int i=0;i<n;i++)
	{
		d=i*5;
		std::cout<<Form("shape_%d_3mm.lst",d)<<std::endl;
		ped[i]=histshape(Form("shape_%d_3mm.lst",d));
	}
// TCanvas *grafico1 = new TCanvas("grafico1","");
	 Double_t dist[n]={0,5,10,15,20,25,30,35,40,45,50,55,60,65,70,75,80};
  // Double_t dist[n]={0,5,10,15,20,25,30,35,40,45,50,55,60,65,70,75,80,85,90,95,100};
   // Double_t prob[n]={0.142,0.127,0.09723,0.08487,0.06619,0.0605,0.06211,0.04587,0.03912,0.03506,0.03191,0.02646,0.0257,0.01985,0.01810,0.01605,0.01717,0.01539,0.01417,0.01387,0.01392};
  // TGraph* gr = new TGraph(n,dist,prob);
  // gr->Draw("AC*");
	TCanvas *grafico2 = new TCanvas("grafico2","");
   TGraph* gr2 = new TGraph(n,dist,ped);
   gr2->Draw("AC*");
 	gr2->SetTitle("Conditional Probability of Event Occurrence on PMT2 since there were events on 511 keV Photopeak of PMT1");
 	gr2->GetXaxis()->SetTitle("Distance [mm]");
	gr2->GetYaxis()->SetTitle("Probability");

	return 0;
}
