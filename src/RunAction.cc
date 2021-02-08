// RunAction.cc
// --------------------------------------------------------------
// History:
//
// Created by Emily Tsai (emily.tsai11@gmail.com) 2020-7-9
// --------------------------------------------------------------

#include "RunAction.hh"
#include "globals.hh"

#include "G4RunManager.hh"
#include "G4Run.hh"
#include "G4Timer.hh"

#include "HistManager.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::RunAction() : G4UserRunAction()
{
	fHistManager = HistManager::GetInstance();
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
	G4cout << "RunAction::BeginOfRunAction(): Run " << aRun->GetRunID() << " begins!" << G4endl;
	fTimer->Start();

	G4RunManager::GetRunManager()->SetRandomNumberStore(false);

	// Open output file
	fAnalysisManager = G4Analysis::ManagerInstance("root");
	fAnalysisManager->SetVerboseLevel(1);
	fAnalysisManager->OpenFile(fHistManager->GetFileName());

	fNumEvents = aRun->GetNumberOfEventToBeProcessed();

	CreateHistograms();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::EndOfRunAction(const G4Run* aRun)
{
	fTimer->Stop();
	G4cout << "RunAction::EndOfRunAction(): Run " << aRun->GetRunID() << " completed!" << G4endl;

	// Save output file
	fAnalysisManager = G4AnalysisManager::Instance();
	fAnalysisManager->Write();
	fAnalysisManager->CloseFile();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::CreateHistograms()
{
	std::map<G4String, f1DHistInfo> f1DH = fHistManager->Get1DHistInfo();
	std::map<G4String, f1DHistInfo>::iterator iter1;
	for(iter1 = f1DH.begin(); iter1 != f1DH.end(); iter1++)
	{
		fAnalysisManager->CreateH1(iter1->first, iter1->second.description,
			iter1->second.nBinsX, iter1->second.lowX, iter1->second.highX);
	}

	std::map<G4String, f2DHistInfo> f2DH = fHistManager->Get2DHistInfo();
	std::map<G4String, f2DHistInfo>::iterator iter2;
	for(iter2 = f2DH.begin(); iter2 != f2DH.end(); iter2++)
	{
		fAnalysisManager->CreateH2(iter2->first, iter2->second.description,
			iter2->second.nBinsX, iter2->second.lowX, iter2->second.highX,
			iter2->second.nBinsY, iter2->second.lowY, iter2->second.highY);
	}

	std::map<G4String, f3DHistInfo> f3DH = fHistManager->Get3DHistInfo();
	std::map<G4String, f3DHistInfo>::iterator iter3;
	for(iter3 = f3DH.begin(); iter3 != f3DH.end(); iter3++)
	{
		fAnalysisManager->CreateH3(iter3->first, iter3->second.description,
			iter3->second.nBinsX, iter3->second.lowX, iter3->second.highX,
			iter3->second.nBinsY, iter3->second.lowY, iter3->second.highY,
			iter3->second.nBinsZ, iter3->second.lowZ, iter3->second.highZ);
	}
}
