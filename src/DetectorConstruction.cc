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
#include "G4Box.hh"
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
	fWorldXLength = 1.0 * m;
	fWorldYLength = 1.0 * m;
	fWorldZLength = 1.0 * m;

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
	fSAC->SetMotherVolume(logicalWorld);
	fSAC->CreateGeometry();

	// make world invisible
	// logicalWorld->SetVisAttributes(G4VisAttributes::Invisible);

	// return physical world
	return physicalWorld;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::DefineMaterials()
{
	// don't call DefineMaterials() again
	static G4bool already_called = false;
	if(already_called) return;
	already_called = true;

	// create custom vacuum material
	G4String name;
	G4double density = CLHEP::universe_mean_density;	// from PhysicalConstants.h
	G4double pressure = 1.0e-19 * pascal;
	G4double temperature = 0.1 * kelvin;
	G4double a, z;										// a = mass of a mole; z = mean number of protons

	G4Material* TubeVacuum = new G4Material(name = "TubeVacuum", z = 1.0, a = 1.01 * g / mole, density, kStateGas, temperature, pressure);

	// standard materials
	G4NistManager* nistMgr = G4NistManager::Instance();
	nistMgr->FindOrBuildMaterial("G4_Galactic");
	nistMgr->FindOrBuildMaterial("TubeVacuum");
	nistMgr->FindOrBuildMaterial("G4_AIR");
	nistMgr->FindOrBuildElement("N");
	nistMgr->FindOrBuildElement("O");
	nistMgr->FindOrBuildElement("Pb");
	nistMgr->FindOrBuildElement("F");
	nistMgr->FindOrBuildElement("Ti");
	nistMgr->FindOrBuildElement("C");
	nistMgr->FindOrBuildElement("H");

	// for extensive output about material definition
	nistMgr->SetVerbose(0);
	// nistMgr->PrintElement("all");
	// nistMgr->ListMaterials("all");

	// "black hole" material: all particles entering it are suppressed
	new G4Material("KLBlackHole", 1.0, 1.0 * g / mole, 1.0 * g / cm3);

	// vacuum: leave some residual air with low density (chamber, world)
	G4Material* Vacuum = new G4Material("Vacuum", (1.290 * 1E-10) * mg / cm3, 2); // 1mbar
	Vacuum->AddElement(G4Element::GetElement("N"), 70.0 * perCent);
	Vacuum->AddElement(G4Element::GetElement("O"), 30.0 * perCent);

	// lead fluoride PbF2 (SAC)
	G4Material* PbF2 = new G4Material("PbF2", 7.77 * g / cm3, 2);
	PbF2->AddElement(G4Element::GetElement("Pb"), 1.0 / 3.0);
	PbF2->AddElement(G4Element::GetElement("F"), 2.0 / 3.0);

	// EJ510 reflective paint (SAC)
	G4Material* EJ510Paint = new G4Material("EJ510Paint", 1.182 * g / cm3, 4);
	EJ510Paint->AddElement(G4Element::GetElement("Ti"), 41.053 * perCent);
	EJ510Paint->AddElement(G4Element::GetElement("C"), 17.194 * perCent);
	EJ510Paint->AddElement(G4Element::GetElement("H"), 2.899 * perCent);
	EJ510Paint->AddElement(G4Element::GetElement("O"), 38.854 * perCent);
}
