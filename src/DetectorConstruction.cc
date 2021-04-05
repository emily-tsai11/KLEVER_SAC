// DetectorConstruction.cc
// --------------------------------------------------------------
// History:
//
// Created by Emily Tsai (emily.tsai11@gmail.com) 2020-7-8
// - Optical properties taken from NA62 LAVs
// --------------------------------------------------------------

#include "DetectorConstruction.hh"
#include "G4SystemOfUnits.hh"

#include "G4NistManager.hh"
#include "G4RunManager.hh"
#include "G4GeometryManager.hh"

#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4OpticalSurface.hh"

#include "G4Material.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4ThreeVector.hh"
#include "G4PVPlacement.hh"
#include "G4VisAttributes.hh"

#include "DetectorConstructionMessenger.hh"
#include "SACDetector.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::DetectorConstruction() : G4VUserDetectorConstruction(),
	fSolidWorld(0), fLogicalWorld(0), fPhysicalWorld(0)
{
	fMessenger = new DetectorConstructionMessenger(this);
	fSACDetector = new SACDetector(0, 0);

	fWorldLengthX = 4.0 * m;
	fWorldLengthY = 4.0 * m;
	fWorldLengthZ = 4.0 * m;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::~DetectorConstruction()
{
	delete fSACDetector;
	delete fMessenger;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* DetectorConstruction::Construct()
{
	// Clean old geometry (if it exists)
	CleanGeometry();

	// Define materials
	DefineMaterials();

	// Create world
	fSolidWorld = new G4Box("World", 0.5 * fWorldLengthX, 0.5 * fWorldLengthY, 0.5 * fWorldLengthZ);
	fLogicalWorld = new G4LogicalVolume(
		fSolidWorld,							// Solid
		G4Material::GetMaterial("G4_Galactic"),	// Material
		"World",								// Name
		0,										// Field manager
		0,										// Sensitive detector
		0);										// User limits
	fPhysicalWorld = new G4PVPlacement(
		0,										// No rotation
		G4ThreeVector(),						// At (0, 0, 0)
		fLogicalWorld,							// Logical volume
		"World",								// Name
		0,										// Mother volume
		false,									// No boolean operations
		0);										// Copy number
	// fLogicalWorld->SetVisAttributes(G4VisAttributes::Invisible);
	fLogicalWorld->SetVisAttributes(G4VisAttributes(G4Colour::Cyan()));

	// Create SAC detector
	fSACDetector->SetMaterial(G4Material::GetMaterial("G4_Galactic"));
	fSACDetector->SetMotherVolume(fLogicalWorld);
	fSACDetector->CreateGeometry();

	// Return world
	return fPhysicalWorld;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::CleanGeometry()
{
	G4GeometryManager::GetInstance()->OpenGeometry();
	G4PhysicalVolumeStore::GetInstance()->Clean();
	G4LogicalVolumeStore::GetInstance()->Clean();
	G4SolidStore::GetInstance()->Clean();
	G4LogicalBorderSurface::CleanSurfaceTable();
	G4LogicalSkinSurface::CleanSurfaceTable();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::DefineMaterials()
{
	// --------------------------- DEFINE MATERIALS ---------------------------
	// Standard materials
	G4NistManager* nistMgr = G4NistManager::Instance();
	nistMgr->FindOrBuildMaterial("G4_Galactic");
	nistMgr->FindOrBuildElement("N");
	nistMgr->FindOrBuildElement("O");
	nistMgr->FindOrBuildElement("Pb");
	nistMgr->FindOrBuildElement("F");
	nistMgr->FindOrBuildElement("Ti");
	nistMgr->FindOrBuildElement("C");
	nistMgr->FindOrBuildElement("H");

	// For extensive output about material definition
	nistMgr->SetVerbose(0);
	// nistMgr->PrintElement("all");
	// nistMgr->ListMaterials("all");

	// ----------------------------- "BLACK HOLE" -----------------------------
	// "Black hole" material: all particles entering it are suppressed
	new G4Material("KLBlackHole", 1.0, 1.0 * g / mole, 1.0 * g / cm3);

	// --------------------------------- EJ510 ---------------------------------
	// EJ510 reflective paint (SAC)
	G4Material* EJ510Paint = new G4Material("EJ510Paint", 1.182 * g / cm3, 4);
	EJ510Paint->AddElement(G4Element::GetElement("Ti"), 41.053 * perCent);
	EJ510Paint->AddElement(G4Element::GetElement("C"), 17.194 * perCent);
	EJ510Paint->AddElement(G4Element::GetElement("H"), 2.899 * perCent);
	EJ510Paint->AddElement(G4Element::GetElement("O"), 38.854 * perCent);

	// Optical properties of EJ510 paint
	G4int EJ510PaintOpticSize = 2;
	G4double EJ510PaintOpticEnergies[] = {1.88 * eV, 3.1 * eV};
	G4double EJ510PaintOpticZero[] = {0.0, 0.0};
	G4double EJ510PaintRef[] = {0.9, 0.9};

	// Material properties table of EJ510 paint
	G4MaterialPropertiesTable* MPTEJ510Paint = new G4MaterialPropertiesTable();
	MPTEJ510Paint->AddProperty("REFLECTIVITY", EJ510PaintOpticEnergies, EJ510PaintRef, EJ510PaintOpticSize);
	MPTEJ510Paint->AddProperty("EFFICIENCY", EJ510PaintOpticEnergies, EJ510PaintOpticZero, EJ510PaintOpticSize);
    MPTEJ510Paint->AddProperty("SPECULARLOBECONSTANT", EJ510PaintOpticEnergies, EJ510PaintOpticZero, EJ510PaintOpticSize);
    MPTEJ510Paint->AddProperty("SPECULARSPIKECONSTANT", EJ510PaintOpticEnergies, EJ510PaintOpticZero, EJ510PaintOpticSize);
    MPTEJ510Paint->AddProperty("BACKSCATTERCONSTANT", EJ510PaintOpticEnergies, EJ510PaintOpticZero, EJ510PaintOpticSize);

	// Optical surface of EJ510 paint
	G4OpticalSurface* EJ510PaintOptSurf = new G4OpticalSurface("Diffusive", unified, groundfrontpainted, dielectric_dielectric);
	EJ510PaintOptSurf->SetMaterialPropertiesTable(MPTEJ510Paint);

	// G4cout << "[DetectorConstruction::DefineMaterials] EJ510 Paint MaterialPropertiesTable:" << G4endl;
	// MPTEJ510Paint->DumpTable();

	// --------------------------------- PbF2 ---------------------------------
	// Lead fluoride PbF2 (SAC)
	G4Material* PbF2 = new G4Material("PbF2", 7.77 * g / cm3, 2);
	PbF2->AddElement(G4Element::GetElement("Pb"), 1);
	PbF2->AddElement(G4Element::GetElement("F"), 2);

	// Optical properties of PbF2
	G4double PhotonEnergy[] =
	  { 0.104 * eV, 0.151 * eV, 0.218 * eV, 0.314 * eV, 0.454 * eV,
		0.656 * eV, 0.948 * eV, 1.370 * eV, 1.980 * eV, 2.860 * eV,
		4.133 * eV };
	G4double RefractiveIndexPbF2[] =
	  { 1.596, 1.668, 1.701, 1.717, 1.725, 1.731, 1.736, 1.744, 1.762, 1.803,
		1.937 };
	G4double AbsorptionLengthPbF2[] =
	  { 10.0 * m, 10.0 * m, 10.0 * m, 10.0 * m, 10.0 * m,
	 	10.0 * m, 10.0 * m, 10.0 * m, 10.0 * m, 10.0 * m,
		10.0 * m };
	const G4int nEntries = sizeof(PhotonEnergy) / sizeof(G4double); // 11

	// Material properties table of PbF2
	G4MaterialPropertiesTable* MPTPbF2 = new G4MaterialPropertiesTable();
	MPTPbF2->AddProperty("RINDEX", PhotonEnergy, RefractiveIndexPbF2, nEntries);
	MPTPbF2->AddProperty("ABSLENGTH", PhotonEnergy, AbsorptionLengthPbF2, nEntries);
	PbF2->SetMaterialPropertiesTable(MPTPbF2);

	// G4cout << "[DetectorConstruction::DefineMaterials] PbF2 MaterialPropertiesTable:" << G4endl;
	// MPTPbF2->DumpTable();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::UpdateGeometry()
{
	G4RunManager::GetRunManager()->GeometryHasBeenModified();
	G4RunManager::GetRunManager()->DefineWorldVolume(Construct());
}
