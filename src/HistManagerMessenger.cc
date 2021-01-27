// HistManagerMessenger.cc
// --------------------------------------------------------------
// History:
//
// Created by Emily Tsai (emily.tsai11@gmail.com) 2021-1-26
// --------------------------------------------------------------

#include "HistManagerMessenger.hh"

#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"

#include "HistManager.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HistManagerMessenger::HistManagerMessenger() : G4UImessenger()
{
	fHistToChange = "";

	fHistManagerDir = new G4UIdirectory("/HistManager/");
	fHistManagerDir->SetGuidance("UI commands to control the histogram manager.");

	fSetFileNameCmd = new G4UIcmdWithAString("/HistManager/FileName", this);
	fSetFileNameCmd->SetGuidance("Specify output save file name.");
	fSetFileNameCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

	fSetHistToChangeCmd = new G4UIcmdWithAString("/HistManager/HistToChange", this);
	fSetHistToChangeCmd->SetGuidance("Tell the program which histogram bound to change. MUST DO THIS BEFORE SETTING A BOUND!!!!");
	fSetHistToChangeCmd->AvailableForStates(G4State_PreInit, G4State_Idle);


	fSetNBinsXCmd = new G4UIcmdWithAnInteger("/HistManager/NBinsX", this);
	fSetNBinsXCmd->SetGuidance("Set number of X histogram bins. MUST TELL PROGRAM WHICH HISTOGRAM BOUND TO SET FIRST USING HistManager/HistToChange!!!!");
	fSetNBinsXCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

	fSetLowXBoundCmd = new G4UIcmdWithADouble("/HistManager/LowXBound", this);
	fSetLowXBoundCmd->SetGuidance("Set lower X histogram bound. MUST TELL PROGRAM WHICH HISTOGRAM BOUND TO SET FIRST USING HistManager/HistToChange!!!!");
	fSetLowXBoundCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

	fSetHighXBoundCmd = new G4UIcmdWithADouble("/HistManager/HighXBound", this);
	fSetHighXBoundCmd->SetGuidance("Set higher X histogram bound. MUST TELL PROGRAM WHICH HISTOGRAM BOUND TO SET FIRST USING HistManager/HistToChange!!!!");
	fSetHighXBoundCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

	fSetLowXBoundWithUnitCmd = new G4UIcmdWithADoubleAndUnit("/HistManager/LowXBoundWithUnit", this);
	fSetLowXBoundWithUnitCmd->SetGuidance("Set lower X histogram bound. MUST TELL PROGRAM WHICH HISTOGRAM BOUND TO SET FIRST USING HistManager/HistToChange!!!!");
	fSetLowXBoundWithUnitCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

	fSetHighXBoundWithUnitCmd = new G4UIcmdWithADoubleAndUnit("/HistManager/HighXBoundWithUnit", this);
	fSetHighXBoundWithUnitCmd->SetGuidance("Set higher X histogram bound. MUST TELL PROGRAM WHICH HISTOGRAM BOUND TO SET FIRST USING HistManager/HistToChange!!!!");
	fSetHighXBoundWithUnitCmd->AvailableForStates(G4State_PreInit, G4State_Idle);


	fSetNBinsYCmd = new G4UIcmdWithAnInteger("/HistManager/NBinsY", this);
	fSetNBinsYCmd->SetGuidance("Set number of Y histogram bins. MUST TELL PROGRAM WHICH HISTOGRAM BOUND TO SET FIRST USING HistManager/HistToChange!!!!");
	fSetNBinsYCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

	fSetLowYBoundCmd = new G4UIcmdWithADouble("/HistManager/LowYBound", this);
	fSetLowYBoundCmd->SetGuidance("Set lower Y histogram bound. MUST TELL PROGRAM WHICH HISTOGRAM BOUND TO SET FIRST USING HistManager/HistToChange!!!!");
	fSetLowYBoundCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

	fSetHighYBoundCmd = new G4UIcmdWithADouble("/HistManager/HighYBound", this);
	fSetHighYBoundCmd->SetGuidance("Set higher Y histogram bound. MUST TELL PROGRAM WHICH HISTOGRAM BOUND TO SET FIRST USING HistManager/HistToChange!!!!");
	fSetHighYBoundCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

	fSetLowYBoundWithUnitCmd = new G4UIcmdWithADoubleAndUnit("/HistManager/LowYBoundWithUnit", this);
	fSetLowYBoundWithUnitCmd->SetGuidance("Set lower Y histogram bound. MUST TELL PROGRAM WHICH HISTOGRAM BOUND TO SET FIRST USING HistManager/HistToChange!!!!");
	fSetLowYBoundWithUnitCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

	fSetHighYBoundWithUnitCmd = new G4UIcmdWithADoubleAndUnit("/HistManager/HighYBoundWithUnit", this);
	fSetHighYBoundWithUnitCmd->SetGuidance("Set higher Y histogram bound. MUST TELL PROGRAM WHICH HISTOGRAM BOUND TO SET FIRST USING HistManager/HistToChange!!!!");
	fSetHighYBoundWithUnitCmd->AvailableForStates(G4State_PreInit, G4State_Idle);


	fSetNBinsZCmd = new G4UIcmdWithAnInteger("/HistManager/NBinsZ", this);
	fSetNBinsZCmd->SetGuidance("Set number of Z histogram bins. MUST TELL PROGRAM WHICH HISTOGRAM BOUND TO SET FIRST USING HistManager/HistToChange!!!!");
	fSetNBinsZCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

	fSetLowZBoundCmd = new G4UIcmdWithADouble("/HistManager/LowZBound", this);
	fSetLowZBoundCmd->SetGuidance("Set lower Z histogram bound. MUST TELL PROGRAM WHICH HISTOGRAM BOUND TO SET FIRST USING HistManager/HistToChange!!!!");
	fSetLowZBoundCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

	fSetHighZBoundCmd = new G4UIcmdWithADouble("/HistManager/HighZBound", this);
	fSetHighZBoundCmd->SetGuidance("Set higher Z histogram bound. MUST TELL PROGRAM WHICH HISTOGRAM BOUND TO SET FIRST USING HistManager/HistToChange!!!!");
	fSetHighZBoundCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

	fSetLowZBoundWithUnitCmd = new G4UIcmdWithADoubleAndUnit("/HistManager/LowZBoundWithUnit", this);
	fSetLowZBoundWithUnitCmd->SetGuidance("Set lower Z histogram bound. MUST TELL PROGRAM WHICH HISTOGRAM BOUND TO SET FIRST USING HistManager/HistToChange!!!!");
	fSetLowZBoundWithUnitCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

	fSetHighZBoundWithUnitCmd = new G4UIcmdWithADoubleAndUnit("/HistManager/HighZBoundWithUnit", this);
	fSetHighZBoundWithUnitCmd->SetGuidance("Set higher Z histogram bound. MUST TELL PROGRAM WHICH HISTOGRAM BOUND TO SET FIRST USING HistManager/HistToChange!!!!");
	fSetHighZBoundWithUnitCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HistManagerMessenger::~HistManagerMessenger()
{
	delete fSetFileNameCmd;
	delete fSetHistToChangeCmd;

	delete fSetNBinsXCmd;
	delete fSetLowXBoundCmd;
	delete fSetHighXBoundCmd;
	delete fSetLowXBoundWithUnitCmd;
	delete fSetHighXBoundWithUnitCmd;

	delete fSetNBinsYCmd;
	delete fSetLowYBoundCmd;
	delete fSetHighYBoundCmd;
	delete fSetLowYBoundWithUnitCmd;
	delete fSetHighYBoundWithUnitCmd;

	delete fSetNBinsZCmd;
	delete fSetLowZBoundCmd;
	delete fSetHighZBoundCmd;
	delete fSetLowZBoundWithUnitCmd;
	delete fSetHighZBoundWithUnitCmd;

	delete fHistManagerDir;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HistManagerMessenger::SetNewValue(G4UIcommand* cmd, G4String par)
{
	HistManager* fHistManager = HistManager::GetInstance();

	if(cmd == fSetFileNameCmd) fHistManager->fFileName = par;
	if(cmd == fSetHistToChangeCmd) fHistToChange = par;

	if(fHistManager->f1DH.count(fHistToChange) > 0 || fHistManager->f2DH.count(fHistToChange) > 0 || fHistManager->f3DH.count(fHistToChange) > 0)
	{
		if(cmd == fSetNBinsXCmd)
		{
			if(fHistToChange[1] == '1')
				fHistManager->f1DH.at(fHistToChange).nBinsX = fSetNBinsXCmd->GetNewIntValue(par);
			else if(fHistToChange[1] == '2')
				fHistManager->f2DH.at(fHistToChange).nBinsX = fSetNBinsXCmd->GetNewIntValue(par);
			else if(fHistToChange[1] == '3')
				fHistManager->f3DH.at(fHistToChange).nBinsX = fSetNBinsXCmd->GetNewIntValue(par);
		}
		if(cmd == fSetLowXBoundCmd || cmd == fSetLowXBoundWithUnitCmd)
		{
			if(fHistToChange[1] == '1')
				fHistManager->f1DH.at(fHistToChange).lowX = fSetLowXBoundCmd->GetNewDoubleValue(par);
			else if(fHistToChange[1] == '2')
				fHistManager->f2DH.at(fHistToChange).lowX = fSetLowXBoundCmd->GetNewDoubleValue(par);
			else if(fHistToChange[1] == '3')
				fHistManager->f3DH.at(fHistToChange).lowX = fSetLowXBoundCmd->GetNewDoubleValue(par);
		}
		if(cmd == fSetHighXBoundCmd || cmd == fSetHighXBoundWithUnitCmd)
		{
			if(fHistToChange[1] == '1')
				fHistManager->f1DH.at(fHistToChange).highX = fSetHighXBoundCmd->GetNewDoubleValue(par);
			else if(fHistToChange[1] == '2')
				fHistManager->f2DH.at(fHistToChange).highX = fSetHighXBoundCmd->GetNewDoubleValue(par);
			else if(fHistToChange[1] == '3')
				fHistManager->f3DH.at(fHistToChange).highX = fSetHighXBoundCmd->GetNewDoubleValue(par);
		}

		if(cmd == fSetNBinsYCmd)
		{
			if(fHistToChange[1] == '2')
				fHistManager->f2DH.at(fHistToChange).nBinsY = fSetNBinsYCmd->GetNewIntValue(par);
			else if(fHistToChange[1] == '3')
				fHistManager->f3DH.at(fHistToChange).nBinsY = fSetNBinsYCmd->GetNewIntValue(par);
		}
		if(cmd == fSetLowYBoundCmd || cmd == fSetLowYBoundWithUnitCmd)
		{
			if(fHistToChange[1] == '2')
				fHistManager->f2DH.at(fHistToChange).lowY = fSetLowYBoundCmd->GetNewDoubleValue(par);
			else if(fHistToChange[1] == '3')
				fHistManager->f3DH.at(fHistToChange).lowY = fSetLowYBoundCmd->GetNewDoubleValue(par);
		}
		if(cmd == fSetHighYBoundCmd || cmd == fSetHighYBoundWithUnitCmd)
		{
			if(fHistToChange[1] == '2')
				fHistManager->f2DH.at(fHistToChange).highY = fSetHighYBoundCmd->GetNewDoubleValue(par);
			else if(fHistToChange[1] == '3')
				fHistManager->f3DH.at(fHistToChange).highY = fSetHighYBoundCmd->GetNewDoubleValue(par);
		}

		if(cmd == fSetNBinsZCmd)
		{
			if(fHistToChange[1] == '3')
				fHistManager->f3DH.at(fHistToChange).nBinsZ = fSetNBinsZCmd->GetNewIntValue(par);
		}
		if(cmd == fSetLowZBoundCmd || cmd == fSetLowZBoundWithUnitCmd)
		{
			if(fHistToChange[1] == '3')
				fHistManager->f3DH.at(fHistToChange).lowZ = fSetLowZBoundCmd->GetNewDoubleValue(par);
		}
		if(cmd == fSetHighZBoundCmd || cmd == fSetHighZBoundWithUnitCmd)
		{
			if(fHistToChange[1] == '3')
				fHistManager->f3DH.at(fHistToChange).highZ = fSetHighZBoundCmd->GetNewDoubleValue(par);
		}
	}
	else G4cout << "[HistManagerMessenger::SetNewValue()] ERROR: \""
		<< fHistToChange << "\" is not a histogram." << G4endl;
}
