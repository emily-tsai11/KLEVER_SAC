// SACAnalysis.cc
// --------------------------------------------------------------
// run with root!!
// --------------------------------------------------------------
// History:
//
// Created by Emily Tsai (emily.tsai11@gmail.com) 2020-7-20
// --------------------------------------------------------------

#include <string>
#include <map>
#include <math.h>

string numPart = "10";
const int numEnergies = 10;
float energies_flt[] = {0.1, 0.2, 0.5, 1, 2, 5, 10, 20, 50, 100};
string energies_str[] = {"100MeV", "200MeV", "500MeV", "1GeV", "2GeV", "5GeV", "10GeV", "20GeV", "50GeV", "100GeV"};
int numPartID = 11;
string partType[] = {"Gamma", "Positron", "Electron", "Proton", "Neutron", "PionPlus", "PionMinus", "PionZero", "MuonPlus", "MuonMinus", "OptPhot"};
string x_axis = "input energy (GeV)";
string draw_opt = "ap*";

map<string, TFile*> mFiles;
map<string, TCanvas*> mCanvas;
map<string, TGraph*> mGraphs;

float x[numEnergies];
float x_stddev[numEnergies];

// 0
float y_Gamma_NPerEvent_mean[numEnergies];
float y_Gamma_NPerEvent_stddev[numEnergies];
float y_Gamma_NPerEvent_nStdDev_dMean[numEnergies];
float y_Gamma_EPerEvent_mean[numEnergies];
float y_Gamma_EPerEvent_stddev[numEnergies];
float y_Gamma_EPerEvent_nStdDev_dMean[numEnergies];

// 1
float y_Positron_NPerEvent_mean[numEnergies];
float y_Positron_NPerEvent_stddev[numEnergies];
float y_Positron_NPerEvent_nStdDev_dMean[numEnergies];
float y_Positron_EPerEvent_mean[numEnergies];
float y_Positron_EPerEvent_stddev[numEnergies];
float y_Positron_EPerEvent_nStdDev_dMean[numEnergies];

// 2
float y_Electron_NPerEvent_mean[numEnergies];
float y_Electron_NPerEvent_stddev[numEnergies];
float y_Electron_NPerEvent_nStdDev_dMean[numEnergies];
float y_Electron_EPerEvent_mean[numEnergies];
float y_Electron_EPerEvent_stddev[numEnergies];
float y_Electron_EPerEvent_nStdDev_dMean[numEnergies];

// 3
float y_Proton_NPerEvent_mean[numEnergies];
float y_Proton_NPerEvent_stddev[numEnergies];
float y_Proton_NPerEvent_nStdDev_dMean[numEnergies];
float y_Proton_EPerEvent_mean[numEnergies];
float y_Proton_EPerEvent_stddev[numEnergies];
float y_Proton_EPerEvent_nStdDev_dMean[numEnergies];

// 4
float y_Neutron_NPerEvent_mean[numEnergies];
float y_Neutron_NPerEvent_stddev[numEnergies];
float y_Neutron_NPerEvent_nStdDev_dMean[numEnergies];
float y_Neutron_EPerEvent_mean[numEnergies];
float y_Neutron_EPerEvent_stddev[numEnergies];
float y_Neutron_EPerEvent_nStdDev_dMean[numEnergies];

// 5
float y_PionPlus_NPerEvent_mean[numEnergies];
float y_PionPlus_NPerEvent_stddev[numEnergies];
float y_PionPlus_NPerEvent_nStdDev_dMean[numEnergies];
float y_PionPlus_EPerEvent_mean[numEnergies];
float y_PionPlus_EPerEvent_stddev[numEnergies];
float y_PionPlus_EPerEvent_nStdDev_dMean[numEnergies];

// 6
float y_PionMinus_NPerEvent_mean[numEnergies];
float y_PionMinus_NPerEvent_stddev[numEnergies];
float y_PionMinus_NPerEvent_nStdDev_dMean[numEnergies];
float y_PionMinus_EPerEvent_mean[numEnergies];
float y_PionMinus_EPerEvent_stddev[numEnergies];
float y_PionMinus_EPerEvent_nStdDev_dMean[numEnergies];

// 7
float y_PionZero_NPerEvent_mean[numEnergies];
float y_PionZero_NPerEvent_stddev[numEnergies];
float y_PionZero_NPerEvent_nStdDev_dMean[numEnergies];
float y_PionZero_EPerEvent_mean[numEnergies];
float y_PionZero_EPerEvent_stddev[numEnergies];
float y_PionZero_EPerEvent_nStdDev_dMean[numEnergies];

// 8
float y_MuonPlus_NPerEvent_mean[numEnergies];
float y_MuonPlus_NPerEvent_stddev[numEnergies];
float y_MuonPlus_NPerEvent_nStdDev_dMean[numEnergies];
float y_MuonPlus_EPerEvent_mean[numEnergies];
float y_MuonPlus_EPerEvent_stddev[numEnergies];
float y_MuonPlus_EPerEvent_nStdDev_dMean[numEnergies];

// 9
float y_MuonMinus_NPerEvent_mean[numEnergies];
float y_MuonMinus_NPerEvent_stddev[numEnergies];
float y_MuonMinus_NPerEvent_nStdDev_dMean[numEnergies];
float y_MuonMinus_EPerEvent_mean[numEnergies];
float y_MuonMinus_EPerEvent_stddev[numEnergies];
float y_MuonMinus_EPerEvent_nStdDev_dMean[numEnergies];

// 10
float y_OptPhot_NPerEvent_mean[numEnergies];
float y_OptPhot_NPerEvent_stddev[numEnergies];
float y_OptPhot_NPerEvent_nStdDev_dMean[numEnergies];
float y_OptPhot_EPerEvent_mean[numEnergies];
float y_OptPhot_EPerEvent_stddev[numEnergies];
float y_OptPhot_EPerEvent_nStdDev_dMean[numEnergies];

float y_TotalE[numEnergies];
float y_TotalE_err[numEnergies];

void GetNEDataPoints(int EIndex, int PIndex,
	float NMean[], float NStdDev[], float N_nStdDev_dMean[],
	float EMean[], float EStdDev[], float E_nStdDev_dMean[])
{
	TH1F* NCurrent = (TH1F*) mFiles["f" + energies_str[EIndex]]->Get(("h" + partType[PIndex] + "_NPerEvent;1").c_str());
	NMean[EIndex] = NCurrent->GetMean();
	NStdDev[EIndex] = NCurrent->GetStdDev();
	float NTemp = NCurrent->GetStdDev() / NCurrent->GetMean();
	N_nStdDev_dMean[EIndex] = isnan(NTemp) ? 0.0 : NTemp;

	TH1F* ECurrent = (TH1F*) mFiles["f" + energies_str[EIndex]]->Get(("h" + partType[PIndex] + "_EPerEvent;1").c_str());
	EMean[EIndex] = ECurrent->GetMean();
	EStdDev[EIndex] = ECurrent->GetStdDev();
	float ETemp = ECurrent->GetStdDev() / ECurrent->GetMean();
	E_nStdDev_dMean[EIndex] = isnan(ETemp) ? 0.0 : ETemp;
}

void PlotNEDataPoints(int PIndex,
	float NMean[], float NStdDev[], float N_nStdDev_dMean[],
	float EMean[], float EStdDev[], float E_nStdDev_dMean[])
{
	TF1* func = new TF1("func", "[0] / sqrt(x)", 0, 10);

	string key1 = partType[PIndex] + "_NPerEvent_mean";
	string title1 = "mean number of " + partType[PIndex] + " per event";
	mCanvas[key1] = new TCanvas(("c" + key1).c_str(), title1.c_str(), 800, 600);
	mGraphs[key1] = new TGraphErrors(numEnergies, x, NMean, x_stddev, NStdDev);
	mGraphs[key1]->SetName(("g" + key1).c_str());
	mGraphs[key1]->SetTitle(title1.c_str());
	mGraphs[key1]->GetXaxis()->SetTitle(x_axis.c_str());
	mGraphs[key1]->GetYaxis()->SetTitle("mean number of particles");
	mGraphs[key1]->Draw(draw_opt.c_str());
	mGraphs[key1]->Write();

	string key2 = partType[PIndex] + "_NPerEvent_stddev";
	string title2 = "stddev of mean number of " + partType[PIndex] + " per event";
	mCanvas[key2] = new TCanvas(("c" + key2).c_str(), title2.c_str(), 800, 600);
	mGraphs[key2] = new TGraph(numEnergies, x, NStdDev);
	mGraphs[key2]->SetName(("g" + key2).c_str());
	mGraphs[key2]->SetTitle(title2.c_str());
	mGraphs[key2]->GetXaxis()->SetTitle(x_axis.c_str());
	mGraphs[key2]->GetYaxis()->SetTitle("stddev of mean number of particles");
	mGraphs[key2]->Draw(draw_opt.c_str());
	mGraphs[key2]->Write();

	string key3 = partType[PIndex] + "_NPerEvent_nStdDev_dMean";
	string title3 = "stddev / mean number of " + partType[PIndex] + " per event";
	mCanvas[key3] = new TCanvas(("c" + key3).c_str(), title3.c_str(), 800, 600);
	mGraphs[key3] = new TGraph(numEnergies, x, N_nStdDev_dMean);
	mGraphs[key3]->SetName(("g" + key3).c_str());
	mGraphs[key3]->SetTitle(title3.c_str());
	mGraphs[key3]->GetXaxis()->SetTitle(x_axis.c_str());
	mGraphs[key3]->GetYaxis()->SetTitle("stddev / mean number of particles");
	mGraphs[key3]->Draw(draw_opt.c_str());
	mGraphs[key3]->Write();

	mGraphs[key3]->Fit(func);
	mGraphs[key3]->SetName(("g" + key3 + "_fit").c_str());
	mGraphs[key3]->SetTitle((title3 + " (fit)").c_str());
	gStyle->SetOptFit(1011);
	mGraphs[key3]->Draw(draw_opt.c_str());
	mGraphs[key3]->Write();

	string key4 = partType[PIndex] + "_EPerEvent_mean";
	string title4 = "mean energy of " + partType[PIndex] + " per event";
	mCanvas[key4] = new TCanvas(("c" + key4).c_str(), title4.c_str(), 800, 600);
	mGraphs[key4] = new TGraphErrors(numEnergies, x, EMean, x_stddev, EStdDev);
	mGraphs[key4]->SetName(("g" + key4).c_str());
	mGraphs[key4]->SetTitle(title4.c_str());
	mGraphs[key4]->GetXaxis()->SetTitle(x_axis.c_str());
	mGraphs[key4]->GetYaxis()->SetTitle("mean energy (MeV)");
	mGraphs[key4]->Draw(draw_opt.c_str());
	mGraphs[key4]->Write();

	string key5 = partType[PIndex] + "_EPerEvent_stddev";
	string title5 = "stddev of mean energy of " + partType[PIndex] + " per event";
	mCanvas[key5] = new TCanvas(("c" + key5).c_str(), title5.c_str(), 800, 600);
	mGraphs[key5] = new TGraph(numEnergies, x, EStdDev);
	mGraphs[key5]->SetName(("g" + key5).c_str());
	mGraphs[key5]->SetTitle(title5.c_str());
	mGraphs[key5]->GetXaxis()->SetTitle(x_axis.c_str());
	mGraphs[key5]->GetYaxis()->SetTitle("stddev of mean energy (MeV)");
	mGraphs[key5]->Draw(draw_opt.c_str());
	mGraphs[key5]->Write();

	string key6 = partType[PIndex] + "_EPerEvent_nStdDev_dMean";
	string title6 = "stddev / mean energy of " + partType[PIndex] + " per event";
	mCanvas[key6] = new TCanvas(("c" + key6).c_str(), title6.c_str(), 800, 600);
	mGraphs[key6] = new TGraph(numEnergies, x, E_nStdDev_dMean);
	mGraphs[key6]->SetName(("g" + key6).c_str());
	mGraphs[key6]->SetTitle(title6.c_str());
	mGraphs[key6]->GetXaxis()->SetTitle(x_axis.c_str());
	mGraphs[key6]->GetYaxis()->SetTitle("stddev / mean energy (MeV)");
	mGraphs[key6]->Draw(draw_opt.c_str());
	mGraphs[key6]->Write();

	mGraphs[key6]->Fit(func);
	mGraphs[key6]->SetName(("g" + key6 + "_fit").c_str());
	mGraphs[key6]->SetTitle((title6 + " (fit)").c_str());
	gStyle->SetOptFit(1011);
	mGraphs[key6]->Draw(draw_opt.c_str());
	mGraphs[key6]->Write();
}

void SACAnalysis()
{
	for(int i = 0; i < numEnergies; i++)
		mFiles["f" + energies_str[i]] = new TFile(("n" + numPart + "/SAC_" + energies_str[i] + ".root").c_str());
	{
		for(int i = 0; i < numPartID; i++)
		{
			x[i] = energies_flt[i];
			x_stddev[i] = 0.0;
		}
		for(int i = 0; i < numEnergies; i++)
		{
			GetNEDataPoints(i, 0, y_Gamma_NPerEvent_mean, y_Gamma_NPerEvent_stddev, y_Gamma_NPerEvent_nStdDev_dMean,
									y_Gamma_EPerEvent_mean, y_Gamma_EPerEvent_stddev, y_Gamma_EPerEvent_nStdDev_dMean);
			GetNEDataPoints(i, 1, y_Positron_NPerEvent_mean, y_Positron_NPerEvent_stddev, y_Positron_NPerEvent_nStdDev_dMean,
									y_Positron_EPerEvent_mean, y_Positron_EPerEvent_stddev, y_Positron_EPerEvent_nStdDev_dMean);
			GetNEDataPoints(i, 2, y_Electron_NPerEvent_mean, y_Electron_NPerEvent_stddev, y_Electron_NPerEvent_nStdDev_dMean,
									y_Electron_EPerEvent_mean, y_Electron_EPerEvent_stddev, y_Electron_EPerEvent_nStdDev_dMean);
			GetNEDataPoints(i, 3, y_Proton_NPerEvent_mean, y_Proton_NPerEvent_stddev, y_Proton_NPerEvent_nStdDev_dMean,
									y_Proton_EPerEvent_mean, y_Proton_EPerEvent_stddev, y_Proton_EPerEvent_nStdDev_dMean);
			GetNEDataPoints(i, 4, y_Neutron_NPerEvent_mean, y_Neutron_NPerEvent_stddev, y_Neutron_NPerEvent_nStdDev_dMean,
									y_Neutron_EPerEvent_mean, y_Neutron_EPerEvent_stddev, y_Neutron_EPerEvent_nStdDev_dMean);
			GetNEDataPoints(i, 5, y_PionPlus_NPerEvent_mean, y_PionPlus_NPerEvent_stddev, y_PionPlus_NPerEvent_nStdDev_dMean,
									y_PionPlus_EPerEvent_mean, y_PionPlus_EPerEvent_stddev, y_PionPlus_EPerEvent_nStdDev_dMean);
			GetNEDataPoints(i, 6, y_PionMinus_NPerEvent_mean, y_PionMinus_NPerEvent_stddev, y_PionMinus_NPerEvent_nStdDev_dMean,
									y_PionMinus_EPerEvent_mean, y_PionMinus_EPerEvent_stddev, y_PionMinus_EPerEvent_nStdDev_dMean);
			GetNEDataPoints(i, 7, y_PionZero_NPerEvent_mean, y_PionZero_NPerEvent_stddev, y_PionZero_NPerEvent_nStdDev_dMean,
									y_PionZero_EPerEvent_mean, y_PionZero_EPerEvent_stddev, y_PionZero_EPerEvent_nStdDev_dMean);
			GetNEDataPoints(i, 8, y_MuonPlus_NPerEvent_mean, y_MuonPlus_NPerEvent_stddev, y_MuonPlus_NPerEvent_nStdDev_dMean,
									y_MuonPlus_EPerEvent_mean, y_MuonPlus_EPerEvent_stddev, y_MuonPlus_EPerEvent_nStdDev_dMean);
			GetNEDataPoints(i, 9, y_MuonMinus_NPerEvent_mean, y_MuonMinus_NPerEvent_stddev, y_MuonMinus_NPerEvent_nStdDev_dMean,
									y_MuonMinus_EPerEvent_mean, y_MuonMinus_EPerEvent_stddev, y_MuonMinus_EPerEvent_nStdDev_dMean);
			GetNEDataPoints(i, 10, y_OptPhot_NPerEvent_mean, y_OptPhot_NPerEvent_stddev, y_OptPhot_NPerEvent_nStdDev_dMean,
									y_OptPhot_EPerEvent_mean, y_OptPhot_EPerEvent_stddev, y_OptPhot_EPerEvent_nStdDev_dMean);
		}

		for(int i = 0; i < numEnergies; i++)
		{
			TH1F* ETemp = (TH1F*) mFiles["f" + energies_str[i]]->Get("hEPerEvent;1");
			y_TotalE[i] = ETemp->GetMean();
			y_TotalE_err[i] = ETemp->GetStdDev();
		}
	}
	for(int i = 0; i < numEnergies; i++) mFiles["f" + energies_str[i]]->Close();

	mFiles["fOut"] = new TFile(("SACAnalysis_n" + numPart + ".root").c_str(), "RECREATE");
	{
		PlotNEDataPoints(0, y_Gamma_NPerEvent_mean, y_Gamma_NPerEvent_stddev, y_Gamma_NPerEvent_nStdDev_dMean,
							y_Gamma_EPerEvent_mean, y_Gamma_EPerEvent_stddev, y_Gamma_EPerEvent_nStdDev_dMean);
		PlotNEDataPoints(1, y_Positron_NPerEvent_mean, y_Positron_NPerEvent_stddev, y_Positron_NPerEvent_nStdDev_dMean,
							y_Positron_EPerEvent_mean, y_Positron_EPerEvent_stddev, y_Positron_EPerEvent_nStdDev_dMean);
		PlotNEDataPoints(2, y_Electron_NPerEvent_mean, y_Electron_NPerEvent_stddev, y_Electron_NPerEvent_nStdDev_dMean,
							y_Electron_EPerEvent_mean, y_Electron_EPerEvent_stddev, y_Electron_EPerEvent_nStdDev_dMean);
		PlotNEDataPoints(3, y_Proton_NPerEvent_mean, y_Proton_NPerEvent_stddev, y_Proton_NPerEvent_nStdDev_dMean,
							y_Proton_EPerEvent_mean, y_Proton_EPerEvent_stddev, y_Proton_EPerEvent_nStdDev_dMean);
		PlotNEDataPoints(4, y_Neutron_NPerEvent_mean, y_Neutron_NPerEvent_stddev, y_Neutron_NPerEvent_nStdDev_dMean,
							y_Neutron_EPerEvent_mean, y_Neutron_EPerEvent_stddev, y_Neutron_EPerEvent_nStdDev_dMean);
		PlotNEDataPoints(5, y_PionPlus_NPerEvent_mean, y_PionPlus_NPerEvent_stddev, y_PionPlus_NPerEvent_nStdDev_dMean,
							y_PionPlus_EPerEvent_mean, y_PionPlus_EPerEvent_stddev, y_PionPlus_EPerEvent_nStdDev_dMean);
		PlotNEDataPoints(6, y_PionMinus_NPerEvent_mean, y_PionMinus_NPerEvent_stddev, y_PionMinus_NPerEvent_nStdDev_dMean,
							y_PionMinus_EPerEvent_mean, y_PionMinus_EPerEvent_stddev, y_PionMinus_EPerEvent_nStdDev_dMean);
		PlotNEDataPoints(7, y_PionZero_NPerEvent_mean, y_PionZero_NPerEvent_stddev, y_PionZero_NPerEvent_nStdDev_dMean,
							y_PionZero_EPerEvent_mean, y_PionZero_EPerEvent_stddev, y_PionZero_EPerEvent_nStdDev_dMean);
		PlotNEDataPoints(8, y_MuonPlus_NPerEvent_mean, y_MuonPlus_NPerEvent_stddev, y_MuonPlus_NPerEvent_nStdDev_dMean,
							y_MuonPlus_EPerEvent_mean, y_MuonPlus_EPerEvent_stddev, y_MuonPlus_EPerEvent_nStdDev_dMean);
		PlotNEDataPoints(9, y_MuonMinus_NPerEvent_mean, y_MuonMinus_NPerEvent_stddev, y_MuonMinus_NPerEvent_nStdDev_dMean,
							y_MuonMinus_EPerEvent_mean, y_MuonMinus_EPerEvent_stddev, y_MuonMinus_EPerEvent_nStdDev_dMean);
		PlotNEDataPoints(10, y_OptPhot_NPerEvent_mean, y_OptPhot_NPerEvent_stddev, y_OptPhot_NPerEvent_nStdDev_dMean,
							y_OptPhot_EPerEvent_mean, y_OptPhot_EPerEvent_stddev, y_OptPhot_EPerEvent_nStdDev_dMean);

		string key = "hEPerEvent";
		string title = "mean total energy deposited in SD per event";
		mCanvas[key] = new TCanvas(("c" + key).c_str(), title.c_str(), 800, 600);
		mGraphs[key] = new TGraphErrors(numEnergies, x, y_TotalE, x_stddev, y_TotalE_err);
		mGraphs[key]->SetName("gEPerEvent");
		mGraphs[key]->SetTitle(title.c_str());
		mGraphs[key]->GetXaxis()->SetTitle(x_axis.c_str());
		mGraphs[key]->GetYaxis()->SetTitle("mean energy deposited in SD (MeV)");
		mGraphs[key]->Draw(draw_opt.c_str());
		mGraphs[key]->Write();
	}
	mFiles["fOut"]->Close();
}
