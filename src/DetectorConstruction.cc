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

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

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

	DefineMaterials();

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

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::DefineMaterials()
{
	// don't call DefineMaterials() again
	static G4bool already_called = kFALSE;
	if(already_called) return;
	already_called = kTRUE;

	// create custom vacuum material
	G4String name;
	G4double density = universe_mean_density;	// from PhysicalConstants.h
	G4double pressure = 1.0e-19 * pascal;
	G4double temperature = 0.1 * kelvin;
	G4double a, z;								// a = mass of a mole; z = mean number of protons

	G4Material* TubeVacuum = new G4Material(name = "TubeVacuum", z = 1.0, a = 1.01 * g/mole, density, kStateGas, temperature, pressure);

	// standard materials
	G4NistManager* nistMgr = G4NistManager::Instance();
	nistMgr->FindOrBuildMaterial("G4_Galactic");
	nistMgr->FindOrBuildMaterial("TubeVacuum");
    nistMgr->FindOrBuildMaterial("G4_AIR");

	// for extensive output about material definition
	nistMgr->SetVerbose(0);
	// nistMgr->PrintElement("all");
	// nistMgr->ListMaterials("all");

	// "black hole" material: all particles entering it are suppressed
	new G4Material("KLBlackHole", 1.0, 1.0 * g/mole, 1.0 * g/cm3);
}
