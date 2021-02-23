// DetectorConstructionMessenger.cc
// --------------------------------------------------------------
// History:
//
// Created by Emily Tsai (emily.tsai11@gmail.com) 2021-1-22
// --------------------------------------------------------------

#include "DetectorConstructionMessenger.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "DetectorConstruction.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstructionMessenger::DetectorConstructionMessenger(DetectorConstruction* det) :
	G4UImessenger(), fDetector(det)
{
	fDetectorConstructionDir = new G4UIdirectory("/Detector/");
	fDetectorConstructionDir->SetGuidance("UI commands to control the Detector Construction");

	fUpdateGeometryCmd = new G4UIcmdWithoutParameter("/Detector/UpdateGeometry", this);
	fUpdateGeometryCmd->SetGuidance("Update the detector geometry.");
	fUpdateGeometryCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

	fSetWorldLengthXCmd = new G4UIcmdWithADoubleAndUnit("/Detector/WorldLengthX", this);
	fSetWorldLengthXCmd->SetGuidance("Set world length X.");
	fSetWorldLengthXCmd->SetParameterName("WLX", false);
	fSetWorldLengthXCmd->SetDefaultUnit("m");
	fSetWorldLengthXCmd->SetRange("WLX > 0.0");
	fSetWorldLengthXCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

	fSetWorldLengthYCmd = new G4UIcmdWithADoubleAndUnit("/Detector/WorldLengthY", this);
	fSetWorldLengthYCmd->SetGuidance("Set world length Y.");
	fSetWorldLengthYCmd->SetParameterName("WLY", false);
	fSetWorldLengthYCmd->SetDefaultUnit("m");
	fSetWorldLengthYCmd->SetRange("WLY > 0.0");
	fSetWorldLengthYCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

	fSetWorldLengthZCmd = new G4UIcmdWithADoubleAndUnit("/Detector/WorldLengthZ", this);
	fSetWorldLengthZCmd->SetGuidance("Set world length Z.");
	fSetWorldLengthZCmd->SetParameterName("WLZ", false);
	fSetWorldLengthZCmd->SetDefaultUnit("m");
	fSetWorldLengthZCmd->SetRange("WLZ > 0.0");
	fSetWorldLengthZCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstructionMessenger::~DetectorConstructionMessenger()
{
	delete fUpdateGeometryCmd;
	delete fSetWorldLengthXCmd;
	delete fSetWorldLengthYCmd;
	delete fSetWorldLengthZCmd;

	delete fDetectorConstructionDir;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstructionMessenger::SetNewValue(G4UIcommand* cmd, G4String par)
{
	if(cmd == fUpdateGeometryCmd) fDetector->UpdateGeometry();
	else if(cmd == fSetWorldLengthXCmd) fDetector->SetWorldLengthX(fSetWorldLengthXCmd->GetNewDoubleValue(par));
	else if(cmd == fSetWorldLengthYCmd) fDetector->SetWorldLengthY(fSetWorldLengthYCmd->GetNewDoubleValue(par));
	else if(cmd == fSetWorldLengthZCmd) fDetector->SetWorldLengthZ(fSetWorldLengthZCmd->GetNewDoubleValue(par));
}
