// RunActionMessenger.hh
// --------------------------------------------------------------
// History:
//
// Created by Emily Tsai (emily.tsai11@gmail.com) 2020-7-28
// --------------------------------------------------------------

#ifndef RunActionMessenger_H
#define RunActionMessenger_H 1

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "G4UImessenger.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class G4UIdirectory;
class G4UIcmdWithAString;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class RunActionMessenger : public G4UImessenger
{
	public:

		RunActionMessenger();
		virtual ~RunActionMessenger();

		void SetNewValue(G4UIcommand*, G4String);
		G4String GetFileName() { return fFileName; }

	private:

		G4UIdirectory* fRunActionDir;
		G4UIcmdWithAString* fSetFileNameCmd;
		G4String fFileName;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
