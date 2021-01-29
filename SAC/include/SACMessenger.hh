// SACMessenger.hh
// --------------------------------------------------------------
// History:
//
// Created by Emanuele Leonardi (emanuele.leonardi@roma1.infn.it) 2015-12-14
// Adapted from Padme by Emily Tsai (emily.tsai11@gmail.com) 2020-7-15
// --------------------------------------------------------------

#ifndef SACMessenger_H
#define SACMessenger_H 1

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "G4UImessenger.hh"
#include "globals.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class G4UIdirectory;
class G4UIcmdWithAnInteger;
class G4UIcmdWithADoubleAndUnit;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class SACMessenger : public G4UImessenger
{
	public:

		SACMessenger();
		~SACMessenger();
		void SetNewValue(G4UIcommand*, G4String);

	private:

		G4UIdirectory* fSACDetectorDir;

		G4UIcmdWithADoubleAndUnit* fSetCrystalSizeCmd;
		G4UIcmdWithADoubleAndUnit* fSetCrystalLengthCmd;

		G4UIcmdWithADoubleAndUnit* fSetCrystalCoatingCmd;
		G4UIcmdWithADoubleAndUnit* fSetCellGapCmd;
		G4UIcmdWithADoubleAndUnit* fSetLayerGapCmd;

		G4UIcmdWithAnInteger* fSetSACNRowsCmd;
		G4UIcmdWithAnInteger* fSetSACNColsCmd;
		G4UIcmdWithAnInteger* fSetSACNLayersCmd;

		G4UIcmdWithAnInteger* fEnablePMTCmd;
		G4UIcmdWithAnInteger* fEnableSiPMCmd;

		G4UIcmdWithAnInteger* fSetVerboseLevelCmd;
		// G4UIcmdWithADoubleAndUnit* fSetSACFrontFaceZCmd;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
