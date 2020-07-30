// RunActionMessenger.hh
// --------------------------------------------------------------
// History:
//
// Created by Emily Tsai (emily.tsai11@gmail.com) 2020-7-28
// --------------------------------------------------------------

#ifndef RunActionMessenger_H
#define RunActionMessenger_H 1

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "G4UImessenger.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithAnInteger;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class RunActionMessenger : public G4UImessenger
{
	public:

		RunActionMessenger();
		virtual ~RunActionMessenger();

		void SetNewValue(G4UIcommand*, G4String);

		G4String GetFileName() { return fFileName; }

		G4double GetGammaPerHitEDepBound() { return fGammaPerHitEDepBound; }
		G4double GetPositronPerHitEDepBound() { return fPositronPerHitEDepBound; }
		G4double GetElectronPerHitEDepBound() { return fElectronPerHitEDepBound; }
		G4double GetProtonPerHitEDepBound() { return fProtonPerHitEDepBound; }
		G4double GetNeutronPerHitEDepBound() { return fNeutronPerHitEDepBound; }
		G4double GetPionPlusPerHitEDepBound() { return fPionPlusPerHitEDepBound; }
		G4double GetPionMinusPerHitEDepBound() { return fPionMinusPerHitEDepBound; }
		G4double GetPionZeroPerHitEDepBound() { return fPionZeroPerHitEDepBound; }
		G4double GetMuonPlusPerHitEDepBound() { return fMuonPlusPerHitEDepBound; }
		G4double GetMuonMinusPerHitEDepBound() { return fMuonMinusPerHitEDepBound; }
		G4double GetOptPhotPerHitEDepBound() { return fOptPhotPerHitEDepBound; }

		G4double GetGammaPerHitTrLenBound() { return fGammaPerHitTrLenBound; }
		G4double GetPositronPerHitTrLenBound() { return fPositronPerHitTrLenBound; }
		G4double GetElectronPerHitTrLenBound() { return fElectronPerHitTrLenBound; }
		G4double GetProtonPerHitTrLenBound() { return fProtonPerHitTrLenBound; }
		G4double GetNeutronPerHitTrLenBound() { return fNeutronPerHitTrLenBound; }
		G4double GetPionPlusPerHitTrLenBound() { return fPionPlusPerHitTrLenBound; }
		G4double GetPionMinusPerHitTrLenBound() { return fPionMinusPerHitTrLenBound; }
		G4double GetPionZeroPerHitTrLenBound() { return fPionZeroPerHitTrLenBound; }
		G4double GetMuonPlusPerHitTrLenBound() { return fMuonPlusPerHitTrLenBound; }
		G4double GetMuonMinusPerHitTrLenBound() { return fMuonMinusPerHitTrLenBound; }
		G4double GetOptPhotPerHitTrLenBound() { return fOptPhotPerHitTrLenBound; }

		G4int GetGammaPerEventMultMinBound() { return fGammaPerEventMultMinBound; }
		G4int GetGammaPerEventMultMaxBound() { return fGammaPerEventMultMaxBound; }
		G4int GetPositronPerEventMultMinBound() { return fPositronPerEventMultMinBound; }
		G4int GetPositronPerEventMultMaxBound() { return fPositronPerEventMultMaxBound; }
		G4int GetElectronPerEventMultMinBound() { return fElectronPerEventMultMinBound; }
		G4int GetElectronPerEventMultMaxBound() { return fElectronPerEventMultMaxBound; }
		G4int GetProtonPerEventMultMinBound() { return fProtonPerEventMultMinBound; }
		G4int GetProtonPerEventMultMaxBound() { return fProtonPerEventMultMaxBound; }
		G4int GetNeutronPerEventMultMinBound() { return fNeutronPerEventMultMinBound; }
		G4int GetNeutronPerEventMultMaxBound() { return fNeutronPerEventMultMaxBound; }
		G4int GetPionPlusPerEventMultMinBound() { return fPionPlusPerEventMultMinBound; }
		G4int GetPionPlusPerEventMultMaxBound() { return fPionPlusPerEventMultMaxBound; }
		G4int GetPionMinusPerEventMultMinBound() { return fPionMinusPerEventMultMinBound; }
		G4int GetPionMinusPerEventMultMaxBound() { return fPionMinusPerEventMultMaxBound; }
		G4int GetPionZeroPerEventMultMinBound() { return fPionZeroPerEventMultMinBound; }
		G4int GetPionZeroPerEventMultMaxBound() { return fPionZeroPerEventMultMaxBound; }
		G4int GetMuonPlusPerEventMultMinBound() { return fMuonPlusPerEventMultMinBound; }
		G4int GetMuonPlusPerEventMultMaxBound() { return fMuonPlusPerEventMultMaxBound; }
		G4int GetMuonMinusPerEventMultMinBound() { return fMuonMinusPerEventMultMinBound; }
		G4int GetMuonMinusPerEventMultMaxBound() { return fMuonMinusPerEventMultMaxBound; }
		G4int GetOptPhotPerEventMultMinBound() { return fOptPhotPerEventMultMinBound; }
		G4int GetOptPhotPerEventMultMaxBound() { return fOptPhotPerEventMultMaxBound; }

		G4double GetGammaPerEventEDepBound() { return fGammaPerEventEDepBound; }
		G4double GetPositronPerEventEDepBound() { return fPositronPerEventEDepBound; }
		G4double GetElectronPerEventEDepBound() { return fElectronPerEventEDepBound; }
		G4double GetProtonPerEventEDepBound() { return fProtonPerEventEDepBound; }
		G4double GetNeutronPerEventEDepBound() { return fNeutronPerEventEDepBound; }
		G4double GetPionPlusPerEventEDepBound() { return fPionPlusPerEventEDepBound; }
		G4double GetPionMinusPerEventEDepBound() { return fPionMinusPerEventEDepBound; }
		G4double GetPionZeroPerEventEDepBound() { return fPionZeroPerEventEDepBound; }
		G4double GetMuonPlusPerEventEDepBound() { return fMuonPlusPerEventEDepBound; }
		G4double GetMuonMinusPerEventEDepBound() { return fMuonMinusPerEventEDepBound; }
		G4double GetOptPhotPerEventEDepBound() { return fOptPhotPerEventEDepBound; }

		G4double GetGammaPerEventInitEBound() { return fGammaPerEventInitEBound; }
		G4double GetPositronPerEventInitEBound() { return fPositronPerEventInitEBound; }
		G4double GetElectronPerEventInitEBound() { return fElectronPerEventInitEBound; }
		G4double GetProtonPerEventInitEBound() { return fProtonPerEventInitEBound; }
		G4double GetNeutronPerEventInitEBound() { return fNeutronPerEventInitEBound; }
		G4double GetPionPlusPerEventInitEBound() { return fPionPlusPerEventInitEBound; }
		G4double GetPionMinusPerEventInitEBound() { return fPionMinusPerEventInitEBound; }
		G4double GetPionZeroPerEventInitEBound() { return fPionZeroPerEventInitEBound; }
		G4double GetMuonPlusPerEventInitEBound() { return fMuonPlusPerEventInitEBound; }
		G4double GetMuonMinusPerEventInitEBound() { return fMuonMinusPerEventInitEBound; }
		G4double GetOptPhotPerEventInitEBound() { return fOptPhotPerEventInitEBound; }

		G4double GetPerEventEDepMinBound() { return fPerEventEDepMinBound; }
		G4double GetPerEventEDepMaxBound() { return fPerEventEDepMaxBound; }
		G4double GetPerEventUntrackedEBound() { return fPerEventUntrackedEBound; }

	private:

		G4UIdirectory* fRunActionDir;

		G4UIcmdWithAString* fSetFileNameCmd;
		G4String fFileName;

		G4UIcmdWithADoubleAndUnit* fSetGammaPerHitEDepBoundCmd;
		G4UIcmdWithADoubleAndUnit* fSetPositronPerHitEDepBoundCmd;
		G4UIcmdWithADoubleAndUnit* fSetElectronPerHitEDepBoundCmd;
		G4UIcmdWithADoubleAndUnit* fSetProtonPerHitEDepBoundCmd;
		G4UIcmdWithADoubleAndUnit* fSetNeutronPerHitEDepBoundCmd;
		G4UIcmdWithADoubleAndUnit* fSetPionPlusPerHitEDepBoundCmd;
		G4UIcmdWithADoubleAndUnit* fSetPionMinusPerHitEDepBoundCmd;
		G4UIcmdWithADoubleAndUnit* fSetPionZeroPerHitEDepBoundCmd;
		G4UIcmdWithADoubleAndUnit* fSetMuonPlusPerHitEDepBoundCmd;
		G4UIcmdWithADoubleAndUnit* fSetMuonMinusPerHitEDepBoundCmd;
		G4UIcmdWithADoubleAndUnit* fSetOptPhotPerHitEDepBoundCmd;

		G4double fGammaPerHitEDepBound;
		G4double fPositronPerHitEDepBound;
		G4double fElectronPerHitEDepBound;
		G4double fProtonPerHitEDepBound;
		G4double fNeutronPerHitEDepBound;
		G4double fPionPlusPerHitEDepBound;
		G4double fPionMinusPerHitEDepBound;
		G4double fPionZeroPerHitEDepBound;
		G4double fMuonPlusPerHitEDepBound;
		G4double fMuonMinusPerHitEDepBound;
		G4double fOptPhotPerHitEDepBound;

		G4UIcmdWithADoubleAndUnit* fSetGammaPerHitTrLenBoundCmd;
		G4UIcmdWithADoubleAndUnit* fSetPositronPerHitTrLenBoundCmd;
		G4UIcmdWithADoubleAndUnit* fSetElectronPerHitTrLenBoundCmd;
		G4UIcmdWithADoubleAndUnit* fSetProtonPerHitTrLenBoundCmd;
		G4UIcmdWithADoubleAndUnit* fSetNeutronPerHitTrLenBoundCmd;
		G4UIcmdWithADoubleAndUnit* fSetPionPlusPerHitTrLenBoundCmd;
		G4UIcmdWithADoubleAndUnit* fSetPionMinusPerHitTrLenBoundCmd;
		G4UIcmdWithADoubleAndUnit* fSetPionZeroPerHitTrLenBoundCmd;
		G4UIcmdWithADoubleAndUnit* fSetMuonPlusPerHitTrLenBoundCmd;
		G4UIcmdWithADoubleAndUnit* fSetMuonMinusPerHitTrLenBoundCmd;
		G4UIcmdWithADoubleAndUnit* fSetOptPhotPerHitTrLenBoundCmd;

		G4double fGammaPerHitTrLenBound;
		G4double fPositronPerHitTrLenBound;
		G4double fElectronPerHitTrLenBound;
		G4double fProtonPerHitTrLenBound;
		G4double fNeutronPerHitTrLenBound;
		G4double fPionPlusPerHitTrLenBound;
		G4double fPionMinusPerHitTrLenBound;
		G4double fPionZeroPerHitTrLenBound;
		G4double fMuonPlusPerHitTrLenBound;
		G4double fMuonMinusPerHitTrLenBound;
		G4double fOptPhotPerHitTrLenBound;

		G4UIcmdWithAnInteger* fSetGammaPerEventMultMinBoundCmd;
		G4UIcmdWithAnInteger* fSetGammaPerEventMultMaxBoundCmd;
		G4UIcmdWithAnInteger* fSetPositronPerEventMultMinBoundCmd;
		G4UIcmdWithAnInteger* fSetPositronPerEventMultMaxBoundCmd;
		G4UIcmdWithAnInteger* fSetElectronPerEventMultMinBoundCmd;
		G4UIcmdWithAnInteger* fSetElectronPerEventMultMaxBoundCmd;
		G4UIcmdWithAnInteger* fSetProtonPerEventMultMinBoundCmd;
		G4UIcmdWithAnInteger* fSetProtonPerEventMultMaxBoundCmd;
		G4UIcmdWithAnInteger* fSetNeutronPerEventMultMinBoundCmd;
		G4UIcmdWithAnInteger* fSetNeutronPerEventMultMaxBoundCmd;
		G4UIcmdWithAnInteger* fSetPionPlusPerEventMultMinBoundCmd;
		G4UIcmdWithAnInteger* fSetPionPlusPerEventMultMaxBoundCmd;
		G4UIcmdWithAnInteger* fSetPionMinusPerEventMultMinBoundCmd;
		G4UIcmdWithAnInteger* fSetPionMinusPerEventMultMaxBoundCmd;
		G4UIcmdWithAnInteger* fSetPionZeroPerEventMultMinBoundCmd;
		G4UIcmdWithAnInteger* fSetPionZeroPerEventMultMaxBoundCmd;
		G4UIcmdWithAnInteger* fSetMuonPlusPerEventMultMinBoundCmd;
		G4UIcmdWithAnInteger* fSetMuonPlusPerEventMultMaxBoundCmd;
		G4UIcmdWithAnInteger* fSetMuonMinusPerEventMultMinBoundCmd;
		G4UIcmdWithAnInteger* fSetMuonMinusPerEventMultMaxBoundCmd;
		G4UIcmdWithAnInteger* fSetOptPhotPerEventMultMinBoundCmd;
		G4UIcmdWithAnInteger* fSetOptPhotPerEventMultMaxBoundCmd;

		G4int fGammaPerEventMultMinBound;
		G4int fGammaPerEventMultMaxBound;
		G4int fPositronPerEventMultMinBound;
		G4int fPositronPerEventMultMaxBound;
		G4int fElectronPerEventMultMinBound;
		G4int fElectronPerEventMultMaxBound;
		G4int fProtonPerEventMultMinBound;
		G4int fProtonPerEventMultMaxBound;
		G4int fNeutronPerEventMultMinBound;
		G4int fNeutronPerEventMultMaxBound;
		G4int fPionPlusPerEventMultMinBound;
		G4int fPionPlusPerEventMultMaxBound;
		G4int fPionMinusPerEventMultMinBound;
		G4int fPionMinusPerEventMultMaxBound;
		G4int fPionZeroPerEventMultMinBound;
		G4int fPionZeroPerEventMultMaxBound;
		G4int fMuonPlusPerEventMultMinBound;
		G4int fMuonPlusPerEventMultMaxBound;
		G4int fMuonMinusPerEventMultMinBound;
		G4int fMuonMinusPerEventMultMaxBound;
		G4int fOptPhotPerEventMultMinBound;
		G4int fOptPhotPerEventMultMaxBound;

		G4UIcmdWithADoubleAndUnit* fSetGammaPerEventEDepBoundCmd;
		G4UIcmdWithADoubleAndUnit* fSetPositronPerEventEDepBoundCmd;
		G4UIcmdWithADoubleAndUnit* fSetElectronPerEventEDepBoundCmd;
		G4UIcmdWithADoubleAndUnit* fSetProtonPerEventEDepBoundCmd;
		G4UIcmdWithADoubleAndUnit* fSetNeutronPerEventEDepBoundCmd;
		G4UIcmdWithADoubleAndUnit* fSetPionPlusPerEventEDepBoundCmd;
		G4UIcmdWithADoubleAndUnit* fSetPionMinusPerEventEDepBoundCmd;
		G4UIcmdWithADoubleAndUnit* fSetPionZeroPerEventEDepBoundCmd;
		G4UIcmdWithADoubleAndUnit* fSetMuonPlusPerEventEDepBoundCmd;
		G4UIcmdWithADoubleAndUnit* fSetMuonMinusPerEventEDepBoundCmd;
		G4UIcmdWithADoubleAndUnit* fSetOptPhotPerEventEDepBoundCmd;

		G4double fGammaPerEventEDepBound;
		G4double fPositronPerEventEDepBound;
		G4double fElectronPerEventEDepBound;
		G4double fProtonPerEventEDepBound;
		G4double fNeutronPerEventEDepBound;
		G4double fPionPlusPerEventEDepBound;
		G4double fPionMinusPerEventEDepBound;
		G4double fPionZeroPerEventEDepBound;
		G4double fMuonPlusPerEventEDepBound;
		G4double fMuonMinusPerEventEDepBound;
		G4double fOptPhotPerEventEDepBound;

		G4UIcmdWithADoubleAndUnit* fSetGammaPerEventInitEBoundCmd;
		G4UIcmdWithADoubleAndUnit* fSetPositronPerEventInitEBoundCmd;
		G4UIcmdWithADoubleAndUnit* fSetElectronPerEventInitEBoundCmd;
		G4UIcmdWithADoubleAndUnit* fSetProtonPerEventInitEBoundCmd;
		G4UIcmdWithADoubleAndUnit* fSetNeutronPerEventInitEBoundCmd;
		G4UIcmdWithADoubleAndUnit* fSetPionPlusPerEventInitEBoundCmd;
		G4UIcmdWithADoubleAndUnit* fSetPionMinusPerEventInitEBoundCmd;
		G4UIcmdWithADoubleAndUnit* fSetPionZeroPerEventInitEBoundCmd;
		G4UIcmdWithADoubleAndUnit* fSetMuonPlusPerEventInitEBoundCmd;
		G4UIcmdWithADoubleAndUnit* fSetMuonMinusPerEventInitEBoundCmd;
		G4UIcmdWithADoubleAndUnit* fSetOptPhotPerEventInitEBoundCmd;

		G4double fGammaPerEventInitEBound;
		G4double fPositronPerEventInitEBound;
		G4double fElectronPerEventInitEBound;
		G4double fProtonPerEventInitEBound;
		G4double fNeutronPerEventInitEBound;
		G4double fPionPlusPerEventInitEBound;
		G4double fPionMinusPerEventInitEBound;
		G4double fPionZeroPerEventInitEBound;
		G4double fMuonPlusPerEventInitEBound;
		G4double fMuonMinusPerEventInitEBound;
		G4double fOptPhotPerEventInitEBound;

		G4UIcmdWithADoubleAndUnit* fSetPerEventEDepMinBoundCmd;
		G4UIcmdWithADoubleAndUnit* fSetPerEventEDepMaxBoundCmd;
		G4UIcmdWithADoubleAndUnit* fSetPerEventUntrackedEBoundCmd;

		G4double fPerEventEDepMinBound;
		G4double fPerEventEDepMaxBound;
		G4double fPerEventUntrackedEBound;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
