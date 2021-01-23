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

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class PrimaryGeneratorActionMessenger : public G4UImessenger
{
	public:

		PrimaryGeneratorActionMessenger();
		~PrimaryGeneratorActionMessenger();

		void SetNewValue(G4UIcommand*, G4String);
		G4int GetBeamType() { return fBeamType; }

	private:

		G4UIdirectory* fPrimaryGeneratorActionDir;
		G4UIcmdWithAnInteger* fSetBeamTypeCmd;
		G4int fBeamType;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
