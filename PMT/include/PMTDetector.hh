// PMTDetector.hh
// --------------------------------------------------------------
// History:
//
// Created by Emily Tsai (emily.tsai11@gmail.com) 2020-12-9
// --------------------------------------------------------------

#ifndef PMTDetector_H
#define PMTDetector_H 1

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "G4Material.hh"
#include "globals.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class G4LogicalVolume;
// class PMTMessenger;

class PMTDetector
{
	public:

		PMTDetector(G4Material*, G4LogicalVolume*);
		~PMTDetector();

		void SetMaterial(G4Material* v) { fMaterial = v; }
		void SetMotherVolume(G4LogicalVolume* v) { fMotherVolume = v; }
		void CreateGeometry();

		G4LogicalVolume* GetPMTLogicalVolume() { return fPMTVolume; }
		G4LogicalVolume* GetCellLogicalVolume() { return fCellVolume; }

	private:

		G4Material* fMaterial;
		G4LogicalVolume* fMotherVolume;
		G4LogicalVolume* fPMTVolume;
		G4LogicalVolume* fCellVolume;

		// PMTMessenger* fPMTMessenger;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
