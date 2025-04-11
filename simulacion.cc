Double_t F_ter=6.75e+13;
Double_t F_epi=1.14e+12;
Double_t Sig_ter=4.47e-28;
//Double_t Sig_epi=1;
Double_t N_a=6.022e+23;
Double_t Grams=1.;
Double_t N_t=0.6915*N_a*Grams/63.546;

/*
Double_t F_ter=1;
Double_t F_epi=1;
Double_t Sig_ter=1;
//Double_t Sig_epi=1;
Double_t N_a=1;
Double_t Grams=1.;
Double_t N_t=1;

*/
Double_t Irr=80.; //hours

Double_t Lam=(TMath::Log(2.))/12.701; //hours⁻1

Double_t Mu(Double_t *x, Double_t *par){
	Double_t f=0.5*(1.+TMath::Sign(1.,x[0]));
	return f;
}

Double_t A_d=100000.;
Double_t A_sat=Sig_ter*F_ter*N_t;
Double_t time=-(TMath::Log(1.-(A_d/A_sat)))/Lam;
Double_t tiempo=time*3600;
Double_t Tau=1./Lam;

Double_t par[]={Lam,time};

//Double_t par[]={Lam,Irr};

//Double_t par[]={1,7};
void escal()
	{
		TF1 *escal = new TF1("escal",Mu,-10,10,1);
		escal->SetParameters(0,1);
		//	TF1(const char* name, void* fcn, Double_t xmin, Double_t xmax, Int_t npar)
		escal->Draw();
}

Double_t Act(Double_t *x, Double_t *par){
	//Double_t f=(1-TMath::Exp(-par[0]*par[1]))*(TMath::Exp(-par[0]*(x[0]-par[1])))*Mu(x[0]-par[1])+(1-TMath::Exp(-par[0]*x[0]))*Mu(par[1]-x[0]);
	Double_t f=A_sat*(1-TMath::Exp(-par[0]*par[1]))*(TMath::Exp(-par[0]*(x[0]-par[1])))*0.5*(1+TMath::Sign(1.,(x[0]-par[1])))+A_sat*(1-TMath::Exp(-par[0]*x[0]))*0.5*(1+TMath::Sign(1.,(par[1]-x[0])));
	return f;
	}

/*
void simulacion()
	{
		TF1 *activity = new TF1("activity",Act,0.,20.,2);
		//	TF1(const char* name, void* fcn, Double_t xmin, Double_t xmax, Int_t npar)
		activity->SetParameters(par);
		activity->SetParNames("Lambda","Irradiation");
		activity->Draw();
}
*/

Double_t total_time=ceil(100.*time);
void simulacion()
	{
		printf("tiempo de activacion %3.2E [h], %3.2E[s]\n",time,tiempo);
		TF1 *activity = new TF1("activity",Act,0.,total_time,2);
	   TCanvas *c1 = new TCanvas("c1");
	   c1->SetGrid();
		//	TF1(const char* name, void* fcn, Double_t xmin, Double_t xmax, Int_t npar)
		activity->SetParameters(par);
		//activity->SetRange(0.,0.,15.,1.);
		activity->SetParNames("Lambda","Irradiation");
		activity->SetTitle("Activation and Decay;Time [h];Activity of Copper Sample [beq]");
		mylatex = new TLatex(0.,100000.,Form("#tau=%4.2f[h] \n #Phi#sigmaN_{t}=%4.2E[s^{-1}] \n Irradiation=%3.2E[s]",Tau,A_sat,tiempo));
		activity->Draw();
		mylatex->Draw("same");
}
