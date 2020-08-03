// SACAnalysis.cc
// --------------------------------------------------------------
// run with "root SACAnalysis.cc"
// --------------------------------------------------------------
// History:
//
// Created by Emily Tsai (emily.tsai11@gmail.com) 2020-7-20
// --------------------------------------------------------------

#include <string>
#include <map>
#include <math.h>

// CHANGE THESE
const int numEnergies = 3;
string initPart = "gamma";
string n = "1000";

// energies
double energies_dbl[] = {100.0, 200.0, 500.0, 1000.0, 2000.0, 5000.0, 10000.0, 20000.0, 50000.0, 100000.0};
string energies_str[] = {"100MeV", "200MeV", "500MeV", "1GeV", "2GeV", "5GeV", "10GeV", "20GeV", "50GeV", "100GeV"};

// particle types
int numPart = 11;
string partType[] = {"Gamma", "Positron", "Electron", "Proton", "Neutron", "PionPlus", "PionMinus", "PionZero", "MuonPlus", "MuonMinus", "OptPhot"};

// TGraph drawing options
string x_axis = "incident energy (MeV)";
string draw_opt = "ap";

// data structures
map<string, TFile*> mFiles;
map<string, TCanvas*> mCanvas;
map<string, TGraph*> mGraphs;

double x[numEnergies];
double x_stddev[numEnergies];

double TotalE_frac_mean[numEnergies];
double TotalE_frac_stddev[numEnergies];

double TotalE_frac_Gamma_mean[numEnergies];
double TotalE_frac_Gamma_stddev[numEnergies];

double TotalE_frac_Positron_mean[numEnergies];
double TotalE_frac_Positron_stddev[numEnergies];

double TotalE_frac_Electron_mean[numEnergies];
double TotalE_frac_Electron_stddev[numEnergies];

double TotalE_frac_Proton_mean[numEnergies];
double TotalE_frac_Proton_stddev[numEnergies];

double TotalE_frac_Neutron_mean[numEnergies];
double TotalE_frac_Neutron_stddev[numEnergies];

double TotalE_frac_OptPhot_mean[numEnergies];
double TotalE_frac_OptPhot_stddev[numEnergies];

double UntrackedE_frac_mean[numEnergies];
double UntrackedE_frac_stddev[numEnergies];

double UntrackedEFromParticles_mean[numEnergies];
double UntrackedEFromParticles_stddev[numEnergies];

// 0
double y_Gamma_PerEvent_Mult_mean[numEnergies];
double y_Gamma_PerEvent_Mult_stddev[numEnergies];
double y_Gamma_PerEvent_Mult_nStdDev_dMean[numEnergies];
double y_Gamma_PerEvent_nMult_dInputE[numEnergies];
double y_Gamma_PerEvent_nMult_dInputE_stddev[numEnergies];
double y_Gamma_PerEvent_EDep_mean[numEnergies];
double y_Gamma_PerEvent_EDep_stddev[numEnergies];
double y_Gamma_PerEvent_EDep_nStdDev_dMean[numEnergies];
double y_Gamma_PerEvent_InitE_mean[numEnergies];
double y_Gamma_PerEvent_InitE_stddev[numEnergies];
double y_Gamma_PerEvent_InitE_nStdDev_dMean[numEnergies];

// 1
double y_Positron_PerEvent_Mult_mean[numEnergies];
double y_Positron_PerEvent_Mult_stddev[numEnergies];
double y_Positron_PerEvent_Mult_nStdDev_dMean[numEnergies];
double y_Positron_PerEvent_nMult_dInputE[numEnergies];
double y_Positron_PerEvent_nMult_dInputE_stddev[numEnergies];
double y_Positron_PerEvent_EDep_mean[numEnergies];
double y_Positron_PerEvent_EDep_stddev[numEnergies];
double y_Positron_PerEvent_EDep_nStdDev_dMean[numEnergies];
double y_Positron_PerEvent_InitE_mean[numEnergies];
double y_Positron_PerEvent_InitE_stddev[numEnergies];
double y_Positron_PerEvent_InitE_nStdDev_dMean[numEnergies];

// 2
double y_Electron_PerEvent_Mult_mean[numEnergies];
double y_Electron_PerEvent_Mult_stddev[numEnergies];
double y_Electron_PerEvent_Mult_nStdDev_dMean[numEnergies];
double y_Electron_PerEvent_nMult_dInputE[numEnergies];
double y_Electron_PerEvent_nMult_dInputE_stddev[numEnergies];
double y_Electron_PerEvent_EDep_mean[numEnergies];
double y_Electron_PerEvent_EDep_stddev[numEnergies];
double y_Electron_PerEvent_EDep_nStdDev_dMean[numEnergies];
double y_Electron_PerEvent_InitE_mean[numEnergies];
double y_Electron_PerEvent_InitE_stddev[numEnergies];
double y_Electron_PerEvent_InitE_nStdDev_dMean[numEnergies];

// 3
double y_Proton_PerEvent_Mult_mean[numEnergies];
double y_Proton_PerEvent_Mult_stddev[numEnergies];
double y_Proton_PerEvent_Mult_nStdDev_dMean[numEnergies];
double y_Proton_PerEvent_nMult_dInputE[numEnergies];
double y_Proton_PerEvent_nMult_dInputE_stddev[numEnergies];
double y_Proton_PerEvent_EDep_mean[numEnergies];
double y_Proton_PerEvent_EDep_stddev[numEnergies];
double y_Proton_PerEvent_EDep_nStdDev_dMean[numEnergies];
double y_Proton_PerEvent_InitE_mean[numEnergies];
double y_Proton_PerEvent_InitE_stddev[numEnergies];
double y_Proton_PerEvent_InitE_nStdDev_dMean[numEnergies];

// 4
double y_Neutron_PerEvent_Mult_mean[numEnergies];
double y_Neutron_PerEvent_Mult_stddev[numEnergies];
double y_Neutron_PerEvent_Mult_nStdDev_dMean[numEnergies];
double y_Neutron_PerEvent_nMult_dInputE[numEnergies];
double y_Neutron_PerEvent_nMult_dInputE_stddev[numEnergies];
double y_Neutron_PerEvent_EDep_mean[numEnergies];
double y_Neutron_PerEvent_EDep_stddev[numEnergies];
double y_Neutron_PerEvent_EDep_nStdDev_dMean[numEnergies];
double y_Neutron_PerEvent_InitE_mean[numEnergies];
double y_Neutron_PerEvent_InitE_stddev[numEnergies];
double y_Neutron_PerEvent_InitE_nStdDev_dMean[numEnergies];

// 10
double y_OptPhot_PerEvent_Mult_mean[numEnergies];
double y_OptPhot_PerEvent_Mult_stddev[numEnergies];
double y_OptPhot_PerEvent_Mult_nStdDev_dMean[numEnergies];
double y_OptPhot_PerEvent_nMult_dInputE[numEnergies];
double y_OptPhot_PerEvent_nMult_dInputE_stddev[numEnergies];
double y_OptPhot_PerEvent_EDep_mean[numEnergies];
double y_OptPhot_PerEvent_EDep_stddev[numEnergies];
double y_OptPhot_PerEvent_EDep_nStdDev_dMean[numEnergies];
double y_OptPhot_PerEvent_InitE_mean[numEnergies];
double y_OptPhot_PerEvent_InitE_stddev[numEnergies];
double y_OptPhot_PerEvent_InitE_nStdDev_dMean[numEnergies];

void GetMEDataPoints(int EIndex, int PIndex,
	double MultMean[], double MultStdDev[], double Mult_nStdDev_dMean[],
	double nMult_dInputE[], double nMult_dInputE_stddev[],
	double EDepMean[], double EDepStdDev[], double EDep_nStdDev_dMean[],
	double InitEMean[], double InitEStdDev[], double InitE_nStdDev_dMean[])
{
	// gaussian fit function
	// TF1* gaussian = new TF1("gaussian", "[0] * exp(-0.5 * ((x - [1]) / [2]) * ((x - [1]) / [2]))", 0.0, 100.0);
	// gaussian->SetParName(0, "normalization");
	// gaussian->SetParName(1, "#mu");
	// gaussian->SetParName(2, "#sigma");

	// multiplicity per event
	TH1D* MultCurrent = (TH1D*) mFiles["f" + energies_str[EIndex]]->Get(("h" + partType[PIndex] + "_PerEvent_Mult").c_str());
	MultMean[EIndex] = MultCurrent->GetMean();
	MultStdDev[EIndex] = MultCurrent->GetStdDev();
	nMult_dInputE[EIndex] = MultMean[EIndex] / energies_dbl[EIndex];
	nMult_dInputE_stddev[EIndex] = MultStdDev[EIndex] / energies_dbl[EIndex];

	double MultTemp = MultCurrent->GetStdDev() / MultCurrent->GetMean();
	Mult_nStdDev_dMean[EIndex] = isnan(MultTemp) ? 0.0 : MultTemp;

	// energy deposition per event
	TH1D* EDepCurrent = (TH1D*) mFiles["f" + energies_str[EIndex]]->Get(("h" + partType[PIndex] + "_PerEvent_EDep").c_str());
	EDepMean[EIndex] = EDepCurrent->GetMean();
	EDepStdDev[EIndex] = EDepCurrent->GetStdDev();

	double EDepTemp = EDepCurrent->GetStdDev() / EDepCurrent->GetMean();
	EDep_nStdDev_dMean[EIndex] = isnan(EDepTemp) ? 0.0 : EDepTemp;

	// initial energy per event
	TH1D* InitECurrent = (TH1D*) mFiles["f" + energies_str[EIndex]]->Get(("h" + partType[PIndex] + "_PerEvent_InitE").c_str());
	InitEMean[EIndex] = InitECurrent->GetMean();
	InitEStdDev[EIndex] = InitECurrent->GetStdDev();

	double InitETemp = InitECurrent->GetStdDev() / InitECurrent->GetMean();
	InitE_nStdDev_dMean[EIndex] = isnan(InitETemp) ? 0.0 : InitETemp;
}

void PlotMEDataPoints(int PIndex,
	double MultMean[], double MultStdDev[], double Mult_nStdDev_dMean[],
	double nMult_dInputE[], double nMult_dInputE_stddev[],
	double EDepMean[], double EDepStdDev[], double EDep_nStdDev_dMean[],
	double InitEMean[], double InitEStdDev[], double InitE_nStdDev_dMean[])
{
	// fit function -- constant / sqrt(E)
	TF1* const_sqrtE = new TF1("const_sqrtE", "[0] / sqrt(x)", 0, 10);
	const_sqrtE->SetParName(0, "constant");

	// multiplicity per event
	string key1 = partType[PIndex] + "_PerEvent_Mult_mean";
	string title1 = "mean number of " + partType[PIndex] + " per event";
	mCanvas[key1] = new TCanvas(("c" + key1).c_str(), title1.c_str(), 800, 600);
	mGraphs[key1] = new TGraphErrors(numEnergies, x, MultMean, x_stddev, MultStdDev);
	mGraphs[key1]->SetName(("g" + key1).c_str());
	mGraphs[key1]->SetTitle(title1.c_str());
	mGraphs[key1]->GetXaxis()->SetTitle(x_axis.c_str());
	mGraphs[key1]->GetYaxis()->SetTitle("mean number of particles");
	mGraphs[key1]->Draw(draw_opt.c_str());
	mGraphs[key1]->Write();

	string key2 = partType[PIndex] + "_PerEvent_Mult_stddev";
	string title2 = "stddev of mean number of " + partType[PIndex] + " per event";
	mCanvas[key2] = new TCanvas(("c" + key2).c_str(), title2.c_str(), 800, 600);
	mGraphs[key2] = new TGraph(numEnergies, x, MultStdDev);
	mGraphs[key2]->SetName(("g" + key2).c_str());
	mGraphs[key2]->SetTitle(title2.c_str());
	mGraphs[key2]->GetXaxis()->SetTitle(x_axis.c_str());
	mGraphs[key2]->GetYaxis()->SetTitle("stddev of mean number of particles");
	mGraphs[key2]->Draw(draw_opt.c_str());
	mGraphs[key2]->Write();

	string keyA = partType[PIndex] + "_PerEvent_nMult_dInputE";
	string titleA = "mean number of " + partType[PIndex] + " per event / input energy";
	mCanvas[keyA] = new TCanvas(("c" + keyA).c_str(), titleA.c_str(), 800, 600);
	mGraphs[keyA] = new TGraphErrors(numEnergies, x, nMult_dInputE, x_stddev, nMult_dInputE_stddev);
	mGraphs[keyA]->SetName(("g" + keyA).c_str());
	mGraphs[keyA]->SetTitle(titleA.c_str());
	mGraphs[keyA]->GetXaxis()->SetTitle(x_axis.c_str());
	mGraphs[keyA]->GetYaxis()->SetTitle("mean number of particles / input energy (1 / MeV)");
	mGraphs[keyA]->Draw(draw_opt.c_str());
	mGraphs[keyA]->Write();

	string key3 = partType[PIndex] + "_PerEvent_Mult_nStdDev_dMean";
	string title3 = "stddev / mean number of " + partType[PIndex] + " per event";
	mCanvas[key3] = new TCanvas(("c" + key3).c_str(), title3.c_str(), 800, 600);
	mGraphs[key3] = new TGraph(numEnergies, x, Mult_nStdDev_dMean);
	mGraphs[key3]->SetName(("g" + key3).c_str());
	mGraphs[key3]->SetTitle(title3.c_str());
	mGraphs[key3]->GetXaxis()->SetTitle(x_axis.c_str());
	mGraphs[key3]->GetYaxis()->SetTitle("stddev / mean number of particles");
	mGraphs[key3]->Draw(draw_opt.c_str());
	mGraphs[key3]->Write();

	mGraphs[key3]->Fit(const_sqrtE);
	mGraphs[key3]->SetName(("g" + key3 + "_fit").c_str());
	mGraphs[key3]->SetTitle((title3 + " (fit)").c_str());
	gStyle->SetOptFit(1011);
	mGraphs[key3]->Draw(draw_opt.c_str());
	mGraphs[key3]->Write();

	// energy deposition per event
	string key4 = partType[PIndex] + "_PerEvent_EDep_mean";
	string title4 = "mean energy deposition of " + partType[PIndex] + " per event";
	mCanvas[key4] = new TCanvas(("c" + key4).c_str(), title4.c_str(), 800, 600);
	mGraphs[key4] = new TGraphErrors(numEnergies, x, EDepMean, x_stddev, EDepStdDev);
	mGraphs[key4]->SetName(("g" + key4).c_str());
	mGraphs[key4]->SetTitle(title4.c_str());
	mGraphs[key4]->GetXaxis()->SetTitle(x_axis.c_str());
	mGraphs[key4]->GetYaxis()->SetTitle("mean energy deposition (MeV)");
	mGraphs[key4]->Draw(draw_opt.c_str());
	mGraphs[key4]->Write();

	string key5 = partType[PIndex] + "_PerEvent_EDep_stddev";
	string title5 = "stddev of mean energy deposition of " + partType[PIndex] + " per event";
	mCanvas[key5] = new TCanvas(("c" + key5).c_str(), title5.c_str(), 800, 600);
	mGraphs[key5] = new TGraph(numEnergies, x, EDepStdDev);
	mGraphs[key5]->SetName(("g" + key5).c_str());
	mGraphs[key5]->SetTitle(title5.c_str());
	mGraphs[key5]->GetXaxis()->SetTitle(x_axis.c_str());
	mGraphs[key5]->GetYaxis()->SetTitle("stddev of mean energy deposition (MeV)");
	mGraphs[key5]->Draw(draw_opt.c_str());
	mGraphs[key5]->Write();

	string key6 = partType[PIndex] + "_PerEvent_EDep_nStdDev_dMean";
	string title6 = "stddev / mean energy deposition of " + partType[PIndex] + " per event";
	mCanvas[key6] = new TCanvas(("c" + key6).c_str(), title6.c_str(), 800, 600);
	mGraphs[key6] = new TGraph(numEnergies, x, EDep_nStdDev_dMean);
	mGraphs[key6]->SetName(("g" + key6).c_str());
	mGraphs[key6]->SetTitle(title6.c_str());
	mGraphs[key6]->GetXaxis()->SetTitle(x_axis.c_str());
	mGraphs[key6]->GetYaxis()->SetTitle("stddev / mean energy deposition (MeV)");
	mGraphs[key6]->Draw(draw_opt.c_str());
	mGraphs[key6]->Write();

	mGraphs[key6]->Fit(const_sqrtE);
	mGraphs[key6]->SetName(("g" + key6 + "_fit").c_str());
	mGraphs[key6]->SetTitle((title6 + " (fit)").c_str());
	gStyle->SetOptFit(1011);
	mGraphs[key6]->Draw(draw_opt.c_str());
	mGraphs[key6]->Write();

	// initial energy per event
	string key7 = partType[PIndex] + "_PerEvent_InitE_mean";
	string title7 = "mean initial energy of " + partType[PIndex] + " per event";
	mCanvas[key7] = new TCanvas(("c" + key7).c_str(), title7.c_str(), 800, 600);
	mGraphs[key7] = new TGraphErrors(numEnergies, x, InitEMean, x_stddev, InitEStdDev);
	mGraphs[key7]->SetName(("g" + key7).c_str());
	mGraphs[key7]->SetTitle(title7.c_str());
	mGraphs[key7]->GetXaxis()->SetTitle(x_axis.c_str());
	mGraphs[key7]->GetYaxis()->SetTitle("mean initial energy (MeV)");
	mGraphs[key7]->Draw(draw_opt.c_str());
	mGraphs[key7]->Write();

	string key8 = partType[PIndex] + "_PerEvent_InitE_stddev";
	string title8 = "stddev of mean initial energy of " + partType[PIndex] + " per event";
	mCanvas[key8] = new TCanvas(("c" + key8).c_str(), title8.c_str(), 800, 600);
	mGraphs[key8] = new TGraph(numEnergies, x, InitEStdDev);
	mGraphs[key8]->SetName(("g" + key8).c_str());
	mGraphs[key8]->SetTitle(title8.c_str());
	mGraphs[key8]->GetXaxis()->SetTitle(x_axis.c_str());
	mGraphs[key8]->GetYaxis()->SetTitle("stddev of mean initial energy (MeV)");
	mGraphs[key8]->Draw(draw_opt.c_str());
	mGraphs[key8]->Write();

	string key9 = partType[PIndex] + "_PerEvent_InitE_nStdDev_dMean";
	string title9 = "stddev / mean initial energy of " + partType[PIndex] + " per event";
	mCanvas[key9] = new TCanvas(("c" + key9).c_str(), title9.c_str(), 800, 600);
	mGraphs[key9] = new TGraph(numEnergies, x, InitE_nStdDev_dMean);
	mGraphs[key9]->SetName(("g" + key9).c_str());
	mGraphs[key9]->SetTitle(title9.c_str());
	mGraphs[key9]->GetXaxis()->SetTitle(x_axis.c_str());
	mGraphs[key9]->GetYaxis()->SetTitle("stddev / mean initial energy (MeV)");
	mGraphs[key9]->Draw(draw_opt.c_str());
	mGraphs[key9]->Write();

	mGraphs[key9]->Fit(const_sqrtE);
	mGraphs[key9]->SetName(("g" + key9 + "_fit").c_str());
	mGraphs[key9]->SetTitle((title9 + " (fit)").c_str());
	gStyle->SetOptFit(1011);
	mGraphs[key9]->Draw(draw_opt.c_str());
	mGraphs[key9]->Write();
}

void SACAnalysis()
{
	for(int i = 0; i < numEnergies; i++)
		mFiles["f" + energies_str[i]] = new TFile(("n" + n + "/SAC_" + energies_str[i] + "_n" + n + "_" + initPart + ".root").c_str());
	{
		for(int i = 0; i < numEnergies; i++)
		{
			x[i] = energies_dbl[i];
			x_stddev[i] = 0.0;
		}

		for(int i = 0; i < numEnergies; i++)
		{
			GetMEDataPoints(i, 0, y_Gamma_PerEvent_Mult_mean, y_Gamma_PerEvent_Mult_stddev, y_Gamma_PerEvent_Mult_nStdDev_dMean,
									y_Gamma_PerEvent_nMult_dInputE, y_Gamma_PerEvent_nMult_dInputE_stddev,
									y_Gamma_PerEvent_EDep_mean, y_Gamma_PerEvent_EDep_stddev, y_Gamma_PerEvent_EDep_nStdDev_dMean,
									y_Gamma_PerEvent_InitE_mean, y_Gamma_PerEvent_InitE_stddev, y_Gamma_PerEvent_InitE_nStdDev_dMean);

			GetMEDataPoints(i, 1, y_Positron_PerEvent_Mult_mean, y_Positron_PerEvent_Mult_stddev, y_Positron_PerEvent_Mult_nStdDev_dMean,
									y_Positron_PerEvent_nMult_dInputE, y_Positron_PerEvent_nMult_dInputE_stddev,
									y_Positron_PerEvent_EDep_mean, y_Positron_PerEvent_EDep_stddev, y_Positron_PerEvent_EDep_nStdDev_dMean,
									y_Positron_PerEvent_InitE_mean, y_Positron_PerEvent_InitE_stddev, y_Positron_PerEvent_InitE_nStdDev_dMean);

			GetMEDataPoints(i, 2, y_Electron_PerEvent_Mult_mean, y_Electron_PerEvent_Mult_stddev, y_Electron_PerEvent_Mult_nStdDev_dMean,
									y_Electron_PerEvent_nMult_dInputE, y_Electron_PerEvent_nMult_dInputE_stddev,
									y_Electron_PerEvent_EDep_mean, y_Electron_PerEvent_EDep_stddev, y_Electron_PerEvent_EDep_nStdDev_dMean,
									y_Electron_PerEvent_InitE_mean, y_Electron_PerEvent_InitE_stddev, y_Electron_PerEvent_InitE_nStdDev_dMean);

			GetMEDataPoints(i, 3, y_Proton_PerEvent_Mult_mean, y_Proton_PerEvent_Mult_stddev, y_Proton_PerEvent_Mult_nStdDev_dMean,
									y_Proton_PerEvent_nMult_dInputE, y_Proton_PerEvent_nMult_dInputE_stddev,
									y_Proton_PerEvent_EDep_mean, y_Proton_PerEvent_EDep_stddev, y_Proton_PerEvent_EDep_nStdDev_dMean,
									y_Proton_PerEvent_InitE_mean, y_Proton_PerEvent_InitE_stddev, y_Proton_PerEvent_InitE_nStdDev_dMean);

			GetMEDataPoints(i, 4, y_Neutron_PerEvent_Mult_mean, y_Neutron_PerEvent_Mult_stddev, y_Neutron_PerEvent_Mult_nStdDev_dMean,
									y_Neutron_PerEvent_nMult_dInputE, y_Neutron_PerEvent_nMult_dInputE_stddev,
									y_Neutron_PerEvent_EDep_mean, y_Neutron_PerEvent_EDep_stddev, y_Neutron_PerEvent_EDep_nStdDev_dMean,
									y_Neutron_PerEvent_InitE_mean, y_Neutron_PerEvent_InitE_stddev, y_Neutron_PerEvent_InitE_nStdDev_dMean);

			GetMEDataPoints(i, 10, y_OptPhot_PerEvent_Mult_mean, y_OptPhot_PerEvent_Mult_stddev, y_OptPhot_PerEvent_Mult_nStdDev_dMean,
									y_OptPhot_PerEvent_nMult_dInputE, y_OptPhot_PerEvent_nMult_dInputE_stddev,
									y_OptPhot_PerEvent_EDep_mean, y_OptPhot_PerEvent_EDep_stddev, y_OptPhot_PerEvent_EDep_nStdDev_dMean,
									y_OptPhot_PerEvent_InitE_mean, y_OptPhot_PerEvent_InitE_stddev, y_OptPhot_PerEvent_InitE_nStdDev_dMean);
		}

		for(int i = 0; i < numEnergies; i++)
		{
			TH1D* EDepTemp = (TH1D*) mFiles["f" + energies_str[i]]->Get("hPerEvent_EDep");
			double trackedE_mean = EDepTemp->GetMean();
			double trackedE_stddev = EDepTemp->GetStdDev();

			TotalE_frac_mean[i] = trackedE_mean / energies_dbl[i];
			TotalE_frac_stddev[i] = trackedE_stddev / energies_dbl[i];

			TotalE_frac_Gamma_mean[i] = ((TH1D*) mFiles["f" + energies_str[i]]->Get("hGamma_PerEvent_EDep"))->GetMean() / energies_dbl[i];
			TotalE_frac_Gamma_stddev[i] = ((TH1D*) mFiles["f" + energies_str[i]]->Get("hGamma_PerEvent_EDep"))->GetStdDev() / energies_dbl[i];

			TotalE_frac_Positron_mean[i] = ((TH1D*) mFiles["f" + energies_str[i]]->Get("hPositron_PerEvent_EDep"))->GetMean() / energies_dbl[i];
			TotalE_frac_Positron_stddev[i] = ((TH1D*) mFiles["f" + energies_str[i]]->Get("hPositron_PerEvent_EDep"))->GetStdDev() / energies_dbl[i];

			TotalE_frac_Electron_mean[i] = ((TH1D*) mFiles["f" + energies_str[i]]->Get("hElectron_PerEvent_EDep"))->GetMean() / energies_dbl[i];
			TotalE_frac_Electron_stddev[i] = ((TH1D*) mFiles["f" + energies_str[i]]->Get("hElectron_PerEvent_EDep"))->GetStdDev() / energies_dbl[i];

			TotalE_frac_Proton_mean[i] = ((TH1D*) mFiles["f" + energies_str[i]]->Get("hProton_PerEvent_EDep"))->GetMean() / energies_dbl[i];
			TotalE_frac_Proton_stddev[i] = ((TH1D*) mFiles["f" + energies_str[i]]->Get("hProton_PerEvent_EDep"))->GetStdDev() / energies_dbl[i];

			TotalE_frac_Neutron_mean[i] = ((TH1D*) mFiles["f" + energies_str[i]]->Get("hNeutron_PerEvent_EDep"))->GetMean() / energies_dbl[i];
			TotalE_frac_Neutron_stddev[i] = ((TH1D*) mFiles["f" + energies_str[i]]->Get("hNeutron_PerEvent_EDep"))->GetStdDev() / energies_dbl[i];

			TotalE_frac_OptPhot_mean[i] = ((TH1D*) mFiles["f" + energies_str[i]]->Get("hOptPhot_PerEvent_EDep"))->GetMean() / energies_dbl[i];
			TotalE_frac_OptPhot_stddev[i] = ((TH1D*) mFiles["f" + energies_str[i]]->Get("hOptPhot_PerEvent_EDep"))->GetStdDev() / energies_dbl[i];

			double untrackedE_mean = energies_dbl[i] - trackedE_mean;
			double untrackedE_stddev = trackedE_stddev;
			UntrackedE_frac_mean[i] = untrackedE_mean / energies_dbl[i];
			UntrackedE_frac_stddev[i] = untrackedE_stddev / energies_dbl[i];

			TH1D* UntrackedETemp = (TH1D*) mFiles["f" + energies_str[i]]->Get("hPerEvent_UntrackedE");
			UntrackedEFromParticles_mean[i] = UntrackedETemp->GetMean();
			UntrackedEFromParticles_stddev[i] = UntrackedETemp->GetStdDev();
		}
	}
	for(int i = 0; i < numEnergies; i++) mFiles["f" + energies_str[i]]->Close();

	mFiles["fOut"] = new TFile(("SACAnalysis_n" + n + ".root").c_str(), "RECREATE");
	{
		PlotMEDataPoints(0, y_Gamma_PerEvent_Mult_mean, y_Gamma_PerEvent_Mult_stddev, y_Gamma_PerEvent_Mult_nStdDev_dMean,
							y_Gamma_PerEvent_nMult_dInputE, y_Gamma_PerEvent_nMult_dInputE_stddev,
							y_Gamma_PerEvent_EDep_mean, y_Gamma_PerEvent_EDep_stddev, y_Gamma_PerEvent_EDep_nStdDev_dMean,
							y_Gamma_PerEvent_InitE_mean, y_Gamma_PerEvent_InitE_stddev, y_Gamma_PerEvent_InitE_nStdDev_dMean);

		PlotMEDataPoints(1, y_Positron_PerEvent_Mult_mean, y_Positron_PerEvent_Mult_stddev, y_Positron_PerEvent_Mult_nStdDev_dMean,
							y_Positron_PerEvent_nMult_dInputE, y_Positron_PerEvent_nMult_dInputE_stddev,
							y_Positron_PerEvent_EDep_mean, y_Positron_PerEvent_EDep_stddev, y_Positron_PerEvent_EDep_nStdDev_dMean,
							y_Positron_PerEvent_InitE_mean, y_Positron_PerEvent_InitE_stddev, y_Positron_PerEvent_InitE_nStdDev_dMean);

		PlotMEDataPoints(2, y_Electron_PerEvent_Mult_mean, y_Electron_PerEvent_Mult_stddev, y_Electron_PerEvent_Mult_nStdDev_dMean,
							y_Electron_PerEvent_nMult_dInputE, y_Electron_PerEvent_nMult_dInputE_stddev,
							y_Electron_PerEvent_EDep_mean, y_Electron_PerEvent_EDep_stddev, y_Electron_PerEvent_EDep_nStdDev_dMean,
							y_Electron_PerEvent_InitE_mean, y_Electron_PerEvent_InitE_stddev, y_Electron_PerEvent_InitE_nStdDev_dMean);

		PlotMEDataPoints(3, y_Proton_PerEvent_Mult_mean, y_Proton_PerEvent_Mult_stddev, y_Proton_PerEvent_Mult_nStdDev_dMean,
							y_Proton_PerEvent_nMult_dInputE, y_Proton_PerEvent_nMult_dInputE_stddev,
							y_Proton_PerEvent_EDep_mean, y_Proton_PerEvent_EDep_stddev, y_Proton_PerEvent_EDep_nStdDev_dMean,
							y_Proton_PerEvent_InitE_mean, y_Proton_PerEvent_InitE_stddev, y_Proton_PerEvent_InitE_nStdDev_dMean);

		PlotMEDataPoints(4, y_Neutron_PerEvent_Mult_mean, y_Neutron_PerEvent_Mult_stddev, y_Neutron_PerEvent_Mult_nStdDev_dMean,
							y_Neutron_PerEvent_nMult_dInputE, y_Neutron_PerEvent_nMult_dInputE_stddev,
							y_Neutron_PerEvent_EDep_mean, y_Neutron_PerEvent_EDep_stddev, y_Neutron_PerEvent_EDep_nStdDev_dMean,
							y_Neutron_PerEvent_InitE_mean, y_Neutron_PerEvent_InitE_stddev, y_Neutron_PerEvent_InitE_nStdDev_dMean);

		PlotMEDataPoints(10, y_OptPhot_PerEvent_Mult_mean, y_OptPhot_PerEvent_Mult_stddev, y_OptPhot_PerEvent_Mult_nStdDev_dMean,
							y_OptPhot_PerEvent_nMult_dInputE, y_OptPhot_PerEvent_nMult_dInputE_stddev,
							y_OptPhot_PerEvent_EDep_mean, y_OptPhot_PerEvent_EDep_stddev, y_OptPhot_PerEvent_EDep_nStdDev_dMean,
							y_OptPhot_PerEvent_InitE_mean, y_OptPhot_PerEvent_InitE_stddev, y_OptPhot_PerEvent_InitE_nStdDev_dMean);

		// plot multiplicity per event / incident energy
		mCanvas["nMult_dInputE"] = new TCanvas("c_nMult_dInputE", "multiplicity per event / incident energy", 800, 600);
		TMultiGraph* mg_nMult_dInputE = new TMultiGraph();
		mg_nMult_dInputE->SetName("mg_nMult_dInputE");
		mg_nMult_dInputE->SetTitle("multiplicity per event / incident energy");
		mg_nMult_dInputE->GetXaxis()->SetTitle(x_axis.c_str());
		mg_nMult_dInputE->GetYaxis()->SetTitle("mult per event / incident energy");
		{
			mGraphs["Gamma_PerEvent_nMult_dInputE"]->SetMarkerColor(2);
			mGraphs["Gamma_PerEvent_nMult_dInputE"]->SetMarkerSize(1.5);
			mGraphs["Gamma_PerEvent_nMult_dInputE"]->SetMarkerStyle(21);

			mGraphs["Positron_PerEvent_nMult_dInputE"]->SetMarkerColor(3);
			mGraphs["Positron_PerEvent_nMult_dInputE"]->SetMarkerSize(1.5);
			mGraphs["Positron_PerEvent_nMult_dInputE"]->SetMarkerStyle(21);

			mGraphs["Electron_PerEvent_nMult_dInputE"]->SetMarkerColor(4);
			mGraphs["Electron_PerEvent_nMult_dInputE"]->SetMarkerSize(1.5);
			mGraphs["Electron_PerEvent_nMult_dInputE"]->SetMarkerStyle(21);

			mGraphs["Proton_PerEvent_nMult_dInputE"]->SetMarkerColor(5);
			mGraphs["Proton_PerEvent_nMult_dInputE"]->SetMarkerSize(1.5);
			mGraphs["Proton_PerEvent_nMult_dInputE"]->SetMarkerStyle(21);

			mGraphs["Neutron_PerEvent_nMult_dInputE"]->SetMarkerColor(6);
			mGraphs["Neutron_PerEvent_nMult_dInputE"]->SetMarkerSize(1.5);
			mGraphs["Neutron_PerEvent_nMult_dInputE"]->SetMarkerStyle(21);

			mGraphs["OptPhot_PerEvent_nMult_dInputE"]->SetMarkerColor(7);
			mGraphs["OptPhot_PerEvent_nMult_dInputE"]->SetMarkerSize(1.5);
			mGraphs["OptPhot_PerEvent_nMult_dInputE"]->SetMarkerStyle(21);
		}
		mg_nMult_dInputE->Add(mGraphs["Gamma_PerEvent_nMult_dInputE"]);
		mg_nMult_dInputE->Add(mGraphs["Positron_PerEvent_nMult_dInputE"]);
		mg_nMult_dInputE->Add(mGraphs["Electron_PerEvent_nMult_dInputE"]);
		mg_nMult_dInputE->Add(mGraphs["Proton_PerEvent_nMult_dInputE"]);
		mg_nMult_dInputE->Add(mGraphs["Neutron_PerEvent_nMult_dInputE"]);
		mg_nMult_dInputE->Add(mGraphs["OptPhot_PerEvent_nMult_dInputE"]);
		mg_nMult_dInputE->Draw();
		mg_nMult_dInputE->Write();

		// plot total untracked energy per event
		mCanvas["UntrackedE_frac"] = new TCanvas("cUntrackedE_frac", "fraction of energy not deposited", 800, 600);
		{
			mGraphs["UntrackedE_frac"] = new TGraphErrors(numEnergies, x, UntrackedE_frac_mean, x_stddev, UntrackedE_frac_stddev);
			mGraphs["UntrackedE_frac"]->SetName("gUntrackedE_frac");
			mGraphs["UntrackedE_frac"]->SetTitle("fraction of energy not deposited");
			mGraphs["UntrackedE_frac"]->GetXaxis()->SetTitle(x_axis.c_str());
			mGraphs["UntrackedE_frac"]->GetYaxis()->SetTitle("1 - fraction of edep");
			mGraphs["UntrackedE_frac"]->SetMarkerColor(9);
			mGraphs["UntrackedE_frac"]->SetMarkerSize(1.5);
			mGraphs["UntrackedE_frac"]->SetMarkerStyle(21);
			mGraphs["UntrackedE_frac"]->Draw();
			mGraphs["UntrackedE_frac"]->Write();
		}

		// plot untracked energy due to energy deposition from other particles
		mCanvas["UntrackedEFromParticles_frac"] = new TCanvas("cUntrackedEFromParticles_frac", "fraction of untracked energy from other particles", 800, 600);
		{
			mGraphs["UntrackedEFromParticles_frac"] = new TGraphErrors(numEnergies, x, UntrackedEFromParticles_mean, x_stddev, UntrackedEFromParticles_stddev);
			mGraphs["UntrackedEFromParticles_frac"]->SetName("gUntrackedEFromParticles_frac");
			mGraphs["UntrackedEFromParticles_frac"]->SetTitle("fraction of energy from other particles not deposited");
			mGraphs["UntrackedEFromParticles_frac"]->GetXaxis()->SetTitle(x_axis.c_str());
			mGraphs["UntrackedEFromParticles_frac"]->GetYaxis()->SetTitle("fraction of untracked energy from other particles");
			mGraphs["UntrackedEFromParticles_frac"]->SetMarkerColor(1);
			mGraphs["UntrackedEFromParticles_frac"]->SetMarkerSize(1.5);
			mGraphs["UntrackedEFromParticles_frac"]->SetMarkerStyle(21);
			mGraphs["UntrackedEFromParticles_frac"]->Draw();
			mGraphs["UntrackedEFromParticles_frac"]->Write();
		}

		// plot total energy deposition
		mCanvas["TotalE_frac"] = new TCanvas("cTotalE_frac", "fraction of energy deposited per event", 800, 600);
		TMultiGraph* mgTotalE_frac = new TMultiGraph();
		mgTotalE_frac->SetName("mgTotalE_frac");
		mgTotalE_frac->SetTitle("fraction of energy deposited per event");
		mgTotalE_frac->GetXaxis()->SetTitle(x_axis.c_str());
		mgTotalE_frac->GetYaxis()->SetTitle("fraction of edep");
		{
			mGraphs["TotalE_frac"] = new TGraphErrors(numEnergies, x, TotalE_frac_mean, x_stddev, TotalE_frac_stddev);
			mGraphs["TotalE_frac"]->SetName("gTotalE_frac");
			mGraphs["TotalE_frac"]->SetTitle("total energy deposition per event");
			mGraphs["TotalE_frac"]->SetMarkerColor(1);
			mGraphs["TotalE_frac"]->SetMarkerSize(1.5);
			mGraphs["TotalE_frac"]->SetMarkerStyle(21);

			mGraphs["TotalE_frac_Gamma"] = new TGraphErrors(numEnergies, x, TotalE_frac_Gamma_mean, x_stddev, TotalE_frac_Gamma_stddev);
			mGraphs["TotalE_frac_Gamma"]->SetName("gTotalE_frac_Gamma");
			mGraphs["TotalE_frac_Gamma"]->SetTitle("gamma energy deposition per event");
			mGraphs["TotalE_frac_Gamma"]->SetMarkerColor(2);
			mGraphs["TotalE_frac_Gamma"]->SetMarkerSize(1.5);
			mGraphs["TotalE_frac_Gamma"]->SetMarkerStyle(21);
			mGraphs["TotalE_frac_Gamma"]->Draw();
			mGraphs["TotalE_frac_Gamma"]->Write();

			mGraphs["TotalE_frac_Positron"] = new TGraphErrors(numEnergies, x, TotalE_frac_Positron_mean, x_stddev, TotalE_frac_Positron_stddev);
			mGraphs["TotalE_frac_Positron"]->SetName("gTotalE_frac_Positron");
			mGraphs["TotalE_frac_Positron"]->SetTitle("positron energy deposition per event");
			mGraphs["TotalE_frac_Positron"]->SetMarkerColor(3);
			mGraphs["TotalE_frac_Positron"]->SetMarkerSize(1.5);
			mGraphs["TotalE_frac_Positron"]->SetMarkerStyle(21);
			mGraphs["TotalE_frac_Positron"]->Draw();
			mGraphs["TotalE_frac_Positron"]->Write();

			mGraphs["TotalE_frac_Electron"] = new TGraphErrors(numEnergies, x, TotalE_frac_Electron_mean, x_stddev, TotalE_frac_Electron_stddev);
			mGraphs["TotalE_frac_Electron"]->SetName("gTotalE_frac_Electron");
			mGraphs["TotalE_frac_Electron"]->SetTitle("electron energy deposition per event");
			mGraphs["TotalE_frac_Electron"]->SetMarkerColor(4);
			mGraphs["TotalE_frac_Electron"]->SetMarkerSize(1.5);
			mGraphs["TotalE_frac_Electron"]->SetMarkerStyle(21);
			mGraphs["TotalE_frac_Electron"]->Draw();
			mGraphs["TotalE_frac_Electron"]->Write();

			mGraphs["TotalE_frac_Proton"] = new TGraphErrors(numEnergies, x, TotalE_frac_Proton_mean, x_stddev, TotalE_frac_Proton_stddev);
			mGraphs["TotalE_frac_Proton"]->SetName("gTotalE_frac_Proton");
			mGraphs["TotalE_frac_Proton"]->SetTitle("proton energy deposition per event");
			mGraphs["TotalE_frac_Proton"]->SetMarkerColor(5);
			mGraphs["TotalE_frac_Proton"]->SetMarkerSize(1.5);
			mGraphs["TotalE_frac_Proton"]->SetMarkerStyle(21);
			mGraphs["TotalE_frac_Proton"]->Draw();
			mGraphs["TotalE_frac_Proton"]->Write();

			mGraphs["TotalE_frac_Neutron"] = new TGraphErrors(numEnergies, x, TotalE_frac_Neutron_mean, x_stddev, TotalE_frac_Neutron_stddev);
			mGraphs["TotalE_frac_Neutron"]->SetName("gTotalE_frac_Neutron");
			mGraphs["TotalE_frac_Neutron"]->SetTitle("neutron energy deposition per event");
			mGraphs["TotalE_frac_Neutron"]->SetMarkerColor(6);
			mGraphs["TotalE_frac_Neutron"]->SetMarkerSize(1.5);
			mGraphs["TotalE_frac_Neutron"]->SetMarkerStyle(21);
			mGraphs["TotalE_frac_Neutron"]->Draw();
			mGraphs["TotalE_frac_Neutron"]->Write();

			mGraphs["TotalE_frac_OptPhot"] = new TGraphErrors(numEnergies, x, TotalE_frac_OptPhot_mean, x_stddev, TotalE_frac_OptPhot_stddev);
			mGraphs["TotalE_frac_OptPhot"]->SetName("gTotalE_frac_OptPhot");
			mGraphs["TotalE_frac_OptPhot"]->SetTitle("optical photon energy deposition per event");
			mGraphs["TotalE_frac_OptPhot"]->SetMarkerColor(7);
			mGraphs["TotalE_frac_OptPhot"]->SetMarkerSize(1.5);
			mGraphs["TotalE_frac_OptPhot"]->SetMarkerStyle(21);
			mGraphs["TotalE_frac_OptPhot"]->Draw();
			mGraphs["TotalE_frac_OptPhot"]->Write();
		}
		mgTotalE_frac->Add(mGraphs["TotalE_frac"]);
		mgTotalE_frac->Add(mGraphs["UntrackedE_frac"]);
		mgTotalE_frac->Add(mGraphs["TotalE_frac_Gamma"]);
		mgTotalE_frac->Add(mGraphs["TotalE_frac_Positron"]);
		mgTotalE_frac->Add(mGraphs["TotalE_frac_Electron"]);
		mgTotalE_frac->Add(mGraphs["TotalE_frac_Proton"]);
		mgTotalE_frac->Add(mGraphs["TotalE_frac_Neutron"]);
		mgTotalE_frac->Add(mGraphs["TotalE_frac_OptPhot"]);
		mgTotalE_frac->Draw();
		mgTotalE_frac->Write();
	}
	mFiles["fOut"]->Close();
}
