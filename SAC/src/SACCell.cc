// SACCell.cc
// --------------------------------------------------------------
// History:
//
// Created by Emily Tsai (emily.tsai11@gmail.com) 2021-1-6
// --------------------------------------------------------------

#include "SACCell.hh"

#include "G4Material.hh"
#include "G4Box.hh"
#include "G4SubtractionSolid.hh"
#include "G4VisAttributes.hh"
#include "G4RotationMatrix.hh"
#include "G4ThreeVector.hh"
#include "G4PVPlacement.hh"
#include "G4SDManager.hh"

#include "SACSD.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SACCell::SACCell()
{
	SACGeometry* Geo = SACGeometry::GetInstance();

	fCrystalSizeX = Geo->GetCrystalSizeX();
	fCrystalSizeY = Geo->GetCrystalSizeY();
	fCrystalSizeZ = Geo->GetCrystalSizeZ();
	printf("SAC crystal size is %f %f %f\n", fCrystalSizeX, fCrystalSizeY, fCrystalSizeZ);

	fCrystalCoating = Geo->GetCrystalCoating();
	printf("Coating around SAC crystals is %f\n", fCrystalCoating);

	fCellSizeX = Geo->GetCellSizeX();
	fCellSizeY = Geo->GetCellSizeY();
	fCellSizeZ = Geo->GetCellSizeZ();
	printf("SAC cell size is %f %f %f\n", fCellSizeX, fCellSizeY, fCellSizeZ);

	fSACSDName = Geo->GetSACSensitiveDetectorName();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SACCell::CreateGeometry(G4int sides)
{
	// create PbF2 crystal
	G4Box* fCrystalSolid = new G4Box(
		"SACCrystal",
		0.5 * fCrystalSizeX,
		0.5 * fCrystalSizeY,
		0.5 * fCrystalSizeZ);

	fCrystalVolume = new G4LogicalVolume(
		fCrystalSolid,
		G4Material::GetMaterial("PbF2"),
		"SACCrystal",
		0, 0, 0);
	fCrystalVolume->SetVisAttributes(G4VisAttributes(G4Colour::Blue()));

	// create SAC cell (PbF2 crystal + coating)
	G4Box* fCellSolid = new G4Box(
		"SACCell",
		0.5 * fCellSizeX,
		0.5 * fCellSizeY,
		0.5 * fCellSizeZ); // PROBABLY NEED TO FIX THESE AT SOME POINT

	fCellVolume = new G4LogicalVolume(
		fCellSolid,
		G4Material::GetMaterial("EJ510Paint"),
		"SACCell",
		0, 0, 0);
	fCellVolume->SetVisAttributes(G4VisAttributes(G4Colour::Magenta()));

	if(sides == 6) // coating on all sides
	{
		new G4PVPlacement(
			0,
			G4ThreeVector(),
			fCrystalVolume,
			"SACCrystal",
			fCellVolume,
			false, 0, false);
	}
	else if(sides == 5) // no coating on back
	{
		// create SAC cell with no back (PbF2 crystal + coating)
		G4Box* fZCoating = new G4Box(
			"SACBack",
			0.525 * fCellSizeX,
			0.525 * fCellSizeY,
			0.525 * fCrystalCoating);
		G4VSolid* fNonRefCellSolid = new G4SubtractionSolid(
			"SACNonRefCell",
			fCellSolid,
			fZCoating,
			0,
			G4ThreeVector(0.0, 0.0, -0.5 * (fCellSizeZ - fCrystalCoating)));

		fNonRefCellVolume = new G4LogicalVolume(
			fNonRefCellSolid,
			G4Material::GetMaterial("EJ510Paint"),
			"SACNonRefCell",
			0, 0, 0);
		fNonRefCellVolume->SetVisAttributes(G4VisAttributes(G4Colour::Cyan()));

		new G4PVPlacement(
			0,
			G4ThreeVector(), // PROBABLY NOT SUPPOSED TO BE IN THE MIDDLE, FIX THIS???
			fCrystalVolume,
			"SACCrystal",
			fNonRefCellVolume,
			false, 0, false);
	}
	else // incorrect input
	{
		printf("I don't think anything will break, but the code probably won't work properly...");
	}

	// make PbF2 crystal a sensitive detector
	G4SDManager* SDMan = G4SDManager::GetSDMpointer();
	SACSD* fSACSD = static_cast<SACSD*>(SDMan->FindSensitiveDetector(fSACSDName));
	if(!fSACSD)
	{
		printf("SAC SD %s not found, registering it now\n", fSACSDName.c_str());
		fSACSD = new SACSD(fSACSDName);
		SDMan->AddNewDetector(fSACSD);
	}
	fCrystalVolume->SetSensitiveDetector(fSACSD);
	printf("Added SAC crystal as sensitive detector\n");
}
