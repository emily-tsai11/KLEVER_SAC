// DetectorConstruction.cc
// --------------------------------------------------------------
// History:
//
// Created by Emily Tsai (emily.tsai11@gmail.com) 2020-7-8
// --------------------------------------------------------------

#include "DetectorConstruction.hh"

#include "G4NistManager.hh"
#include "G4RunManager.hh"
// #include "G4SDManager.hh"
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

	DefineMaterials();

	// define world
	fWorldXLength = 1.0 * m;
	fWorldYLength = 1.0 * m;
	fWorldZLength = 1.0 * m;

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
	PbF2->AddElement(G4Element::GetElement("Pb"), 1);
	PbF2->AddElement(G4Element::GetElement("F"), 2);

	// material properties table of PbF2
	G4MaterialPropertiesTable* MPTPbF2 = new G4MaterialPropertiesTable();

	const G4int nEntries = 30;

	// optical properties of PbF2
	G4double PhotonEnergy[nEntries] =
	  { 10.0 * GeV,  20.0 * GeV,  30.0 * GeV,  40.0 * GeV,  50.0 * GeV,
		60.0 * GeV,  70.0 * GeV,  80.0 * GeV,  90.0 * GeV,  100.0 * GeV,
		110.0 * GeV, 120.0 * GeV, 130.0 * GeV, 140.0 * GeV, 150.0 * GeV,
		160.0 * GeV, 170.0 * GeV, 180.0 * GeV, 190.0 * GeV, 200.0 * GeV,
		210.0 * GeV, 220.0 * GeV, 230.0 * GeV, 240.0 * GeV, 250.0 * GeV,
		260.0 * GeV, 270.0 * GeV, 280.0 * GeV, 290.0 * GeV, 300.0 * GeV };
	G4double RefractiveIndexPbF2[nEntries] =
	  { 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82,
		1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82,
		1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82 };
	G4double AbsorptionLengthPbF2[nEntries] =
	  { 0.11 * cm,  0.1 * cm,  0.09 * cm,  0.08 * cm,  0.07 * cm,  0.06 * cm,  0.05 * cm,  0.04 * cm,  0.03 * cm,  0.02 * cm,
		0.11 * mm,  0.1 * mm,  0.09 * mm,  0.08 * mm,  0.07 * mm,  0.06 * mm,  0.05 * mm,  0.04 * mm,  0.03 * mm,  0.02 * mm,
		0.011 * mm, 0.01 * mm, 0.009 * mm, 0.008 * mm, 0.007 * mm, 0.006 * mm, 0.005 * mm, 0.004 * mm, 0.003 * mm, 0.002 * mm };

	MPTPbF2->AddProperty("RINDEX", PhotonEnergy, RefractiveIndexPbF2, nEntries);
	MPTPbF2->AddProperty("ABSLENGTH", PhotonEnergy, AbsorptionLengthPbF2, nEntries);

	// scintillation properties of PbF2
	G4double ScintillationEnergy[nEntries] =
	  { 10.0 * GeV,  20.0 * GeV,  30.0 * GeV,  40.0 * GeV,  50.0 * GeV,
		60.0 * GeV,  70.0 * GeV,  80.0 * GeV,  90.0 * GeV,  100.0 * GeV,
		110.0 * GeV, 120.0 * GeV, 130.0 * GeV, 140.0 * GeV, 150.0 * GeV,
		160.0 * GeV, 170.0 * GeV, 180.0 * GeV, 190.0 * GeV, 200.0 * GeV,
		210.0 * GeV, 220.0 * GeV, 230.0 * GeV, 240.0 * GeV, 250.0 * GeV,
		260.0 * GeV, 270.0 * GeV, 280.0 * GeV, 290.0 * GeV, 300.0 * GeV };
	G4double ScintillationFast[nEntries] =
	  { 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0,
		1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0,
		1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0 };

	MPTPbF2->AddProperty("FASTCOMPONENT", ScintillationEnergy, ScintillationFast, nEntries);
	MPTPbF2->AddConstProperty("SCINTILLATIONYIELD", 40.0 / keV);
	MPTPbF2->AddConstProperty("RESOLUTIONSCALE", 1.0);
	MPTPbF2->AddConstProperty("FASTTIMECONSTANT", 20.0 * ns);
	MPTPbF2->AddConstProperty("YIELDRATIO", 1.0);

	PbF2->SetMaterialPropertiesTable(MPTPbF2);

	// EJ510 reflective paint (SAC)
	G4Material* EJ510Paint = new G4Material("EJ510Paint", 1.182 * g / cm3, 4);
	EJ510Paint->AddElement(G4Element::GetElement("Ti"), 41.053 * perCent);
	EJ510Paint->AddElement(G4Element::GetElement("C"), 17.194 * perCent);
	EJ510Paint->AddElement(G4Element::GetElement("H"), 2.899 * perCent);
	EJ510Paint->AddElement(G4Element::GetElement("O"), 38.854 * perCent);

	// scintillator surface
	
}
