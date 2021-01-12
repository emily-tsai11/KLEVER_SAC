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

#include "SACCell.hh"
#include "SACPMT.hh"
#include "SACSiPM.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class SACMessenger;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class SACDetector
{
	public:

		SACDetector(G4Material*, G4LogicalVolume*);
		~SACDetector();
		void CreateGeometry();

		void SetMaterial(G4Material* v) { fMaterial = v; }
		void SetMotherVolume(G4LogicalVolume* v) { fMotherVolume = v; }

		G4LogicalVolume* GetSACLogicalVolume() { return fSACVolume; }

		// G4double GetSACFrontFaceZ() { return SACGeometry::GetInstance()->GetSACFrontFacePosZ(); }

	private:

		G4Material* fMaterial;
		G4LogicalVolume* fMotherVolume;
		G4LogicalVolume* fSACVolume;

		G4double fCellSizeX, fCellSizeY, fCellSizeZ;
		G4double fCellGap;

		G4int fNRows, fNCols, fNLayers;
		G4double fSACSizeX, fSACSizeY, fSACSizeZ;
		G4double fSACPosX, fSACPosY, fSACPosZ;

		G4bool fEnablePMT;
		G4bool fEnableSiPM;

		SACCell* fCell;
		SACCell* fNonRefCell;

		SACPMT* fPMT;
		SACSiPM* fSiPM;

		SACMessenger* fSACMessenger;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
