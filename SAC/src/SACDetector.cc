// SACDetector.cc
// --------------------------------------------------------------
// History:
//
// Created by Emanuele Leonardi (emanuele.leonardi@roma1.infn.it) 2015-12-11
// Adapted from PADME experiment by Emily Tsai (emily.tsai11@gmail.com) 2020-7-7
// --------------------------------------------------------------

#include "SACDetector.hh"
#include "G4SystemOfUnits.hh"

#include "G4Material.hh"
#include "G4Box.hh"
#include "G4VisAttributes.hh"
#include "G4RotationMatrix.hh"
#include "G4ThreeVector.hh"
#include "G4PVPlacement.hh"
#include "G4SDManager.hh"

#include "SACGeometry.hh"
#include "SACMessenger.hh"
#include "SACCell.hh"
#include "SACPMT.hh"
#include "SACSiPM.hh"
#include "SACSD.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SACDetector::SACDetector(G4Material* material, G4LogicalVolume* motherVolume) :
	fMaterial(material), fMotherVolume(motherVolume)
{
	fCell = new SACCell();
	fSACMessenger = new SACMessenger();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SACDetector::~SACDetector()
{
	if(!fPMT) delete fPMT;
	if(!fSiPM) delete fSiPM;

	delete fCell;
	delete fSACMessenger;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SACDetector::CreateGeometry()
{
	// Read in geometry parameters
	ReadGeometry();

	// Create main SAC box
	G4Box* fSACSolid = new G4Box(
		"SAC",
		0.5 * fSACSizeX,
		0.5 * fSACSizeY,
		0.5 * fSACSizeZ);

	fSACVolume = new G4LogicalVolume(
		fSACSolid,
		fMaterial,
		"SAC",
		0, 0, 0);
	fSACVolume->SetVisAttributes(G4VisAttributes(G4Colour::White()));

	G4RotationMatrix* fRot = new G4RotationMatrix;
	fRot->rotateY(0.1 * rad);
	fRot->rotateX(0.1 * rad);
	new G4PVPlacement(
		fRot,
		G4ThreeVector(fSACPosX, fSACPosY, fSACPosZ),
		fSACVolume,
		"SAC",
		fMotherVolume,
		false, 0, false);

	// Create SAC cell
	fCell->CreateGeometry();

	// Create SAC PMT or SiPM
	if(fEnablePMT && !fEnableSiPM)
	{
		if(!fSiPM) delete fSiPM;
		fPMT = new SACPMT();
		fPMT->CreateGeometry();
	}
	else if(fEnableSiPM && !fEnablePMT)
	{
		if(!fPMT) delete fPMT;
		fSiPM = new SACSiPM();
		fSiPM->CreateGeometry();
	}

	// Position all crystals
	G4double fCrystalPosX;
	G4double fCrystalPosY;
	G4double fCrystalPosZ;
	for(G4int layer = 0; layer < fNLayers; layer++)
	{
		for(G4int row = 0; row < fNRows; row++)
		{
			for(G4int col = 0; col < fNCols; col++)
			{
				// Get crystal position coordinates
				fCrystalPosX = (fCellSizeX + fCellGap) * (-fNCols * 0.5 + col + 0.5);
				fCrystalPosY = (fCellSizeY + fCellGap) * (-fNRows * 0.5 + row + 0.5);
				fCrystalPosZ = ((fNLayers / 2.0 - 0.5) - layer) * (fCellSizeZ + fLayerGap);
				if(fEnablePMT && !fEnableSiPM) fCrystalPosZ += fPMTThickness / 2.0;
				else if(fEnableSiPM && !fEnablePMT) fCrystalPosZ += fSiPMThickness / 2.0;

				G4int CellID = row * fNCols + col + layer * fNRows * fNCols;
				if(CellID % 100 == 0)
				{
					printf("[SACDetector::CreateGeometry] Crystal position %f %f %f\n", fCrystalPosX, fCrystalPosY, fCrystalPosZ);
					printf("[SACDetector::CreateGeometry] CellID: %d\n", CellID);
				}

				// Place the crystals
				G4ThreeVector fCrystalPos = G4ThreeVector(fCrystalPosX, fCrystalPosY, fCrystalPosZ);
				new G4PVPlacement(
					0,
					fCrystalPos,
					fCell->GetCellLogicalVolume(),
					"SACCell",
					fSACVolume,
					false, CellID, false);
				G4int fPMID = row * fNCols + col;

				// Place the photomultipliers
				if(fEnablePMT && !fEnableSiPM)
				{
					new G4PVPlacement(
						0,
						G4ThreeVector(fCrystalPosX, fCrystalPosY, fCrystalPosZ - 0.5 * (fCellSizeZ + fPMTThickness)),
						fPMT->GetPMTLogicalVolume(),
						"SACPMT",
						fSACVolume,
						false, fPMID, false);
				}
				else if(fEnableSiPM && !fEnablePMT)
				{
					new G4PVPlacement(
						0,
						G4ThreeVector(fCrystalPosX, fCrystalPosY, fCrystalPosZ - 0.5 * (fCellSizeZ + fSiPMThickness)),
						fSiPM->GetSiPMLogicalVolume(),
						"SACSiPM",
						fSACVolume,
						false, fPMID, false);
				}
			}
		}
	}
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SACDetector::ReadGeometry()
{
	SACGeometry* Geo = SACGeometry::GetInstance();

	fCellSizeX = Geo->GetCellSizeX();
	fCellSizeY = Geo->GetCellSizeY();
	fCellSizeZ = Geo->GetCellSizeZ();

	fCellGap = Geo->GetCellGap();
	printf("[SACDetector::ReadGeometry] Gap between SAC cells is %f\n", fCellGap);

	fLayerGap = Geo->GetLayerGap();
	printf("[SACDetector::ReadGeometry] Gap between SAC layers is %f\n", fLayerGap);

	fEnablePMT = Geo->GetEnablePMT();
	fEnableSiPM = Geo->GetEnableSiPM();

	fPMTThickness = Geo->GetPMTThickness();
	fSiPMThickness = Geo->GetSiPMThickness();

	fNRows = Geo->GetSACNRows();
	fNCols = Geo->GetSACNCols();
	fNLayers = Geo->GetSACNLayers();
	printf("[SACDetector::ReadGeometry] SAC dimensions are %d %d %d\n", fNRows, fNCols, fNLayers);

	fSACSizeX = Geo->GetSACSizeX();
	fSACSizeY = Geo->GetSACSizeY();
	fSACSizeZInit = Geo->GetSACSizeZ();
	if(fEnablePMT && !fEnableSiPM)
	{
		G4cout << "[SACDetector::ReadGeometry] PMTs are enabled and SiPMs are disabled" << G4endl;
		fSACSizeZ = fSACSizeZInit + fPMTThickness;
	}
	else if(fEnableSiPM && !fEnablePMT)
	{
		G4cout << "[SACDetector::ReadGeometry] SiPMs are enabled and PMTs are disabled" << G4endl;
		fSACSizeZ = fSACSizeZInit + fSiPMThickness;
	}
	else
	{
		fSACSizeZ = fSACSizeZInit;
		if(!fEnablePMT && !fEnableSiPM) G4cout << "[SACDetector::ReadGeometry] Both PMTs and SiPMS are disabled" << G4endl;
		else G4cout << "[SACDetector::ReadGeometry] ERROR --- Both PMTs and SiPMS are disabled" << G4endl;
	}
	printf("[SACDetector::ReadGeometry] SAC size is %f %f %f\n", fSACSizeX, fSACSizeY, fSACSizeZ);

	fSACPosX = Geo->GetSACPosX();
	fSACPosY = Geo->GetSACPosY();
	fSACPosZ = Geo->GetSACPosZ();
	printf("[SACDetector::ReadGeometry] SAC will be placed at %f %f %f\n", fSACPosX, fSACPosY, fSACPosZ);
}
