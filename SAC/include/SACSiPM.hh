// SACSiPM.hh
// --------------------------------------------------------------
// History:
//
// Created by Emily Tsai (emily.tsai11@gmail.com) 2021-1-11
// --------------------------------------------------------------

#ifndef SACSiPM_H
#define SACSiPM_H 1

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "globals.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class G4LogicalVolume;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class SACSiPM
{
	public:

		explicit SACSiPM();
		void CreateGeometry();
		G4LogicalVolume* GetSiPMLogicalVolume() { return fSiPMVolume; }

	private:

		G4double fSiPMSize, fSiPMThickness;
		G4String fSACSDName;
		G4LogicalVolume* fSiPMVolume;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
