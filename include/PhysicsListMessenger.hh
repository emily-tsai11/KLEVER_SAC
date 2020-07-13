// PhysicsListMessenger.hh
// --------------------------------------------------------------
// $Id: PhysicsListMessenger.hh,v 1.3.6.1 2010/03/18 10:33:19 gcosmo Exp $
// GEANT4 tag $Name: geant4-09-03-patch-02 $
// --------------------------------------------------------------
// History:
//
// Created by V. Ivanchenko 31.01.2006
// V. Ivanchenko (Adaptation of hadr01) 04.06.2006
// Sergey Podolsky (siarhei.padolski@cern.ch) 03-09-2012
// Adapted from Padme by Emily Tsai (emily.tsai11@gmail.com) 2020-7-13
// --------------------------------------------------------------

#ifndef PhysicsListMessenger_H
#define PhysicsListMessenger_H 1

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "G4UImessenger.hh"
#include "globals.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class PhysicsList;
class G4UIcmdWithADouble;
class G4UIcmdWithAnInteger;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithAString;
class G4UIcmdWithoutParameter;
class G4UIcmdWithABool;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class PhysicsListMessenger : public G4UImessenger
{
	private:

		PhysicsListMessenger();
		virtual ~PhysicsListMessenger();

	public:

		static PhysicsListMessenger* GetInstance();
		void SetNewValue(G4UIcommand*, G4String);

		void SetCutForGamma(G4double value) { fCutGamma = value; }
		G4double GetCutForGamma() { return fCutGamma; }
		void SetCutForElectron(G4double value) { fCutElectron = value; }
		G4double GetCutForElectron() { return fCutElectron; }
		void SetCutForPositron(G4double value) { fCutPositron = value; }
		G4double GetCutForPositron() { return fCutPositron; }
		void SetCutForProton(G4double value) { fCutProton = value; }
		G4double GetCutForProton() { return fCutProton; }

		void SetPhysicsList(G4String name) { fPhysicsListName = name; }
		G4String GetPhysicsList() { return fPhysicsListName; }

		void SetAddParameterisation(G4bool value) { fAddParameterisation = value; }
		G4bool GetAddParameterisation() { return fAddParameterisation; }

		void SetBrPie2(G4double value) { fBrPie2 = value; }
		G4double GetBrPie2() const { return fBrPie2; }
		void SetMuonDecay(G4int value) { fMuonDecayMode = value; } // set muon decay mode
		G4double GetMuonDecay() { return fMuonDecayMode; } // set muon decay mode
		void SetListPhysList(G4bool value) { fListPhysList = value; }
		G4bool GetListPhysList() { return fListPhysList; }

	private:

		static PhysicsListMessenger* fInstance;

		G4UIcmdWithADoubleAndUnit* fGammaCutCmd;
		G4UIcmdWithADoubleAndUnit* fElectCutCmd;
		G4UIcmdWithADoubleAndUnit* fPosCutCmd;
		G4UIcmdWithADoubleAndUnit* fCutCmd;
		G4UIcmdWithADoubleAndUnit* fAllCutCmd;
		G4UIcmdWithAString* fPListCmd;
		G4UIcmdWithoutParameter* fListCmd;
		G4UIcmdWithoutParameter* fFastCmd;

		G4UIcmdWithADouble* fDecayPiplusDecayCmd;
		G4UIcmdWithAnInteger* fMuonDecayCmd;

		G4double fBrPie2;
		G4int fMuonDecayMode;
		G4double fCutGamma;
		G4double fCutElectron;
		G4double fCutPositron;
		G4double fCutProton;
		G4String fPhysicsListName;
		G4bool fAddParameterisation;
		G4bool fListPhysList;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
