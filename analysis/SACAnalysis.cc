// SACAnalysis.cc
// --------------------------------------------------------------
// History:
//
// Created by Emily Tsai (emily.tsai11@gmail.com) 2020-7-20
// --------------------------------------------------------------

#include <map>
#include <cmath>
#include <fstream>
#include <sstream>

// -------------------- Constants -------------------- //

static const string LayerGap = "50um";
static const double NRuns[] = { // Each correspoding to an energy
	4.0,
	4.0,
	4.0,
	4.0,
	10.0,
	20.0,
	40.0,
	80.0,
	100.0,
	200.0
};

static const int NEnergies = 10;
static const string SEnergies[] = {
	"0.1GeV",
	"0.2GeV",
	"0.5GeV",
	"1.0GeV",
	"2.0GeV",
	"5.0GeV",
	"10.0GeV",
	"20.0GeV",
	"50.0GeV",
	"100.0GeV"
};
static const double DEnergies[] = { // in GeV
	0.1,
	0.2,
	0.5,
	1.0,
	2.0,
	5.0,
	10.0,
	20.0,
	50.0,
	100.0
};
static const double StdEnergies[] = { // in GeV
	0.0,
	0.0,
	0.0,
	0.0,
	0.0,
	0.0,
	0.0,
	0.0,
	0.0,
	0.0
};
static const double BEnergies[] = { // in GeV
	0.07,
	0.12,
	0.3,
	0.7,
	1.2,
	3.0,
	7.0,
	12.0,
	30.0,
	70.0,
	120.0
};

static const int NThresholds = 6;
static const double DThresholds[] = { // in GeV
	1.0,
	2.0,
	5.0,
	15.0,
	0.093,
	100.0
};

static const int NParticles = 14;
static const string SParticles[] = {
	"all",
	"e+",
	"e-",
	"gamma",
	"mu+",
	"mu-",
	"neutron",
	"opticalphoton",
	"other",
	"pi+",
	"pi-",
	"pi0",
	"proton",
	"untracked"
};
static const string SymbParticles[] = {
	"E_{total}",
	"e^{+}",
	"e^{-}",
	"#gamma",
	"#mu^{+}",
	"#mu^{-}",
	"n",
	"Cer. #gamma",
	"E_{other}",
	"#pi^{+}",
	"#pi^{-}",
	"#pi^{0}",
	"p",
	"E_{untracked}"
};

static const int SACNRows = 10;
static const int SACNCols = 10;
static const int SACNLayers = 4;

static const char* DrawOpt = "AP";
static const int Colors[] = {
	1,
	2,
	4,
	3,
	29,
	30,
	6,
	7,
	9,
	39,
	40,
	49,
	5,
	28
};
static const double MSize = 1.3;
static const int MStyle = 8;
static const int LWidth = 1;
static const int LStyle = 1;

static const int StackColors[] = {
	2,
	3,
	4,
	5
};

static bool firstPlot = true;
static bool lastPlot = false;

static string FName;
static string Beam;
static int NEvents;
static const string FOutName = "plots.pdf";

// -------------------- Data structures -------------------- //

static map<string, TFile*> MFiles;
static map<string, vector<double>> MData;

// -------------------- Helper methods -------------------- //

string GetBeamType(int bt)
{
	switch(bt)
	{
		case 1: return "kaon0L_" + LayerGap;
		case 2: return "kaon0L_" + LayerGap;
		case 3: return "neutron_" + LayerGap;
		default: return "gamma_" + LayerGap;
	}
}

void ReadInTxt(string masterKey)
{
	string line, word, key;
	int n_events = 100;

	// Calculate means
	for(int e = 0; e < 3; e++)
	{
		ifstream input_file("../build/SAC_3_" + SEnergies[e] + "_n"
			+ std::to_string(n_events) + "_EnergyDeposition.txt");

		vector<double> yValMean(NParticles);
		while(getline(input_file, line))
		{
			stringstream ss(line);
			for(int p = 0; p < NParticles; p++)
			{
				ss >> word;
				yValMean[p] += std::stod(word) / 100.0;
			}
		}
		key = masterKey + SEnergies[e] + "m" + "temp";
		MData[key] = yValMean;

		input_file.close();
	}

	// Calculate stds
	for(int e = 0; e < 3; e++)
	{
		ifstream input_file("../build/SAC_3_" + SEnergies[e] + "_n"
			+ std::to_string(n_events) + "_EnergyDeposition.txt");

		vector<double> yValStd(NParticles);
		while(getline(input_file, line))
		{
			stringstream ss(line);
			for(int p = 0; p < NParticles; p++)
			{
				ss >> word;
				string mean_key = masterKey + SEnergies[e] + "m" + "temp";
				yValStd[p] += (std::stod(word) - MData[mean_key][p])
					* (std::stod(word) - MData[mean_key][p]);
			}
		}
		for(int p = 0; p < NParticles; p++)
		{
			yValStd[p] = sqrt(yValStd[p] / n_events) / 100.0;
		}
		key = masterKey + SEnergies[e] + "std" + "temp";
		MData[key] = yValStd;

		input_file.close();
	}

	// Rearrange
	for(int p = 0; p < NParticles; p++)
	{
		string mean_key, std_key;
		vector<double> yValMean(NEnergies);
		vector<double> yValStd(NEnergies);
		for(int e = 0; e < 3; e++)
		{
			mean_key = masterKey + SEnergies[e] + "m" + "temp";
			std_key = masterKey + SEnergies[e] + "std" + "temp";
			yValMean[e] = MData[mean_key][p];
			yValStd[e] = MData[std_key][p];
		}
		key = masterKey + SParticles[p];
		MData[key + "_m"] = yValMean;
		MData[key + "_std"] = yValStd;
	}
}

void ReadInMeanAndStd(string masterKey)
{
	string key;
	TH1D* temp;
	vector<double> yValMean(NEnergies);
	vector<double> yValStd(NEnergies);
	for(int p = 0; p < NParticles; p++)
	{
		key = masterKey + SParticles[p];
		for(int e = 0; e < NEnergies; e++)
		{
			temp = (TH1D*) MFiles[SEnergies[e]]->Get(key.c_str());
			yValMean[e] = temp->GetMean();
			yValStd[e] = temp->GetStdDev() / sqrt(NEvents);
		}
		MData[key + "_m"] = yValMean;
		MData[key + "_std"] = yValStd;
	}
}

void ReadInStackedBarVals(string masterKey)
{
	string key;

	// Read in values from files
	vector<double> yVal(SACNLayers);
	TH1D* temp;
	for(int p = 0; p < NParticles; p++)
	{
		key = masterKey + SParticles[p];
		for(int e = 0; e < NEnergies; e++)
		{
			temp = (TH1D*) MFiles[SEnergies[e]]->Get(key.c_str());
			for(int l = 0; l < SACNLayers; l++)
				yVal[l] = temp->GetBinContent(l + 1);
			MData[key + SEnergies[e]] = yVal;
		}
	}

	// Rearrange values into what I need to plot them
	vector<double> yStackedVal(NEnergies);
	for(int p = 0; p < NParticles; p++)
	{
		key = masterKey + SParticles[p];
		for(int l = 0; l < SACNLayers; l++)
		{
			for(int e = 0; e < NEnergies; e++)
				yStackedVal[e] = MData[key + SEnergies[e]][l] / NRuns[e]; // Normalize with # of runs
			MData[key + std::to_string(l)] = yStackedVal;
		}
	}
}

void CalcThresholds(string masterKey)
{
	string key;

	// Calculate threshold values
	vector<double> yVal(NEnergies);
	vector<double> yValStd(NEnergies);
	TH1D* temp;
	double integral, integralError;
	for(double threshold : DThresholds)
	{
		for(string particle : SParticles)
		{
			key = masterKey + particle;
			for(int e = 0; e < NEnergies; e++)
			{
				temp = (TH1D*) MFiles[SEnergies[e]]->Get(key.c_str());

				if(threshold >= DEnergies[e]) yVal[e] = 0.0;
				else
				{
					integral = 0.0, integralError = 0.0;
					for(int i = 1; i <= temp->GetNbinsX(); i++)
					{
						if(temp->GetBinLowEdge(i) * DEnergies[e] >= threshold)
						{
							integral += temp->GetBinContent(i) * temp->GetBinWidth(i);
							integralError += temp->GetBinError(i) * temp->GetBinError(i)
								* temp->GetBinWidth(i) * temp->GetBinWidth(i);
						}
					}
					yVal[e] = integral;
					yValStd[e] = sqrt(integralError);
				}
			}
			MData[std::to_string(threshold) + key + "_m"] = yVal;
			MData[std::to_string(threshold) + key + "_std"] = yValStd;
		}
	}
}

void ReadCompare(string masterKey)
{
	int nXBins = 200;
	double increment = 0.5;
	double halfBin = 0.25;
	vector<double> xVal(nXBins);
	vector<double> xStd(nXBins);

	TH1D* temp_all;
	vector<double> yVal_all(nXBins);
	vector<double> yStd_all(nXBins);

	TH1D* temp_proton;
	vector<double> yVal_proton(nXBins);
	vector<double> yStd_proton(nXBins);

	for(int e = 0; e < NEnergies; e++)
	{
		temp_all = (TH1D*) MFiles[SEnergies[e]]->Get((masterKey + "all").c_str());
		temp_proton = (TH1D*) MFiles[SEnergies[e]]->Get((masterKey + "proton").c_str());

		for(int i = 0; i < nXBins; i++)
		{
			xVal[i] = i * increment + halfBin;
			xStd[i] = 0.0;
			yVal_all[i] = temp_all->GetBinContent(i + 1);
			yStd_all[i] = temp_all->GetBinError(i + 1);
			yVal_proton[i] = temp_proton->GetBinContent(i + 1);
			yStd_proton[i] = temp_proton->GetBinError(i + 1);
		}

		MData[SEnergies[e] + "compare_x"] = xVal;
		MData[SEnergies[e] + "compare_x_std"] = xStd;
		MData[SEnergies[e] + "compare_y_all"] = yVal_all;
		MData[SEnergies[e] + "compare_y_std_all"] = yStd_all;
		MData[SEnergies[e] + "compare_y_proton"] = yVal_proton;
		MData[SEnergies[e] + "compare_y_std_proton"] = yStd_proton;
	}
}

void DrawTGraphErrors(TGraphErrors* g, int p, string n, string t, string x,
	string y)
{
	g->SetName(n.c_str());
	g->SetTitle((t + ";" + x + ";" + y).c_str());

	g->SetMarkerColor(Colors[p]);
	g->SetMarkerSize(MSize);
	g->SetMarkerStyle(MStyle);
	g->SetLineWidth(LWidth);
	g->SetLineStyle(LStyle);

	TCanvas* c = new TCanvas(n.c_str(), n.c_str(), 800, 600);
	gPad->SetLogx(1);
	g->Draw(DrawOpt);

	string fname = Beam + "/" + Beam + FOutName;
	if(firstPlot) fname += "(";
	else if(lastPlot) fname += ")";

	c->Print(fname.c_str());
}

void DrawTMultiGraph(TMultiGraph* mg, TLegend* l, string n, string t, string x,
	string y)
{
	mg->SetName(n.c_str());
	mg->SetTitle((t + ";" + x + ";" + y).c_str());

	l->SetFillStyle(0);
	l->SetBorderSize(0);

	TCanvas* c = new TCanvas(n.c_str(), n.c_str(), 800, 600);
	gPad->SetLogx(1);
	mg->Draw(DrawOpt);
	l->Draw();

	string fname = Beam + "/" + Beam + FOutName;
	if(firstPlot) fname += "(";
	else if(lastPlot) fname += ")";

	c->Print(fname.c_str());
}

void DrawGraphs(string masterKey, string t, string x, string y)
{
	TMultiGraph* mg = new TMultiGraph();
	TLegend* legend = new TLegend(0.905, 0.2, 0.995, 0.8);

	string key;
	string title;
	for(int p = 0; p < NParticles; p++)
	{
		key = masterKey + SParticles[p];

		TGraphErrors* g = new TGraphErrors(NEnergies, DEnergies,
			MData[key + "_m"].data(), StdEnergies, MData[key + "_std"].data());
		DrawTGraphErrors(g, p, key, (SymbParticles[p] + t).c_str(), x, y);
		firstPlot = false;

		mg->Add(g);
		legend->AddEntry(g, SymbParticles[p].c_str());
	}

	if(key == std::to_string(DThresholds[NThresholds - 1]) + "h1EDep_PerEvent_"
	+ SParticles[NParticles - 1]) lastPlot = true;

	DrawTMultiGraph(mg, legend, masterKey, t, x, y);
}

void DrawStackedBar(string masterKey, string t, string x, string y)
{
	string key;
	for(int p = 0; p < NParticles; p++)
	{
		key = masterKey + SParticles[p];
		THStack* hs = new THStack(key.c_str(), key.c_str());
		hs->SetTitle((SymbParticles[p] + t + ";" + x + ";" + y).c_str());
		for(int l = 0; l < SACNLayers; l++)
		{
			TH1D* temp = new TH1D((key + "_" + std::to_string(l)).c_str(),
				("z = " + std::to_string(l)).c_str(), NEnergies, BEnergies);
			for(int e = 0; e < NEnergies; e++) // bin width = 1
				temp->AddBinContent(e + 1, MData[key + std::to_string(l)][e]);
			temp->SetFillColor(StackColors[l]);
			hs->Add(temp);
		}

		TCanvas* c = new TCanvas(key.c_str(), key.c_str(), 800, 600);
		gPad->SetLogx(1);
		hs->Draw();
		gPad->BuildLegend(0.905, 0.2, 0.995, 0.8);

		string fname = Beam + "/" + Beam + FOutName;
		if(firstPlot) fname += "(";
		else if(lastPlot) fname += ")";

		c->Print(fname.c_str());
	}
}

void DrawCompare()
{
	TCanvas* c = new TCanvas("mult_all_vs_proton", "mult_all_vs_proton", 4000, 3000);
	c->Divide(5, 4);
	int plot_index = 1;

	for(int e = 0; e < NEnergies; e++)
	{
		TGraphErrors* g = new TGraphErrors(200,
			MData[SEnergies[e] + "compare_x"].data(),
			MData[SEnergies[e] + "compare_y_all"].data(),
			MData[SEnergies[e] + "compare_x_std"].data(),
			MData[SEnergies[e] + "compare_y_std_all"].data());
		g->SetTitle((SEnergies[e] + " total energy deposited").c_str());
		c->cd(plot_index);
		g->Draw();
		plot_index++;
	}

	for(int e = 0; e < NEnergies; e++)
	{
		TGraphErrors* g = new TGraphErrors(200,
			MData[SEnergies[e] + "compare_x"].data(),
			MData[SEnergies[e] + "compare_y_proton"].data(),
			MData[SEnergies[e] + "compare_x_std"].data(),
			MData[SEnergies[e] + "compare_y_std_proton"].data());
		g->SetTitle((SEnergies[e] + " proton energy deposited").c_str());
		c->cd(plot_index);
		g->Draw();
		plot_index++;
	}

	lastPlot = true;
	string fname = Beam + "/" + Beam + FOutName;
	if(firstPlot) fname += "(";
	else if(lastPlot) fname += ")";

	c->Print(fname.c_str());
}

// -------------------- SAC Analysis -------------------- //

void SACAnalysis(int BeamType, int NEvt)
{
	// Read in input files
	Beam = GetBeamType(BeamType);
	NEvents = NEvt;

	FName = Beam + "/SAC_" + std::to_string(BeamType) + "_";
	string temp;
	for(int e = 0; e < NEnergies; e++)
	{
		temp = FName + SEnergies[e] + "_n" + std::to_string(NEvents) + ".root";
		MFiles[SEnergies[e]] = new TFile(temp.c_str());
	}

	// Read in y values
	ReadInTxt("h1EDep_PerEvent_");
	ReadInMeanAndStd("h1Mult_PerEvent_");
	ReadInStackedBarVals("h1EDep_PerLayer_");
	CalcThresholds("h1EDep_PerEvent_");
	// ReadCompare("h1EDep_PerEvent_");

	// Close input files
	for(int e = 0; e < NEnergies; e++) MFiles[SEnergies[e]]->Close();

	// Draw graphs
	DrawGraphs("h1EDep_PerEvent_", " Fraction of Energy Deposition Per Event",
		"Incident Energy [GeV]", "Fraction");
	DrawGraphs("h1Mult_PerEvent_", " Multiplicity Per Event",
		"Incident Energy [GeV]", "[1 / GeV]");
	DrawStackedBar("h1EDep_PerLayer_",
		" Fraction of Energy Deposition Per SAC Layer",
		"Incident Energy [GeV]", "Fraction");
	for(double threshold : DThresholds)
		DrawGraphs(std::to_string(threshold) + "h1EDep_PerEvent_",
			 (" " + std::to_string(threshold) + " GeV Threshold").c_str(),
			 "Incident Energy [GeV]", "Fraction");
	// DrawCompare();
}

int main(int argc, char** argv)
{
	SACAnalysis(std::stoi(argv[0]), std::stoi(argv[1]));
	return 0;
}
