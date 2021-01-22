// SACMessenger.cc
// --------------------------------------------------------------
// History:
//
// Created by Emanuele Leonardi (emanuele.leonardi@roma1.infn.it) 2015-12-14
// Adapted from Padme by Emily Tsai (emily.tsai11@gmail.com) 2020-7-15
// --------------------------------------------------------------

#include "SACMessenger.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"

#include "SACGeometry.hh"
#include "SACDetector.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SACMessenger::SACMessenger()
{
	fSACGeometry = SACGeometry::GetInstance();

	fSACDetectorDir = new G4UIdirectory("/Detector/SAC/");
	fSACDetectorDir->SetGuidance("UI commands to control SAC detector geometry");


	fSetCrystalSizeCmd = new G4UIcmdWithADoubleAndUnit("/Detector/SAC/CrystalSize", this);
	fSetCrystalSizeCmd->SetGuidance("Set size (side of square) of SAC crystal front face.");
	fSetCrystalSizeCmd->SetParameterName("S", false);
	fSetCrystalSizeCmd->SetDefaultUnit("cm");
	fSetCrystalSizeCmd->SetRange("S > 0.0 && S <= 10.0");
	fSetCrystalSizeCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

	fSetCrystalLengthCmd = new G4UIcmdWithADoubleAndUnit("/Detector/SAC/CrystalLength", this);
	fSetCrystalLengthCmd->SetGuidance("Set length of SAC crystal.");
	fSetCrystalLengthCmd->SetParameterName("L", false);
	fSetCrystalLengthCmd->SetDefaultUnit("cm");
	fSetCrystalLengthCmd->SetRange("L > 0.0 && L <= 30.0");
	fSetCrystalLengthCmd->AvailableForStates(G4State_PreInit, G4State_Idle);


	fSetCrystalCoatingCmd = new G4UIcmdWithADoubleAndUnit("/Detector/SAC/CrystalCoating", this);
	fSetCrystalCoatingCmd->SetGuidance("Set thickness of paint coating around crystal.");
	fSetCrystalCoatingCmd->SetParameterName("C", false);
	fSetCrystalCoatingCmd->SetDefaultUnit("um");
	fSetCrystalCoatingCmd->SetRange("C > 0.0 && C <= 1000.0");
	fSetCrystalCoatingCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

	fSetCellGapCmd = new G4UIcmdWithADoubleAndUnit("/Detector/SAC/CellGap", this);
	fSetCellGapCmd->SetGuidance("Set size of gap between cells.");
	fSetCellGapCmd->SetParameterName("G", false);
	fSetCellGapCmd->SetDefaultUnit("um");
	fSetCellGapCmd->SetRange("G > 0.0 && G <= 1000.0");
	fSetCellGapCmd->AvailableForStates(G4State_PreInit, G4State_Idle);


	fSetSACNRowsCmd = new G4UIcmdWithAnInteger("/Detector/SAC/NRows", this);
	fSetSACNRowsCmd->SetGuidance("Set number of cell rows in SAC.");
	fSetSACNRowsCmd->SetParameterName("NR", false);
	fSetSACNRowsCmd->SetRange("NR >= 1 && NR <= 10");
	fSetSACNRowsCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

	fSetSACNColsCmd = new G4UIcmdWithAnInteger("/Detector/SAC/NCols", this);
	fSetSACNColsCmd->SetGuidance("Set number of cell columns in SAC.");
	fSetSACNColsCmd->SetParameterName("NC", false);
	fSetSACNColsCmd->SetRange("NC >= 1 && NC <= 10");
	fSetSACNColsCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

	fSetSACNLayersCmd = new G4UIcmdWithAnInteger("/Detector/SAC/NLayers", this);
	fSetSACNLayersCmd->SetGuidance("Set number of cell layers in SAC.");
	fSetSACNLayersCmd->SetParameterName("NL", false);
	fSetSACNLayersCmd->SetRange("NL >= 1 && NL <= 10");
	fSetSACNLayersCmd->AvailableForStates(G4State_PreInit, G4State_Idle);


	fEnablePMTCmd = new G4UIcmdWithAnInteger("/Detector/SAC/EnablePMT", this);
	fEnablePMTCmd->SetGuidance("Enable PMTs on SAC.");
	fEnablePMTCmd->SetParameterName("EP", false);
	fEnablePMTCmd->SetRange("EP >= 0 && EP <= 1");
	fEnablePMTCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

	fEnableSiPMCmd = new G4UIcmdWithAnInteger("/Detector/SAC/EnableSiPM", this);
	fEnableSiPMCmd->SetGuidance("Enable SiPMs on SAC.");
	fEnableSiPMCmd->SetParameterName("ES", false);
	fEnableSiPMCmd->SetRange("ES >= 0 && ES <= 1");
	fEnableSiPMCmd->AvailableForStates(G4State_PreInit, G4State_Idle);


	fSetVerboseLevelCmd = new G4UIcmdWithAnInteger("/Detector/SAC/VerboseLevel", this);
	fSetVerboseLevelCmd->SetGuidance("Set verbose level for SAC code.");
	fSetVerboseLevelCmd->SetParameterName("VL", false);
	fSetVerboseLevelCmd->SetRange("VL >= 0");
	fSetVerboseLevelCmd->AvailableForStates(G4State_PreInit, G4State_Idle);


	fSetIncidentECmd = new G4UIcmdWithADoubleAndUnit("/Detector/SAC/IncidentE", this);
	fSetIncidentECmd->SetGuidance("Pass in incident particle energy.");
	fSetIncidentECmd->SetParameterName("IncidentE", false);
	fSetIncidentECmd->SetDefaultUnit("MeV");
	fSetIncidentECmd->SetRange("IncidentE >= 0.0");
	fSetIncidentECmd->AvailableForStates(G4State_PreInit, G4State_Idle);

	// fSetSACFrontFaceZCmd = new G4UIcmdWithADoubleAndUnit("/Detector/SAC/FrontFaceZ", this);
	// fSetSACFrontFaceZCmd->SetGuidance("Set position along Z of SAC front face.");
	// fSetSACFrontFaceZCmd->SetParameterName("Z", false);
	// fSetSACFrontFaceZCmd->SetDefaultUnit("cm");
	// fSetSACFrontFaceZCmd->SetRange("Z > 100.0 && Z <= 1000.0");
	// fSetSACFrontFaceZCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SACMessenger::~SACMessenger()
{
	delete fSetCrystalSizeCmd;
	delete fSetCrystalLengthCmd;

	delete fSetCrystalCoatingCmd;
	delete fSetCellGapCmd;

	delete fSetSACNRowsCmd;
	delete fSetSACNColsCmd;
	delete fSetSACNLayersCmd;

	delete fEnablePMTCmd;
	delete fEnableSiPMCmd;

	delete fSetVerboseLevelCmd;

	delete fSetIncidentECmd;
	// delete fSetSACFrontFaceZCmd;

	delete fSACDetectorDir;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SACMessenger::SetNewValue(G4UIcommand* cmd, G4String par)
{
	if(cmd == fSetCrystalSizeCmd)
	{
		fSACGeometry->SetCrystalSizeX(fSetCrystalSizeCmd->GetNewDoubleValue(par));
		fSACGeometry->SetCrystalSizeY(fSetCrystalSizeCmd->GetNewDoubleValue(par));
	}
	if(cmd == fSetCrystalLengthCmd) fSACGeometry->SetCrystalSizeZ(fSetCrystalLengthCmd->GetNewDoubleValue(par));

	if(cmd == fSetCrystalCoatingCmd) fSACGeometry->SetCrystalCoating(fSetCrystalCoatingCmd->GetNewDoubleValue(par));
	if(cmd == fSetCellGapCmd) fSACGeometry->SetCellGap(fSetCellGapCmd->GetNewDoubleValue(par));

	if(cmd == fSetSACNRowsCmd) fSACGeometry->SetSACNRows(fSetSACNRowsCmd->GetNewIntValue(par));
	if(cmd == fSetSACNColsCmd) fSACGeometry->SetSACNCols(fSetSACNColsCmd->GetNewIntValue(par));
	if(cmd == fSetSACNLayersCmd) fSACGeometry->SetSACNLayers(fSetSACNLayersCmd->GetNewIntValue(par));

	if(cmd == fEnablePMTCmd)
	{
		fSACGeometry->SetEnablePMT(fEnablePMTCmd->GetNewIntValue(par));
		// printf("-----------> reached here!\n");
		// ~SACDetector();
	}
	if(cmd == fEnableSiPMCmd) fSACGeometry->SetEnableSiPM(fEnableSiPMCmd->GetNewIntValue(par));

	if(cmd == fSetVerboseLevelCmd) fSACGeometry->SetVerboseLevel(fSetVerboseLevelCmd->GetNewIntValue(par));

	if(cmd == fSetIncidentECmd) fSACGeometry->SetIncidentE(fSetIncidentECmd->GetNewDoubleValue(par));
	// if(cmd == fSetSACFrontFaceZCmd) fSACGeometry->SetSACFrontFacePosZ(fSetSACFrontFaceZCmd->GetNewDoubleValue(par));
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

// G4String SACMessenger::GetCurrentValue(G4UIcommand* cmd)
// {
// 	// TODO
// }
