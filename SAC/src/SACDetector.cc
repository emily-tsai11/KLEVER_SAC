// SACDetector.cc
// --------------------------------------------------------------
// History:
//
// Created by Emanuele Leonardi (emanuele.leonardi@roma1.infn.it) 2015-12-11
// Adapted from PADME experiment by Emily Tsai (emily.tsai11@gmail.com) 2020-7-7
// --------------------------------------------------------------

#include "SACDetector.hh"

#include "G4PVPlacement.hh"
#include "G4ThreeVector.hh"
#include "G4RotationMatrix.hh"
#include "G4Box.hh"
#include "G4SDManager.hh"
#include "G4Material.hh"
#include "G4VisAttributes.hh"

#include "SACMessenger.hh"
#include "SACGeometry.hh"
#include "SACSD.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SACDetector::SACDetector(G4Material* material, G4LogicalVolume* motherVolume) : fMaterial(material), fMotherVolume(motherVolume)
{
	// connect to SACMessenger to enable datacard configuration
	fSACMessenger = new SACMessenger(this);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SACDetector::~SACDetector()
{
	delete fSACMessenger;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SACDetector::CreateGeometry()
{
	SACGeometry* geo = SACGeometry::GetInstance();

	// create main SAC box
	G4double sacPosX = geo->GetSACPosX();
	G4double sacPosY = geo->GetSACPosY();
	G4double sacPosZ = geo->GetSACPosZ();
	G4ThreeVector sacPos = G4ThreeVector(sacPosX, sacPosY, sacPosZ);
	printf("SAC will be placed at %f %f %f\n", sacPosX, sacPosY, sacPosZ);

	G4double sacSizeX = geo->GetSACSizeX();
	G4double sacSizeY = geo->GetSACSizeY();
	G4double sacSizeZ = geo->GetSACSizeZ();
	printf("SAC size is %f %f %f\n", sacSizeX, sacSizeY, sacSizeZ);

	G4Box* solidSAC = new G4Box("SAC", 0.5 * sacSizeX, 0.5 * sacSizeY, 0.5 * sacSizeZ);
	fSACVolume = new G4LogicalVolume(solidSAC, fMaterial, "SAC", 0, 0, 0);
	// fSACVolume->SetVisAttributes(G4VisAttributes::Invisible);

	G4RotationMatrix* Rot = new G4RotationMatrix;
	Rot->rotateY(0.1 * rad);
	Rot->rotateX(0.1 * rad);
	new G4PVPlacement(Rot, sacPos, fSACVolume, "SAC", fMotherVolume, false, 0, false);

	// show size of gap between crystals
	printf("Gap between SAC crystals is %f\n", geo->GetCrystalGap());

	// show thickness of coating around crystals
	printf("Coating around SAC crystals is %f\n", geo->GetCrystalCoating());

	// create standard PbF2 crystal
	G4double crySizeX = geo->GetCrystalSizeX();
	G4double crySizeY = geo->GetCrystalSizeY();
	G4double crySizeZ = geo->GetCrystalSizeZ();
	printf("SAC Crystal size is %f %f %f\n", crySizeX, crySizeY, crySizeZ);

	G4Box* solidCry = new G4Box("SACCry", 0.5 * crySizeX, 0.5 * crySizeY, 0.5 * crySizeZ);
	fCrystalVolume = new G4LogicalVolume(solidCry, G4Material::GetMaterial("PbF2"), "SACCry", 0, 0, 0);
	fCrystalVolume->SetVisAttributes(G4VisAttributes(G4Colour::Magenta()));

	// make crystal a sensitive detector
	G4SDManager* sdMan = G4SDManager::GetSDMpointer();
	G4String sacSDName = geo->GetSACSensitiveDetectorName();
	printf("Registering SAC SD %s\n", sacSDName.data());
	SACSD* sacSD = new SACSD(sacSDName);
	sdMan->AddNewDetector(sacSD);

	fCrystalVolume->SetSensitiveDetector(sacSD);

	// create SAC cell (PbF2 crystal + coating)
	G4double cellSizeX = geo->GetCellSizeX();
	G4double cellSizeY = geo->GetCellSizeY();
	G4double cellSizeZ = geo->GetCellSizeZ();
	printf("SAC cell size is %f %f %f\n", cellSizeX, cellSizeY, cellSizeZ);

	G4Box* solidCell = new G4Box("SACCell", 0.5 * cellSizeX, 0.5 * cellSizeY, 0.5 * cellSizeZ);
	fCellVolume = new G4LogicalVolume(solidCell, G4Material::GetMaterial("EJ510Paint"), "SACCell", 0, 0, 0);
	fCellVolume->SetVisAttributes(G4VisAttributes(G4Colour::Cyan()));

	// position SAC crystal inside cell
	new G4PVPlacement(0, G4ThreeVector(0.0, 0.0, 0.0), fCrystalVolume, "SACCry", fCellVolume, false, 0, false);

	// get number of rows and columns of crystals and position all crystals
	G4int nRow = geo->GetSACNRows();
	G4int nCol = geo->GetSACNCols();
	G4int nLayers = geo->GetSACNLayers();

	G4double Zoffset = -1.5 * geo->GetCellSizeZ();

	// i should repeat the structure for different layers
	for(G4int layer = 0; layer < nLayers; layer++)
	{
		for(G4int row = 0; row < nRow; row++)
		{
			for(G4int col = 0; col < nCol; col++)
			{
				if(geo->ExistsCrystalAt(row, col))
				{
					G4ThreeVector positionCry = G4ThreeVector(geo->GetCrystalPosX(row, col), geo->GetCrystalPosY(row, col), geo->GetCrystalPosZ(row, col) + Zoffset);
					G4int idxCell = row * SACGEOMETRY_N_COLS_MAX + col + layer * nRow * nCol;
					if(idxCell % 100 == 0)
					{
						printf("Crystal position %f %f %f\n", geo->GetCrystalPosX(row, col), geo->GetCrystalPosY(row, col), geo->GetCrystalPosZ(row, col) + Zoffset);
						printf("*******idxCell %d\n", idxCell);
					}
					new G4PVPlacement(0, positionCry, fCellVolume, "SACCell", fSACVolume, false, idxCell, false);
					// G4PVPlacement* daughter = new G4PVPlacement(0, positionCry, fCellVolume, "SACCell", fSACVolume, false, idxCell, false);
					// G4LogicalVolume* daughter_log = daughter->GetLogicalVolume();
					// if(daughter->CheckOverlaps(1000, false))
					// {
					// 	printf("WARNING - overlaps found in %s\n", daughter_log->GetName().data());
					// 	return;
					// }
				}
			}
		}
		Zoffset += geo->GetCellSizeZ();
	}
}
