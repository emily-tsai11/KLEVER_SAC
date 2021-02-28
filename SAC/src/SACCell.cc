// SACCell.cc
// --------------------------------------------------------------
// History:
//
// Created by Emily Tsai (emily.tsai11@gmail.com) 2021-1-6
// --------------------------------------------------------------

#include "SACCell.hh"

#include "G4Material.hh"
#include "G4Box.hh"
#include "G4VisAttributes.hh"
#include "G4ThreeVector.hh"
#include "G4PVPlacement.hh"
#include "G4SDManager.hh"

#include "SACGeometry.hh"
#include "SACSD.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SACCell::SACCell()
{
	SACGeometry* Geo = SACGeometry::GetInstance();

	fCrystalSizeX = Geo->GetCrystalSizeX();
	fCrystalSizeY = Geo->GetCrystalSizeY();
	fCrystalSizeZ = Geo->GetCrystalSizeZ();
	printf("[SACCell::SACCell] SAC crystal size is %f %f %f\n", fCrystalSizeX, fCrystalSizeY, fCrystalSizeZ);

	fCrystalCoating = Geo->GetCrystalCoating();
	printf("[SACCell::SACCell] Coating around SAC crystals is %f\n", fCrystalCoating);

	fCellSizeX = Geo->GetCellSizeX();
	fCellSizeY = Geo->GetCellSizeY();
	fCellSizeZ = Geo->GetCellSizeZ();
	printf("[SACCell::SACCell] SAC cell size is %f %f %f\n", fCellSizeX, fCellSizeY, fCellSizeZ);

	fSACSDName = Geo->GetSACSensitiveDetectorName();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SACCell::~SACCell() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SACCell::CreateGeometry()
{
	// Create PbF2 crystal
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

	// Create paint coating
	G4Box* fCellSolid = new G4Box(
		"SACCell",
		0.5 * fCellSizeX,
		0.5 * fCellSizeY,
		0.5 * fCellSizeZ);

	fCellVolume = new G4LogicalVolume(
		fCellSolid,
		G4Material::GetMaterial("EJ510Paint"),
		"SACCell",
		0, 0, 0);
	fCellVolume->SetVisAttributes(G4VisAttributes(G4Colour::Magenta()));

	// Place crystal inside paint coating
	new G4PVPlacement(
		0,
		G4ThreeVector(0.0, 0.0, -0.5 * fCrystalCoating),
		fCrystalVolume,
		"SACCrystal",
		fCellVolume,
		false, 0, false);

	// Make PbF2 crystal a sensitive detector
	G4SDManager* SDMan = G4SDManager::GetSDMpointer();
	SACSD* fSACSD = static_cast<SACSD*>(SDMan->FindSensitiveDetector(fSACSDName));
	if(!fSACSD)
	{
		printf("[SACCell::CreateGeometry] SAC SD %s not found, registering it now\n", fSACSDName.c_str());
		fSACSD = new SACSD(fSACSDName);
		SDMan->AddNewDetector(fSACSD);
	}
	fCrystalVolume->SetSensitiveDetector(fSACSD);
	printf("[SACCell::CreateGeometry] Added SAC crystal as sensitive detector\n");
}
