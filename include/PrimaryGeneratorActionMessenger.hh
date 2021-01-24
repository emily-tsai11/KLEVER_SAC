// PrimaryGeneratorActionMessenger.hh
// --------------------------------------------------------------
// History:
//
// Created by Emily Tsai (emily.tsai11@gmail.com) 2021-1-22
// --------------------------------------------------------------

#ifndef PrimaryGeneratorActionMessenger_H
#define PrimaryGeneratorActionMessenger_H 1

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "G4UImessenger.hh"
#include "globals.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class G4UIdirectory;
class G4UIcmdWithAnInteger;
class G4UIcmdWithADoubleAndUnit;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class PrimaryGeneratorActionMessenger : public G4UImessenger
{
	public:

		PrimaryGeneratorActionMessenger();
		~PrimaryGeneratorActionMessenger();

		void SetNewValue(G4UIcommand*, G4String);

		G4int GetBeamType() { return fBeamType; }
		G4double GetBeamEnergy() { return fBeamEnergy; }

	private:

		G4int fBeamType;
		G4double fBeamEnergy;

		G4UIdirectory* fPrimaryGeneratorActionDir;

		G4UIcmdWithAnInteger* fSetBeamTypeCmd;
		G4UIcmdWithADoubleAndUnit* fSetBeamEnergyCmd;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
