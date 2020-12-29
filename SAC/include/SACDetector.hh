// SACDetector.hh
// --------------------------------------------------------------
// History:
//
// Created by Emanuele Leonardi (emanuele.leonardi@roma1.infn.it) 2015-12-11
// Adapted from PADME experiment by Emily Tsai (emily.tsai11@gmail.com) 2020-7-7
// --------------------------------------------------------------

#ifndef SACDetector_H
#define SACDetector_H 1

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "G4Material.hh"
#include "globals.hh"

#include "SACGeometry.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class G4LogicalVolume;
class SACMessenger;

class SACDetector
{
	public:

		SACDetector(G4Material*, G4LogicalVolume*);
		~SACDetector();

		void SetMaterial(G4Material* v) { fMaterial = v; }
		void SetMotherVolume(G4LogicalVolume* v) { fMotherVolume = v; }
		void CreateGeometry();

		G4LogicalVolume* GetSACLogicalVolume() { return fSACVolume; }
		G4LogicalVolume* GetCrystalLogicalVolume() { return fCrystalVolume; }
		G4LogicalVolume* GetCellLogicalVolume() { return fCellVolume; }
		G4LogicalVolume* GetNonRefCellLogicalVolume() { return fNonRefCellVolume; }

		G4double GetSACFrontFaceZ() { return SACGeometry::GetInstance()->GetSACFrontFacePosZ(); }

	private:

		G4Material* fMaterial;
		G4LogicalVolume* fMotherVolume;
		G4LogicalVolume* fSACVolume;
		G4LogicalVolume* fCrystalVolume;
		G4LogicalVolume* fCellVolume;
		G4LogicalVolume* fNonRefCellVolume;

		SACMessenger* fSACMessenger;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
