// PMTMessenger.cc
// --------------------------------------------------------------
// History:
//
// Created by Emily Tsai (emily.tsai11@gmail.com) 2020-12-30
// --------------------------------------------------------------

#include "PMTMessenger.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"

#include "PMTDetector.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PMTMessenger::PMTMessenger(PMTDetector* det) : fPMTDetector(det)
{
	// fPMTDetector = PMTDetector::GetInstance();

	// fPMTDetectorDir = new G4UIdirectory("/Detector/PMT/");
	// fPMTDetectorDir->SetGuidance("UI commands to control PMT detector");
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PMTMessenger::~PMTMessenger()
{
	// delete fSetVerboseLevelCmd;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

// void PMTMessenger::SetNewValue(G4UIcommand* cmd, G4String par)
// {
// 	if(cmd == fSetVerboseLevelCmd) fPMTDetector->SetVerboseLevel(fSetVerboseLevelCmd->GetNewIntValue(par));
// }
