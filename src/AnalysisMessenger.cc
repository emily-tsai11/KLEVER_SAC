// AnalysisMessenger.cc
// --------------------------------------------------------------
// History:
//
// Created by Emily Tsai (emily.tsai11@gmail.com) 2021-1-26
// --------------------------------------------------------------

#include "AnalysisMessenger.hh"
#include "globals.hh"
#include "G4UIcmdWithAString.hh"
#include "Analysis.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

AnalysisMessenger::AnalysisMessenger() : G4UImessenger()
{
	fAnalysisDir = new G4UIdirectory("/Analysis/");
	fAnalysisDir->SetGuidance("UI commands to control the histogram manager.");

	fSetFileNameCmd = new G4UIcmdWithAString("/Analysis/FileName", this);
	fSetFileNameCmd->SetGuidance("Specify output save file name.");
	fSetFileNameCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

AnalysisMessenger::~AnalysisMessenger()
{
	delete fSetFileNameCmd;
	delete fAnalysisDir;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void AnalysisMessenger::SetNewValue(G4UIcommand* cmd, G4String par)
{
	if(cmd == fSetFileNameCmd) Analysis::GetInstance()->SetFileName(par);
}
