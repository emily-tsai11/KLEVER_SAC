// SACMessenger.hh
// --------------------------------------------------------------
// History:
//
// Created by Emanuele Leonardi (emanuele.leonardi@roma1.infn.it) 2015-12-14
// Adapted from PADME experiment by Emily Tsai (emily.tsai11@gmail.com) 2020-7-8
// --------------------------------------------------------------

#ifndef SACMessenger_H
#define SACMessenger_H 1

#include "globals.hh"
#include "G4UImessenger.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class SACDetector;
class SACGeometry;

class G4UIdirectory;
class G4UIcmdWithAnInteger;
class G4UIcmdWithADoubleAndUnit;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class SACMessenger: public G4UImessenger
{
	public:

		SACMessenger(SACDetector*);
		~SACMessenger();

		void SetNewValue(G4UIcommand*, G4String);

	private:

		SACDetector* fSACDetector;
		SACGeometry* fSACGeometry;

		G4UIdirectory* fSACDetectorDir;

		G4UIcmdWithAnInteger* fSetSACNRowsCmd;
		G4UIcmdWithAnInteger* fSetSACNColsCmd;
		G4UIcmdWithAnInteger* fSetSACNLayersCmd;

		G4UIcmdWithADoubleAndUnit* fSetCrystalSizeCmd;
		G4UIcmdWithADoubleAndUnit* fSetCrystalLengthCmd;

		G4UIcmdWithADoubleAndUnit* fSetCrystalGapCmd;
		G4UIcmdWithADoubleAndUnit* fSetCrystalCoatingCmd;

		G4UIcmdWithADoubleAndUnit* fSetSACFrontFaceZCmd;

		G4UIcmdWithAnInteger* fSetVerboseLevelCmd;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
