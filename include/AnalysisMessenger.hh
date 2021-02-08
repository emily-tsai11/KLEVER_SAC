// AnalysisMessenger.hh
// --------------------------------------------------------------
// History:
//
// Created by Emily Tsai (emily.tsai11@gmail.com) 2021-1-26
// --------------------------------------------------------------

#ifndef AnalysisMessenger_H
#define AnalysisMessenger_H 1

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "G4UImessenger.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class G4String;
class G4UIdirectory;
class G4UIcmdWithAString;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class AnalysisMessenger : public G4UImessenger
{
	public:

		AnalysisMessenger();
		~AnalysisMessenger();
		void SetNewValue(G4UIcommand*, G4String);

	private:

		G4UIdirectory* fAnalysisDir;
		G4UIcmdWithAString* fSetFileNameCmd;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
