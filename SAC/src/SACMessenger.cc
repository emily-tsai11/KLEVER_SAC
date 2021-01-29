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

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SACMessenger::SACMessenger()
{
	fSACDetectorDir = new G4UIdirectory("/Detector/SAC/");
	fSACDetectorDir->SetGuidance("UI commands to control the SAC detector");


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
	fSetCellGapCmd->SetParameterName("CG", false);
	fSetCellGapCmd->SetDefaultUnit("um");
	fSetCellGapCmd->SetRange("CG > 0.0 && CG <= 1000.0");
	fSetCellGapCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

	fSetLayerGapCmd = new G4UIcmdWithADoubleAndUnit("/Detector/SAC/LayerGap", this);
	fSetLayerGapCmd->SetGuidance("Set size of gap between layers.");
	fSetLayerGapCmd->SetParameterName("LG", false);
	fSetLayerGapCmd->SetDefaultUnit("cm");
	fSetLayerGapCmd->SetRange("LG > 0.0 && LG <= 10.0");
	fSetLayerGapCmd->AvailableForStates(G4State_PreInit, G4State_Idle);


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
	delete fSetLayerGapCmd;

	delete fSetSACNRowsCmd;
	delete fSetSACNColsCmd;
	delete fSetSACNLayersCmd;

	delete fEnablePMTCmd;
	delete fEnableSiPMCmd;

	delete fSetVerboseLevelCmd;
	// delete fSetSACFrontFaceZCmd;

	delete fSACDetectorDir;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SACMessenger::SetNewValue(G4UIcommand* cmd, G4String par)
{
	SACGeometry* fSACGeometry = SACGeometry::GetInstance();

	if(cmd == fSetCrystalSizeCmd)
	{
		fSACGeometry->SetCrystalSizeX(fSetCrystalSizeCmd->GetNewDoubleValue(par));
		fSACGeometry->SetCrystalSizeY(fSetCrystalSizeCmd->GetNewDoubleValue(par));
	}
	if(cmd == fSetCrystalLengthCmd) fSACGeometry->SetCrystalSizeZ(fSetCrystalLengthCmd->GetNewDoubleValue(par));

	if(cmd == fSetCrystalCoatingCmd) fSACGeometry->SetCrystalCoating(fSetCrystalCoatingCmd->GetNewDoubleValue(par));
	if(cmd == fSetCellGapCmd) fSACGeometry->SetCellGap(fSetCellGapCmd->GetNewDoubleValue(par));
	if(cmd == fSetLayerGapCmd) fSACGeometry->SetLayerGap(fSetLayerGapCmd->GetNewDoubleValue(par));

	if(cmd == fSetSACNRowsCmd) fSACGeometry->SetSACNRows(fSetSACNRowsCmd->GetNewIntValue(par));
	if(cmd == fSetSACNColsCmd) fSACGeometry->SetSACNCols(fSetSACNColsCmd->GetNewIntValue(par));
	if(cmd == fSetSACNLayersCmd) fSACGeometry->SetSACNLayers(fSetSACNLayersCmd->GetNewIntValue(par));

	if(cmd == fEnablePMTCmd) fSACGeometry->SetEnablePMT(fEnablePMTCmd->GetNewIntValue(par));
	if(cmd == fEnableSiPMCmd) fSACGeometry->SetEnableSiPM(fEnableSiPMCmd->GetNewIntValue(par));

	if(cmd == fSetVerboseLevelCmd) fSACGeometry->SetVerboseLevel(fSetVerboseLevelCmd->GetNewIntValue(par));
	// if(cmd == fSetSACFrontFaceZCmd) fSACGeometry->SetSACFrontFacePosZ(fSetSACFrontFaceZCmd->GetNewDoubleValue(par));
}
