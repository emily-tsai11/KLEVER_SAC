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

#include "Analysis.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::RunAction() : G4UserRunAction()
{
	fAnalysis = Analysis::GetInstance();
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
	G4cout << "[RunAction::BeginOfRunAction] Run " << aRun->GetRunID() << " beginning!" << G4endl;
	fTimer->Start();

	G4RunManager::GetRunManager()->SetRandomNumberStore(false);

	fAnalysis->SetTotalNEvents(aRun->GetNumberOfEventToBeProcessed());
	fAnalysis->OpenFile();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::EndOfRunAction(const G4Run* aRun)
{
	fTimer->Stop();
	G4cout << "[RunAction::EndOfRunAction] Run " << aRun->GetRunID() << " completed!" << G4endl;

	fAnalysis->Write();
	fAnalysis->CloseFile();
}
