#include <fstream>
#include <string>
#include <TH1F.h>
TLatex *peakpmt1text = new TLatex(800,1200,"hola");
int densimeter(char *brillance_file_list,char *lead_file_list,char *plastic_file_list,char* alum_file_list){

  ifstream brillance_list(brillance_file_list);
  ifstream lead_list(lead_file_list);
  ifstream plastic_list(plastic_file_list);
  ifstream alum_list(alum_file_list);

  char brillance_file[300];
  char lead_file[300];
  char plastic_file[300];
  char alum_file[300];

	//finding Spectra Peaks
	TSpectrum *leadPMT1 = new TSpectrum(10);
	TSpectrum *leadPMT2 = new TSpectrum(10);
	
	TSpectrum *brillancePMT1 = new TSpectrum(10);
	TSpectrum *brillancePMT2 = new TSpectrum(10);

	TSpectrum *plasticPMT1 = new TSpectrum(10);
	TSpectrum *plasticPMT2 = new TSpectrum(10);

	TSpectrum *alumPMT1 = new TSpectrum(10);
	TSpectrum *alumPMT2 = new TSpectrum(10);

	TCanvas *canvas1 = new TCanvas("canvas1","Histogramas");

	TH1F* histo1;
	TH1F* histo2;
	canvas1->Divide(2,2);


	//Creating Ntuples to save data from CAMAC
	TNtuple *lead = new TNtuple("lead","Lead QDC data","PMT1:TARGET:PMT2");
	TNtuple *brillance = new TNtuple("brillance","Brillance QDC data","PMT1:TARGET:PMT2");
	TNtuple *plastic = new TNtuple("plastic","Plastic QDC data","PMT1:TARGET:PMT2");
	TNtuple *alum = new TNtuple("alum","Aluminium QDC data","PMT1:TARGET:PMT2");

while(brillance_list>>brillance_file){
	brillance->ReadFile(brillance_file);
}

while(lead_list>>lead_file){
	lead->ReadFile(lead_file);
}

while(plastic_list>>plastic_file){
	plastic->ReadFile(plastic_file);
}

while(alum_list>>alum_file){
	alum->ReadFile(alum_file);
}
	alum_list.close();
	plastic_list.close();
	lead_list.close();
	brillance_list.close();

	canvas1->cd(1);
	lead->Draw("PMT1>>hist(1200,0,1200)"); //Dibujo la columna PMT1 del TNtuple como un Histograma
	TH1F* h;
	h = (TH1F *)gROOT->FindObject("hist"); // Busco el objeto y asigno un puntero para el histograma recien dibujado
	Int_t npeaks = leadPMT1->Search(h,3); // Al objeto de clase TSpectrum le hago buscar peaks en el histograma apuntado por el puntero h 
	Double_t pb_pmt1=h->Integral(781,881);


	//Finding peaks and fitting gaussians
	Float_t *peaks = leadPMT1->GetPositionX(); // Obtengo las posiciones de los peaks, guardadas en un arreglo de mayor a menor en amplitud
	Double_t sigma = (h->GetRMS())/6; // establezco un sigma para el fitting,unas 4 veces menor al RMS de todo el histograma

	if(npeaks > 1) 
		std::cout<<"Se encontraron los peaks del PMT1 del plomo, donde peaks[0] es "<<peaks[0]<<"y peaks[1] es "<<peaks[1]<<std::endl;
	else
	{
		std::cout<<"Se encontraron menos de 2 peaks en el plomo"<<std::endl;
		return 1;
	}

	TF1 *func = new TF1("func","gaus(0) + pol1(3)",peaks[1]-sigma,peaks[1]+sigma); //hago un fit de la gausiana, con el primer parametro de nombre 0
	func->SetLineColor(kRed);
	func->SetParameters(1000,peaks[1],sigma/2,50,-0.1); //parametros iniciales
	func->SetParLimits(0,0,3000);
	func->SetParLimits(1,peaks[1]-sigma,peaks[1]+sigma);
	func->SetParLimits(2,0,2*sigma);
	func->SetParLimits(3,0,200); //constante polinomio b
	func->SetParLimits(4,-1,0); //pendiente m

	h->Fit("func","RB+"); //  El "+" agrega la función a la lista de funciones del histograma y el "B" es para considerar los limites.
	Double_t sig = func->GetParameter(2); // asigno a sig el valor del sigma encontrado por el fitting

	canvas1->cd(2);
	TH1F* h1; // creo un puntero para el histograma del PMT2
	lead->Draw("PMT2>>hist1(1200,0,1200)",Form("(PMT1>%f-3*%f)&&(PMT1<%f+3*%f)",peaks[1],sig,peaks[1],sig)); // establezco la condicion para generar el segundo histograma
	lead->Draw("PMT2>>histo1(100,681,781)",Form("((PMT1>%f-3*%f)&&(PMT1<%f+3*%f))&&((PMT2>681)&&(PMT2<781))",peaks[1],sig,peaks[1],sig));
	histo1 = (TH1F *)gROOT->FindObject("histo1"); // Asigno el puntero al histograma dibujado

	h1 = (TH1F *)gROOT->FindObject("hist1"); // Asigno el puntero al histograma dibujado
	npeaks = leadPMT2->Search(h1,3); // Al objeto de clase TSpectrum le hago buscar peaks en el histograma apuntado por el puntero h 
	//comenzamos a trabajar con los datos del Brillance
	canvas1->cd(3);
	Double_t pb_pmt2=h1->Integral(681,781);

	TH1F* h2;
	brillance->Draw("PMT1>>hist2(1200,0,1200)");
	h2 = (TH1F *)gROOT->FindObject("hist2");
	npeaks = brillancePMT1->Search(h2,3); // buscamos los peaks del histograma del PMT1 para brillance
	Double_t brill_pmt1=h2->Integral(781,881);
	//Finding peaks and fitting gaussians


	peaks = brillancePMT1->GetPositionX(); //obtengo las posiciones de los peaks
	if(npeaks > 1) 
		std::cout<<"Se encontraron los peaks del PMT1 del brillance, donde peaks[0] es "<<peaks[0]<<"y peaks[1] es "<<peaks[1]<<std::endl;
	else
	{
		std::cout<<"Se encontraron menos de 2 peaks en el Brillance"<<std::endl;
		return 1;
	}
	sigma = (h2->GetRMS())/6;

	TF1 *func1 = new TF1("func1","gaus(0) + pol1(3)",peaks[1]-sigma,peaks[1]+sigma); //hago un fit de la gausiana, con el primer parametro de nombre 0
	func1->SetLineColor(kRed);
	func1->SetParameters(1000,peaks[1],sigma/2,50,-0.1);
	func1->SetParLimits(0,0,3000);
	func1->SetParLimits(1,peaks[1]-sigma,peaks[1]+sigma);
	func1->SetParLimits(2,0,2*sigma);
	func1->SetParLimits(3,0,200);
	func1->SetParLimits(4,-1,0);
	h2->Fit("func1","RB+"); //  El "+" agrega la función a la lista de funciones del histograma y el "B" es para considerar los limites.

	sig = func1->GetParameter(2);
	canvas1->cd(4);

	TH1F* h3;
	brillance->Draw("PMT2>>hist3(1200,0,1200)",Form("(PMT1>%f-3*%f)&&(PMT1<%f+3*%f)",peaks[1],sig,peaks[1],sig));
	h3 = (TH1F *)gROOT->FindObject("hist3");
	brillance->Draw("PMT2>>histo2(100,681,781)",Form("((PMT1>%f-3*%f)&&(PMT1<%f+3*%f))&&((PMT2>681)&&(PMT2<781))",peaks[1],sig,peaks[1],sig));
	histo2 = (TH1F *)gROOT->FindObject("histo2"); // Asigno el puntero al histograma dibujado

	Double_t brill_pmt2=h3->Integral(681,781);
	npeaks = brillancePMT2->Search(h3,3); // Al objeto de clase TSpectrum le hago buscar peaks en el histograma apuntado por el puntero h 

	TCanvas *canvas2 = new TCanvas("canvas2","Histogramas");
	histo1->Draw();
	TCanvas *canvas3 = new TCanvas("canvas3","Histogramas");
	histo2->Draw();

// Comienzo a buscar datos del Plastico
	TCanvas *canvas4 = new TCanvas("canvas4","Histogramas");
	canvas4->Divide(2,1);
	canvas4->cd(1);

	plastic->Draw("PMT1>>hist4(1200,0,1200)"); //Dibujo la columna PMT1 del TNtuple como un Histograma
	TH1F* h4;
	h4 = (TH1F *)gROOT->FindObject("hist4"); // Busco el objeto y asigno un puntero para el histograma recien dibujado
	npeaks = plasticPMT1->Search(h4,3); // Al objeto de clase TSpectrum le hago buscar peaks en el histograma apuntado por el puntero h 
	Double_t plastic_pmt1=h4->Integral(781,881);


	//Finding peaks and fitting gaussians
	peaks = plasticPMT1->GetPositionX(); // Obtengo las posiciones de los peaks, guardadas en un arreglo de mayor a menor en amplitud
	sigma = (h4->GetRMS())/6; // establezco un sigma para el fitting,unas 4 veces menor al RMS de todo el histograma

	if(npeaks > 1) 
		std::cout<<"Se encontraron los peaks del PMT1 del plastico, donde peaks[0] es "<<peaks[0]<<"y peaks[1] es "<<peaks[1]<<std::endl;
	else
	{
		std::cout<<"Se encontraron menos de 2 peaks en el plastico"<<std::endl;
		return 1;
	}

	TF1 *func2 = new TF1("func2","gaus(0) + pol1(3)",peaks[1]-sigma,peaks[1]+sigma); //hago un fit de la gausiana, con el primer parametro de nombre 0
	func2->SetLineColor(kRed);
	func2->SetParameters(1000,peaks[1],sigma/2,50,-0.1); //parametros iniciales
	func2->SetParLimits(0,0,3000);
	func2->SetParLimits(1,peaks[1]-sigma,peaks[1]+sigma);
	func2->SetParLimits(2,0,2*sigma);
	func2->SetParLimits(3,0,200); //constante polinomio b
	func2->SetParLimits(4,-1,0); //pendiente m

	h4->Fit("func2","RB+"); //  El "+" agrega la función a la lista de funciones del histograma y el "B" es para considerar los limites.
	sig = func2->GetParameter(2); // asigno a sig el valor del sigma encontrado por el fitting

	canvas4->cd(2);
	TH1F* h5; // creo un puntero para el histograma del PMT2
	plastic->Draw("PMT2>>hist5(1200,0,1200)",Form("(PMT1>%f-3*%f)&&(PMT1<%f+3*%f)",peaks[1],sig,peaks[1],sig)); // establezco la condicion para generar el segundo histograma
	plastic->Draw("PMT2>>histo3(100,681,781)",Form("((PMT1>%f-3*%f)&&(PMT1<%f+3*%f))&&((PMT2>681)&&(PMT2<781))",peaks[1],sig,peaks[1],sig));
	TH1F* histo3 = (TH1F *)gROOT->FindObject("histo3"); // Asigno el puntero al histograma dibujado

	h5 = (TH1F *)gROOT->FindObject("hist5"); // Asigno el puntero al histograma dibujado
	npeaks = plasticPMT2->Search(h5,3); // Al objeto de clase TSpectrum le hago buscar peaks en el histograma apuntado por el puntero h 
	//comenzamos a trabajar con los datos del Brillance

	Double_t plastic_pmt2=h5->Integral(681,781);
	TCanvas *canvas5 = new TCanvas("canvas5","Histogramas");
	histo3->Draw();

// Comienzo a buscar datos del Aluminio
	TCanvas *canvas6 = new TCanvas("canvas6","Histogramas");
	canvas6->Divide(2,1);
	canvas6->cd(1);

	alum->Draw("PMT1>>hist6(1200,0,1200)"); //Dibujo la columna PMT1 del TNtuple como un Histograma
	TH1F* h6;
	h6 = (TH1F *)gROOT->FindObject("hist6"); // Busco el objeto y asigno un puntero para el histograma recien dibujado
	npeaks = alumPMT1->Search(h6,3); // Al objeto de clase TSpectrum le hago buscar peaks en el histograma apuntado por el puntero h 
	Double_t alum_pmt1=h4->Integral(781,881);


	//Finding peaks and fitting gaussians
	peaks = alumPMT1->GetPositionX(); // Obtengo las posiciones de los peaks, guardadas en un arreglo de mayor a menor en amplitud
	sigma = (h6->GetRMS())/6; // establezco un sigma para el fitting,unas 4 veces menor al RMS de todo el histograma

	if(npeaks > 1) 
		std::cout<<"Se encontraron los peaks del PMT1 del aluminio, donde peaks[0] es "<<peaks[0]<<"y peaks[1] es "<<peaks[1]<<std::endl;
	else
	{
		std::cout<<"Se encontraron menos de 2 peaks en el aluminio"<<std::endl;
		return 1;
	}

	TF1 *func3 = new TF1("func3","gaus(0) + pol1(3)",peaks[1]-sigma,peaks[1]+sigma); //hago un fit de la gausiana, con el primer parametro de nombre 0
	func3->SetLineColor(kRed);
	func3->SetParameters(1000,peaks[1],sigma/2,50,-0.1); //parametros iniciales
	func3->SetParLimits(0,0,3000);
	func3->SetParLimits(1,peaks[1]-sigma,peaks[1]+sigma);
	func3->SetParLimits(2,0,2*sigma);
	func3->SetParLimits(3,0,200); //constante polinomio b
	func3->SetParLimits(4,-1,0); //pendiente m

	h6->Fit("func3","RB+"); //  El "+" agrega la función a la lista de funciones del histograma y el "B" es para considerar los limites.
	sig = func3->GetParameter(2); // asigno a sig el valor del sigma encontrado por el fitting

	canvas6->cd(2);
	TH1F* h7; // creo un puntero para el histograma del PMT2
	alum->Draw("PMT2>>hist7(1200,0,1200)",Form("(PMT1>%f-3*%f)&&(PMT1<%f+3*%f)",peaks[1],sig,peaks[1],sig)); // establezco la condicion para generar el segundo histograma
	plastic->Draw("PMT2>>histo4(100,681,781)",Form("((PMT1>%f-3*%f)&&(PMT1<%f+3*%f))&&((PMT2>681)&&(PMT2<781))",peaks[1],sig,peaks[1],sig));
	TH1F* histo4 = (TH1F *)gROOT->FindObject("histo4"); // Asigno el puntero al histograma dibujado

	h7 = (TH1F *)gROOT->FindObject("hist7"); // Asigno el puntero al histograma dibujado
	npeaks = alumPMT2->Search(h7,3); // Al objeto de clase TSpectrum le hago buscar peaks en el histograma apuntado por el puntero h 
	//comenzamos a trabajar con los datos del Brillance

	Double_t alum_pmt2=h7->Integral(681,781);
	TCanvas *canvas7 = new TCanvas("canvas7","Histogramas");
	histo4->Draw();

std::cout<<"*****************************LEAD PMT1 & PMT2*******************************"<<std::endl;
	printf("Counts PMT1 780-880 %.0f\n",pb_pmt1);
	printf("Counts PMT2 680-780 %.0f\n",pb_pmt2);
std::cout<<"***************************************************************************"<<std::endl;
std::cout<<"***************************Brillance PMT1 & PMT2*******************************"<<std::endl;
	printf("Counts PMT1 780-880 %.0f\n",brill_pmt1);
	printf("Counts PMT2 680-780 %.0f\n",brill_pmt2);
std::cout<<"***************************************************************************"<<std::endl;
std::cout<<"***************************Plastic PMT1 & PMT2*******************************"<<std::endl;
	printf("Counts PMT1 780-880 %.0f\n",plastic_pmt1);
	printf("Counts PMT2 680-780 %.0f\n",plastic_pmt2);
std::cout<<"***************************************************************************"<<std::endl;
std::cout<<"***************************Aluminium PMT1 & PMT2*******************************"<<std::endl;
	printf("Counts PMT1 780-880 %.0f\n",alum_pmt1);
	printf("Counts PMT2 680-780 %.0f\n",alum_pmt2);
std::cout<<"***************************************************************************"<<std::endl;

	return 0;
}
