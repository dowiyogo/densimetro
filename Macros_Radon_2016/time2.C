#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include "TFile.h"
#include "TH1I.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TNtuple.h"
#include "TCanvas.h"

int time2(){
  	TFile* file = new TFile("all_data_trig1.root");
	TNtuple* datos= (TNtuple*) file->Get("nt");
	datos->Print();

	Float_t evento, ch0, ch1;

   datos->SetBranchAddress("time",&evento);
	datos->SetBranchAddress("s1",&ch0);
	datos->SetBranchAddress("s2",&ch1);

	Int_t nd = (Int_t)datos->GetEntries();

	Int_t aux0=0;
	Int_t aux1=0;
	Int_t auxt0=0;
	Int_t auxt1=0;
	Int_t ev;
	Int_t j=0;
	Int_t k=0;
	std::cout.fill('.');
	datos->GetEntry(0);
	std::cout<<(Int_t)evento<<std::endl;
	datos->GetEntry(84995000);
	std::cout<<(Int_t)evento<<std::endl;
/*
   for (Int_t i=0;i<=19753889;i++) {
	datos->GetEntry(j);
	ev=(Int_t)evento;
	while((Int_t)evento==ev&&(j<nd)){ 
				aux0+=ch0;
				auxt0+=ch0*st[k];
				aux1+=ch1;
				auxt1+=ch1*st[k];
				j=j+1;
				k=k+1;
				datos->GetEntry(j);
			}
	k=0;
	tiempos1->Fill(auxt0);
	tiempos2->Fill(auxt1);
	tiempos3->Fill(auxt0-auxt1);
	t1vsd1->Fill(auxt0,aux0);
	t2vsd2->Fill(auxt1,aux1);
	t1mt2vsd1->Fill(auxt0-auxt1,aux0);
	aux0=0;
	aux1=0;
	auxt0=0;
	auxt1=0;
	std::cout<<(float)i/nd*100.0<<"\r";
	std::cout.flush();		
		}
  TCanvas *c = new TCanvas("c","c",800,600);
  t1vsd1->Draw("col");
  c->SaveAs("t1vsd1.gif");
  c->SaveAs("t1vsd1.C");
  t2vsd2->Draw("col");
  c->SaveAs("t2vsd2.gif");
  c->SaveAs("t2vsd2.C");  
 t1mt2vsd1->Draw("col");
  c->SaveAs("t1mt2vsd1.gif");
  c->SaveAs("t1mt2vsd1.C");	
tiempos1->Draw("col");
c->SaveAs("tiempos1.gif");
c->SaveAs("tiempos1.C");
tiempos2->Draw("col");
c->SaveAs("tiempos2.gif");
c->SaveAs("tiempos2.C");
tiempos3->Draw("col");
c->SaveAs("tiempos3.gif");
c->SaveAs("tiempos3.C");*/
  file->Close();
  delete file;
  return 0;
}
