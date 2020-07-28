// RunAction.cc
// --------------------------------------------------------------
// History:
//
// Created by Emily Tsai (emily.tsai11@gmail.com) 2020-7-9
// --------------------------------------------------------------

#include "RunAction.hh"

#include "G4RunManager.hh"

#include "G4Run.hh"
#include "G4Timer.hh"

#include "PrimaryGeneratorAction.hh"
#include "Analysis.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::RunAction()
{
	fTimer = new G4Timer();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::~RunAction()
{
	delete fTimer;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::BeginOfRunAction(const G4Run* aRun)
{
	fTimer->Start();

	G4RunManager::GetRunManager()->SetRandomNumberStore(false);

	// energies hard coded (for now...)
	G4int runID = aRun->GetRunID();
	G4String inputE = "none";
	switch(runID)
	{
		case 0: inputE = "100MeV"; break;
		case 1: inputE = "200MeV"; break;
		case 2: inputE = "500MeV"; break;
		case 3: inputE = "1GeV"; break;
		case 4: inputE = "2GeV"; break;
		case 5: inputE = "5GeV"; break;
		case 6: inputE = "10GeV"; break;
		case 7: inputE = "20GeV"; break;
		case 8: inputE = "50GeV"; break;
		case 9: inputE = "100GeV"; break;
		default: G4cout << "RunAction::BeginOfRunAction(): invalid run ID :(" << G4endl;
	}

	// get analysis manager and open output file
	fAnalysisManager = G4AnalysisManager::Instance();
	fAnalysisManager->SetVerboseLevel(1);
	fAnalysisManager->OpenFile("SAC_" + inputE);

	// -------------------- CREATE HISTOGRAMS --------------------

	// -------------------- 1D HISTOGRAMS --------------------

	// energy deposition for each step -- 0
	fAnalysisManager->CreateH1("hGamma_EDep", "energy deposition of gammas", 500, 0.0 * CLHEP::MeV, 0.1 * CLHEP::MeV);						// 0
	fAnalysisManager->CreateH1("hPositron_EDep", "energy deposition of e+", 500, 0.0 * CLHEP::MeV, 2.0 * CLHEP::MeV);						// 1
	fAnalysisManager->CreateH1("hElectron_EDep", "energy deposition of e-", 500, 0.0 * CLHEP::MeV, 2.0 * CLHEP::MeV);						// 2
	fAnalysisManager->CreateH1("hProton_EDep", "energy deposition of protons", 500, 0.0 * CLHEP::MeV, 2.0 * CLHEP::MeV);					// 3
	fAnalysisManager->CreateH1("hNeutron_EDep", "energy deposition of neutrons", 500, 0.0 * CLHEP::MeV, 2.0 * CLHEP::MeV);					// 4
	fAnalysisManager->CreateH1("hPionPlus_EDep", "energy deposition of pi+", 500, 0.0 * CLHEP::MeV, 2.0 * CLHEP::MeV);						// 5
	fAnalysisManager->CreateH1("hPionMinus_EDep", "energy deposition of pi-", 500, 0.0 * CLHEP::MeV, 2.0 * CLHEP::MeV);						// 6
	fAnalysisManager->CreateH1("hPionZero_EDep", "energy deposition of pi0", 500, 0.0 * CLHEP::MeV, 2.0 * CLHEP::MeV);						// 7
	fAnalysisManager->CreateH1("hMuonPlus_EDep", "energy deposition of muon+", 500, 0.0 * CLHEP::MeV, 2.0 * CLHEP::MeV);					// 8
	fAnalysisManager->CreateH1("hMuonMinus_EDep", "energy deposition of muon-", 500, 0.0 * CLHEP::MeV, 2.0 * CLHEP::MeV);					// 9
	fAnalysisManager->CreateH1("hOptPhot_EDep", "energy deposition of optical photons", 500, 0.0 * CLHEP::MeV, 5.0 * CLHEP::eV);			// 10

	// track length for each step -- 1
	fAnalysisManager->CreateH1("hGamma_TrLen", "track length of gammas", 500, 0.0 * CLHEP::cm, 20.0 * CLHEP::cm);							// 11
	fAnalysisManager->CreateH1("hPositron_TrLen", "track length of e+", 500, 0.0 * CLHEP::cm, 20.0 * CLHEP::cm);							// 12
	fAnalysisManager->CreateH1("hElectron_TrLen", "track length of e-", 500, 0.0 * CLHEP::cm, 20.0 * CLHEP::cm);							// 13
	fAnalysisManager->CreateH1("hProton_TrLen", "track length of protons", 500, 0.0 * CLHEP::cm, 20.0 * CLHEP::cm);							// 14
	fAnalysisManager->CreateH1("hNeutron_TrLen", "track length of neutrons", 500, 0.0 * CLHEP::cm, 20.0 * CLHEP::cm);						// 15
	fAnalysisManager->CreateH1("hPionPlus_TrLen", "track length of pi+", 500, 0.0 * CLHEP::cm, 20.0 * CLHEP::cm);							// 16
	fAnalysisManager->CreateH1("hPionMinus_TrLen", "track length of pi-", 500, 0.0 * CLHEP::cm, 20.0 * CLHEP::cm);							// 17
	fAnalysisManager->CreateH1("hPionZero_TrLen", "track length of pi0", 500, 0.0 * CLHEP::cm, 20.0 * CLHEP::cm);							// 18
	fAnalysisManager->CreateH1("hMuonPlus_TrLen", "track length of muon+", 500, 0.0 * CLHEP::cm, 20.0 * CLHEP::cm);							// 19
	fAnalysisManager->CreateH1("hMuonMinus_TrLen", "track length of muon-", 500, 0.0 * CLHEP::cm, 20.0 * CLHEP::cm);						// 20
	fAnalysisManager->CreateH1("hOptPhot_TrLen", "track length of optical photons", 500, 0.0 * CLHEP::cm, 20.0 * CLHEP::cm);				// 21

	// number of particles per event -- 2
	fAnalysisManager->CreateH1("hGamma_NPerEvent", "number of gammas per event", 500, 0, 43000);											// 22
	fAnalysisManager->CreateH1("hPositron_NPerEvent", "number of e+ per event", 500, 0, 140000);											// 23
	fAnalysisManager->CreateH1("hElectron_NPerEvent", "number of e- per event", 500, 0, 440000);											// 24
	fAnalysisManager->CreateH1("hProton_NPerEvent", "number of protons per event", 500, 0, 1000);											// 25
	fAnalysisManager->CreateH1("hNeutron_NPerEvent", "number of neutrons per event", 1000, 0, 1000);										// 26
	fAnalysisManager->CreateH1("hPionPlus_NPerEvent", "number of pi+ per event", 500, 0, 1000);												// 27
	fAnalysisManager->CreateH1("hPionMinus_NPerEvent", "number of pi- per event", 500, 0, 1000);											// 28
	fAnalysisManager->CreateH1("hPionZero_NPerEvent", "number of pi0 per event", 500, 0, 1000);												// 29
	fAnalysisManager->CreateH1("hMuonPlus_NPerEvent", "number of muon+ per event", 500, 0, 1000);											// 30
	fAnalysisManager->CreateH1("hMuonMinus_NPerEvent", "number of muon- per event", 500, 0, 1000);											// 31
	fAnalysisManager->CreateH1("hOptPhot_NPerEvent", "number of optical photons per event", 500, 0, 8200000);								// 32

	// total particle energy per event -- 3
	fAnalysisManager->CreateH1("hGamma_EPerEvent", "total gamma energy per event", 500, 0.0 * CLHEP::MeV, 3100.0 * CLHEP::MeV);				// 33
	fAnalysisManager->CreateH1("hPositron_EPerEvent", "total e+ energy per event", 500, 0.0 * CLHEP::MeV, 29000.0 * CLHEP::MeV);			// 34
	fAnalysisManager->CreateH1("hElectron_EPerEvent", "total e- energy per event", 500, 0.0 * CLHEP::MeV, 61000.0 * CLHEP::MeV);			// 35
	fAnalysisManager->CreateH1("hProton_EPerEvent", "total proton energy per event", 500, 0.0 * CLHEP::MeV, 100.0 * CLHEP::MeV);			// 36
	fAnalysisManager->CreateH1("hNeutron_EPerEvent", "total neutron energy per event", 500, 0.0 * CLHEP::MeV, 100.0 * CLHEP::MeV);			// 37
	fAnalysisManager->CreateH1("hPionPlus_EPerEvent", "total pi+ energy per event", 500, 0.0 * CLHEP::MeV, 100.0 * CLHEP::MeV);				// 38
	fAnalysisManager->CreateH1("hPionMinus_EPerEvent", "total pi- energy per event", 500, 0.0 * CLHEP::MeV, 100.0 * CLHEP::MeV);			// 39
	fAnalysisManager->CreateH1("hPionZero_EPerEvent", "total pi0 energy per event", 500, 0.0 * CLHEP::MeV, 100.0 * CLHEP::MeV);				// 40
	fAnalysisManager->CreateH1("hMuonPlus_EPerEvent", "total muon+ energy per event", 500, 0.0 * CLHEP::MeV, 100.0 * CLHEP::MeV);			// 41
	fAnalysisManager->CreateH1("hMuonMinus_EPerEvent", "total muon- energy per event", 500, 0.0 * CLHEP::MeV, 100.0 * CLHEP::MeV);			// 42
	fAnalysisManager->CreateH1("hOptPhot_EPerEvent", "total optical photon energy per event", 500, 0.0 * CLHEP::MeV, 20.0 * CLHEP::MeV);	// 43

	// total energy deposited in the SD per event
	fAnalysisManager->CreateH1("hEPerEvent", "total energy deposited in SD per event", 1000, 0.0 * CLHEP::GeV, 100.0 * CLHEP::GeV);			// 44

	// -------------------- 2D HISTOGRAMS --------------------

	// energy deposition vs. track length -- 0
	fAnalysisManager->CreateH2("hGamma_EDep_TrLen", "gamma energy deposition vs. track length",
		100, 0.0 * CLHEP::MeV, 0.1 * CLHEP::MeV, 100, 0.0 * CLHEP::cm, 20.0 * CLHEP::cm);					// 0
	fAnalysisManager->CreateH2("hPositron_EDep_TrLen", "positron energy deposition vs. track length",
		100, 0.0 * CLHEP::MeV, 2.0 * CLHEP::MeV, 100, 0.0 * CLHEP::cm, 20.0 * CLHEP::cm);					// 1
	fAnalysisManager->CreateH2("hElectron_EDep_TrLen", "electron energy deposition vs. track length",
		100, 0.0 * CLHEP::MeV, 2.0 * CLHEP::MeV, 100, 0.0 * CLHEP::cm, 20.0 * CLHEP::cm);					// 2
	fAnalysisManager->CreateH2("hProton_EDep_TrLen", "proton energy deposition vs. track length",
		100, 0.0 * CLHEP::MeV, 2.0 * CLHEP::MeV, 100, 0.0 * CLHEP::cm, 20.0 * CLHEP::cm);					// 3
	fAnalysisManager->CreateH2("hElectron_EDep_TrLen", "electron energy deposition vs. track length",
		100, 0.0 * CLHEP::MeV, 2.0 * CLHEP::MeV, 100, 0.0 * CLHEP::cm, 20.0 * CLHEP::cm);					// 4
	fAnalysisManager->CreateH2("hNeutron_EDep_TrLen", "neutron energy deposition vs. track length",
		100, 0.0 * CLHEP::MeV, 2.0 * CLHEP::MeV, 100, 0.0 * CLHEP::cm, 20.0 * CLHEP::cm);					// 5
	fAnalysisManager->CreateH2("hPionPlus_EDep_TrLen", "pi+ energy deposition vs. track length",
		100, 0.0 * CLHEP::MeV, 2.0 * CLHEP::MeV, 100, 0.0 * CLHEP::cm, 20.0 * CLHEP::cm);					// 6
	fAnalysisManager->CreateH2("hPionZero_EDep_TrLen", "pi0 energy deposition vs. track length",
		100, 0.0 * CLHEP::MeV, 2.0 * CLHEP::MeV, 100, 0.0 * CLHEP::cm, 20.0 * CLHEP::cm);					// 7
	fAnalysisManager->CreateH2("hMuonPlus_EDep_TrLen", "mu+ energy deposition vs. track length",
		100, 0.0 * CLHEP::MeV, 2.0 * CLHEP::MeV, 100, 0.0 * CLHEP::cm, 20.0 * CLHEP::cm);					// 8
	fAnalysisManager->CreateH2("hMuonMinus_EDep_TrLen", "mu- energy deposition vs. track length",
		100, 0.0 * CLHEP::MeV, 2.0 * CLHEP::MeV, 100, 0.0 * CLHEP::cm, 20.0 * CLHEP::cm);					// 9
	fAnalysisManager->CreateH2("hOptPhot_EDep_TrLen", "optical photon energy deposition vs. track length",
		100, 0.0 * CLHEP::MeV, 5.0 * CLHEP::eV, 100, 0.0 * CLHEP::cm, 20.0 * CLHEP::cm);					// 10
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::EndOfRunAction(const G4Run* aRun)
{
	fTimer->Stop();
	G4cout << "RunAction::EndOfRunAction(): Run " << aRun->GetRunID() << " completed!" << G4endl;

	// save histograms
	fAnalysisManager = G4AnalysisManager::Instance();
	fAnalysisManager->Write();
	fAnalysisManager->CloseFile();
}
