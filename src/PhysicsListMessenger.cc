// PhysicsListMessenger.cc
// --------------------------------------------------------------
// $Id: PhysicsListMessenger.hh,v 1.3.6.1 2010/03/18 10:33:19 gcosmo Exp $
// GEANT4 tag $Name: geant4-09-03-patch-02 $
// --------------------------------------------------------------
// History:
//
// Created by V.Ivanchenko 31.01.2006
// Modified by V.Ivanchenko 04.06.2006
//		Adaptation of hadr01
// Sergey Podolsky (siarhei.padolski@cern.ch) 03-09-2012
// Adapted from Padme by Emily Tsai (emily.tsai11@gmail.com) 2020-7-15
// --------------------------------------------------------------

#include "PhysicsListMessenger.hh"

#include "G4RunManager.hh"

#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithoutParameter.hh"

#include "PhysicsList.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PhysicsListMessenger* PhysicsListMessenger::fInstance = nullptr;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PhysicsListMessenger::PhysicsListMessenger() : G4UImessenger(), fGammaCutCmd(0),
	fElectCutCmd(0), fPosCutCmd(0), fCutCmd(0), fAllCutCmd(0), fPListCmd(0),
	fListCmd(0), fFastCmd(0), fDecayPiplusDecayCmd(0), fMuonDecayCmd(0)
{
	fSimulationDir = new G4UIdirectory("/Simulation/");
	fSimulationDir->SetGuidance("UI commands to control Simulation");

	fGammaCutCmd = new G4UIcmdWithADoubleAndUnit("/Simulation/CutGamma", this);
	fGammaCutCmd->SetGuidance("Set gamma cut.");
	fGammaCutCmd->SetParameterName("Gcut", false);
	fGammaCutCmd->SetUnitCategory("Length");
	fGammaCutCmd->SetRange("Gcut >= 0.0");
	fGammaCutCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

	fElectCutCmd = new G4UIcmdWithADoubleAndUnit("/Simulation/CutEl", this);
	fElectCutCmd->SetGuidance("Set electron cut.");
	fElectCutCmd->SetParameterName("Ecut", false);
	fElectCutCmd->SetUnitCategory("Length");
	fElectCutCmd->SetRange("Ecut >= 0.0");
	fElectCutCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

	fPosCutCmd = new G4UIcmdWithADoubleAndUnit("/Simulation/CutPos", this);
	fPosCutCmd->SetGuidance("Set positron cut.");
	fPosCutCmd->SetParameterName("Pcut", false);
	fPosCutCmd->SetUnitCategory("Length");
	fPosCutCmd->SetRange("Pcut >= 0.0");
	fPosCutCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

	fCutCmd = new G4UIcmdWithADoubleAndUnit("/Simulation/CutProt", this);
	fCutCmd->SetGuidance("Set proton cut.");
	fCutCmd->SetParameterName("ProtCut", false);
	fCutCmd->SetUnitCategory("Length");
	fCutCmd->SetRange("ProtCut >= 0.0");
	fCutCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

	fAllCutCmd = new G4UIcmdWithADoubleAndUnit("/Simulation/CutsAll", this);
	fAllCutCmd->SetGuidance("Set cut for all.");
	fAllCutCmd->SetParameterName("cut", false);
	fAllCutCmd->SetUnitCategory("Length");
	fAllCutCmd->SetRange("cut >= 0.0");
	fAllCutCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

	fPListCmd = new G4UIcmdWithAString("/Simulation/Physics", this);
	fPListCmd->SetGuidance("Add modular physics list.");
	fPListCmd->SetParameterName("PList", false);
	fPListCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

	fListCmd = new G4UIcmdWithoutParameter("/Simulation/ListPhysics", this);
	fListCmd->SetGuidance("Available Physics Lists");
	fListCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

	fFastCmd = new G4UIcmdWithoutParameter("/Simulation/TurnOnFastLkr", this);
	fFastCmd->SetGuidance("Switch on fast LKr Simulation");
	fFastCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

	fDecayDir = new G4UIdirectory("/decay/");
	fDecayDir->SetGuidance("UI commands to control decay");

	fDecayPiplusDecayCmd = new G4UIcmdWithADouble("/decay/BrPie2", this);
	fDecayPiplusDecayCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

	fMuonDecayCmd = new G4UIcmdWithAnInteger("/decay/muonDecay", this);
	fMuonDecayCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

	fBrPie2 = -1;
	fMuonDecayMode = -1.0;
	fCutGamma = -1.0;
	fCutElectron = -1.0;
	fCutPositron = -1.0;
	fCutProton = -1.0;
	fPhysicsListName = -1.0;
	fAddParameterisation = false;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PhysicsListMessenger::~PhysicsListMessenger()
{
	delete fGammaCutCmd;
	delete fElectCutCmd;
	delete fPosCutCmd;
	delete fCutCmd;
	delete fAllCutCmd;
	delete fPListCmd;
	delete fListCmd;
	delete fFastCmd;

	delete fDecayPiplusDecayCmd;
	delete fMuonDecayCmd;

	delete fSimulationDir;
	delete fDecayDir;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PhysicsListMessenger* PhysicsListMessenger::GetInstance()
{
	if(!fInstance) fInstance = new PhysicsListMessenger();
	return fInstance;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PhysicsListMessenger::SetNewValue(G4UIcommand* command, G4String val)
{
	G4cout << command->GetCommandPath() << " " << val << G4endl;

	if(command == fGammaCutCmd)					fCutGamma = fGammaCutCmd->GetNewDoubleValue(val);
	else if(command == fElectCutCmd)			fCutElectron = fElectCutCmd->GetNewDoubleValue(val);
	else if(command == fPosCutCmd)				fCutPositron = fPosCutCmd->GetNewDoubleValue(val);
	else if(command == fCutCmd)					fCutProton = fCutCmd->GetNewDoubleValue(val);
	else if(command == fAllCutCmd)
	{
		G4double cut = fAllCutCmd->GetNewDoubleValue(val);
		fCutGamma = cut;
		fCutElectron = cut;
		fCutPositron = cut;
		fCutProton = cut;
	}
	else if(command == fPListCmd)
	{
		G4String name = val;
		if(name == "PHYSLIST")
		{
			char* path = getenv(name);
			if(path) name = G4String(path);
			else
			{
				G4cerr << "### PhysicsListMessenger WARNING: " << " environment variable PHYSLIST is not defined" << G4endl;
				return;
			}
		}
		fPhysicsListName = name;
	}
	else if(command == fListCmd)				fListPhysList = true;
	else if(command == fFastCmd)				fAddParameterisation = true;
	else if(command == fDecayPiplusDecayCmd)	fBrPie2 = fDecayPiplusDecayCmd->GetNewDoubleValue(val);
	else if(command == fMuonDecayCmd)			fMuonDecayMode = fMuonDecayCmd->GetNewIntValue(val);
}
