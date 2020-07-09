// DetectorConstruction.cc
// --------------------------------------------------------------
// History:
//
// Created by Emily Tsai (emily.tsai11@gmail.com) 2020-7-8
// --------------------------------------------------------------

#include "DetectorConstruction.hh"

#include "G4NistManager.hh"
#include "G4RunManager.hh"
#include "G4SDManager.hh"
#include "G4GeometryManager.hh"

#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4LogicalSkinSurface.hh"

#include "G4Material.hh"
#include "G4LogicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4PVPlacement.hh"
#include "G4VisAttributes.hh"

#include "SACDetector.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::DetectorConstruction() : solidWorld(0), logicalWorld(0), physicalWorld(0)
{
	fSAC = new SACDetector(0, 0);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
DetectorConstruction::~DetectorConstruction()
{
	delete fSAC;
}

G4VPhysicalVolume* DetectorConstruction::Construct()
{
	// clean old geometry if it exists
	G4GeometryManager::GetInstance()->OpenGeometry();
	G4PhysicalVolumeStore::GetInstance()->Clean();
	G4LogicalVolumeStore::GetInstance()->Clean();
	G4SolidStore::GetInstance()->Clean();
	G4LogicalBorderSurface::CleanSurfaceTable();
	G4LogicalSkinSurface::CleanSurfaceTable();

	// define world
	fWorldXLength = 10.0 * m;
	fWorldYLength = 10.0 * m;
	fWorldZLength = 600.0 * m;

	solidWorld = new G4Box("World", 0.5 * fWorldXLength, 0.5 * fWorldYLength, 0.5 * fWorldZLength);

	logicalWorld = new G4LogicalVolume(
		solidWorld,								// solid
		G4Material::GetMaterial("TubeVacuum"),	// material
		"World",								// name
		0,										// field manager
		0,										// sensitive detector
		0);										// user limits

	physicalWorld = new G4PVPlacement(
		0,										// no rotation
		G4ThreeVector(),						// at (0, 0, 0)
		logicalWorld,							// logical volume
		"World",								// name
		0,										// mother volume
		false,									// no boolean operations
		0);										// copy number

	// create SAC detector
	fSAC->SetMaterial(G4Material::GetMaterial("TubeVacuum"));
	fSAC->SetMotherVolume(logicWorld);
	fSAC->CreateGeometry();

	// make world invisible
	logicWorld->SetVisAttributes(G4VisAttributes::Invisible);

	// return physical world
	return physicalWorld;
}
