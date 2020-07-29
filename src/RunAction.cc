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

#include <string>

#include "PrimaryGeneratorAction.hh"
#include "RunActionMessenger.hh"
#include "Analysis.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::RunAction()
{
	fTimer = new G4Timer();
	fMessenger = new RunActionMessenger();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::~RunAction()
{
	delete fTimer;
	delete fMessenger;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::BeginOfRunAction(const G4Run* aRun)
{
	G4cout << "RunAction::BeginOfRunAction(): Run " << aRun->GetRunID() << " begins!" << G4endl;
	fTimer->Start();

	G4RunManager::GetRunManager()->SetRandomNumberStore(false);

	// get analysis manager and open output file
	fAnalysisManager = G4AnalysisManager::Instance();
	fAnalysisManager->SetVerboseLevel(1);
	fAnalysisManager->OpenFile(fMessenger->GetFileName());

	// -------------------- CREATE 1D HISTOGRAMS --------------------

	G4int NumParticles = 11;
	G4String ParticleNames[] = {"Gamma", "Positron", "Electron", "Proton", "Neutron",
		"PionPlus", "PionMinus", "PionZero", "MuonPlus", "MuonMinus", "OptPhot"};

	// energy deposition per hit, unweighted -- 0
	for(G4int i = 0; i < NumParticles; i++)
		fAnalysisManager->CreateH1("h" + ParticleNames[i] + "_PerHit_EDep",
			ParticleNames[i] + " energy deposition per hit",
			100, 0.0 * CLHEP::MeV, 2.0 * CLHEP::MeV);
	// energy deposition per hit, weighted by energy deposition -- 1
	for(G4int i = 0; i < NumParticles; i++)
		fAnalysisManager->CreateH1("h" + ParticleNames[i] + "_PerHit_wEDep",
			ParticleNames[i] + " weighted energy deposition per hit",
			100, 0.0 * CLHEP::MeV, 2.0 * CLHEP::MeV);
	// track length per hit -- 2
	for(G4int i = 0; i < NumParticles; i++)
		fAnalysisManager->CreateH1("h" + ParticleNames[i] + "_PerHit_TrLen",
			ParticleNames[i] + " track length per hit",
			100, 0.0 * CLHEP::cm, 20.0 * CLHEP::cm);
	// particle species multiplicity per event -- 3
	for(G4int i = 0; i < NumParticles; i++)
		fAnalysisManager->CreateH1("h" + ParticleNames[i] + "_PerEvent_Mult",
			ParticleNames[i] + " multiplicity per event",
			100, 0, 10000000);
	// particle species energy deposition per event -- 4
	for(G4int i = 0; i < NumParticles; i++)
		fAnalysisManager->CreateH1("h" + ParticleNames[i] + "_PerEvent_Energy",
			ParticleNames[i] + " energy deposition per event",
			100, 0.0 * CLHEP::MeV, 5.0 * CLHEP::MeV);
	// initial particle energy, unweighted -- 5
	for(G4int i = 0; i < NumParticles; i++)
		fAnalysisManager->CreateH1("h" + ParticleNames[i] + "_InitE",
			ParticleNames[i] + " initial energy",
			100, 0.0 * CLHEP::MeV, 20.0 * CLHEP::MeV);
	// initial particle energy, weighted by initial energy -- 6
	for(G4int i = 0; i < NumParticles; i++)
		fAnalysisManager->CreateH1("h" + ParticleNames[i] + "_wInitE",
			ParticleNames[i] + " weighted initial energy",
			100, 0.0 * CLHEP::MeV, 20.0 * CLHEP::MeV);
	// total energy deposition per event -- 77
	fAnalysisManager->CreateH1("hPerEvent_Energy",
		"total energy deposition per event",
		1000, 0.0 * CLHEP::GeV, 100.0 * CLHEP::MeV);
	// total untracked energy deposition in event -- 78
	fAnalysisManager->CreateH1("hPerEvent_UntrackedE",
		"untracked energy deposition per event",
		1000, 0.0 * CLHEP::GeV, 100.0 * CLHEP::MeV);

	// -------------------- CREATE 2D HISTOGRAMS --------------------

	// energy deposition vs. track length per hit -- 0
	for(G4int i = 0; i < NumParticles; i++)
		fAnalysisManager->CreateH2("h" + ParticleNames[i] + "_PerHit_EDep_TrLen",
			ParticleNames[i] + " energy deposition vs. track length per hit",
			100, 0.0 * CLHEP::MeV, 2.0 * CLHEP::MeV, 100, 0.0 * CLHEP::cm, 20.0 * CLHEP::cm);
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
