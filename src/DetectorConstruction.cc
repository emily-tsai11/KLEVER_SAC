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

	// DefineMaterials();

	// ------------------------- DEFINE MATERIALS -------------------------
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

	// photon energies & number of entries for optical properties
	G4double PhotonEnergy[] =
	  { 2.034 * eV, 2.068 * eV, 2.103 * eV, 2.139 * eV, 2.177 * eV, 2.216 * eV,
		2.256 * eV, 2.298 * eV, 2.341 * eV, 2.386 * eV, 2.433 * eV, 2.481 * eV,
		2.532 * eV, 2.585 * eV, 2.640 * eV, 2.697 * eV, 2.757 * eV, 2.820 * eV,
		2.885 * eV, 2.954 * eV, 3.026 * eV, 3.102 * eV, 3.181 * eV, 3.265 * eV,
		3.353 * eV, 3.446 * eV, 3.545 * eV, 3.649 * eV, 3.760 * eV, 3.877 * eV,
		4.002 * eV, 4.136 * eV };
	const G4int nEntries = sizeof(PhotonEnergy) / sizeof(G4double); // 32

	// ----------------------- "black hole" -----------------------
	// "black hole" material: all particles entering it are suppressed
	new G4Material("KLBlackHole", 1.0, 1.0 * g / mole, 1.0 * g / cm3);

	// ----------------------- tube vacuum -----------------------
	// optical properties of tube vacuum
	G4double RefractiveIndexTubeVacuum[] =
	  { 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
		1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
		1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00 };

	// material properties table of tube vacuum
	G4MaterialPropertiesTable* MPTTubeVacuum = new G4MaterialPropertiesTable();
	MPTTubeVacuum->AddProperty("RINDEX", PhotonEnergy, RefractiveIndexTubeVacuum, nEntries);
	TubeVacuum->SetMaterialPropertiesTable(MPTTubeVacuum);

	G4cout << "TubeVacuum G4MaterialPropertiesTable:" << G4endl;
	MPTTubeVacuum->DumpTable();

	// ----------------------- EJ510 -----------------------
	// EJ510 reflective paint (SAC)
	G4Material* EJ510Paint = new G4Material("EJ510Paint", 1.182 * g / cm3, 4);
	EJ510Paint->AddElement(G4Element::GetElement("Ti"), 41.053 * perCent);
	EJ510Paint->AddElement(G4Element::GetElement("C"), 17.194 * perCent);
	EJ510Paint->AddElement(G4Element::GetElement("H"), 2.899 * perCent);
	EJ510Paint->AddElement(G4Element::GetElement("O"), 38.854 * perCent);

	// optical properties of EJ510
	G4double RefractiveIndexEJ510[] =
	  { 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
		1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
		1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00 };

	// material properties table of EJ510
	G4MaterialPropertiesTable* MPTEJ510Paint = new G4MaterialPropertiesTable();
	MPTEJ510Paint->AddProperty("RINDEX", PhotonEnergy, RefractiveIndexEJ510, nEntries);
	EJ510Paint->SetMaterialPropertiesTable(MPTEJ510Paint);

	G4cout << "EJ510Paint G4MaterialPropertiesTable:" << G4endl;
	MPTEJ510Paint->DumpTable();

	// ----------------------- PbF2 -----------------------
	// lead fluoride PbF2 (SAC)
	G4Material* PbF2 = new G4Material("PbF2", 7.77 * g / cm3, 2);
	PbF2->AddElement(G4Element::GetElement("Pb"), 1);
	PbF2->AddElement(G4Element::GetElement("F"), 2);

	// optical properties of PbF2
	// think this is for Rayleigh scattering
	G4double RefractiveIndexPbF2[] =
	  { 1.3435, 1.344,  1.3445, 1.345,  1.3455, 1.346,  1.3465, 1.347,
		1.3475, 1.348,  1.3485, 1.3492, 1.35,   1.3505, 1.351,  1.3518,
		1.3522, 1.3530, 1.3535, 1.354,  1.3545, 1.355,  1.3555, 1.356,
		1.3568, 1.3572, 1.358,  1.3585, 1.359,  1.3595, 1.36,   1.3608 };
	G4double AbsorptionLengthPbF2[] =
	  { 3.448 * m,  4.082 * m,  6.329 * m,  9.174 * m,  12.346 * m, 13.889 * m,
		15.152 * m, 17.241 * m, 18.868 * m, 20.000 * m, 26.316 * m, 35.714 * m,
		45.455 * m, 47.619 * m, 52.632 * m, 52.632 * m, 55.556 * m, 52.632 * m,
		52.632 * m, 47.619 * m, 45.455 * m, 41.667 * m, 37.037 * m, 33.333 * m,
		30.000 * m, 28.500 * m, 27.000 * m, 24.500 * m, 22.000 * m, 19.500 * m,
		17.500 * m, 14.500 * m };

	G4double ScintFastPbF2[] =
	  { 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
		1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
		1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00 };
	G4double ScintSlowPbF2[] =
	  { 0.01, 1.00, 2.00, 3.00, 4.00, 5.00, 6.00, 7.00, 8.00, 9.00, 8.00, 7.00,
		6.00, 4.00, 3.00, 2.00, 1.00, 0.01, 1.00, 2.00, 3.00, 4.00, 5.00, 6.00,
		7.00, 8.00, 9.00, 8.00, 7.00, 6.00, 5.00, 4.00 };

	// for Mie scattering
	G4double EnergyPbF2[] =
	  { 1.56962 * eV, 1.58974 * eV, 1.61039 * eV, 1.63157 * eV, 1.65333 * eV,
		1.67567 * eV, 1.69863 * eV, 1.72222 * eV, 1.74647 * eV, 1.77142 * eV,
		1.7971  * eV, 1.82352 * eV, 1.85074 * eV, 1.87878 * eV, 1.90769 * eV,
		1.93749 * eV, 1.96825 * eV, 1.99999 * eV, 2.03278 * eV, 2.06666 * eV,
		2.10169 * eV, 2.13793 * eV, 2.17543 * eV, 2.21428 * eV, 2.25454 * eV,
		2.29629 * eV, 2.33962 * eV, 2.38461 * eV, 2.43137 * eV, 2.47999 * eV,
		2.53061 * eV, 2.58333 * eV, 2.63829 * eV, 2.69565 * eV, 2.75555 * eV,
		2.81817 * eV, 2.88371 * eV, 2.95237 * eV, 3.02438 * eV, 3.09999 * eV,
		3.17948 * eV, 3.26315 * eV, 3.35134 * eV, 3.44444 * eV, 3.54285 * eV,
		3.64705 * eV, 3.75757 * eV, 3.87499 * eV, 3.99999 * eV, 4.13332 * eV,
		4.27585 * eV, 4.42856 * eV, 4.59258 * eV, 4.76922 * eV, 4.95999 * eV,
		5.16665 * eV, 5.39129 * eV, 5.63635 * eV, 5.90475 * eV, 6.19998 * eV };
	G4double MiePbF2[] =
	  { 167024.4 * m, 158726.7 * m, 150742 * m,   143062.5 * m, 135680.2 * m,
		128587.4 * m, 121776.3 * m, 115239.5 * m, 108969.5 * m, 102958.8 * m,
		97200.35 * m, 91686.86 * m, 86411.33 * m, 81366.79 * m, 76546.42 * m,
		71943.46 * m, 67551.29 * m, 63363.36 * m, 59373.25 * m, 55574.61 * m,
		51961.24 * m, 48527.00 * m, 45265.87 * m, 42171.94 * m, 39239.39 * m,
		36462.50 * m, 33835.68 * m, 31353.41 * m, 29010.30 * m, 26801.03 * m,
		24720.42 * m, 22763.36 * m, 20924.88 * m, 19200.07 * m, 17584.16 * m,
		16072.45 * m, 14660.38 * m, 13343.46 * m, 12117.33 * m, 10977.70 * m,
		9920.416 * m, 8941.407 * m, 8036.711 * m, 7202.470 * m, 6434.927 * m,
		5730.429 * m, 5085.425 * m, 4496.467 * m, 3960.210 * m, 3473.413 * m,
		3032.937 * m, 2635.746 * m, 2278.907 * m, 1959.588 * m, 1675.064 * m,
		1422.710 * m, 1200.004 * m, 1004.528 * m, 833.9666 * m, 686.1063 * m };
	const G4int nEntriesPbF2 = sizeof(EnergyPbF2) / sizeof(G4double);
	G4double MieConstPbF2[3] = {0.99, 0.99, 0.8};

	// material properties table of PbF2
	G4MaterialPropertiesTable* MPTPbF2 = new G4MaterialPropertiesTable();
	MPTPbF2->AddProperty("RINDEX", PhotonEnergy, RefractiveIndexPbF2, nEntries);
	MPTPbF2->AddProperty("ABSLENGTH", PhotonEnergy, AbsorptionLengthPbF2, nEntries);
	MPTPbF2->AddProperty("FASTCOMPONENT", PhotonEnergy, ScintFastPbF2, nEntries);
	MPTPbF2->AddProperty("FASTCOMPONENT", PhotonEnergy, ScintSlowPbF2, nEntries);

	MPTPbF2->AddConstProperty("SCINTILLATIONYIELD", 50.0 / MeV);
	MPTPbF2->AddConstProperty("RESOLUTIONSCALE", 1.0);
	MPTPbF2->AddConstProperty("FASTTIMECONSTANT", 1.0 * ns);
	MPTPbF2->AddConstProperty("SLOWTIMECONSTANT", 10.0 * ns);
	MPTPbF2->AddConstProperty("YIELDRATIO", 0.8);

	MPTPbF2->AddProperty("MIEHG", EnergyPbF2, MiePbF2, nEntriesPbF2)->SetSpline(true);
	MPTPbF2->AddConstProperty("MIEHG_FORWARD", MieConstPbF2[0]);
	MPTPbF2->AddConstProperty("MIEHG_BACKWARD", MieConstPbF2[1]);
	MPTPbF2->AddConstProperty("MIEHG_FORWARD_RATIO", MieConstPbF2[2]);

	PbF2->SetMaterialPropertiesTable(MPTPbF2);

	G4cout << "PbF2 G4MaterialPropertiesTable:" << G4endl;
	MPTPbF2->DumpTable();

	// set Birks Constant for PbF2 scintillator
	PbF2->GetIonisation()->SetBirksConstant(0.126 * mm / MeV);

	// ------------------------- DEFINE VOLUMES -------------------------
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

	// ------------------------- DEFINE SURFACES -------------------------
	// TODO

	// create SAC detector
	fSAC->SetMaterial(G4Material::GetMaterial("TubeVacuum"));
	fSAC->SetMotherVolume(logicalWorld);
	fSAC->CreateGeometry();

	// make world invisible
	// logicalWorld->SetVisAttributes(G4VisAttributes::Invisible);

	// return physical world
	return physicalWorld;
}
