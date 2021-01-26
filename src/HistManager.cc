// HistManager.cc
// --------------------------------------------------------------
// History:
//
// Created by Emily Tsai (emily.tsai11@gmail.com) 2020-1-24
// --------------------------------------------------------------

#include "HistManager.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HistManager* HistManager::fInstance = 0;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HistManager* HistManager::GetInstance()
{
	if(fInstance == 0) fInstance = new HistManager();
	return fInstance;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HistManager::HistManager()
{
	const G4int numHist = 98;
	// SAC specs probably shouldn't be hard coded, but they are sadly
	// have to change code to accomodate for different number of SAC rows/cols
	const G4int SACRows = 10;
	const G4int SACCols = 10;
	const G4int SACLayers = 4;

	// fMessenger = new HistManagerMessenger(); // DON'T SET DEFAULTS IN MESSENGER

	// root save file name
	fFileName = "SACRun";
	fNBins = 100;

	// map particle names
	// TODO

	// map histogram names -- FIX THE GENERATION
	G4String histNames[] = {
		"hEDep_PerEvent_all",
		"hEDep_PerEvent_e-",
		"hEDep_PerEvent_gamma",
		"hEDep_PerEvent_mu-",
		"hEDep_PerEvent_mu+",
		"hEDep_PerEvent_neutron",
		"hEDep_PerEvent_opticalphoton",
		"hEDep_PerEvent_other",
		"hEDep_PerEvent_pi-",
		"hEDep_PerEvent_pi+",
		"hEDep_PerEvent_pi0",
		"hEDep_PerEvent_e+",
		"hEDep_PerEvent_proton",
		"hEDep_PerEvent_untracked",

		"hEDep_PerEvent_all_norm",
		"hEDep_PerEvent_e-_norm",
		"hEDep_PerEvent_gamma_norm",
		"hEDep_PerEvent_mu-_norm",
		"hEDep_PerEvent_mu+_norm",
		"hEDep_PerEvent_neutron_norm",
		"hEDep_PerEvent_opticalphoton_norm",
		"hEDep_PerEvent_other_norm",
		"hEDep_PerEvent_pi-_norm",
		"hEDep_PerEvent_pi+_norm",
		"hEDep_PerEvent_pi0_norm",
		"hEDep_PerEvent_e+_norm",
		"hEDep_PerEvent_proton_norm",
		"hEDep_PerEvent_untracked_norm",

		"hMult_PerEvent_all",
		"hMult_PerEvent_e-",
		"hMult_PerEvent_gamma",
		"hMult_PerEvent_mu-",
		"hMult_PerEvent_mu+",
		"hMult_PerEvent_neutron",
		"hMult_PerEvent_opticalphoton",
		"hMult_PerEvent_other",
		"hMult_PerEvent_pi-",
		"hMult_PerEvent_pi+",
		"hMult_PerEvent_pi0",
		"hMult_PerEvent_e+",
		"hMult_PerEvent_proton",
		"hMult_PerEvent_untracked",

		"hMult_PerEvent_all_low",
		"hMult_PerEvent_e-_low",
		"hMult_PerEvent_gamma_low",
		"hMult_PerEvent_mu-_low",
		"hMult_PerEvent_mu+_low",
		"hMult_PerEvent_neutron_low",
		"hMult_PerEvent_opticalphoton_low",
		"hMult_PerEvent_other_low",
		"hMult_PerEvent_pi-_low",
		"hMult_PerEvent_pi+_low",
		"hMult_PerEvent_pi0_low",
		"hMult_PerEvent_e+_low",
		"hMult_PerEvent_proton_low",
		"hMult_PerEvent_untracked_low",

		"hMult_PerLayer_all",
		"hMult_PerLayer_e-",
		"hMult_PerLayer_gamma",
		"hMult_PerLayer_mu-",
		"hMult_PerLayer_mu+",
		"hMult_PerLayer_neutron",
		"hMult_PerLayer_opticalphoton",
		"hMult_PerLayer_other",
		"hMult_PerLayer_pi-",
		"hMult_PerLayer_pi+",
		"hMult_PerLayer_pi0",
		"hMult_PerLayer_e+",
		"hMult_PerLayer_proton",
		"hMult_PerLayer_untracked",

		"hMult_PerLayer_all_xy",
		"hMult_PerLayer_e-_xy",
		"hMult_PerLayer_gamma_xy",
		"hMult_PerLayer_mu-_xy",
		"hMult_PerLayer_mu+_xy",
		"hMult_PerLayer_neutron_xy",
		"hMult_PerLayer_opticalphoton_xy",
		"hMult_PerLayer_other_xy",
		"hMult_PerLayer_pi-_xy",
		"hMult_PerLayer_pi+_xy",
		"hMult_PerLayer_pi0_xy",
		"hMult_PerLayer_e+_xy",
		"hMult_PerLayer_proton_xy",
		"hMult_PerLayer_untracked_xy",

		"hMult_SAC_all",
		"hMult_SAC_e-",
		"hMult_SAC_gamma",
		"hMult_SAC_mu-",
		"hMult_SAC_mu+",
		"hMult_SAC_neutron",
		"hMult_SAC_opticalphoton",
		"hMult_SAC_other",
		"hMult_SAC_pi-",
		"hMult_SAC_pi+",
		"hMult_SAC_pi0",
		"hMult_SAC_e+",
		"hMult_SAC_proton",
		"hMult_SAC_untracked",

		"hTrLen_PerHit_all",
		"hTrLen_PerHit_e-",
		"hTrLen_PerHit_gamma",
		"hTrLen_PerHit_mu-",
		"hTrLen_PerHit_mu+",
		"hTrLen_PerHit_neutron",
		"hTrLen_PerHit_opticalphoton",
		"hTrLen_PerHit_other",
		"hTrLen_PerHit_pi-",
		"hTrLen_PerHit_pi+",
		"hTrLen_PerHit_pi0",
		"hTrLen_PerHit_e+",
		"hTrLen_PerHit_proton",
		"hTrLen_PerHit_untracked"
	};
	for(G4int i = 0; i < numHist; i++) fHistogramNames.insert({histNames[i], i});

	// map histogram bounds
	fHistogramBounds.insert({"hEDep_PerEvent_all", std::make_pair(0.0 * MeV, 700.0 * MeV)});
	fHistogramBounds.insert({"hEDep_PerEvent_e-", std::make_pair(0.0 * MeV, 700.0 * MeV)});
	fHistogramBounds.insert({"hEDep_PerEvent_gamma", std::make_pair(0.0 * MeV, 700.0 * MeV)});
	fHistogramBounds.insert({"hEDep_PerEvent_mu-", std::make_pair(0.0 * MeV, 700.0 * MeV)});
	fHistogramBounds.insert({"hEDep_PerEvent_mu+", std::make_pair(0.0 * MeV, 700.0 * MeV)});
	fHistogramBounds.insert({"hEDep_PerEvent_neutron", std::make_pair(0.0 * MeV, 700.0 * MeV)});
	fHistogramBounds.insert({"hEDep_PerEvent_opticalphoton", std::make_pair(0.0 * MeV, 7.0 * MeV)});
	fHistogramBounds.insert({"hEDep_PerEvent_other", std::make_pair(0.0 * MeV, 700.0 * MeV)});
	fHistogramBounds.insert({"hEDep_PerEvent_pi-", std::make_pair(0.0 * MeV, 700.0 * MeV)});
	fHistogramBounds.insert({"hEDep_PerEvent_pi+", std::make_pair(0.0 * MeV, 700.0 * MeV)});
	fHistogramBounds.insert({"hEDep_PerEvent_pi0", std::make_pair(0.0 * MeV, 700.0 * MeV)});
	fHistogramBounds.insert({"hEDep_PerEvent_e+", std::make_pair(0.0 * MeV, 700.0 * MeV)});
	fHistogramBounds.insert({"hEDep_PerEvent_proton", std::make_pair(0.0 * MeV, 700.0 * MeV)});
	fHistogramBounds.insert({"hEDep_PerEvent_untracked", std::make_pair(0.0 * MeV, 700.0 * MeV)});

	fHistogramBounds.insert({"hEDep_PerEvent_all_norm", std::make_pair(0.0, 1.0)});
	fHistogramBounds.insert({"hEDep_PerEvent_e-_norm", std::make_pair(0.0, 1.0)});
	fHistogramBounds.insert({"hEDep_PerEvent_gamma_norm", std::make_pair(0.0, 1.0)});
	fHistogramBounds.insert({"hEDep_PerEvent_mu-_norm", std::make_pair(0.0, 1.0)});
	fHistogramBounds.insert({"hEDep_PerEvent_mu+_norm", std::make_pair(0.0, 1.0)});
	fHistogramBounds.insert({"hEDep_PerEvent_neutron_norm", std::make_pair(0.0, 1.0)});
	fHistogramBounds.insert({"hEDep_PerEvent_opticalphoton_norm", std::make_pair(0.0, 1.0)});
	fHistogramBounds.insert({"hEDep_PerEvent_other_norm", std::make_pair(0.0, 1.0)});
	fHistogramBounds.insert({"hEDep_PerEvent_pi-_norm", std::make_pair(0.0, 1.0)});
	fHistogramBounds.insert({"hEDep_PerEvent_pi+_norm", std::make_pair(0.0, 1.0)});
	fHistogramBounds.insert({"hEDep_PerEvent_pi0_norm", std::make_pair(0.0, 1.0)});
	fHistogramBounds.insert({"hEDep_PerEvent_e+_norm", std::make_pair(0.0, 1.0)});
	fHistogramBounds.insert({"hEDep_PerEvent_proton_norm", std::make_pair(0.0, 1.0)});
	fHistogramBounds.insert({"hEDep_PerEvent_untracked_norm", std::make_pair(0.0, 1.0)});

	fHistogramBounds.insert({"hMult_PerEvent_all", std::make_pair(0.0, 10000.0)});
	fHistogramBounds.insert({"hMult_PerEvent_e-", std::make_pair(0.0, 10000.0)});
	fHistogramBounds.insert({"hMult_PerEvent_gamma", std::make_pair(0.0, 10000.0)});
	fHistogramBounds.insert({"hMult_PerEvent_mu-", std::make_pair(0.0, 10000.0)});
	fHistogramBounds.insert({"hMult_PerEvent_mu+", std::make_pair(0.0, 10000.0)});
	fHistogramBounds.insert({"hMult_PerEvent_neutron", std::make_pair(0.0, 10000.0)});
	fHistogramBounds.insert({"hMult_PerEvent_opticalphoton", std::make_pair(0.0, 60000.0)});
	fHistogramBounds.insert({"hMult_PerEvent_other", std::make_pair(0.0, 10000.0)});
	fHistogramBounds.insert({"hMult_PerEvent_pi-", std::make_pair(0.0, 10000.0)});
	fHistogramBounds.insert({"hMult_PerEvent_pi+", std::make_pair(0.0, 10000.0)});
	fHistogramBounds.insert({"hMult_PerEvent_pi0", std::make_pair(0.0, 10000.0)});
	fHistogramBounds.insert({"hMult_PerEvent_e+", std::make_pair(0.0, 10000.0)});
	fHistogramBounds.insert({"hMult_PerEvent_proton", std::make_pair(0.0, 10000.0)});
	fHistogramBounds.insert({"hMult_PerEvent_untracked", std::make_pair(0.0, 10000.0)});

	fHistogramBounds.insert({"hMult_PerEvent_all_low", std::make_pair(0.0, 10.0)});
	fHistogramBounds.insert({"hMult_PerEvent_e-_low", std::make_pair(0.0, 10.0)});
	fHistogramBounds.insert({"hMult_PerEvent_gamma_low", std::make_pair(0.0, 10.0)});
	fHistogramBounds.insert({"hMult_PerEvent_mu-_low", std::make_pair(0.0, 10.0)});
	fHistogramBounds.insert({"hMult_PerEvent_mu+_low", std::make_pair(0.0, 10.0)});
	fHistogramBounds.insert({"hMult_PerEvent_neutron_low", std::make_pair(0.0, 10.0)});
	fHistogramBounds.insert({"hMult_PerEvent_opticalphoton_low", std::make_pair(0.0, 10.0)});
	fHistogramBounds.insert({"hMult_PerEvent_other_low", std::make_pair(0.0, 10.0)});
	fHistogramBounds.insert({"hMult_PerEvent_pi-_low", std::make_pair(0.0, 10.0)});
	fHistogramBounds.insert({"hMult_PerEvent_pi+_low", std::make_pair(0.0, 10.0)});
	fHistogramBounds.insert({"hMult_PerEvent_pi0_low", std::make_pair(0.0, 10.0)});
	fHistogramBounds.insert({"hMult_PerEvent_e+_low", std::make_pair(0.0, 10.0)});
	fHistogramBounds.insert({"hMult_PerEvent_proton_low", std::make_pair(0.0, 10.0)});
	fHistogramBounds.insert({"hMult_PerEvent_untracked_low", std::make_pair(0.0, 10.0)});

	fHistogramBounds.insert({"hMult_PerLayer_all", std::make_pair(0.0, SACLayers)});
	fHistogramBounds.insert({"hMult_PerLayer_e-", std::make_pair(0.0, SACLayers)});
	fHistogramBounds.insert({"hMult_PerLayer_gamma", std::make_pair(0.0, SACLayers)});
	fHistogramBounds.insert({"hMult_PerLayer_mu-", std::make_pair(0.0, SACLayers)});
	fHistogramBounds.insert({"hMult_PerLayer_mu+", std::make_pair(0.0, SACLayers)});
	fHistogramBounds.insert({"hMult_PerLayer_neutron", std::make_pair(0.0, SACLayers)});
	fHistogramBounds.insert({"hMult_PerLayer_opticalphoton", std::make_pair(0.0, SACLayers)});
	fHistogramBounds.insert({"hMult_PerLayer_other", std::make_pair(0.0, SACLayers)});
	fHistogramBounds.insert({"hMult_PerLayer_pi-", std::make_pair(0.0, SACLayers)});
	fHistogramBounds.insert({"hMult_PerLayer_pi+", std::make_pair(0.0, SACLayers)});
	fHistogramBounds.insert({"hMult_PerLayer_pi0", std::make_pair(0.0, SACLayers)});
	fHistogramBounds.insert({"hMult_PerLayer_e+", std::make_pair(0.0, SACLayers)});
	fHistogramBounds.insert({"hMult_PerLayer_proton", std::make_pair(0.0, SACLayers)});
	fHistogramBounds.insert({"hMult_PerLayer_untracked", std::make_pair(0.0, SACLayers)});

	// bounds not properly defined (yet) -- need 2 for 2D
	fHistogramBounds.insert({"hMult_PerLayer_all_xy", std::make_pair(0.0, SACRows)});
	fHistogramBounds.insert({"hMult_PerLayer_e-_xy", std::make_pair(0.0, SACRows)});
	fHistogramBounds.insert({"hMult_PerLayer_gamma_xy", std::make_pair(0.0, SACRows)});
	fHistogramBounds.insert({"hMult_PerLayer_mu-_xy", std::make_pair(0.0, SACRows)});
	fHistogramBounds.insert({"hMult_PerLayer_mu+_xy", std::make_pair(0.0, SACRows)});
	fHistogramBounds.insert({"hMult_PerLayer_neutron_xy", std::make_pair(0.0, SACRows)});
	fHistogramBounds.insert({"hMult_PerLayer_opticalphoton_xy", std::make_pair(0.0, SACLayers)});
	fHistogramBounds.insert({"hMult_PerLayer_other_xy", std::make_pair(0.0, SACRows)});
	fHistogramBounds.insert({"hMult_PerLayer_pi-_xy", std::make_pair(0.0, SACRows)});
	fHistogramBounds.insert({"hMult_PerLayer_pi+_xy", std::make_pair(0.0, SACRows)});
	fHistogramBounds.insert({"hMult_PerLayer_pi0_xy", std::make_pair(0.0, SACRows)});
	fHistogramBounds.insert({"hMult_PerLayer_e+_xy", std::make_pair(0.0, SACRows)});
	fHistogramBounds.insert({"hMult_PerLayer_proton_xy", std::make_pair(0.0, SACRows)});
	fHistogramBounds.insert({"hMult_PerLayer_untracked_xy", std::make_pair(0.0, SACRows)});

	// bounds not properly defined (yet) -- need 3 for 3D
	fHistogramBounds.insert({"hMult_SAC_all", std::make_pair(0.0, SACRows)});
	fHistogramBounds.insert({"hMult_SAC_e-", std::make_pair(0.0, SACRows)});
	fHistogramBounds.insert({"hMult_SAC_gamma", std::make_pair(0.0, SACRows)});
	fHistogramBounds.insert({"hMult_SAC_mu-", std::make_pair(0.0, SACRows)});
	fHistogramBounds.insert({"hMult_SAC_mu+", std::make_pair(0.0, SACRows)});
	fHistogramBounds.insert({"hMult_SAC_neutron", std::make_pair(0.0, SACRows)});
	fHistogramBounds.insert({"hMult_SAC_opticalphoton", std::make_pair(0.0, SACRows)});
	fHistogramBounds.insert({"hMult_SAC_other", std::make_pair(0.0, SACRows)});
	fHistogramBounds.insert({"hMult_SAC_pi-", std::make_pair(0.0, SACRows)});
	fHistogramBounds.insert({"hMult_SAC_pi+", std::make_pair(0.0, SACRows)});
	fHistogramBounds.insert({"hMult_SAC_pi0", std::make_pair(0.0, SACRows)});
	fHistogramBounds.insert({"hMult_SAC_e+", std::make_pair(0.0, SACRows)});
	fHistogramBounds.insert({"hMult_SAC_proton", std::make_pair(0.0, SACRows)});
	fHistogramBounds.insert({"hMult_SAC_untracked", std::make_pair(0.0, SACRows)});

	fHistogramBounds.insert({"hTrLen_PerHit_all", std::make_pair(0.0 * cm, 202.5 * cm * cm)});
	fHistogramBounds.insert({"hTrLen_PerHit_e-", std::make_pair(0.0 * cm, 202.5 * cm)});
	fHistogramBounds.insert({"hTrLen_PerHit_gamma", std::make_pair(0.0 * cm, 202.5 * cm)});
	fHistogramBounds.insert({"hTrLen_PerHit_mu-", std::make_pair(0.0 * cm, 202.5 * cm)});
	fHistogramBounds.insert({"hTrLen_PerHit_mu+", std::make_pair(0.0 * cm, 202.5 * cm)});
	fHistogramBounds.insert({"hTrLen_PerHit_neutron", std::make_pair(0.0 * cm, 202.5 * cm)});
	fHistogramBounds.insert({"hTrLen_PerHit_opticalphoton", std::make_pair(0.0 * cm, 202.5 * cm)});
	fHistogramBounds.insert({"hTrLen_PerHit_other", std::make_pair(0.0 * cm, 202.5 * cm)});
	fHistogramBounds.insert({"hTrLen_PerHit_pi-", std::make_pair(0.0 * cm, 202.5 * cm)});
	fHistogramBounds.insert({"hTrLen_PerHit_pi+", std::make_pair(0.0 * cm, 202.5 * cm)});
	fHistogramBounds.insert({"hTrLen_PerHit_pi0", std::make_pair(0.0 * cm, 202.5 * cm)});
	fHistogramBounds.insert({"hTrLen_PerHit_e+", std::make_pair(0.0 * cm, 202.5 * cm)});
	fHistogramBounds.insert({"hTrLen_PerHit_proton", std::make_pair(0.0 * cm, 202.5 * cm)});
	fHistogramBounds.insert({"hTrLen_PerHit_untracked", std::make_pair(0.0 * cm, 202.5 * cm)});
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HistManager::~HistManager() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HistManager::PrintHistogramNames()
{
	G4cout << "---------- List of histogram names ----------" << G4endl;
	std::map<G4String, G4int>::iterator iter;
	for(iter = fHistogramNames.begin(); iter != fHistogramNames.end(); iter++)
		G4cout << iter->first << " : " << iter->second << G4endl;
	G4cout << "---------------------------------------------" << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HistManager::PrintHistogramBounds()
{
	G4cout << "---------- List of histogram bounds ----------" << G4endl;
	std::map<G4String, std::pair<G4double, G4double>>::iterator iter;
	for(iter = fHistogramBounds.begin(); iter != fHistogramBounds.end(); iter++)
		G4cout << iter->first << " : (" << iter->second.first << ", " << iter->second.second << ")" << G4endl;
	G4cout << "----------------------------------------------" << G4endl;
}
