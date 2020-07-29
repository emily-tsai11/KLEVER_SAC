// RunActionMessenger.cc
// --------------------------------------------------------------
// History:
//
// Created by Emily Tsai (emily.tsai11@gmail.com) 2020-7-28
// --------------------------------------------------------------

#include "RunActionMessenger.hh"

#include "G4RunManager.hh"
#include "G4UIcmdWithAString.hh"

#include "RunAction.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunActionMessenger::RunActionMessenger() : G4UImessenger(), fFileName("SACRun")
{
	fRunActionDir = new G4UIdirectory("/RunAction/");
	fRunActionDir->SetGuidance("UI commands to control RunAction");

	fSetFileNameCmd = new G4UIcmdWithAString("/RunAction/FileName", this);
	fSetFileNameCmd->SetGuidance("Specify output root file name");
	fSetFileNameCmd->SetParameterName("FileName", false);
	fSetFileNameCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunActionMessenger::~RunActionMessenger()
{
	delete fSetFileNameCmd;
	delete fRunActionDir;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunActionMessenger::SetNewValue(G4UIcommand* cmd, G4String par)
{
	if(cmd == fSetFileNameCmd) fFileName = par;
}
