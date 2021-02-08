// HistManagerMessenger.hh
// --------------------------------------------------------------
// History:
//
// Created by Emily Tsai (emily.tsai11@gmail.com) 2021-1-26
// --------------------------------------------------------------

#ifndef HistManagerMessenger_H
#define HistManagerMessenger_H 1

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "G4UImessenger.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class G4String;
class G4UIdirectory;
class G4UIcmdWithAnInteger;
class G4UIcmdWithAString;
class G4UIcmdWithADouble;
class G4UIcmdWithADoubleAndUnit;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class HistManagerMessenger : public G4UImessenger
{
	public:

		HistManagerMessenger();
		~HistManagerMessenger();
		void SetNewValue(G4UIcommand*, G4String);

	private:

		G4UIdirectory* fHistManagerDir;
		G4UIcmdWithAString* fSetFileNameCmd;

		G4String fHistToChange;
		G4UIcmdWithAString* fSetHistToChangeCmd;

		G4UIcmdWithAnInteger* fSetNBinsXCmd;
		G4UIcmdWithADouble* fSetLowXBoundCmd;
		G4UIcmdWithADouble* fSetHighXBoundCmd;
		G4UIcmdWithADoubleAndUnit* fSetLowXBoundWithUnitCmd;
		G4UIcmdWithADoubleAndUnit* fSetHighXBoundWithUnitCmd;

		G4UIcmdWithAnInteger* fSetNBinsYCmd;
		G4UIcmdWithADouble* fSetLowYBoundCmd;
		G4UIcmdWithADouble* fSetHighYBoundCmd;
		G4UIcmdWithADoubleAndUnit* fSetLowYBoundWithUnitCmd;
		G4UIcmdWithADoubleAndUnit* fSetHighYBoundWithUnitCmd;

		G4UIcmdWithAnInteger* fSetNBinsZCmd;
		G4UIcmdWithADouble* fSetLowZBoundCmd;
		G4UIcmdWithADouble* fSetHighZBoundCmd;
		G4UIcmdWithADoubleAndUnit* fSetLowZBoundWithUnitCmd;
		G4UIcmdWithADoubleAndUnit* fSetHighZBoundWithUnitCmd;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
