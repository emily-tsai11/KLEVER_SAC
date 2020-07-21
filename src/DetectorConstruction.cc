// DetectorConstruction.cc
// --------------------------------------------------------------
// History:
//
// Created by Emily Tsai (emily.tsai11@gmail.com) 2020-7-8
//		optical properties based on OpNovice
// --------------------------------------------------------------

#include "DetectorConstruction.hh"

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

	// ------------------------- DEFINE MATERIALS -------------------------
	// standard materials
	G4NistManager* nistMgr = G4NistManager::Instance();
	nistMgr->FindOrBuildMaterial("G4_Galactic");
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

	// ----------------------- "black hole" -----------------------
	// "black hole" material: all particles entering it are suppressed
	new G4Material("KLBlackHole", 1.0, 1.0 * g / mole, 1.0 * g / cm3);

	// ----------------------- EJ510 -----------------------
	// EJ510 reflective paint (SAC)
	G4Material* EJ510Paint = new G4Material("EJ510Paint", 1.182 * g / cm3, 4);
	EJ510Paint->AddElement(G4Element::GetElement("Ti"), 41.053 * perCent);
	EJ510Paint->AddElement(G4Element::GetElement("C"), 17.194 * perCent);
	EJ510Paint->AddElement(G4Element::GetElement("H"), 2.899 * perCent);
	EJ510Paint->AddElement(G4Element::GetElement("O"), 38.854 * perCent);

	// optical properties of EJ510Paint
	G4int EJ510PaintOpticSize = 2;
	G4double EJ510PaintOpticEnergies[] = {1.88 * eV, 3.1 * eV};
	G4double EJ510PaintOpticZero[] = {0.0, 0.0};
	G4double EJ510PaintRef[] = {0.9, 0.9};

	// material properties table of EJ510Paint
	G4MaterialPropertiesTable* MPTEJ510Paint = new G4MaterialPropertiesTable();
	MPTEJ510Paint->AddProperty("REFLECTIVITY", EJ510PaintOpticEnergies, EJ510PaintRef, EJ510PaintOpticSize);
	MPTEJ510Paint->AddProperty("EFFICIENCY", EJ510PaintOpticEnergies, EJ510PaintOpticZero, EJ510PaintOpticSize);
    MPTEJ510Paint->AddProperty("SPECULARLOBECONSTANT", EJ510PaintOpticEnergies, EJ510PaintOpticZero, EJ510PaintOpticSize);
    MPTEJ510Paint->AddProperty("SPECULARSPIKECONSTANT", EJ510PaintOpticEnergies, EJ510PaintOpticZero, EJ510PaintOpticSize);
    MPTEJ510Paint->AddProperty("BACKSCATTERCONSTANT", EJ510PaintOpticEnergies, EJ510PaintOpticZero, EJ510PaintOpticSize);

	// optical surface of EJ510Paint
	G4OpticalSurface* EJ510PaintOptSurf = new G4OpticalSurface("Diffusive", unified, groundfrontpainted, dielectric_dielectric);
	EJ510PaintOptSurf->SetMaterialPropertiesTable(MPTEJ510Paint);

	// ----------------------- PbF2 -----------------------
	// lead fluoride PbF2 (SAC)
	G4Material* PbF2 = new G4Material("PbF2", 7.77 * g / cm3, 2);
	PbF2->AddElement(G4Element::GetElement("Pb"), 1);
	PbF2->AddElement(G4Element::GetElement("F"), 2);

	// optical properties of PbF2
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

	// material properties table of PbF2
	G4MaterialPropertiesTable* MPTPbF2 = new G4MaterialPropertiesTable();
	MPTPbF2->AddProperty("RINDEX", PhotonEnergy, RefractiveIndexPbF2, nEntries);
	MPTPbF2->AddProperty("ABSLENGTH", PhotonEnergy, AbsorptionLengthPbF2, nEntries);
	PbF2->SetMaterialPropertiesTable(MPTPbF2);

	G4cout << "PbF2 G4MaterialPropertiesTable:" << G4endl;
	MPTPbF2->DumpTable();

	// ------------------------- DEFINE VOLUMES -------------------------
	fWorldXLength = 1.0 * m;
	fWorldYLength = 1.0 * m;
	fWorldZLength = 1.0 * m;

	solidWorld = new G4Box("World", 0.5 * fWorldXLength, 0.5 * fWorldYLength, 0.5 * fWorldZLength);

	logicalWorld = new G4LogicalVolume(
		solidWorld,								// solid
		G4Material::GetMaterial("G4_Galactic"),	// material
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
	fSAC->SetMaterial(G4Material::GetMaterial("G4_Galactic"));
	fSAC->SetMotherVolume(logicalWorld);
	fSAC->CreateGeometry();

	// make world invisible
	// logicalWorld->SetVisAttributes(G4VisAttributes::Invisible);

	// return physical world
	return physicalWorld;
}
