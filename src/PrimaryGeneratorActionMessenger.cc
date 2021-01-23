// PrimaryGeneratorActionMessenger.cc
// --------------------------------------------------------------
// History:
//
// Created by Emily Tsai (emily.tsai11@gmail.com) 2021-1-22
// --------------------------------------------------------------

#include "PrimaryGeneratorActionMessenger.hh"
#include "G4UIcmdWithAnInteger.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorActionMessenger::PrimaryGeneratorActionMessenger() : G4UImessenger(), fBeamType(-1)
{
	fPrimaryGeneratorActionDir = new G4UIdirectory("/PrimaryGeneratorAction/");
	fPrimaryGeneratorActionDir->SetGuidance("UI commands to control the Primary Generator Action");

	fSetBeamTypeCmd = new G4UIcmdWithAnInteger("/PrimaryGeneratorAction/BeamType", this);
	fSetBeamTypeCmd->SetParameterName("BT", false);
	fSetBeamTypeCmd->SetGuidance("Set beam type. Default is gammas. 1 for kaons.");
	fSetBeamTypeCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorActionMessenger::~PrimaryGeneratorActionMessenger()
{
	delete fSetBeamTypeCmd;
	delete fPrimaryGeneratorActionDir;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PrimaryGeneratorActionMessenger::SetNewValue(G4UIcommand* cmd, G4String par)
{
	if(cmd == fSetBeamTypeCmd) fBeamType = fSetBeamTypeCmd->GetNewIntValue(par);
}
