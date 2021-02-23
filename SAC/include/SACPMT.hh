// SACPMT.hh
// --------------------------------------------------------------
// History:
//
// Created by Emily Tsai (emily.tsai11@gmail.com) 2021-1-1
// --------------------------------------------------------------

#ifndef SACPMT_H
#define SACPMT_H 1

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "globals.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class G4LogicalVolume;
class G4String;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class SACPMT
{
	public:

		explicit SACPMT();
		~SACPMT();

		void CreateGeometry();
		G4LogicalVolume* GetPMTLogicalVolume() { return fPMTVolume; }

    private:

		G4double fPMTDiameter, fPMTThickness, fPMTRound;
		G4String fSACSDName;
		G4LogicalVolume* fPMTVolume;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
