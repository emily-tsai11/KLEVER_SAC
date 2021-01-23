// DetectorConstructionMessenger.cc
// --------------------------------------------------------------
// History:
//
// Created by Emily Tsai (emily.tsai11@gmail.com) 2021-1-22
// --------------------------------------------------------------

#include "DetectorConstructionMessenger.hh"
#include "G4UIcmdWithoutParameter.hh"
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
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstructionMessenger::~DetectorConstructionMessenger()
{
	delete fUpdateGeometryCmd;
	delete fDetectorConstructionDir;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstructionMessenger::SetNewValue(G4UIcommand* cmd, G4String par)
{
	if(cmd == fUpdateGeometryCmd) fDetector->UpdateGeometry();
}
