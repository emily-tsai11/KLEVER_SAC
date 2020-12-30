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

class SACDetector;
class SACGeometry;
class SACSD;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class SACMessenger : public G4UImessenger
{
	public:

		SACMessenger(SACDetector*);
		~SACMessenger();

		void SetNewValue(G4UIcommand*, G4String);

	private:

		SACDetector* fSACDetector;
		SACGeometry* fSACGeometry;
		SACSD* fSACSD;

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

		G4UIcmdWithADoubleAndUnit* fSetIncidentECmd;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
