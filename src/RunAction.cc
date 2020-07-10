// RunAction.cc
// --------------------------------------------------------------
// History:
//
// Created by Emily Tsai (emily.tsai11@gmail.com) 2020-7-9
// --------------------------------------------------------------

#include "RunAction.hh"

#include "G4RunManager.hh"
// #include "RootIOManager.hh"

#include "G4Run.hh"
#include "G4Timer.hh"

#include "PrimaryGeneratorAction.hh"

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
	G4cout << "RunAction::BeginOfRunAction: Run " << aRun->GetRunID() << " start!" << G4endl;
	fTimer->Start();

	// ROOT OUTPUT FOR RUN
	G4RunManager::GetRunManager()->SetRandomNumberStore(false);
	CLHEP::HepRandom::showEngineStatus();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::EndOfRunAction(const G4Run* aRun)
{
	fTimer->Stop();
	G4cout << "RunAction::EndOfRunAction: Total events = " << aRun->GetNumberOfEvent() << " | Timer: " << *fTimer << G4endl;

	// FINALIZE(?) ROOT OUTPUT FOR RUN

}
