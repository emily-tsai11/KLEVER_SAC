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
int numEnergies = 10;
string initPart = "gamma";
string n = "100000";

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
string attr[] = {"EDep", "Mult", "InitE"};
string attr_title[] = {"energy deposition", "multiplicity", "initial energy"};

// optical photon mult gaussian fit ranges
double fitRangeMin[] = {82.0, 82.0, 82.0, 82.0, 80.0, 78.0, 78.0, 78.0, 75.0, 73.0};
double fitRangeMax[] = {95.0, 95.0, 95.0, 92.0, 90.0, 88.0, 87.0, 86.0, 85.0, 83.0};

// optical photon efficiency thresholds in MeV
int numThresholds = 20;
double bin_width = 10.0; // in MeV
double thresholds[] = {50.0, 100.0, 150.0, 200.0, 450.0, 1000.0, 2000.0,
	3000.0, 4000.0, 5000.0, 6000.0, 7000.0, 8000.0, 9000.0, 10000.0, 11000.0,
	12000.0, 13000.0, 14000.0, 15000.0};
string thresh_str[] = {"50MeV", "100MeV", "150MeV", "200MeV", "450MeV", "1GeV",
	"2GeV", "3GeV", "4GeV", "5GeV", "6GeV", "7GeV", "8GeV", "9GeV", "10GeV",
	"11GeV", "12GeV", "13GeV", "14GeV", "15GeV"};

// TGraph drawing options
string x_axis = "incident energy (MeV)";
string draw_opt = "ap";
int colors[] = {2, 3, 4, 5, 6, 7, 38, 29, 28, 45, 39};
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
		mFiles["f" + energies_str[i]] = new TFile(("n" + n + "_" + initPart + "/SAC_" + energies_str[i] + "_" + initPart + "_n" + n + ".root").c_str());
	{
		// fill x-axis arrays
		mData["x"] = energies_dbl;
		mData["x_stddev"] = vector<double>(mData["x"].size(), 0.0);

		// optical photon stddev / mean energy resolution
		mData["y_OptPhot_Mult_nStdDev_dMean_mean"] = vector<double>(numEnergies, 0.0);
		mData["y_OptPhot_Mult_nStdDev_dMean_stddev"] = vector<double>(numEnergies, 0.0);

		// fill attr
		for(int i = 0; i < numAttr; i++)
		{
			for(int j = 0; j < numParticles; j++)
			{
				mData["y_" + partType[j] + "_" + attr[i] + "_mean"] = vector<double>(numEnergies, 0.0);
				mData["y_" + partType[j] + "_" + attr[i] + "_stddev"] = vector<double>(numEnergies, 0.0);

				for(int k = 0; k < numEnergies; k++)
				{
					TH1D* tempHist = (TH1D*) mFiles["f" + energies_str[k]]->Get(("h" + partType[j] + "_PerEvent_" + attr[i]).c_str());

					double m;
					double sd;
					if(partType[j] == "OptPhot" && attr[i] == "Mult")
					{
						// fit gaussian in range
						TF1* func = new TF1("func", "gaus", fitRangeMin[k], fitRangeMax[k]);
						tempHist->Fit("func", "R");
						TF1* fit = tempHist->GetFunction("func");

						// fill optical photon stddev / mean energy resolution
						m = fit->GetParameter(1);
						sd = fit->GetParameter(2);
						double dm = fit->GetParError(1);
						double dsd = fit->GetParError(2);

						double m_new = sd / m;
						double a = dm / m;
						double b = dsd / sd;
						double sd_new = m_new * sqrt(a * a + b * b);

						mData["y_OptPhot_Mult_nStdDev_dMean_mean"][k] = m_new;
						mData["y_OptPhot_Mult_nStdDev_dMean_stddev"][k] = sd_new;
					}
					else
					{
						// or just pull mean and stddev from root histograms
						m = tempHist->GetMean();
						sd = tempHist->GetStdDev();
					}

					mData["y_" + partType[j] + "_" + attr[i] + "_mean"][k] = m;
					mData["y_" + partType[j] + "_" + attr[i] + "_stddev"][k] = sd;
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

		// optical photon efficiency threshold plots
		for(int i = 0; i < numThresholds; i++)
		{
			mData["OptPhotIneff_t" + thresh_str[i]] = vector<double>(numEnergies);
			for(int j = 0; j < numEnergies; j++)
			{
				TH1D* OptPhotMultTemp = (TH1D*) mFiles["f" + energies_str[j]]->Get("hOptPhot_PerEvent_lowMult");
				double integral = OptPhotMultTemp->Integral(0, (int) (thresholds[i] / bin_width));
				mData["OptPhotIneff_t" + thresh_str[i]][j] = integral / stoi(n);
			}
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

		// optical photon stddev / mean energy resolution
		mCanvas["cOptPhot_Mult_nStdDev_dMean"] = new TCanvas("cOptPhot_Mult_nStdDev_dMean",
			"optical photon multiplicity std / mean", 800, 600);
		mGraphs["gOptPhot_Mult_nStdDev_dMean"] = new TGraphErrors(numEnergies,
			mData["x"].data(), mData["y_OptPhot_Mult_nStdDev_dMean_mean"].data(),
			mData["x_stddev"].data(), mData["y_OptPhot_Mult_nStdDev_dMean_stddev"].data());

		mGraphs["gOptPhot_Mult_nStdDev_dMean"]->SetName("gOptPhot_Mult_nStdDev_dMean");
		mGraphs["gOptPhot_Mult_nStdDev_dMean"]->SetTitle("optical photon multiplicity std / mean");
		mGraphs["gOptPhot_Mult_nStdDev_dMean"]->GetXaxis()->SetTitle(x_axis.c_str());
		mGraphs["gOptPhot_Mult_nStdDev_dMean"]->GetYaxis()->SetTitle("optical photon multiplicity std / mean");

		mGraphs["gOptPhot_Mult_nStdDev_dMean"]->SetMarkerColor(colors[10]);
		mGraphs["gOptPhot_Mult_nStdDev_dMean"]->SetMarkerSize(mSize);
		mGraphs["gOptPhot_Mult_nStdDev_dMean"]->SetMarkerStyle(mStyle);

		mGraphs["gOptPhot_Mult_nStdDev_dMean"]->Draw();
		mGraphs["gOptPhot_Mult_nStdDev_dMean"]->Write();

		// threshold graphs
		for(int i = 0; i < numThresholds; i++)
		{
			mCanvas["cOptPhotIneff_t" + thresh_str[i]] = new TCanvas(("cOptPhotIneff_t" + thresh_str[i]).c_str(),
				("optical photon inefficiency, threshold = " + thresh_str[i]).c_str(), 800, 600);
			mGraphs["gOptPhotIneff_t" + thresh_str[i]] = new TGraph(numEnergies,
				mData["x"].data(), mData["OptPhotIneff_t" + thresh_str[i]].data());

			mGraphs["gOptPhotIneff_t" + thresh_str[i]]->SetName(("gOptPhotIneff_t" + thresh_str[i]).c_str());
			mGraphs["gOptPhotIneff_t" + thresh_str[i]]->SetTitle(("Cerenkov photon inefficiency, threshold = " + thresh_str[i]).c_str());
			mGraphs["gOptPhotIneff_t" + thresh_str[i]]->GetXaxis()->SetTitle(x_axis.c_str());
			mGraphs["gOptPhotIneff_t" + thresh_str[i]]->GetYaxis()->SetTitle("Cerenkov photon inefficiency");

			mGraphs["gOptPhotIneff_t" + thresh_str[i]]->SetMarkerColor(colors[10]);
			mGraphs["gOptPhotIneff_t" + thresh_str[i]]->SetMarkerSize(mSize);
			mGraphs["gOptPhotIneff_t" + thresh_str[i]]->SetMarkerStyle(mStyle);

			mGraphs["gOptPhotIneff_t" + thresh_str[i]]->Draw();
			mGraphs["gOptPhotIneff_t" + thresh_str[i]]->Write();

			if(thresh_str[i] == "150MeV" || thresh_str[i] == "5GeV" || thresh_str[i] == "10GeV" || thresh_str[i] == "15GeV")
			{
				mData["x_GeV"] = vector<double> {0.1, 0.2, 0.5, 1.0, 2.0, 5.0, 10.0, 20.0, 50.0, 100.0};
				mCanvas["temp" + thresh_str[i]] = new TCanvas(("temp" + thresh_str[i]).c_str(),
					("Cerenkov photon inefficiency, threshold = " + thresh_str[i]).c_str(), 800, 600);
				mGraphs["temp" + thresh_str[i]] = new TGraph(numEnergies,
					mData["x_GeV"].data(), mData["OptPhotIneff_t" + thresh_str[i]].data());

				mGraphs["temp" + thresh_str[i]]->SetTitle(("Cerenkov photon inefficiency, threshold = " + thresh_str[i]).c_str());
				mGraphs["temp" + thresh_str[i]]->GetXaxis()->SetTitle("incident energy (GeV)");
				mGraphs["temp" + thresh_str[i]]->GetYaxis()->SetTitle("Cerenkov photon inefficiency");

				mGraphs["temp" + thresh_str[i]]->SetMarkerColor(4);
				mGraphs["temp" + thresh_str[i]]->SetMarkerSize(0.9);
				mGraphs["temp" + thresh_str[i]]->SetMarkerStyle(mStyle);
				mGraphs["temp" + thresh_str[i]]->Draw();

				gPad->SetLogx(1);
				gPad->SetLogy(1);
				mCanvas["temp" + thresh_str[i]]->SaveAs(("OptPhot_threshold" + thresh_str[i] + ".pdf").c_str());
				gPad->SetLogx(0);
				gPad->SetLogy(0);
			}
		}
	}
	mFiles["fOut"]->Close();

	// make other plots
	gStyle->SetOptStat("mr");

	TFile* f200MeV = new TFile("n100000_gamma/SAC_200MeV_gamma_n100000.root");
	TCanvas* c200MeV = new TCanvas("c200MeV", "Cerenkov photon multiplicity per event / incident energy", 800, 600);
	TH1D* h200MeV = (TH1D*) ((f200MeV->Get("hOptPhot_PerEvent_Mult"))->Clone());
	h200MeV->SetTitle("Cerenkov photon multiplicity per event / incident energy");
	h200MeV->GetXaxis()->SetTitle("incident energy (MeV)");
	h200MeV->SetLineColor(4);
	h200MeV->Draw("hist");
	c200MeV->SaveAs("OptPhot_Mult_200MeV_n100000.pdf");
	f200MeV->Close();

	TFile* f20GeV = new TFile("n100000_gamma/SAC_20GeV_gamma_n100000.root");
	TCanvas* c20GeV = new TCanvas("c20GeV", "Cerenkov photon multiplicity per event / incident energy", 800, 600);
	TH1D* h20GeV = (TH1D*) ((f20GeV->Get("hOptPhot_PerEvent_Mult"))->Clone());
	h20GeV->SetTitle("Cerenkov photon multiplicity per event / incident energy");
	h20GeV->GetXaxis()->SetTitle("incident energy (MeV)");
	h20GeV->SetLineColor(4);
	h20GeV->Draw("hist");
	c20GeV->SaveAs("OptPhot_Mult_20GeV_n100000.pdf");
	f20GeV->Close();


	TFile* fGamma = new TFile("n100000_gamma/SAC_100MeV_gamma_n100000.root");
	TH1D* hGammaTemp = (TH1D*) (fGamma->Get("hOptPhot_PerEvent_Mult"));
	TCanvas* cGamma = new TCanvas("cGamma", "Cerenkov photon multiplicity per event", 800, 600);
	TH1D* hGamma = new TH1D("hGamma", "Cerenkov photon multiplicity per event", 500, 0, 20000);
	for(int i = 0; i < hGammaTemp->GetNbinsX(); i++)
		hGamma->Fill(i * 40.0, hGammaTemp->GetBinContent(i));
	hGamma->GetXaxis()->SetTitle("incident energy (MeV)");
	hGamma->SetLineColor(4);
	hGamma->Draw("hist");
	TLine* lGamma = new TLine(2000, 0, 2000, 4630);
	lGamma->SetLineColor(2);
	lGamma->Draw();
	cGamma->SaveAs("OptPhot_Mult_Gamma.pdf");
	fGamma->Close();

	TFile* fNeutron = new TFile("n100000_neutron/SAC_100MeV_neutron_n100000.root");
	TH1D* hNeutronTemp = (TH1D*) (fNeutron->Get("hOptPhot_PerEvent_Mult"));
	TCanvas* cNeutron = new TCanvas("cNeutron", "Cerenkov photon multiplicity per event", 800, 600);
	TH1D* hNeutron = new TH1D("hNeutron", "Cerenkov photon multiplicity per event", 500, 0, 20000);
	for(int i = 0; i < hNeutronTemp->GetNbinsX(); i++)
		hNeutron->Fill(i * 40.0, hNeutronTemp->GetBinContent(i));
	hNeutron->GetXaxis()->SetTitle("incident energy (MeV)");
	hNeutron->SetLineColor(4);
	hNeutron->Draw("hist");
	TLine* lNeutron = new TLine(2000, 0, 2000, 3110);
	lNeutron->SetLineColor(2);
	lNeutron->Draw();
	cNeutron->SaveAs("OptPhot_Mult_Neutron.pdf");
	fNeutron->Close();

	// unset batch mode
	gROOT->SetBatch(0);
}
