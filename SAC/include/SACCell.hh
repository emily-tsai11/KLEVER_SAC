// SACCell.hh
// --------------------------------------------------------------
// History:
//
// Created by Emily Tsai (emily.tsai11@gmail.com) 2021-1-6
// --------------------------------------------------------------

#ifndef SACCell_H
#define SACCell_H 1

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "globals.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class G4LogicalVolume;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class SACCell
{
	public:

		explicit SACCell();
		void CreateGeometry();

		G4LogicalVolume* GetCrystalLogicalVolume() { return fCrystalVolume; }
		G4LogicalVolume* GetCellLogicalVolume() { return fCellVolume; }

	private:

		G4double fCrystalSizeX, fCrystalSizeY, fCrystalSizeZ;
		G4double fCrystalCoating;
		G4double fCellSizeX, fCellSizeY, fCellSizeZ;

		G4String fSACSDName;

		G4LogicalVolume* fCrystalVolume;
		G4LogicalVolume* fCellVolume;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
