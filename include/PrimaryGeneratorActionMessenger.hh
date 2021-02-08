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

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class G4UIdirectory;
class G4UIcmdWithAnInteger;
class G4UIcmdWithADoubleAndUnit;

class PrimaryGeneratorAction;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class PrimaryGeneratorActionMessenger : public G4UImessenger
{
	public:

		PrimaryGeneratorActionMessenger(PrimaryGeneratorAction*);
		~PrimaryGeneratorActionMessenger();
		void SetNewValue(G4UIcommand*, G4String);

	private:

		G4UIdirectory* fPrimaryGeneratorActionDir;
		G4UIcmdWithAnInteger* fSetBeamTypeCmd;
		G4UIcmdWithADoubleAndUnit* fSetBeamEnergyCmd;
		PrimaryGeneratorAction* fPrimaryGeneratorAction;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
