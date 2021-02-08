// DetectorConstructionMessenger.hh
// --------------------------------------------------------------
// History:
//
// Created by Emily Tsai (emily.tsai11@gmail.com) 2021-1-22
// --------------------------------------------------------------

#ifndef DetectorConstructionMessenger_H
#define DetectorConstructionMessenger_H 1

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "G4UImessenger.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class G4UIdirectory;
class G4UIcmdWithoutParameter;
class G4UIcmdWithADoubleAndUnit;
class DetectorConstruction;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class DetectorConstructionMessenger : public G4UImessenger
{
	public:

		DetectorConstructionMessenger(DetectorConstruction*);
		~DetectorConstructionMessenger();
		void SetNewValue(G4UIcommand*, G4String);

	private:

		DetectorConstruction* fDetector;
		G4UIdirectory* fDetectorConstructionDir;

		G4UIcmdWithoutParameter* fUpdateGeometryCmd;
		G4UIcmdWithADoubleAndUnit* fSetWorldLengthXCmd;
		G4UIcmdWithADoubleAndUnit* fSetWorldLengthYCmd;
		G4UIcmdWithADoubleAndUnit* fSetWorldLengthZCmd;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
