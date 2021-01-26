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
#include "G4UnitsTable.hh"

#include "RunActionMessenger.hh"
#include "HistManager.hh"
#include "PrimaryGeneratorAction.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::RunAction()
{
	fTimer = new G4Timer();
	fMessenger = new RunActionMessenger();
	fHistManager = HistManager::GetInstance();
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
	fAnalysisManager = G4Analysis::ManagerInstance("root");
	fAnalysisManager->SetVerboseLevel(1);
	fAnalysisManager->OpenFile(fHistManager->GetFileName());

	CreateHistograms();
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

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::CreateHistograms()
{
	G4int nBins = fHistManager->GetNBins();
	std::map<G4String, G4int> fN = fHistManager->GetHistogramNames();
	std::map<G4String, std::pair<G4double, G4double>> fB = fHistManager->GetHistogramBounds();

	// TODO: FIX 2D AND 3D PLOTS
	G4String histName;
	G4double low;
	G4double high;
	std::map<G4String, G4int>::iterator iterN;
	for(iterN = fN.begin(); iterN != fN.end(); iterN++)
	{
		histName = iterN->first;
		low = fB.at(histName).first;
		high = fB.at(histName).second;
		fAnalysisManager->CreateH1(histName, histName, nBins, low, high);
	}
}
