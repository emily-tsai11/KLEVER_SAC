#include <map>
#include <string>

void SACAna_time()
{
	// set to run in batch mode
	gROOT->SetBatch(1);

	// open input file
	TFile* file_g = new TFile("../build/SAC_100MeV_gamma_n100.root");
	TFile* file_n = new TFile("../build/SAC_100MeV_neutron_n100.root");

	// estimated time correction (ns)
	double correction = 3.08413;

	// get input data
	int sum = 0;
	map<string, TH1D*> mData;
	for(int i = 0; i < 4; i++)
	{
		for(int cell = 0; cell < 100; cell++)
		{
			string x = std::to_string(cell / 10);
			string y = std::to_string(cell % 10);
			string z = std::to_string(i);
			mData[z + y + x + "g"] = (TH1D*) file_g->Get(("h" + z + "_" + y + x + "_arrTime").c_str());
			mData[z + y + x + "n"] = (TH1D*) file_n->Get(("h" + z + "_" + y + x + "_arrTime").c_str());
		}
	}

	// plot time cutoff vs. # of Cer. photons captured
	Int_t n = 1000;

	// gamma
	TCanvas* c_g = new TCanvas("c_g", "c_g", 800.0, 600.0);
	Double_t x_g[1000], y_g[1000];
	for(int i = 0; i < 1000; i++)
	{
		double sum = 0;
		for(int j = 0; j < 1; j++) // set which layers
		{
			for(int k = 0; k < 100; k++)
			{
				string x = std::to_string(k / 10);
				string y = std::to_string(k % 10);
				string z = std::to_string(j);
				sum += ((double) mData[z + y + x + "g"]->Integral(i, 1000)) / 100.0 / 100.0;
			}
		}
		x_g[i] = ((double) i) / 100.0;
		y_g[i] = sum;
	}
	TGraph* graph_g = new TGraph(n, x_g, y_g);
	graph_g->SetTitle("time cutoff (ns) vs. # of Cer. photons counted per MeV (gamma, last layer)");
	graph_g->Draw();
	c_g->SaveAs("timecutoff_vs_cerphot_gamma_LAST.png");

	// neutron
	TCanvas* c_n = new TCanvas("c_n", "c_n", 800.0, 600.0);
	Double_t x_n[1000], y_n[1000];
	for(int i = 0; i < 1000; i++)
	{
		double sum = 0;
		for(int j = 0; j < 1; j++) // set which layers
		{
			for(int k = 0; k < 100; k++)
			{
				string x = std::to_string(k / 10);
				string y = std::to_string(k % 10);
				string z = std::to_string(j);
				sum += ((double) mData[z + y + x + "n"]->Integral(i, 1000)) / 100.0 / 100.0;
			}
		}
		x_n[i] = ((double) i) / 100.0;
		y_n[i] = sum;
	}
	TGraph* graph_n = new TGraph(n, x_n, y_n);
	graph_n->SetTitle("time cutoff (ns) vs. # of Cer. photons counted per MeV (neutron, last layer)");
	graph_n->Draw();
	c_n->SaveAs("timecutoff_vs_cerphot_neutron_LAST.png");

	// both
	TCanvas* c_gn = new TCanvas("c_gn", "c_gn", 800.0, 600.0);
	Double_t x_gn[1000], y_gn[1000];
	for(int i = 0; i < 1000; i++)
	{
		double sum = 0;
		for(int j = 0; j < 1; j++) // set which layers
		{
			for(int k = 0; k < 100; k++)
			{
				string x = std::to_string(k / 10);
				string y = std::to_string(k % 10);
				string z = std::to_string(j);
				sum += ((double) mData[z + y + x + "g"]->Integral(i, 1000)) / 100.0 / 100.0;
				sum += ((double) mData[z + y + x + "n"]->Integral(i, 1000)) / 100.0 / 100.0;
			}
		}
		x_gn[i] = ((double) i) / 100.0;
		y_gn[i] = sum;
	}
	TGraph* graph_gn = new TGraph(n, x_gn, y_gn);
	graph_gn->SetTitle("time cutoff (ns) vs. # of Cer. photons counted per MeV (gamma & neutron, last layer)");
	graph_gn->Draw();
	c_gn->SaveAs("timecutoff_vs_cerphot_gn_LAST.png");

	// unset batch mode
	gROOT->SetBatch(0);
}
