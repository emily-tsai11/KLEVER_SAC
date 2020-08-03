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
int numEnergies = 3;
string initPart = "gamma";
string n = "1000";

// energies
vector<double> energies_dbl {100.0, 200.0, 500.0, 1000.0, 2000.0, 5000.0,
	10000.0, 20000.0, 50000.0, 100000.0};
string energies_str[] = {"100MeV", "200MeV", "500MeV", "1GeV", "2GeV", "5GeV",
	"10GeV", "20GeV", "50GeV", "100GeV"};

// particle types
int numParticles = 11;
string partType[] = {"Gamma", "Positron", "Electron", "Proton", "Neutron",
	"PionPlus", "PionMinus", "PionZero", "MuonPlus", "MuonMinus", "OptPhot"};

// things to plot
int numAttr = 3;
string attr[] = {"Mult", "EDep", "InitE"};
string attr_title[] = {"multiplicity", "energy deposition", "initial energy"};

// TGraph drawing options
string x_axis = "incident energy (MeV)";
string draw_opt = "ap";
int colors[] = {2, 3, 4, 5, 6, 39, 38, 29, 28, 45, 7};
double mSize = 1.5;
int mStyle = 21;

// data structures
map<string, TFile*> mFiles;
map<string, vector<double>> mData;
map<string, TCanvas*> mCanvas;
map<string, TGraph*> mGraphs;

void SACAnalysis()
{
	// set to run in batch mode
	gROOT->SetBatch(1);

	// open input files
	for(int i = 0; i < numEnergies; i++)
		mFiles["f" + energies_str[i]] = new TFile(("n" + n + "_" + initPart + "/SAC_" + energies_str[i] + "_n" + n + "_" + initPart + ".root").c_str());
	{
		// fill x-axis arrays
		mData["x"] = energies_dbl;
		mData["x_stddev"] = vector<double>(mData["x"].size(), 0.0);

		// fit function
		TF1* gaussian = new TF1("gaussian", "[0] * exp(-0.5 * ((x - [1]) / [2]) * ((x - [1]) / [2]))", 0.0, 200.0);
		gaussian->SetParName(0, "normalization");
		gaussian->SetParName(1, "#mu");
		gaussian->SetParName(2, "#sigma");

		// fill attr
		for(int i = 0; i < numAttr; i++)
		{
			for(int j = 0; j < numParticles; j++)
			{
				mData["y_" + partType[j] + "_" + attr[i] + "_mean"] = vector<double>(numEnergies, 0.0);
				mData["y_" + partType[j] + "_" + attr[i] + "_stddev"] = vector<double>(numEnergies, 0.0);

				for(int k = 0; k < numEnergies; k++)
				{
					// CHANGE THIS TO GAUSSIAN FIT LATER
					TH1D* tempHist = (TH1D*) mFiles["f" + energies_str[k]]->Get(("h" + partType[j] + "_PerEvent_" + attr[i]).c_str());
					mData["y_" + partType[j] + "_" + attr[i] + "_mean"][k] = tempHist->GetMean();
					mData["y_" + partType[j] + "_" + attr[i] + "_stddev"][k] = tempHist->GetStdDev();
				}
			}
		}

		// fill total energy deposition
		mData["y_trackedE_mean"] = vector<double>(numEnergies, 0.0);
		mData["y_trackedE_stddev"] = vector<double>(numEnergies, 0.0);

		for(int i = 0; i < numEnergies; i++)
		{
			// CHANGE THIS TO GAUSSIAN FIT LATER
			TH1D* trackedETemp = (TH1D*) mFiles["f" + energies_str[i]]->Get("hPerEvent_EDep");
			mData["y_trackedE_mean"][i] = trackedETemp->GetMean() / energies_dbl[i];
			mData["y_trackedE_stddev"][i] = trackedETemp->GetStdDev() / energies_dbl[i];
		}

		// fill total untracked energy deposition
		mData["y_untrackedE_mean"] = vector<double>(numEnergies, 0.0);
		mData["y_untrackedE_stddev"] = vector<double>(numEnergies, 0.0);

		for(int i = 0; i < numEnergies; i++)
		{
			mData["y_untrackedE_mean"][i] = 1.0 - mData["y_trackedE_mean"][i];
			mData["y_untrackedE_stddev"][i] = mData["y_trackedE_stddev"][i];
		}

		// fill total untracked energy from other particles
		mData["y_untrackedEFromParticles_mean"] = vector<double>(numEnergies, 0.0);
		mData["y_untrackedEFromParticles_stddev"] = vector<double>(numEnergies, 0.0);

		for(int i = 0; i < numEnergies; i++)
		{
			// CHANGE THIS TO GAUSSIAN FIT LATER
			TH1D* UntrackedEFromParticlesTemp = (TH1D*) mFiles["f" + energies_str[i]]->Get("hPerEvent_UntrackedE");
			mData["y_untrackedEFromParticles_mean"][i] = UntrackedEFromParticlesTemp->GetMean() / energies_dbl[i];
			mData["y_untrackedEFromParticles_stddev"][i] = UntrackedEFromParticlesTemp->GetStdDev() / energies_dbl[i];
		}
	}
	for(int i = 0; i < numEnergies; i++) mFiles["f" + energies_str[i]]->Close();

	// open output file
	mFiles["fOut"] = new TFile(("SACAnalysis_n" + n + "_" + initPart + ".root").c_str(), "RECREATE");
	{
		// total energy deposition graph
		mCanvas["cTrackedE"] = new TCanvas("cTrackedE", "total energy deposition", 800, 600);
		{
			mGraphs["gTrackedE"] = new TGraphErrors(numEnergies,
				mData["x"].data(), mData["y_trackedE_mean"].data(),
				mData["x_stddev"].data(), mData["y_trackedE_stddev"].data());

			mGraphs["gTrackedE"]->SetName("gTrackedE");
			mGraphs["gTrackedE"]->SetTitle("total energy deposition");
			mGraphs["gTrackedE"]->GetXaxis()->SetTitle(x_axis.c_str());
			mGraphs["gTrackedE"]->GetYaxis()->SetTitle("energy deposition / incident energy (1 / MeV)");

			mGraphs["gTrackedE"]->SetMarkerColor(1);
			mGraphs["gTrackedE"]->SetMarkerSize(mSize);
			mGraphs["gTrackedE"]->SetMarkerStyle(mStyle);

			mGraphs["gTrackedE"]->Draw();
			mGraphs["gTrackedE"]->Write();
		}

		// total untracked energy deposition graph
		mCanvas["cUntrackedE"] = new TCanvas("cUntrackedE", "untracked energy deposition", 800, 600);
		{
			mGraphs["gUntrackedE"] = new TGraphErrors(numEnergies,
				mData["x"].data(), mData["y_untrackedE_mean"].data(),
				mData["x_stddev"].data(), mData["y_untrackedE_stddev"].data());

			mGraphs["gUntrackedE"]->SetName("gUntrackedE");
			mGraphs["gUntrackedE"]->SetTitle("total untracked energy deposition");
			mGraphs["gUntrackedE"]->GetXaxis()->SetTitle(x_axis.c_str());
			mGraphs["gUntrackedE"]->GetYaxis()->SetTitle("untracked energy deposition / incident energy (1 / MeV)");

			mGraphs["gUntrackedE"]->SetMarkerColor(9);
			mGraphs["gUntrackedE"]->SetMarkerSize(mSize);
			mGraphs["gUntrackedE"]->SetMarkerStyle(mStyle);

			mGraphs["gUntrackedE"]->Draw();
			mGraphs["gUntrackedE"]->Write();
		}

		// total untracked energy from other particles graph
		mCanvas["cUntrackedEFromParticles"] = new TCanvas("cUntrackedEFromParticles",
			"untracked energy deposition from other particles", 800, 600);
		{
			mGraphs["gUntrackedEFromParticles"] = new TGraphErrors(numEnergies,
				mData["x"].data(), mData["y_untrackedEFromParticles_mean"].data(),
				mData["x_stddev"].data(), mData["y_untrackedEFromParticles_stddev"].data());

			mGraphs["gUntrackedEFromParticles"]->SetName("gUntrackedEFromParticles");
			mGraphs["gUntrackedEFromParticles"]->SetTitle("untracked energy deposition from other particles");
			mGraphs["gUntrackedEFromParticles"]->GetXaxis()->SetTitle(x_axis.c_str());
			mGraphs["gUntrackedEFromParticles"]->GetYaxis()->SetTitle("untracked energy (MeV)");

			mGraphs["gUntrackedEFromParticles"]->SetMarkerColor(12);
			mGraphs["gUntrackedEFromParticles"]->SetMarkerSize(mSize);
			mGraphs["gUntrackedEFromParticles"]->SetMarkerStyle(mStyle);

			mGraphs["gUntrackedEFromParticles"]->Draw();
			mGraphs["gUntrackedEFromParticles"]->Write();
		}

		// attr graphs
		for(int i = 0; i < numAttr; i++)
		{
			mCanvas["c" + attr[i]] = new TCanvas(("c" + attr[i]).c_str(), (attr_title[i] + " per event").c_str(), 800, 600);
			TMultiGraph* mg = new TMultiGraph();

			mg->SetName(("mg" + attr[i]).c_str());
			mg->SetTitle((attr_title[i] + " per event").c_str());
			mg->GetXaxis()->SetTitle(x_axis.c_str());
			if(attr[i] == "Mult" || attr[i] == "EDep")
				mg->GetYaxis()->SetTitle((attr_title[i] + " per event / incident energy (1 / MeV)").c_str());
			else
				mg->GetYaxis()->SetTitle((attr_title[i] + " per event (MeV)").c_str());

			if(attr[i] == "EDep")
			{
				mg->Add(mGraphs["gTrackedE"]);
				mg->Add(mGraphs["gUntrackedE"]);
			}

			for(int j = 0; j < numParticles; j++)
			{
				mCanvas["c" + partType[j] + "_" + attr[i]] = new TCanvas(("c" + partType[j] + "_" + attr[i]).c_str(),
					(partType[j] + " " + attr_title[i]).c_str(), 800, 600);
				mGraphs["g" + partType[j] + "_" + attr[i]] = new TGraphErrors(numEnergies,
					mData["x"].data(), mData["y_" + partType[j] + "_" + attr[i] + "_mean"].data(),
					mData["x_stddev"].data(), mData["y_" + partType[j] + "_" + attr[i] + "_stddev"].data());

				mGraphs["g" + partType[j] + "_" + attr[i]]->SetName(("g" + partType[j] + "_" + attr[i]).c_str());
				mGraphs["g" + partType[j] + "_" + attr[i]]->SetTitle((partType[j] + " " + attr_title[i]).c_str());
				mGraphs["g" + partType[j] + "_" + attr[i]]->GetXaxis()->SetTitle(x_axis.c_str());
				if(attr[i] == "Mult" || attr[i] == "EDep")
					mGraphs["g" + partType[j] + "_" + attr[i]]->GetYaxis()->SetTitle((attr_title[i] + " per event / incident energy (1 / MeV)").c_str());
				else
					mGraphs["g" + partType[j] + "_" + attr[i]]->GetYaxis()->SetTitle((attr_title[i] + " per event (MeV)").c_str());

				mGraphs["g" + partType[j] + "_" + attr[i]]->SetMarkerColor(colors[j]);
				mGraphs["g" + partType[j] + "_" + attr[i]]->SetMarkerSize(mSize);
				mGraphs["g" + partType[j] + "_" + attr[i]]->SetMarkerStyle(mStyle);

				mGraphs["g" + partType[j] + "_" + attr[i]]->Draw();
				mGraphs["g" + partType[j] + "_" + attr[i]]->Write();

				mg->Add(mGraphs["g" + partType[j] + "_" + attr[i]]);
			}

			mg->Draw();
			mg->Write();
		}
	}
	mFiles["fOut"]->Close();

	// unset batch mode
	gROOT->SetBatch(0);
}
