// PhysicsListMessenger.hh
// --------------------------------------------------------------
// $Id: PhysicsListMessenger.hh,v 1.3.6.1 2010/03/18 10:33:19 gcosmo Exp $
// GEANT4 tag $Name: geant4-09-03-patch-02 $
// --------------------------------------------------------------
// History:
//
// Created by V.Ivanchenko 31.01.2006
// Modified by V.Ivanchenko 04.06.2006
// - Adaptation of hadr01
// Sergey Podolsky (siarhei.padolski@cern.ch) 03-09-2012
// Adapted from Padme by Emily Tsai (emily.tsai11@gmail.com) 2020-7-15
// --------------------------------------------------------------

#ifndef PhysicsListMessenger_H
#define PhysicsListMessenger_H 1

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "G4UImessenger.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class G4UIdirectory;
class G4UIcmdWithADouble;
class G4UIcmdWithAnInteger;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithAString;
class G4UIcmdWithoutParameter;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class PhysicsListMessenger : public G4UImessenger
{
	public:

		PhysicsListMessenger();
		virtual ~PhysicsListMessenger();

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

		void SetListPhysList(G4bool value) { fListPhysList = value; }
		G4bool GetListPhysList() { return fListPhysList; }


		void SetAddParameterisation(G4bool value) { fAddParameterisation = value; }
		G4bool GetAddParameterisation() { return fAddParameterisation; }


		void SetBrPie2(G4double value) { fBrPie2 = value; }
		G4double GetBrPie2() const { return fBrPie2; }

		void SetMuonDecay(G4int value) { fMuonDecayMode = value; } // Set muon decay mode
		G4double GetMuonDecay() { return fMuonDecayMode; } // Set muon decay mode

	private:

		static PhysicsListMessenger* fInstance;

		G4UIdirectory* fSimulationDir;
		G4UIdirectory* fDecayDir;

		G4UIcmdWithADoubleAndUnit* fCutCmd;
		G4UIcmdWithADoubleAndUnit* fGammaCutCmd;
		G4UIcmdWithADoubleAndUnit* fElectCutCmd;
		G4UIcmdWithADoubleAndUnit* fPosCutCmd;
		G4UIcmdWithADoubleAndUnit* fAllCutCmd;

		G4UIcmdWithAString* fPListCmd;
		G4UIcmdWithoutParameter* fListCmd;
		G4UIcmdWithoutParameter* fFastCmd;

		G4UIcmdWithADouble* fDecayPiplusDecayCmd;
		G4UIcmdWithAnInteger* fMuonDecayCmd;

		G4double fCutGamma;
		G4double fCutElectron;
		G4double fCutPositron;
		G4double fCutProton;

		G4String fPhysicsListName;
		G4bool fListPhysList;
		G4bool fAddParameterisation;

		G4double fBrPie2;
		G4int fMuonDecayMode;

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
