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
const int numEnergies = 10;
string initPart = "gamma";
string n = "100000";

// energies
vector<double> energies_dbl {100.0, 200.0, 500.0, 1000.0, 2000.0, 5000.0,
	10000.0, 20000.0, 50000.0, 100000.0};
string energies_str[] = {"100MeV", "200MeV", "500MeV", "1GeV", "2GeV", "5GeV",
	"10GeV", "20GeV", "50GeV", "100GeV"};

// particle types
int numParticles = 11;
// string partType[] = {"Gamma", "Positron", "Electron", "Proton", "Neutron",
// 	"PionPlus", "PionMinus", "PionZero", "MuonPlus", "MuonMinus", "OptPhot"};
string partType[] = {"MuonPlus", "MuonMinus", "PionPlus", "PionMinus", "PionZero",
	"Neutron", "Proton", "Gamma", "Electron", "Positron", "OptPhot"};
string partType_symb[] = {"#mu^{+}", "#mu^{-}", "#pi^{+}", "#pi^{-}", "#pi^{0}",
	"n", "p", "#gamma", "e^{-}", "e^{+}", "Cer. #gamma"};

// things to plot
const int numAttr = 3;
string attr[] = {"EDep", "Mult", "InitE"};
string attr_title[] = {"energy deposition", "multiplicity", "initial energy"};

// optical photon mult gaussian fit ranges for incident photon events
double fitRangeMin[] = {82.0, 82.0, 82.0, 82.0, 80.0, 78.0, 78.0, 78.0, 75.0, 73.0};
double fitRangeMax[] = {95.0, 95.0, 95.0, 92.0, 90.0, 88.0, 87.0, 86.0, 85.0, 83.0};

// optical photon tracked energy gaussian fit ranges incident photon events
double fitTrackedEMin[] = {90.0, 180.0, 460.0, 940.0, 1900.0, 4700.0, 9300.0, 18000.0, 44000.0, 85000.0};
double fitTrackedEMax[] = {100.0, 200.0, 500.0, 990.0, 1960.0, 4850.0, 9600.0, 19000.0, 46000.0, 90000.0};

// optical photon untracked energy from particles gaussian fit ranges incident photon events
double fitUntrackedEFromParticlesMin[] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
double fitUntrackedEFromParticlesMax[] = {2.0, 2.0, 10.0, 10.0, 20.0, 50.0, 80.0, 120.0, 300.0, 300.0};

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
string draw_opt = "AP";
// int colors[] = {2, 3, 4, 5, 6, 39, 38, 29, 28, 45, 7};
int colors[] = {28, 45, 39, 38, 29, 6, 5, 2, 7, 3, 4};
double mSize = 1.3;
int mStyle = 8;
int lWidth = 1;
int lStyle = 1;
vector<double> zoomRangeGamma {-0.01, 0.18, -0.05, 1.0, -20.0, 560.0};
vector<double> zoomRangeNeutron {-0.02, 0.257, -0.02, 0.23, -100.0, 2400.0};

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
		mData["x_GeV"] = vector<double> {0.1, 0.2, 0.5, 1.0, 2.0, 5.0, 10.0, 20.0, 50.0, 100.0};
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
					if(initPart == "gamma" && partType[j] == "OptPhot" && attr[i] == "Mult")
					{
						// fit gaussian in range
						TF1* func = new TF1("func", "gaus", fitRangeMin[k], fitRangeMax[k]);
						tempHist->Fit("func", "RQ");
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

						mData["y_" + partType[j] + "_" + attr[i] + "_mean"][k] = m;
						mData["y_" + partType[j] + "_" + attr[i] + "_stddev"][k] = sd;
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
			TH1D* trackedETemp = (TH1D*) mFiles["f" + energies_str[i]]->Get("hPerEvent_EDep");
			double m;
			double sd;

			// fit gaussian in range if incident particle is photon
			if(initPart == "gamma")
			{
				TF1* funcTrackedE = new TF1("funcTrackedE", "gaus", fitTrackedEMin[i], fitTrackedEMax[i]);
				funcTrackedE->SetParLimits(1, fitTrackedEMin[i], fitTrackedEMax[i]);
				trackedETemp->Fit("funcTrackedE", "QR");

				TF1* fitTrackedE = trackedETemp->GetFunction("funcTrackedE");
				m = fitTrackedE->GetParameter(1);
				sd = fitTrackedE->GetParameter(2);
			}
			else if(initPart == "neutron")
			{
				m = trackedETemp->GetMean();
				sd = trackedETemp->GetStdDev();
			}

			mData["y_trackedE_mean"][i] = m / energies_dbl[i];
			mData["y_trackedE_stddev"][i] = sd / energies_dbl[i];
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
			TH1D* UntrackedEFromParticlesTemp = (TH1D*) mFiles["f" + energies_str[i]]->Get("hPerEvent_UntrackedE");
			double m;
			double sd;

			// fit gaussian in range if incident particle is photon
			TF1* funcUntrackedEFromParticles;
			if(initPart == "gamma")
			{
				funcUntrackedEFromParticles = new TF1("funcUntrackedEFromParticles", "gaus",
					fitUntrackedEFromParticlesMin[i], fitUntrackedEFromParticlesMax[i]);
				funcUntrackedEFromParticles->SetParLimits(1,
					fitUntrackedEFromParticlesMin[i], fitUntrackedEFromParticlesMax[i]);
				UntrackedEFromParticlesTemp->Fit("funcUntrackedEFromParticles", "QR");

				TF1* fitUntrackedEFromParticles = UntrackedEFromParticlesTemp->GetFunction("funcUntrackedEFromParticles");
				m = fitUntrackedEFromParticles->GetParameter(1);
				sd = fitUntrackedEFromParticles->GetParameter(2);
			}
			else if(initPart == "neutron")
			{
				m = UntrackedEFromParticlesTemp->GetMean();
				sd = UntrackedEFromParticlesTemp->GetStdDev();
			}

			mData["y_untrackedEFromParticles_mean"][i] = m / energies_dbl[i];
			mData["y_untrackedEFromParticles_stddev"][i] = sd / energies_dbl[i];
		}

		// optical photon inefficiency threshold plots
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
	mFiles["fOut"] = new TFile(("root_files/SACAnalysis_n" + n + "_" + initPart + ".root").c_str(), "RECREATE");
	{
		// remove title from plots
		gStyle->SetOptTitle(0);

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
			mGraphs["gTrackedE"]->SetLineWidth(lWidth);
			mGraphs["gTrackedE"]->SetLineStyle(lStyle);

			mGraphs["gTrackedE"]->Draw(draw_opt.c_str());
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
			mGraphs["gUntrackedE"]->SetLineWidth(lWidth);
			mGraphs["gUntrackedE"]->SetLineStyle(lStyle);

			mGraphs["gUntrackedE"]->Draw(draw_opt.c_str());
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
			mGraphs["gUntrackedEFromParticles"]->SetLineWidth(lWidth);
			mGraphs["gUntrackedEFromParticles"]->SetLineStyle(lStyle);

			mGraphs["gUntrackedEFromParticles"]->Draw(draw_opt.c_str());
			mGraphs["gUntrackedEFromParticles"]->Write();
		}

		// attr graphs
		for(int i = 0; i < numAttr; i++)
		{
			mCanvas["c" + attr[i]] = new TCanvas(("c" + attr[i]).c_str(),
				(attr_title[i] + " per event").c_str(), 800, 600);
			TMultiGraph* mg = new TMultiGraph();
			{
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
					{
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
						mGraphs["g" + partType[j] + "_" + attr[i]]->SetLineWidth(lWidth);
						mGraphs["g" + partType[j] + "_" + attr[i]]->SetLineStyle(lStyle);

						mGraphs["g" + partType[j] + "_" + attr[i]]->Draw(draw_opt.c_str());
						mGraphs["g" + partType[j] + "_" + attr[i]]->Write();
					}
					mg->Add(mGraphs["g" + partType[j] + "_" + attr[i]]);
				}

				mCanvas["c" + attr[i]]->cd();
				mg->Draw(draw_opt.c_str());
				mg->Write();
			}

			// create legend
			TLegend* attr_legend = new TLegend(0.905, 0.2, 0.995, 0.8);
			{
				attr_legend->SetFillStyle(0);
				attr_legend->SetBorderSize(0);
				if(attr[i] == "EDep")
				{
					attr_legend->AddEntry(mGraphs["gTrackedE"], "E_{total}");
					attr_legend->AddEntry(mGraphs["gUntrackedE"], "E_{untracked}");
				}
				for(int j = 0; j < numParticles; j++)
					attr_legend->AddEntry(mGraphs["g" + partType[j] + "_" + attr[i]], partType_symb[j].c_str());
			}

			// save plots
			gPad->SetLogx(1);
			attr_legend->Draw();
			mCanvas["c" + attr[i]]->SaveAs(("plots/" + initPart + "_mg" + attr[i] + ".pdf").c_str());

			// zoomed version of plots
			mCanvas["c" + attr[i] + "_zoomed"] = new TCanvas(("c" + attr[i] + "_zoomed").c_str(),
				(attr_title[i] + " per event").c_str(), 800, 600);
			{
				mg->Draw(draw_opt.c_str());

				// it matters what order Edep, Mult, and InitE are in!!!
				vector<double> zoomRange;
				if(initPart == "gamma") zoomRange = zoomRangeGamma;
				else if(initPart == "neutron") zoomRange = zoomRangeNeutron;
				switch(i)
				{
					case 0: mg->SetMinimum(zoomRange[0]); mg->SetMaximum(zoomRange[1]); break;	// EDep
					case 1: mg->SetMinimum(zoomRange[2]); mg->SetMaximum(zoomRange[3]); break;	// Mult
					case 2: mg->SetMinimum(zoomRange[4]); mg->SetMaximum(zoomRange[5]); break;	// InitE
					default: cout << "Hmm...this wasn't supposed to happen..." << endl;
				}
			}
			gPad->SetLogx(1);
			attr_legend->Draw();
			mCanvas["c" + attr[i] + "_zoomed"]->SaveAs(("plots/" + initPart + "_mg" + attr[i] + "_zoomed.pdf").c_str());
		}

		// optical photon mean -- stability
		mCanvas["cOptPhot_Mult_mean"] = new TCanvas("cOptPhot_Mult_mean",
			"Cerenkov photon multiplicity mean -- stability", 800, 600);
		{
			mGraphs["gOptPhot_Mult_mean"] = new TGraph(numEnergies,
				mData["x"].data(), mData["y_OptPhot_Mult_mean"].data());

			mGraphs["gOptPhot_Mult_mean"]->SetName("gOptPhot_Mult_mean");
			mGraphs["gOptPhot_Mult_mean"]->SetTitle("Cerenkov photon multiplicity mean -- stability");
			mGraphs["gOptPhot_Mult_mean"]->GetXaxis()->SetTitle(x_axis.c_str());
			mGraphs["gOptPhot_Mult_mean"]->GetYaxis()->SetTitle("Cerenkov photon multiplicity mean");

			mGraphs["gOptPhot_Mult_mean"]->SetMarkerColor(colors[10]);
			mGraphs["gOptPhot_Mult_mean"]->SetMarkerSize(mSize);
			mGraphs["gOptPhot_Mult_mean"]->SetMarkerStyle(mStyle);
			mGraphs["gOptPhot_Mult_mean"]->SetLineWidth(lWidth);
			mGraphs["gOptPhot_Mult_mean"]->SetLineStyle(lStyle);

			mGraphs["gOptPhot_Mult_mean"]->Draw(draw_opt.c_str());
			mGraphs["gOptPhot_Mult_mean"]->Write();
		}
		gPad->SetLogx(1);
		mCanvas["cOptPhot_Mult_mean"]->SaveAs(("plots/" + initPart + "_gOptPhot_Mult_mean.pdf").c_str());

		// optical photon stddev -- energy resolution
		mCanvas["cOptPhot_Mult_stddev"] = new TCanvas("cOptPhot_Mult_stddev",
			"Cerenkov photon multiplicity stddev -- stability", 800, 600);
		{
			mGraphs["gOptPhot_Mult_stddev"] = new TGraph(numEnergies,
				mData["x"].data(), mData["y_OptPhot_Mult_stddev"].data());

			mGraphs["gOptPhot_Mult_stddev"]->SetName("gOptPhot_Mult_stddev");
			mGraphs["gOptPhot_Mult_stddev"]->SetTitle("Cerenkov photon multiplicity stddev -- energy resolution");
			mGraphs["gOptPhot_Mult_stddev"]->GetXaxis()->SetTitle(x_axis.c_str());
			mGraphs["gOptPhot_Mult_stddev"]->GetYaxis()->SetTitle("Cerenkov photon multiplicity stddev");

			mGraphs["gOptPhot_Mult_stddev"]->SetMarkerColor(colors[10]);
			mGraphs["gOptPhot_Mult_stddev"]->SetMarkerSize(mSize);
			mGraphs["gOptPhot_Mult_stddev"]->SetMarkerStyle(mStyle);
			mGraphs["gOptPhot_Mult_stddev"]->SetLineWidth(lWidth);
			mGraphs["gOptPhot_Mult_stddev"]->SetLineStyle(lStyle);

			mGraphs["gOptPhot_Mult_stddev"]->Draw(draw_opt.c_str());
			mGraphs["gOptPhot_Mult_stddev"]->Write();
		}
		gPad->SetLogx(1);
		mCanvas["cOptPhot_Mult_stddev"]->SaveAs(("plots/" + initPart + "_gOptPhot_Mult_stddev.pdf").c_str());

		// optical photon stddev / mean
		mCanvas["cOptPhot_Mult_nStdDev_dMean"] = new TCanvas("cOptPhot_Mult_nStdDev_dMean",
			"optical photon multiplicity std / mean", 800, 600);
		{
			mGraphs["gOptPhot_Mult_nStdDev_dMean"] = new TGraphErrors(numEnergies,
				mData["x"].data(), mData["y_OptPhot_Mult_nStdDev_dMean_mean"].data(),
				mData["x_stddev"].data(), mData["y_OptPhot_Mult_nStdDev_dMean_stddev"].data());

			mGraphs["gOptPhot_Mult_nStdDev_dMean"]->SetName("gOptPhot_Mult_nStdDev_dMean");
			mGraphs["gOptPhot_Mult_nStdDev_dMean"]->SetTitle("Cerenkov photon multiplicity stddev / mean");
			mGraphs["gOptPhot_Mult_nStdDev_dMean"]->GetXaxis()->SetTitle(x_axis.c_str());
			mGraphs["gOptPhot_Mult_nStdDev_dMean"]->GetYaxis()->SetTitle("Cerenkov photon multiplicity stddev / mean");

			mGraphs["gOptPhot_Mult_nStdDev_dMean"]->SetMarkerColor(colors[10]);
			mGraphs["gOptPhot_Mult_nStdDev_dMean"]->SetMarkerSize(mSize);
			mGraphs["gOptPhot_Mult_nStdDev_dMean"]->SetMarkerStyle(mStyle);
			mGraphs["gOptPhot_Mult_nStdDev_dMean"]->SetLineWidth(lWidth);
			mGraphs["gOptPhot_Mult_nStdDev_dMean"]->SetLineStyle(lStyle);

			mGraphs["gOptPhot_Mult_nStdDev_dMean"]->Draw(draw_opt.c_str());
			mGraphs["gOptPhot_Mult_nStdDev_dMean"]->Write();
		}
		gPad->SetLogx(1);
		mCanvas["cOptPhot_Mult_nStdDev_dMean"]->SaveAs(("plots/" + initPart + "_gOptPhot_Mult_nStdDev_dMean.pdf").c_str());

		// threshold graphs
		for(int i = 0; i < numThresholds; i++)
		{
			mCanvas["cOptPhotIneff_t" + thresh_str[i]] = new TCanvas(("cOptPhotIneff_t" + thresh_str[i]).c_str(),
				("Cerenkov photon inefficiency, threshold = " + thresh_str[i]).c_str(), 800, 600);
			{
				mGraphs["gOptPhotIneff_t" + thresh_str[i]] = new TGraph(numEnergies,
					mData["x_GeV"].data(), mData["OptPhotIneff_t" + thresh_str[i]].data());

				mGraphs["gOptPhotIneff_t" + thresh_str[i]]->SetName(("gOptPhotIneff_t" + thresh_str[i]).c_str());
				mGraphs["gOptPhotIneff_t" + thresh_str[i]]->SetTitle(("Cerenkov photon inefficiency, threshold = " + thresh_str[i]).c_str());
				mGraphs["gOptPhotIneff_t" + thresh_str[i]]->GetXaxis()->SetTitle("incident energy (GeV)");
				mGraphs["gOptPhotIneff_t" + thresh_str[i]]->GetYaxis()->SetTitle("Cerenkov photon inefficiency");

				mGraphs["gOptPhotIneff_t" + thresh_str[i]]->SetMarkerColor(colors[10]);
				mGraphs["gOptPhotIneff_t" + thresh_str[i]]->SetMarkerSize(mSize);
				mGraphs["gOptPhotIneff_t" + thresh_str[i]]->SetMarkerStyle(mStyle);
				mGraphs["gOptPhotIneff_t" + thresh_str[i]]->SetLineWidth(lWidth);
				mGraphs["gOptPhotIneff_t" + thresh_str[i]]->SetLineStyle(lStyle);

				mGraphs["gOptPhotIneff_t" + thresh_str[i]]->Draw(draw_opt.c_str());
				mGraphs["gOptPhotIneff_t" + thresh_str[i]]->Write();
			}
			gPad->SetLogx(1);
			gPad->SetLogy(1);
			mCanvas["cOptPhotIneff_t" + thresh_str[i]]->SaveAs(("plots/" + initPart + "_OptPhot_threshold" + thresh_str[i] + ".pdf").c_str());
		}
	}
	mFiles["fOut"]->Close();

	// make other plots
	gStyle->SetOptStat("mr");

	for(int i = 0; i < numEnergies; i++)
	{
		TFile* fGamma = new TFile(("n100000_gamma/SAC_" + energies_str[i] + "_gamma_n100000.root").c_str());
		TH1D* hGammaTemp = (TH1D*) (fGamma->Get("hOptPhot_PerEvent_Mult"));
		TCanvas* cGamma = new TCanvas(("cGamma_" + energies_str[i]).c_str(), "Cerenkov photon multiplicity per event", 800, 600);
		TH1D* hGamma = new TH1D("hGamma", "Cerenkov photon multiplicity per event", 500, 0, 20000);
		for(int j = 0; j < hGammaTemp->GetNbinsX(); j++)
			hGamma->Fill(j * 40.0, hGammaTemp->GetBinContent(j));
		hGamma->GetXaxis()->SetTitle("number of Cerenkov photons");
		hGamma->SetLineColor(4);
		hGamma->Draw("hist");
		cGamma->SaveAs(("plots/Gamma" + energies_str[i] + ".pdf").c_str());
		fGamma->Close();
	}

	for(int i = 0; i < numEnergies; i++)
	{
		TFile* fNeutron = new TFile(("n100000_neutron/SAC_" + energies_str[i] + "_neutron_n100000.root").c_str());
		TH1D* hNeutronTemp = (TH1D*) (fNeutron->Get("hOptPhot_PerEvent_Mult"));
		TCanvas* cNeutron = new TCanvas(("cNeutron_" + energies_str[i]).c_str(), "Cerenkov photon multiplicity per event", 800, 600);
		TH1D* hNeutron = new TH1D("hNeutron", "Cerenkov photon multiplicity per event", 500, 0, 20000);
		for(int j = 0; j < hNeutronTemp->GetNbinsX(); j++)
			hNeutron->Fill(j * 40.0, hNeutronTemp->GetBinContent(j));
		hNeutron->GetXaxis()->SetTitle("number of Cerenkov photons");
		hNeutron->SetLineColor(4);
		hNeutron->Draw("hist");
		cNeutron->SaveAs(("plots/Neutron" + energies_str[i] + ".pdf").c_str());
		fNeutron->Close();
	}

	// unset batch mode
	gROOT->SetBatch(0);
}
