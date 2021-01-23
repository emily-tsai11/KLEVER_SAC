// RunActionMessenger.cc
// --------------------------------------------------------------
// History:
//
// Created by Emily Tsai (emily.tsai11@gmail.com) 2020-7-28
// --------------------------------------------------------------

#include "RunActionMessenger.hh"

#include "G4RunManager.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"

#include "RunAction.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunActionMessenger::RunActionMessenger() : G4UImessenger(), fFileName("SACRun"),
	fGammaPerHitEDepBound(0.5), fPositronPerHitEDepBound(20.0), fElectronPerHitEDepBound(20.0),
	fProtonPerHitEDepBound(20.0), fNeutronPerHitEDepBound(20.0), fPionPlusPerHitEDepBound(20.0),
	fPionMinusPerHitEDepBound(20.0), fPionZeroPerHitEDepBound(20.0), fMuonPlusPerHitEDepBound(20.0),
	fMuonMinusPerHitEDepBound(20.0), fOptPhotPerHitEDepBound(0.000005),

	fGammaPerHitTrLenBound(202.5), fPositronPerHitTrLenBound(202.5), fElectronPerHitTrLenBound(202.5),
	fProtonPerHitTrLenBound(202.5), fNeutronPerHitTrLenBound(202.5), fPionPlusPerHitTrLenBound(202.5),
	fPionMinusPerHitTrLenBound(202.5), fPionZeroPerHitTrLenBound(202.5), fMuonPlusPerHitTrLenBound(202.5),
	fMuonMinusPerHitTrLenBound(202.5), fOptPhotPerHitTrLenBound(202.5),

	fGammaPerEventMultMinBound(0.0), fGammaPerEventMultMaxBound(5.0),
	fPositronPerEventMultMinBound(0.0), fPositronPerEventMultMaxBound(5.0),
	fElectronPerEventMultMinBound(0.0), fElectronPerEventMultMaxBound(5.0),
	fProtonPerEventMultMinBound(0.0), fProtonPerEventMultMaxBound(5.0),
	fNeutronPerEventMultMinBound(0.0), fNeutronPerEventMultMaxBound(5.0),
	fPionPlusPerEventMultMinBound(0.0), fPionPlusPerEventMultMaxBound(5.0),
	fPionMinusPerEventMultMinBound(0.0), fPionMinusPerEventMultMaxBound(5.0),
	fPionZeroPerEventMultMinBound(0.0), fPionZeroPerEventMultMaxBound(5.0),
	fMuonPlusPerEventMultMinBound(0.0), fMuonPlusPerEventMultMaxBound(5.0),
	fMuonMinusPerEventMultMinBound(0.0), fMuonMinusPerEventMultMaxBound(5.0),
	fOptPhotPerEventMultMinBound(0.0), fOptPhotPerEventMultMaxBound(200.0),

	fGammaPerEventEDepBound(1.0), fPositronPerEventEDepBound(1.0), fElectronPerEventEDepBound(1.0),
	fProtonPerEventEDepBound(1.0), fNeutronPerEventEDepBound(1.0), fPionPlusPerEventEDepBound(1.0),
	fPionMinusPerEventEDepBound(1.0), fPionZeroPerEventEDepBound(1.0), fMuonPlusPerEventEDepBound(1.0),
	fMuonMinusPerEventEDepBound(1.0), fOptPhotPerEventEDepBound(1.0),

	fGammaPerEventInitEBound(1.0), fPositronPerEventInitEBound(1.0), fElectronPerEventInitEBound(1.0),
	fProtonPerEventInitEBound(1.0), fNeutronPerEventInitEBound(1.0), fPionPlusPerEventInitEBound(1.0),
	fPionMinusPerEventInitEBound(1.0), fPionZeroPerEventInitEBound(1.0), fMuonPlusPerEventInitEBound(1.0),
	fMuonMinusPerEventInitEBound(1.0), fOptPhotPerEventInitEBound(1.0),

	fPerEventEDepBound(1.0), fPerEventUntrackedEBound(1.0)
{
	fRunActionDir = new G4UIdirectory("/RunAction/");
	fRunActionDir->SetGuidance("UI commands to control the Run Action");

	fSetFileNameCmd = new G4UIcmdWithAString("/RunAction/FileName", this);
	fSetFileNameCmd->SetGuidance("Specify output root file name");
	fSetFileNameCmd->SetParameterName("FileName", false);
	fSetFileNameCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

	// -------------------- energy deposition per hit bound --------------------
	{
		fSetGammaPerHitEDepBoundCmd = new G4UIcmdWithADoubleAndUnit("/RunAction/GammaPerHitEDep", this);
		fSetGammaPerHitEDepBoundCmd->SetGuidance("Set Gamma energy deposition per hit bound");
		fSetGammaPerHitEDepBoundCmd->SetParameterName("GammaPerHitEDep", false);
		fSetGammaPerHitEDepBoundCmd->SetUnitCategory("Energy");
		fSetGammaPerHitEDepBoundCmd->SetRange("GammaPerHitEDep >= 0.0");
		fSetGammaPerHitEDepBoundCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

		fSetPositronPerHitEDepBoundCmd = new G4UIcmdWithADoubleAndUnit("/RunAction/PositronPerHitEDep", this);
		fSetPositronPerHitEDepBoundCmd->SetGuidance("Set Positron energy deposition per hit bound");
		fSetPositronPerHitEDepBoundCmd->SetParameterName("PositronPerHitEDep", false);
		fSetPositronPerHitEDepBoundCmd->SetUnitCategory("Energy");
		fSetPositronPerHitEDepBoundCmd->SetRange("PositronPerHitEDep >= 0.0");
		fSetPositronPerHitEDepBoundCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

		fSetElectronPerHitEDepBoundCmd = new G4UIcmdWithADoubleAndUnit("/RunAction/ElectronPerHitEDep", this);
		fSetElectronPerHitEDepBoundCmd->SetGuidance("Set Electron energy deposition per hit bound");
		fSetElectronPerHitEDepBoundCmd->SetParameterName("ElectronPerHitEDep", false);
		fSetElectronPerHitEDepBoundCmd->SetUnitCategory("Energy");
		fSetElectronPerHitEDepBoundCmd->SetRange("ElectronPerHitEDep >= 0.0");
		fSetElectronPerHitEDepBoundCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

		fSetProtonPerHitEDepBoundCmd = new G4UIcmdWithADoubleAndUnit("/RunAction/ProtonPerHitEDep", this);
		fSetProtonPerHitEDepBoundCmd->SetGuidance("Set Proton energy deposition per hit bound");
		fSetProtonPerHitEDepBoundCmd->SetParameterName("ProtonPerHitEDep", false);
		fSetProtonPerHitEDepBoundCmd->SetUnitCategory("Energy");
		fSetProtonPerHitEDepBoundCmd->SetRange("ProtonPerHitEDep >= 0.0");
		fSetProtonPerHitEDepBoundCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

		fSetNeutronPerHitEDepBoundCmd = new G4UIcmdWithADoubleAndUnit("/RunAction/NeutronPerHitEDep", this);
		fSetNeutronPerHitEDepBoundCmd->SetGuidance("Set Neutron energy deposition per hit bound");
		fSetNeutronPerHitEDepBoundCmd->SetParameterName("NeutronPerHitEDep", false);
		fSetNeutronPerHitEDepBoundCmd->SetUnitCategory("Energy");
		fSetNeutronPerHitEDepBoundCmd->SetRange("NeutronPerHitEDep >= 0.0");
		fSetNeutronPerHitEDepBoundCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

		fSetPionPlusPerHitEDepBoundCmd = new G4UIcmdWithADoubleAndUnit("/RunAction/PionPlusPerHitEDep", this);
		fSetPionPlusPerHitEDepBoundCmd->SetGuidance("Set PionPlus energy deposition per hit bound");
		fSetPionPlusPerHitEDepBoundCmd->SetParameterName("PionPlusPerHitEDep", false);
		fSetPionPlusPerHitEDepBoundCmd->SetUnitCategory("Energy");
		fSetPionPlusPerHitEDepBoundCmd->SetRange("PionPlusPerHitEDep >= 0.0");
		fSetPionPlusPerHitEDepBoundCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

		fSetPionMinusPerHitEDepBoundCmd = new G4UIcmdWithADoubleAndUnit("/RunAction/PionMinusPerHitEDep", this);
		fSetPionMinusPerHitEDepBoundCmd->SetGuidance("Set PionMinus energy deposition per hit bound");
		fSetPionMinusPerHitEDepBoundCmd->SetParameterName("PionMinusPerHitEDep", false);
		fSetPionMinusPerHitEDepBoundCmd->SetUnitCategory("Energy");
		fSetPionMinusPerHitEDepBoundCmd->SetRange("PionMinusPerHitEDep >= 0.0");
		fSetPionMinusPerHitEDepBoundCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

		fSetPionZeroPerHitEDepBoundCmd = new G4UIcmdWithADoubleAndUnit("/RunAction/PionZeroPerHitEDep", this);
		fSetPionZeroPerHitEDepBoundCmd->SetGuidance("Set PionZero energy deposition per hit bound");
		fSetPionZeroPerHitEDepBoundCmd->SetParameterName("PionZeroPerHitEDep", false);
		fSetPionZeroPerHitEDepBoundCmd->SetUnitCategory("Energy");
		fSetPionZeroPerHitEDepBoundCmd->SetRange("PionZeroPerHitEDep >= 0.0");
		fSetPionZeroPerHitEDepBoundCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

		fSetMuonPlusPerHitEDepBoundCmd = new G4UIcmdWithADoubleAndUnit("/RunAction/MuonPlusPerHitEDep", this);
		fSetMuonPlusPerHitEDepBoundCmd->SetGuidance("Set MuonPlus energy deposition per hit bound");
		fSetMuonPlusPerHitEDepBoundCmd->SetParameterName("MuonPlusPerHitEDep", false);
		fSetMuonPlusPerHitEDepBoundCmd->SetUnitCategory("Energy");
		fSetMuonPlusPerHitEDepBoundCmd->SetRange("MuonPlusPerHitEDep >= 0.0");
		fSetMuonPlusPerHitEDepBoundCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

		fSetMuonMinusPerHitEDepBoundCmd = new G4UIcmdWithADoubleAndUnit("/RunAction/MuonMinusPerHitEDep", this);
		fSetMuonMinusPerHitEDepBoundCmd->SetGuidance("Set MuonMinus energy deposition per hit bound");
		fSetMuonMinusPerHitEDepBoundCmd->SetParameterName("MuonMinusPerHitEDep", false);
		fSetMuonMinusPerHitEDepBoundCmd->SetUnitCategory("Energy");
		fSetMuonMinusPerHitEDepBoundCmd->SetRange("MuonMinusPerHitEDep >= 0.0");
		fSetMuonMinusPerHitEDepBoundCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

		fSetOptPhotPerHitEDepBoundCmd = new G4UIcmdWithADoubleAndUnit("/RunAction/OptPhotPerHitEDep", this);
		fSetOptPhotPerHitEDepBoundCmd->SetGuidance("Set OptPhot energy deposition per hit bound");
		fSetOptPhotPerHitEDepBoundCmd->SetParameterName("OptPhotPerHitEDep", false);
		fSetOptPhotPerHitEDepBoundCmd->SetUnitCategory("Energy");
		fSetOptPhotPerHitEDepBoundCmd->SetRange("OptPhotPerHitEDep >= 0.0");
		fSetOptPhotPerHitEDepBoundCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
	}

	// -------------------- track length per hit bound --------------------
	{
		fSetGammaPerHitTrLenBoundCmd = new G4UIcmdWithADoubleAndUnit("/RunAction/GammaPerHitTrLen", this);
		fSetGammaPerHitTrLenBoundCmd->SetGuidance("Set Gamma track length per hit bound");
		fSetGammaPerHitTrLenBoundCmd->SetParameterName("GammaPerHitTrLen", false);
		fSetGammaPerHitTrLenBoundCmd->SetUnitCategory("Length");
		fSetGammaPerHitTrLenBoundCmd->SetRange("GammaPerHitTrLen >= 0.0");
		fSetGammaPerHitTrLenBoundCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

		fSetPositronPerHitTrLenBoundCmd = new G4UIcmdWithADoubleAndUnit("/RunAction/PositronPerHitTrLen", this);
		fSetPositronPerHitTrLenBoundCmd->SetGuidance("Set Positron track length per hit bound");
		fSetPositronPerHitTrLenBoundCmd->SetParameterName("PositronPerHitTrLen", false);
		fSetPositronPerHitTrLenBoundCmd->SetUnitCategory("Length");
		fSetPositronPerHitTrLenBoundCmd->SetRange("PositronPerHitTrLen >= 0.0");
		fSetPositronPerHitTrLenBoundCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

		fSetElectronPerHitTrLenBoundCmd = new G4UIcmdWithADoubleAndUnit("/RunAction/ElectronPerHitTrLen", this);
		fSetElectronPerHitTrLenBoundCmd->SetGuidance("Set Electron track length per hit bound");
		fSetElectronPerHitTrLenBoundCmd->SetParameterName("ElectronPerHitTrLen", false);
		fSetElectronPerHitTrLenBoundCmd->SetUnitCategory("Length");
		fSetElectronPerHitTrLenBoundCmd->SetRange("ElectronPerHitTrLen >= 0.0");
		fSetElectronPerHitTrLenBoundCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

		fSetProtonPerHitTrLenBoundCmd = new G4UIcmdWithADoubleAndUnit("/RunAction/ProtonPerHitTrLen", this);
		fSetProtonPerHitTrLenBoundCmd->SetGuidance("Set Proton track length per hit bound");
		fSetProtonPerHitTrLenBoundCmd->SetParameterName("ProtonPerHitTrLen", false);
		fSetProtonPerHitTrLenBoundCmd->SetUnitCategory("Length");
		fSetProtonPerHitTrLenBoundCmd->SetRange("ProtonPerHitTrLen >= 0.0");
		fSetProtonPerHitTrLenBoundCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

		fSetNeutronPerHitTrLenBoundCmd = new G4UIcmdWithADoubleAndUnit("/RunAction/NeutronPerHitTrLen", this);
		fSetNeutronPerHitTrLenBoundCmd->SetGuidance("Set Neutron track length per hit bound");
		fSetNeutronPerHitTrLenBoundCmd->SetParameterName("NeutronPerHitTrLen", false);
		fSetNeutronPerHitTrLenBoundCmd->SetUnitCategory("Length");
		fSetNeutronPerHitTrLenBoundCmd->SetRange("NeutronPerHitTrLen >= 0.0");
		fSetNeutronPerHitTrLenBoundCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

		fSetPionPlusPerHitTrLenBoundCmd = new G4UIcmdWithADoubleAndUnit("/RunAction/PionPlusPerHitTrLen", this);
		fSetPionPlusPerHitTrLenBoundCmd->SetGuidance("Set PionPlus track length per hit bound");
		fSetPionPlusPerHitTrLenBoundCmd->SetParameterName("PionPlusPerHitTrLen", false);
		fSetPionPlusPerHitTrLenBoundCmd->SetUnitCategory("Length");
		fSetPionPlusPerHitTrLenBoundCmd->SetRange("PionPlusPerHitTrLen >= 0.0");
		fSetPionPlusPerHitTrLenBoundCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

		fSetPionMinusPerHitTrLenBoundCmd = new G4UIcmdWithADoubleAndUnit("/RunAction/PionMinusPerHitTrLen", this);
		fSetPionMinusPerHitTrLenBoundCmd->SetGuidance("Set PionMinus track length per hit bound");
		fSetPionMinusPerHitTrLenBoundCmd->SetParameterName("PionMinusPerHitTrLen", false);
		fSetPionMinusPerHitTrLenBoundCmd->SetUnitCategory("Length");
		fSetPionMinusPerHitTrLenBoundCmd->SetRange("PionMinusPerHitTrLen >= 0.0");
		fSetPionMinusPerHitTrLenBoundCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

		fSetPionZeroPerHitTrLenBoundCmd = new G4UIcmdWithADoubleAndUnit("/RunAction/PionZeroPerHitTrLen", this);
		fSetPionZeroPerHitTrLenBoundCmd->SetGuidance("Set PionZero track length per hit bound");
		fSetPionZeroPerHitTrLenBoundCmd->SetParameterName("PionZeroPerHitTrLen", false);
		fSetPionZeroPerHitTrLenBoundCmd->SetUnitCategory("Length");
		fSetPionZeroPerHitTrLenBoundCmd->SetRange("PionZeroPerHitTrLen >= 0.0");
		fSetPionZeroPerHitTrLenBoundCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

		fSetMuonPlusPerHitTrLenBoundCmd = new G4UIcmdWithADoubleAndUnit("/RunAction/MuonPlusPerHitTrLen", this);
		fSetMuonPlusPerHitTrLenBoundCmd->SetGuidance("Set MuonPlus track length per hit bound");
		fSetMuonPlusPerHitTrLenBoundCmd->SetParameterName("MuonPlusPerHitTrLen", false);
		fSetMuonPlusPerHitTrLenBoundCmd->SetUnitCategory("Length");
		fSetMuonPlusPerHitTrLenBoundCmd->SetRange("MuonPlusPerHitTrLen >= 0.0");
		fSetMuonPlusPerHitTrLenBoundCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

		fSetMuonMinusPerHitTrLenBoundCmd = new G4UIcmdWithADoubleAndUnit("/RunAction/MuonMinusPerHitTrLen", this);
		fSetMuonMinusPerHitTrLenBoundCmd->SetGuidance("Set MuonMinus track length per hit bound");
		fSetMuonMinusPerHitTrLenBoundCmd->SetParameterName("MuonMinusPerHitTrLen", false);
		fSetMuonMinusPerHitTrLenBoundCmd->SetUnitCategory("Length");
		fSetMuonMinusPerHitTrLenBoundCmd->SetRange("MuonMinusPerHitTrLen >= 0.0");
		fSetMuonMinusPerHitTrLenBoundCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

		fSetOptPhotPerHitTrLenBoundCmd = new G4UIcmdWithADoubleAndUnit("/RunAction/OptPhotPerHitTrLen", this);
		fSetOptPhotPerHitTrLenBoundCmd->SetGuidance("Set OptPhot track length per hit bound");
		fSetOptPhotPerHitTrLenBoundCmd->SetParameterName("OptPhotPerHitTrLen", false);
		fSetOptPhotPerHitTrLenBoundCmd->SetUnitCategory("Length");
		fSetOptPhotPerHitTrLenBoundCmd->SetRange("OptPhotPerHitTrLen >= 0.0");
		fSetOptPhotPerHitTrLenBoundCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
	}

	// -------------------- multiplicity per event --------------------
	{
		fSetGammaPerEventMultMinBoundCmd = new G4UIcmdWithADouble("/RunAction/GammaPerEventMultMin", this);
		fSetGammaPerEventMultMinBoundCmd->SetGuidance("Set Gamma multiplicity minimum per event bound");
		fSetGammaPerEventMultMinBoundCmd->SetParameterName("GammaPerEventMultMin", false);
		fSetGammaPerEventMultMinBoundCmd->SetRange("GammaPerEventMultMin >= 0.0");
		fSetGammaPerEventMultMinBoundCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
		fSetGammaPerEventMultMaxBoundCmd = new G4UIcmdWithADouble("/RunAction/GammaPerEventMultMax", this);
		fSetGammaPerEventMultMaxBoundCmd->SetGuidance("Set Gamma multiplicity maximum per event bound");
		fSetGammaPerEventMultMaxBoundCmd->SetParameterName("GammaPerEventMultMax", false);
		fSetGammaPerEventMultMaxBoundCmd->SetRange("GammaPerEventMultMax >= 0.0");
		fSetGammaPerEventMultMaxBoundCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

		fSetPositronPerEventMultMinBoundCmd = new G4UIcmdWithADouble("/RunAction/PositronPerEventMultMin", this);
		fSetPositronPerEventMultMinBoundCmd->SetGuidance("Set Positron multiplicity minimum per event bound");
		fSetPositronPerEventMultMinBoundCmd->SetParameterName("PositronPerEventMultMin", false);
		fSetPositronPerEventMultMinBoundCmd->SetRange("PositronPerEventMultMin >= 0.0");
		fSetPositronPerEventMultMinBoundCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
		fSetPositronPerEventMultMaxBoundCmd = new G4UIcmdWithADouble("/RunAction/PositronPerEventMultMax", this);
		fSetPositronPerEventMultMaxBoundCmd->SetGuidance("Set Positron multiplicity maximum per event bound");
		fSetPositronPerEventMultMaxBoundCmd->SetParameterName("PositronPerEventMultMax", false);
		fSetPositronPerEventMultMaxBoundCmd->SetRange("PositronPerEventMultMax >= 0.0");
		fSetPositronPerEventMultMaxBoundCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

		fSetElectronPerEventMultMinBoundCmd = new G4UIcmdWithADouble("/RunAction/ElectronPerEventMultMin", this);
		fSetElectronPerEventMultMinBoundCmd->SetGuidance("Set Electron multiplicity minimum per event bound");
		fSetElectronPerEventMultMinBoundCmd->SetParameterName("ElectronPerEventMultMin", false);
		fSetElectronPerEventMultMinBoundCmd->SetRange("ElectronPerEventMultMin >= 0.0");
		fSetElectronPerEventMultMinBoundCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
		fSetElectronPerEventMultMaxBoundCmd = new G4UIcmdWithADouble("/RunAction/ElectronPerEventMultMax", this);
		fSetElectronPerEventMultMaxBoundCmd->SetGuidance("Set Electron multiplicity maximum per event bound");
		fSetElectronPerEventMultMaxBoundCmd->SetParameterName("ElectronPerEventMultMax", false);
		fSetElectronPerEventMultMaxBoundCmd->SetRange("ElectronPerEventMultMax >= 0.0");
		fSetElectronPerEventMultMaxBoundCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

		fSetProtonPerEventMultMinBoundCmd = new G4UIcmdWithADouble("/RunAction/ProtonPerEventMultMin", this);
		fSetProtonPerEventMultMinBoundCmd->SetGuidance("Set Proton multiplicity minimum per event bound");
		fSetProtonPerEventMultMinBoundCmd->SetParameterName("ProtonPerEventMultMin", false);
		fSetProtonPerEventMultMinBoundCmd->SetRange("ProtonPerEventMultMin >= 0.0");
		fSetProtonPerEventMultMinBoundCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
		fSetProtonPerEventMultMaxBoundCmd = new G4UIcmdWithADouble("/RunAction/ProtonPerEventMultMax", this);
		fSetProtonPerEventMultMaxBoundCmd->SetGuidance("Set Proton multiplicity maximum per event bound");
		fSetProtonPerEventMultMaxBoundCmd->SetParameterName("ProtonPerEventMultMax", false);
		fSetProtonPerEventMultMaxBoundCmd->SetRange("ProtonPerEventMultMax >= 0.0");
		fSetProtonPerEventMultMaxBoundCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

		fSetNeutronPerEventMultMinBoundCmd = new G4UIcmdWithADouble("/RunAction/NeutronPerEventMultMin", this);
		fSetNeutronPerEventMultMinBoundCmd->SetGuidance("Set Neutron multiplicity minimum per event bound");
		fSetNeutronPerEventMultMinBoundCmd->SetParameterName("NeutronPerEventMultMin", false);
		fSetNeutronPerEventMultMinBoundCmd->SetRange("NeutronPerEventMultMin >= 0.0");
		fSetNeutronPerEventMultMinBoundCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
		fSetNeutronPerEventMultMaxBoundCmd = new G4UIcmdWithADouble("/RunAction/NeutronPerEventMultMax", this);
		fSetNeutronPerEventMultMaxBoundCmd->SetGuidance("Set Neutron multiplicity maximum per event bound");
		fSetNeutronPerEventMultMaxBoundCmd->SetParameterName("NeutronPerEventMultMax", false);
		fSetNeutronPerEventMultMaxBoundCmd->SetRange("NeutronPerEventMultMax >= 0.0");
		fSetNeutronPerEventMultMaxBoundCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

		fSetPionPlusPerEventMultMinBoundCmd = new G4UIcmdWithADouble("/RunAction/PionPlusPerEventMultMin", this);
		fSetPionPlusPerEventMultMinBoundCmd->SetGuidance("Set PionPlus multiplicity minimum per event bound");
		fSetPionPlusPerEventMultMinBoundCmd->SetParameterName("PionPlusPerEventMultMin", false);
		fSetPionPlusPerEventMultMinBoundCmd->SetRange("PionPlusPerEventMultMin >= 0.0");
		fSetPionPlusPerEventMultMinBoundCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
		fSetPionPlusPerEventMultMaxBoundCmd = new G4UIcmdWithADouble("/RunAction/PionPlusPerEventMultMax", this);
		fSetPionPlusPerEventMultMaxBoundCmd->SetGuidance("Set PionPlus multiplicity maximum per event bound");
		fSetPionPlusPerEventMultMaxBoundCmd->SetParameterName("PionPlusPerEventMultMax", false);
		fSetPionPlusPerEventMultMaxBoundCmd->SetRange("PionPlusPerEventMultMax >= 0.0");
		fSetPionPlusPerEventMultMaxBoundCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

		fSetPionMinusPerEventMultMinBoundCmd = new G4UIcmdWithADouble("/RunAction/PionMinusPerEventMultMin", this);
		fSetPionMinusPerEventMultMinBoundCmd->SetGuidance("Set PionMinus multiplicity minimum per event bound");
		fSetPionMinusPerEventMultMinBoundCmd->SetParameterName("PionMinusPerEventMultMin", false);
		fSetPionMinusPerEventMultMinBoundCmd->SetRange("PionMinusPerEventMultMin >= 0.0");
		fSetPionMinusPerEventMultMinBoundCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
		fSetPionMinusPerEventMultMaxBoundCmd = new G4UIcmdWithADouble("/RunAction/PionMinusPerEventMultMax", this);
		fSetPionMinusPerEventMultMaxBoundCmd->SetGuidance("Set PionMinus multiplicity maximum per event bound");
		fSetPionMinusPerEventMultMaxBoundCmd->SetParameterName("PionMinusPerEventMultMax", false);
		fSetPionMinusPerEventMultMaxBoundCmd->SetRange("PionMinusPerEventMultMax >= 0.0");
		fSetPionMinusPerEventMultMaxBoundCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

		fSetPionZeroPerEventMultMinBoundCmd = new G4UIcmdWithADouble("/RunAction/PionZeroPerEventMultMin", this);
		fSetPionZeroPerEventMultMinBoundCmd->SetGuidance("Set PionZero multiplicity minimum per event bound");
		fSetPionZeroPerEventMultMinBoundCmd->SetParameterName("PionZeroPerEventMultMin", false);
		fSetPionZeroPerEventMultMinBoundCmd->SetRange("PionZeroPerEventMultMin >= 0.0");
		fSetPionZeroPerEventMultMinBoundCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
		fSetPionZeroPerEventMultMaxBoundCmd = new G4UIcmdWithADouble("/RunAction/PionZeroPerEventMultMax", this);
		fSetPionZeroPerEventMultMaxBoundCmd->SetGuidance("Set PionZero multiplicity maximum per event bound");
		fSetPionZeroPerEventMultMaxBoundCmd->SetParameterName("PionZeroPerEventMultMax", false);
		fSetPionZeroPerEventMultMaxBoundCmd->SetRange("PionZeroPerEventMultMax >= 0.0");
		fSetPionZeroPerEventMultMaxBoundCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

		fSetMuonPlusPerEventMultMinBoundCmd = new G4UIcmdWithADouble("/RunAction/MuonPlusPerEventMultMin", this);
		fSetMuonPlusPerEventMultMinBoundCmd->SetGuidance("Set MuonPlus multiplicity minimum per event bound");
		fSetMuonPlusPerEventMultMinBoundCmd->SetParameterName("MuonPlusPerEventMultMin", false);
		fSetMuonPlusPerEventMultMinBoundCmd->SetRange("MuonPlusPerEventMultMin >= 0.0");
		fSetMuonPlusPerEventMultMinBoundCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
		fSetMuonPlusPerEventMultMaxBoundCmd = new G4UIcmdWithADouble("/RunAction/MuonPlusPerEventMultMax", this);
		fSetMuonPlusPerEventMultMaxBoundCmd->SetGuidance("Set MuonPlus multiplicity maximum per event bound");
		fSetMuonPlusPerEventMultMaxBoundCmd->SetParameterName("MuonPlusPerEventMultMax", false);
		fSetMuonPlusPerEventMultMaxBoundCmd->SetRange("MuonPlusPerEventMultMax >= 0.0");
		fSetMuonPlusPerEventMultMaxBoundCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

		fSetMuonMinusPerEventMultMinBoundCmd = new G4UIcmdWithADouble("/RunAction/MuonMinusPerEventMultMin", this);
		fSetMuonMinusPerEventMultMinBoundCmd->SetGuidance("Set MuonMinus multiplicity minimum per event bound");
		fSetMuonMinusPerEventMultMinBoundCmd->SetParameterName("MuonMinusPerEventMultMin", false);
		fSetMuonMinusPerEventMultMinBoundCmd->SetRange("MuonMinusPerEventMultMin >= 0.0");
		fSetMuonMinusPerEventMultMinBoundCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
		fSetMuonMinusPerEventMultMaxBoundCmd = new G4UIcmdWithADouble("/RunAction/MuonMinusPerEventMultMax", this);
		fSetMuonMinusPerEventMultMaxBoundCmd->SetGuidance("Set MuonMinus multiplicity maximum per event bound");
		fSetMuonMinusPerEventMultMaxBoundCmd->SetParameterName("MuonMinusPerEventMultMax", false);
		fSetMuonMinusPerEventMultMaxBoundCmd->SetRange("MuonMinusPerEventMultMax >= 0.0");
		fSetMuonMinusPerEventMultMaxBoundCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

		fSetOptPhotPerEventMultMinBoundCmd = new G4UIcmdWithADouble("/RunAction/OptPhotPerEventMultMin", this);
		fSetOptPhotPerEventMultMinBoundCmd->SetGuidance("Set OptPhot multiplicity minimum per event bound");
		fSetOptPhotPerEventMultMinBoundCmd->SetParameterName("OptPhotPerEventMultMin", false);
		fSetOptPhotPerEventMultMinBoundCmd->SetRange("OptPhotPerEventMultMin >= 0.0");
		fSetOptPhotPerEventMultMinBoundCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
		fSetOptPhotPerEventMultMaxBoundCmd = new G4UIcmdWithADouble("/RunAction/OptPhotPerEventMultMax", this);
		fSetOptPhotPerEventMultMaxBoundCmd->SetGuidance("Set OptPhot multiplicity maximum per event bound");
		fSetOptPhotPerEventMultMaxBoundCmd->SetParameterName("OptPhotPerEventMultMax", false);
		fSetOptPhotPerEventMultMaxBoundCmd->SetRange("OptPhotPerEventMultMax >= 0.0");
		fSetOptPhotPerEventMultMaxBoundCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
	}

	// -------------------- energy deposition per event bound --------------------
	{
		fSetGammaPerEventEDepBoundCmd = new G4UIcmdWithADoubleAndUnit("/RunAction/GammaPerEventEDep", this);
		fSetGammaPerEventEDepBoundCmd->SetGuidance("Set Gamma energy deposition per event bound");
		fSetGammaPerEventEDepBoundCmd->SetParameterName("GammaPerEventEDep", false);
		fSetGammaPerEventEDepBoundCmd->SetUnitCategory("Energy");
		fSetGammaPerEventEDepBoundCmd->SetRange("GammaPerEventEDep >= 0.0");
		fSetGammaPerEventEDepBoundCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

		fSetPositronPerEventEDepBoundCmd = new G4UIcmdWithADoubleAndUnit("/RunAction/PositronPerEventEDep", this);
		fSetPositronPerEventEDepBoundCmd->SetGuidance("Set Positron energy deposition per event bound");
		fSetPositronPerEventEDepBoundCmd->SetParameterName("PositronPerEventEDep", false);
		fSetPositronPerEventEDepBoundCmd->SetUnitCategory("Energy");
		fSetPositronPerEventEDepBoundCmd->SetRange("PositronPerEventEDep >= 0.0");
		fSetPositronPerEventEDepBoundCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

		fSetElectronPerEventEDepBoundCmd = new G4UIcmdWithADoubleAndUnit("/RunAction/ElectronPerEventEDep", this);
		fSetElectronPerEventEDepBoundCmd->SetGuidance("Set Electron energy deposition per event bound");
		fSetElectronPerEventEDepBoundCmd->SetParameterName("ElectronPerEventEDep", false);
		fSetElectronPerEventEDepBoundCmd->SetUnitCategory("Energy");
		fSetElectronPerEventEDepBoundCmd->SetRange("ElectronPerEventEDep >= 0.0");
		fSetElectronPerEventEDepBoundCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

		fSetProtonPerEventEDepBoundCmd = new G4UIcmdWithADoubleAndUnit("/RunAction/ProtonPerEventEDep", this);
		fSetProtonPerEventEDepBoundCmd->SetGuidance("Set Proton energy deposition per event bound");
		fSetProtonPerEventEDepBoundCmd->SetParameterName("ProtonPerEventEDep", false);
		fSetProtonPerEventEDepBoundCmd->SetUnitCategory("Energy");
		fSetProtonPerEventEDepBoundCmd->SetRange("ProtonPerEventEDep >= 0.0");
		fSetProtonPerEventEDepBoundCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

		fSetNeutronPerEventEDepBoundCmd = new G4UIcmdWithADoubleAndUnit("/RunAction/NeutronPerEventEDep", this);
		fSetNeutronPerEventEDepBoundCmd->SetGuidance("Set Neutron energy deposition per event bound");
		fSetNeutronPerEventEDepBoundCmd->SetParameterName("NeutronPerEventEDep", false);
		fSetNeutronPerEventEDepBoundCmd->SetUnitCategory("Energy");
		fSetNeutronPerEventEDepBoundCmd->SetRange("NeutronPerEventEDep >= 0.0");
		fSetNeutronPerEventEDepBoundCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

		fSetPionPlusPerEventEDepBoundCmd = new G4UIcmdWithADoubleAndUnit("/RunAction/PionPlusPerEventEDep", this);
		fSetPionPlusPerEventEDepBoundCmd->SetGuidance("Set PionPlus energy deposition per event bound");
		fSetPionPlusPerEventEDepBoundCmd->SetParameterName("PionPlusPerEventEDep", false);
		fSetPionPlusPerEventEDepBoundCmd->SetUnitCategory("Energy");
		fSetPionPlusPerEventEDepBoundCmd->SetRange("PionPlusPerEventEDep >= 0.0");
		fSetPionPlusPerEventEDepBoundCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

		fSetPionMinusPerEventEDepBoundCmd = new G4UIcmdWithADoubleAndUnit("/RunAction/PionMinusPerEventEDep", this);
		fSetPionMinusPerEventEDepBoundCmd->SetGuidance("Set PionMinus energy deposition per event bound");
		fSetPionMinusPerEventEDepBoundCmd->SetParameterName("PionMinusPerEventEDep", false);
		fSetPionMinusPerEventEDepBoundCmd->SetUnitCategory("Energy");
		fSetPionMinusPerEventEDepBoundCmd->SetRange("PionMinusPerEventEDep >= 0.0");
		fSetPionMinusPerEventEDepBoundCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

		fSetPionZeroPerEventEDepBoundCmd = new G4UIcmdWithADoubleAndUnit("/RunAction/PionZeroPerEventEDep", this);
		fSetPionZeroPerEventEDepBoundCmd->SetGuidance("Set PionZero energy deposition per event bound");
		fSetPionZeroPerEventEDepBoundCmd->SetParameterName("PionZeroPerEventEDep", false);
		fSetPionZeroPerEventEDepBoundCmd->SetUnitCategory("Energy");
		fSetPionZeroPerEventEDepBoundCmd->SetRange("PionZeroPerEventEDep >= 0.0");
		fSetPionZeroPerEventEDepBoundCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

		fSetMuonPlusPerEventEDepBoundCmd = new G4UIcmdWithADoubleAndUnit("/RunAction/MuonPlusPerEventEDep", this);
		fSetMuonPlusPerEventEDepBoundCmd->SetGuidance("Set MuonPlus energy deposition per event bound");
		fSetMuonPlusPerEventEDepBoundCmd->SetParameterName("MuonPlusPerEventEDep", false);
		fSetMuonPlusPerEventEDepBoundCmd->SetUnitCategory("Energy");
		fSetMuonPlusPerEventEDepBoundCmd->SetRange("MuonPlusPerEventEDep >= 0.0");
		fSetMuonPlusPerEventEDepBoundCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

		fSetMuonMinusPerEventEDepBoundCmd = new G4UIcmdWithADoubleAndUnit("/RunAction/MuonMinusPerEventEDep", this);
		fSetMuonMinusPerEventEDepBoundCmd->SetGuidance("Set MuonMinus energy deposition per event bound");
		fSetMuonMinusPerEventEDepBoundCmd->SetParameterName("MuonMinusPerEventEDep", false);
		fSetMuonMinusPerEventEDepBoundCmd->SetUnitCategory("Energy");
		fSetMuonMinusPerEventEDepBoundCmd->SetRange("MuonMinusPerEventEDep >= 0.0");
		fSetMuonMinusPerEventEDepBoundCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

		fSetOptPhotPerEventEDepBoundCmd = new G4UIcmdWithADoubleAndUnit("/RunAction/OptPhotPerEventEDep", this);
		fSetOptPhotPerEventEDepBoundCmd->SetGuidance("Set OptPhot energy deposition per event bound");
		fSetOptPhotPerEventEDepBoundCmd->SetParameterName("OptPhotPerEventEDep", false);
		fSetOptPhotPerEventEDepBoundCmd->SetUnitCategory("Energy");
		fSetOptPhotPerEventEDepBoundCmd->SetRange("OptPhotPerEventEDep >= 0.0");
		fSetOptPhotPerEventEDepBoundCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
	}

	// -------------------- initial energy per event bound --------------------
	{
		fSetGammaPerEventInitEBoundCmd = new G4UIcmdWithADoubleAndUnit("/RunAction/GammaPerEventInitE", this);
		fSetGammaPerEventInitEBoundCmd->SetGuidance("Set Gamma initial energy per event bound");
		fSetGammaPerEventInitEBoundCmd->SetParameterName("GammaPerEventInitE", false);
		fSetGammaPerEventInitEBoundCmd->SetUnitCategory("Energy");
		fSetGammaPerEventInitEBoundCmd->SetRange("GammaPerEventInitE >= 0.0");
		fSetGammaPerEventInitEBoundCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

		fSetPositronPerEventInitEBoundCmd = new G4UIcmdWithADoubleAndUnit("/RunAction/PositronPerEventInitE", this);
		fSetPositronPerEventInitEBoundCmd->SetGuidance("Set Positron initial energy per event bound");
		fSetPositronPerEventInitEBoundCmd->SetParameterName("PositronPerEventInitE", false);
		fSetPositronPerEventInitEBoundCmd->SetUnitCategory("Energy");
		fSetPositronPerEventInitEBoundCmd->SetRange("PositronPerEventInitE >= 0.0");
		fSetPositronPerEventInitEBoundCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

		fSetElectronPerEventInitEBoundCmd = new G4UIcmdWithADoubleAndUnit("/RunAction/ElectronPerEventInitE", this);
		fSetElectronPerEventInitEBoundCmd->SetGuidance("Set Electron initial energy per event bound");
		fSetElectronPerEventInitEBoundCmd->SetParameterName("ElectronPerEventInitE", false);
		fSetElectronPerEventInitEBoundCmd->SetUnitCategory("Energy");
		fSetElectronPerEventInitEBoundCmd->SetRange("ElectronPerEventInitE >= 0.0");
		fSetElectronPerEventInitEBoundCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

		fSetProtonPerEventInitEBoundCmd = new G4UIcmdWithADoubleAndUnit("/RunAction/ProtonPerEventInitE", this);
		fSetProtonPerEventInitEBoundCmd->SetGuidance("Set Proton initial energy per event bound");
		fSetProtonPerEventInitEBoundCmd->SetParameterName("ProtonPerEventInitE", false);
		fSetProtonPerEventInitEBoundCmd->SetUnitCategory("Energy");
		fSetProtonPerEventInitEBoundCmd->SetRange("ProtonPerEventInitE >= 0.0");
		fSetProtonPerEventInitEBoundCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

		fSetNeutronPerEventInitEBoundCmd = new G4UIcmdWithADoubleAndUnit("/RunAction/NeutronPerEventInitE", this);
		fSetNeutronPerEventInitEBoundCmd->SetGuidance("Set Neutron initial energy per event bound");
		fSetNeutronPerEventInitEBoundCmd->SetParameterName("NeutronPerEventInitE", false);
		fSetNeutronPerEventInitEBoundCmd->SetUnitCategory("Energy");
		fSetNeutronPerEventInitEBoundCmd->SetRange("NeutronPerEventInitE >= 0.0");
		fSetNeutronPerEventInitEBoundCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

		fSetPionPlusPerEventInitEBoundCmd = new G4UIcmdWithADoubleAndUnit("/RunAction/PionPlusPerEventInitE", this);
		fSetPionPlusPerEventInitEBoundCmd->SetGuidance("Set PionPlus initial energy per event bound");
		fSetPionPlusPerEventInitEBoundCmd->SetParameterName("PionPlusPerEventInitE", false);
		fSetPionPlusPerEventInitEBoundCmd->SetUnitCategory("Energy");
		fSetPionPlusPerEventInitEBoundCmd->SetRange("PionPlusPerEventInitE >= 0.0");
		fSetPionPlusPerEventInitEBoundCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

		fSetPionMinusPerEventInitEBoundCmd = new G4UIcmdWithADoubleAndUnit("/RunAction/PionMinusPerEventInitE", this);
		fSetPionMinusPerEventInitEBoundCmd->SetGuidance("Set PionMinus initial energy per event bound");
		fSetPionMinusPerEventInitEBoundCmd->SetParameterName("PionMinusPerEventInitE", false);
		fSetPionMinusPerEventInitEBoundCmd->SetUnitCategory("Energy");
		fSetPionMinusPerEventInitEBoundCmd->SetRange("PionMinusPerEventInitE >= 0.0");
		fSetPionMinusPerEventInitEBoundCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

		fSetPionZeroPerEventInitEBoundCmd = new G4UIcmdWithADoubleAndUnit("/RunAction/PionZeroPerEventInitE", this);
		fSetPionZeroPerEventInitEBoundCmd->SetGuidance("Set PionZero initial energy per event bound");
		fSetPionZeroPerEventInitEBoundCmd->SetParameterName("PionZeroPerEventInitE", false);
		fSetPionZeroPerEventInitEBoundCmd->SetUnitCategory("Energy");
		fSetPionZeroPerEventInitEBoundCmd->SetRange("PionZeroPerEventInitE >= 0.0");
		fSetPionZeroPerEventInitEBoundCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

		fSetMuonPlusPerEventInitEBoundCmd = new G4UIcmdWithADoubleAndUnit("/RunAction/MuonPlusPerEventInitE", this);
		fSetMuonPlusPerEventInitEBoundCmd->SetGuidance("Set MuonPlus initial energy per event bound");
		fSetMuonPlusPerEventInitEBoundCmd->SetParameterName("MuonPlusPerEventInitE", false);
		fSetMuonPlusPerEventInitEBoundCmd->SetUnitCategory("Energy");
		fSetMuonPlusPerEventInitEBoundCmd->SetRange("MuonPlusPerEventInitE >= 0.0");
		fSetMuonPlusPerEventInitEBoundCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

		fSetMuonMinusPerEventInitEBoundCmd = new G4UIcmdWithADoubleAndUnit("/RunAction/MuonMinusPerEventInitE", this);
		fSetMuonMinusPerEventInitEBoundCmd->SetGuidance("Set MuonMinus initial energy per event bound");
		fSetMuonMinusPerEventInitEBoundCmd->SetParameterName("MuonMinusPerEventInitE", false);
		fSetMuonMinusPerEventInitEBoundCmd->SetUnitCategory("Energy");
		fSetMuonMinusPerEventInitEBoundCmd->SetRange("MuonMinusPerEventInitE >= 0.0");
		fSetMuonMinusPerEventInitEBoundCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

		fSetOptPhotPerEventInitEBoundCmd = new G4UIcmdWithADoubleAndUnit("/RunAction/OptPhotPerEventInitE", this);
		fSetOptPhotPerEventInitEBoundCmd->SetGuidance("Set OptPhot initial energy per event bound");
		fSetOptPhotPerEventInitEBoundCmd->SetParameterName("OptPhotPerEventInitE", false);
		fSetOptPhotPerEventInitEBoundCmd->SetUnitCategory("Energy");
		fSetOptPhotPerEventInitEBoundCmd->SetRange("OptPhotPerEventInitE >= 0.0");
		fSetOptPhotPerEventInitEBoundCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
	}

	fSetPerEventEDepBoundCmd = new G4UIcmdWithADoubleAndUnit("/RunAction/PerEventEDep", this);
	fSetPerEventEDepBoundCmd->SetGuidance("Set energy deposition per event bound");
	fSetPerEventEDepBoundCmd->SetParameterName("PerEventEDep", false);
	fSetPerEventEDepBoundCmd->SetUnitCategory("Energy");
	fSetPerEventEDepBoundCmd->SetRange("PerEventEDep >= 0.0");
	fSetPerEventEDepBoundCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

	fSetPerEventUntrackedEBoundCmd = new G4UIcmdWithADoubleAndUnit("/RunAction/PerEventUntrackedE", this);
	fSetPerEventUntrackedEBoundCmd->SetGuidance("Set untracked energy deposition per event bound");
	fSetPerEventUntrackedEBoundCmd->SetParameterName("PerEventUntrackedE", false);
	fSetPerEventUntrackedEBoundCmd->SetUnitCategory("Energy");
	fSetPerEventUntrackedEBoundCmd->SetRange("PerEventUntrackedE >= 0.0");
	fSetPerEventUntrackedEBoundCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunActionMessenger::~RunActionMessenger()
{
	delete fSetFileNameCmd;

	delete fSetGammaPerHitEDepBoundCmd;
	delete fSetPositronPerHitEDepBoundCmd;
	delete fSetElectronPerHitEDepBoundCmd;
	delete fSetProtonPerHitEDepBoundCmd;
	delete fSetNeutronPerHitEDepBoundCmd;
	delete fSetPionPlusPerHitEDepBoundCmd;
	delete fSetPionMinusPerHitEDepBoundCmd;
	delete fSetPionZeroPerHitEDepBoundCmd;
	delete fSetMuonPlusPerHitEDepBoundCmd;
	delete fSetMuonMinusPerHitEDepBoundCmd;
	delete fSetOptPhotPerHitEDepBoundCmd;

	delete fSetGammaPerHitTrLenBoundCmd;
	delete fSetPositronPerHitTrLenBoundCmd;
	delete fSetElectronPerHitTrLenBoundCmd;
	delete fSetProtonPerHitTrLenBoundCmd;
	delete fSetNeutronPerHitTrLenBoundCmd;
	delete fSetPionPlusPerHitTrLenBoundCmd;
	delete fSetPionMinusPerHitTrLenBoundCmd;
	delete fSetPionZeroPerHitTrLenBoundCmd;
	delete fSetMuonPlusPerHitTrLenBoundCmd;
	delete fSetMuonMinusPerHitTrLenBoundCmd;
	delete fSetOptPhotPerHitTrLenBoundCmd;

	delete fSetGammaPerEventMultMinBoundCmd;
	delete fSetGammaPerEventMultMaxBoundCmd;
	delete fSetPositronPerEventMultMinBoundCmd;
	delete fSetPositronPerEventMultMaxBoundCmd;
	delete fSetElectronPerEventMultMinBoundCmd;
	delete fSetElectronPerEventMultMaxBoundCmd;
	delete fSetProtonPerEventMultMinBoundCmd;
	delete fSetProtonPerEventMultMaxBoundCmd;
	delete fSetNeutronPerEventMultMinBoundCmd;
	delete fSetNeutronPerEventMultMaxBoundCmd;
	delete fSetPionPlusPerEventMultMinBoundCmd;
	delete fSetPionPlusPerEventMultMaxBoundCmd;
	delete fSetPionMinusPerEventMultMinBoundCmd;
	delete fSetPionMinusPerEventMultMaxBoundCmd;
	delete fSetPionZeroPerEventMultMinBoundCmd;
	delete fSetPionZeroPerEventMultMaxBoundCmd;
	delete fSetMuonPlusPerEventMultMinBoundCmd;
	delete fSetMuonPlusPerEventMultMaxBoundCmd;
	delete fSetMuonMinusPerEventMultMinBoundCmd;
	delete fSetMuonMinusPerEventMultMaxBoundCmd;
	delete fSetOptPhotPerEventMultMinBoundCmd;
	delete fSetOptPhotPerEventMultMaxBoundCmd;

	delete fSetGammaPerEventEDepBoundCmd;
	delete fSetPositronPerEventEDepBoundCmd;
	delete fSetElectronPerEventEDepBoundCmd;
	delete fSetProtonPerEventEDepBoundCmd;
	delete fSetNeutronPerEventEDepBoundCmd;
	delete fSetPionPlusPerEventEDepBoundCmd;
	delete fSetPionMinusPerEventEDepBoundCmd;
	delete fSetPionZeroPerEventEDepBoundCmd;
	delete fSetMuonPlusPerEventEDepBoundCmd;
	delete fSetMuonMinusPerEventEDepBoundCmd;
	delete fSetOptPhotPerEventEDepBoundCmd;

	delete fSetGammaPerEventInitEBoundCmd;
	delete fSetPositronPerEventInitEBoundCmd;
	delete fSetElectronPerEventInitEBoundCmd;
	delete fSetProtonPerEventInitEBoundCmd;
	delete fSetNeutronPerEventInitEBoundCmd;
	delete fSetPionPlusPerEventInitEBoundCmd;
	delete fSetPionMinusPerEventInitEBoundCmd;
	delete fSetPionZeroPerEventInitEBoundCmd;
	delete fSetMuonPlusPerEventInitEBoundCmd;
	delete fSetMuonMinusPerEventInitEBoundCmd;
	delete fSetOptPhotPerEventInitEBoundCmd;

	delete fSetPerEventEDepBoundCmd;
	delete fSetPerEventUntrackedEBoundCmd;

	delete fRunActionDir;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunActionMessenger::SetNewValue(G4UIcommand* cmd, G4String par)
{
	if(cmd == fSetFileNameCmd) fFileName = par;

	// -------------------- energy deposition per hit bound --------------------
	{
		if(cmd == fSetGammaPerHitEDepBoundCmd)
			fGammaPerHitEDepBound = fSetGammaPerHitEDepBoundCmd->GetNewDoubleValue(par);
		if(cmd == fSetPositronPerHitEDepBoundCmd)
			fPositronPerHitEDepBound = fSetPositronPerHitEDepBoundCmd->GetNewDoubleValue(par);
		if(cmd == fSetElectronPerHitEDepBoundCmd)
			fElectronPerHitEDepBound = fSetElectronPerHitEDepBoundCmd->GetNewDoubleValue(par);
		if(cmd == fSetProtonPerHitEDepBoundCmd)
			fProtonPerHitEDepBound = fSetProtonPerHitEDepBoundCmd->GetNewDoubleValue(par);
		if(cmd == fSetNeutronPerHitEDepBoundCmd)
			fNeutronPerHitEDepBound = fSetNeutronPerHitEDepBoundCmd->GetNewDoubleValue(par);
		if(cmd == fSetPionPlusPerHitEDepBoundCmd)
			fPionPlusPerHitEDepBound = fSetPionPlusPerHitEDepBoundCmd->GetNewDoubleValue(par);
		if(cmd == fSetPionMinusPerHitEDepBoundCmd)
			fPionMinusPerHitEDepBound = fSetPionMinusPerHitEDepBoundCmd->GetNewDoubleValue(par);
		if(cmd == fSetPionZeroPerHitEDepBoundCmd)
			fPionZeroPerHitEDepBound = fSetPionZeroPerHitEDepBoundCmd->GetNewDoubleValue(par);
		if(cmd == fSetMuonPlusPerHitEDepBoundCmd)
			fMuonPlusPerHitEDepBound = fSetMuonPlusPerHitEDepBoundCmd->GetNewDoubleValue(par);
		if(cmd == fSetMuonMinusPerHitEDepBoundCmd)
			fMuonMinusPerHitEDepBound = fSetMuonMinusPerHitEDepBoundCmd->GetNewDoubleValue(par);
		if(cmd == fSetOptPhotPerHitEDepBoundCmd)
			fOptPhotPerHitEDepBound = fSetGammaPerHitEDepBoundCmd->GetNewDoubleValue(par);
	}

	// -------------------- track length per hit bound --------------------
	{
		if(cmd == fSetGammaPerHitTrLenBoundCmd)
			fGammaPerHitTrLenBound = fSetGammaPerHitTrLenBoundCmd->GetNewDoubleValue(par);
		if(cmd == fSetPositronPerHitTrLenBoundCmd)
			fPositronPerHitTrLenBound = fSetPositronPerHitTrLenBoundCmd->GetNewDoubleValue(par);
		if(cmd == fSetElectronPerHitTrLenBoundCmd)
			fElectronPerHitTrLenBound = fSetElectronPerHitTrLenBoundCmd->GetNewDoubleValue(par);
		if(cmd == fSetProtonPerHitTrLenBoundCmd)
			fProtonPerHitTrLenBound = fSetProtonPerHitTrLenBoundCmd->GetNewDoubleValue(par);
		if(cmd == fSetNeutronPerHitTrLenBoundCmd)
			fNeutronPerHitTrLenBound = fSetNeutronPerHitTrLenBoundCmd->GetNewDoubleValue(par);
		if(cmd == fSetPionPlusPerHitTrLenBoundCmd)
			fPionPlusPerHitTrLenBound = fSetPionPlusPerHitTrLenBoundCmd->GetNewDoubleValue(par);
		if(cmd == fSetPionMinusPerHitTrLenBoundCmd)
			fPionMinusPerHitTrLenBound = fSetPionMinusPerHitTrLenBoundCmd->GetNewDoubleValue(par);
		if(cmd == fSetPionZeroPerHitTrLenBoundCmd)
			fPionZeroPerHitTrLenBound = fSetPionZeroPerHitTrLenBoundCmd->GetNewDoubleValue(par);
		if(cmd == fSetMuonPlusPerHitTrLenBoundCmd)
			fMuonPlusPerHitTrLenBound = fSetMuonPlusPerHitTrLenBoundCmd->GetNewDoubleValue(par);
		if(cmd == fSetMuonMinusPerHitTrLenBoundCmd)
			fMuonMinusPerHitTrLenBound = fSetMuonMinusPerHitTrLenBoundCmd->GetNewDoubleValue(par);
		if(cmd == fSetOptPhotPerHitTrLenBoundCmd)
			fOptPhotPerHitTrLenBound = fSetGammaPerHitTrLenBoundCmd->GetNewDoubleValue(par);
	}

	// -------------------- multiplicity per event --------------------
	{
		if(cmd == fSetGammaPerEventMultMinBoundCmd)
			fGammaPerEventMultMinBound = fSetGammaPerEventMultMinBoundCmd->GetNewDoubleValue(par);
		if(cmd == fSetGammaPerEventMultMaxBoundCmd)
			fGammaPerEventMultMaxBound = fSetGammaPerEventMultMaxBoundCmd->GetNewDoubleValue(par);

		if(cmd == fSetPositronPerEventMultMinBoundCmd)
			fPositronPerEventMultMinBound = fSetPositronPerEventMultMinBoundCmd->GetNewDoubleValue(par);
		if(cmd == fSetPositronPerEventMultMaxBoundCmd)
			fPositronPerEventMultMaxBound = fSetPositronPerEventMultMaxBoundCmd->GetNewDoubleValue(par);

		if(cmd == fSetElectronPerEventMultMinBoundCmd)
			fElectronPerEventMultMinBound = fSetElectronPerEventMultMinBoundCmd->GetNewDoubleValue(par);
		if(cmd == fSetElectronPerEventMultMaxBoundCmd)
			fElectronPerEventMultMaxBound = fSetElectronPerEventMultMaxBoundCmd->GetNewDoubleValue(par);

		if(cmd == fSetProtonPerEventMultMinBoundCmd)
			fProtonPerEventMultMinBound = fSetProtonPerEventMultMinBoundCmd->GetNewDoubleValue(par);
		if(cmd == fSetProtonPerEventMultMaxBoundCmd)
			fProtonPerEventMultMaxBound = fSetProtonPerEventMultMaxBoundCmd->GetNewDoubleValue(par);

		if(cmd == fSetNeutronPerEventMultMinBoundCmd)
			fNeutronPerEventMultMinBound = fSetNeutronPerEventMultMinBoundCmd->GetNewDoubleValue(par);
		if(cmd == fSetNeutronPerEventMultMaxBoundCmd)
			fNeutronPerEventMultMaxBound = fSetNeutronPerEventMultMaxBoundCmd->GetNewDoubleValue(par);

		if(cmd == fSetPionPlusPerEventMultMinBoundCmd)
			fPionPlusPerEventMultMinBound = fSetPionPlusPerEventMultMinBoundCmd->GetNewDoubleValue(par);
		if(cmd == fSetPionPlusPerEventMultMaxBoundCmd)
			fPionPlusPerEventMultMaxBound = fSetPionPlusPerEventMultMaxBoundCmd->GetNewDoubleValue(par);

		if(cmd == fSetPionMinusPerEventMultMinBoundCmd)
			fPionMinusPerEventMultMinBound = fSetPionMinusPerEventMultMinBoundCmd->GetNewDoubleValue(par);
		if(cmd == fSetPionMinusPerEventMultMaxBoundCmd)
			fPionMinusPerEventMultMaxBound = fSetPionMinusPerEventMultMaxBoundCmd->GetNewDoubleValue(par);

		if(cmd == fSetPionZeroPerEventMultMinBoundCmd)
			fPionZeroPerEventMultMinBound = fSetPionZeroPerEventMultMinBoundCmd->GetNewDoubleValue(par);
		if(cmd == fSetPionZeroPerEventMultMaxBoundCmd)
			fPionZeroPerEventMultMaxBound = fSetPionZeroPerEventMultMaxBoundCmd->GetNewDoubleValue(par);

		if(cmd == fSetMuonPlusPerEventMultMinBoundCmd)
			fMuonPlusPerEventMultMinBound = fSetMuonPlusPerEventMultMinBoundCmd->GetNewDoubleValue(par);
		if(cmd == fSetMuonPlusPerEventMultMaxBoundCmd)
			fMuonPlusPerEventMultMaxBound = fSetMuonPlusPerEventMultMaxBoundCmd->GetNewDoubleValue(par);

		if(cmd == fSetMuonMinusPerEventMultMinBoundCmd)
			fMuonMinusPerEventMultMinBound = fSetMuonMinusPerEventMultMinBoundCmd->GetNewDoubleValue(par);
		if(cmd == fSetMuonMinusPerEventMultMaxBoundCmd)
			fMuonMinusPerEventMultMaxBound = fSetMuonMinusPerEventMultMaxBoundCmd->GetNewDoubleValue(par);

		if(cmd == fSetOptPhotPerEventMultMinBoundCmd)
			fOptPhotPerEventMultMinBound = fSetGammaPerEventMultMinBoundCmd->GetNewDoubleValue(par);
		if(cmd == fSetOptPhotPerEventMultMaxBoundCmd)
			fOptPhotPerEventMultMaxBound = fSetGammaPerEventMultMaxBoundCmd->GetNewDoubleValue(par);
	}

	// -------------------- energy deposition per event bound --------------------
	{
		if(cmd == fSetGammaPerEventEDepBoundCmd)
			fGammaPerEventEDepBound = fSetGammaPerEventEDepBoundCmd->GetNewDoubleValue(par);
		if(cmd == fSetPositronPerEventEDepBoundCmd)
			fPositronPerEventEDepBound = fSetPositronPerEventEDepBoundCmd->GetNewDoubleValue(par);
		if(cmd == fSetElectronPerEventEDepBoundCmd)
			fElectronPerEventEDepBound = fSetElectronPerEventEDepBoundCmd->GetNewDoubleValue(par);
		if(cmd == fSetProtonPerEventEDepBoundCmd)
			fProtonPerEventEDepBound = fSetProtonPerEventEDepBoundCmd->GetNewDoubleValue(par);
		if(cmd == fSetNeutronPerEventEDepBoundCmd)
			fNeutronPerEventEDepBound = fSetNeutronPerEventEDepBoundCmd->GetNewDoubleValue(par);
		if(cmd == fSetPionPlusPerEventEDepBoundCmd)
			fPionPlusPerEventEDepBound = fSetPionPlusPerEventEDepBoundCmd->GetNewDoubleValue(par);
		if(cmd == fSetPionMinusPerEventEDepBoundCmd)
			fPionMinusPerEventEDepBound = fSetPionMinusPerEventEDepBoundCmd->GetNewDoubleValue(par);
		if(cmd == fSetPionZeroPerEventEDepBoundCmd)
			fPionZeroPerEventEDepBound = fSetPionZeroPerEventEDepBoundCmd->GetNewDoubleValue(par);
		if(cmd == fSetMuonPlusPerEventEDepBoundCmd)
			fMuonPlusPerEventEDepBound = fSetMuonPlusPerEventEDepBoundCmd->GetNewDoubleValue(par);
		if(cmd == fSetMuonMinusPerEventEDepBoundCmd)
			fMuonMinusPerEventEDepBound = fSetMuonMinusPerEventEDepBoundCmd->GetNewDoubleValue(par);
		if(cmd == fSetOptPhotPerEventEDepBoundCmd)
			fOptPhotPerEventEDepBound = fSetGammaPerEventEDepBoundCmd->GetNewDoubleValue(par);
	}

	// -------------------- initial energy bound --------------------
	{
		if(cmd == fSetGammaPerEventInitEBoundCmd)
			fGammaPerEventInitEBound = fSetGammaPerEventInitEBoundCmd->GetNewDoubleValue(par);
		if(cmd == fSetPositronPerEventInitEBoundCmd)
			fPositronPerEventInitEBound = fSetPositronPerEventInitEBoundCmd->GetNewDoubleValue(par);
		if(cmd == fSetElectronPerEventInitEBoundCmd)
			fElectronPerEventInitEBound = fSetElectronPerEventInitEBoundCmd->GetNewDoubleValue(par);
		if(cmd == fSetProtonPerEventInitEBoundCmd)
			fProtonPerEventInitEBound = fSetProtonPerEventInitEBoundCmd->GetNewDoubleValue(par);
		if(cmd == fSetNeutronPerEventInitEBoundCmd)
			fNeutronPerEventInitEBound = fSetNeutronPerEventInitEBoundCmd->GetNewDoubleValue(par);
		if(cmd == fSetPionPlusPerEventInitEBoundCmd)
			fPionPlusPerEventInitEBound = fSetPionPlusPerEventInitEBoundCmd->GetNewDoubleValue(par);
		if(cmd == fSetPionMinusPerEventInitEBoundCmd)
			fPionMinusPerEventInitEBound = fSetPionMinusPerEventInitEBoundCmd->GetNewDoubleValue(par);
		if(cmd == fSetPionZeroPerEventInitEBoundCmd)
			fPionZeroPerEventInitEBound = fSetPionZeroPerEventInitEBoundCmd->GetNewDoubleValue(par);
		if(cmd == fSetMuonPlusPerEventInitEBoundCmd)
			fMuonPlusPerEventInitEBound = fSetMuonPlusPerEventInitEBoundCmd->GetNewDoubleValue(par);
		if(cmd == fSetMuonMinusPerEventInitEBoundCmd)
			fMuonMinusPerEventInitEBound = fSetMuonMinusPerEventInitEBoundCmd->GetNewDoubleValue(par);
		if(cmd == fSetOptPhotPerEventInitEBoundCmd)
			fOptPhotPerEventInitEBound = fSetGammaPerEventInitEBoundCmd->GetNewDoubleValue(par);
	}

	if(cmd == fSetPerEventEDepBoundCmd)
		fPerEventEDepBound = fSetPerEventEDepBoundCmd->GetNewDoubleValue(par);
	if(cmd == fSetPerEventUntrackedEBoundCmd)
		fPerEventUntrackedEBound = fSetPerEventUntrackedEBoundCmd->GetNewDoubleValue(par);
}
