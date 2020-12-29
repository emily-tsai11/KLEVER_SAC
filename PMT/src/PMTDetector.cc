// PMTDetector.cc
// --------------------------------------------------------------
// History:
//
// Created by Emily Tsai (emily.tsai11@gmail.com) 2020-12-9
// --------------------------------------------------------------

#include "PMTDetector.hh"

#include "G4PVPlacement.hh"
#include "G4ThreeVector.hh"
#include "G4RotationMatrix.hh"
#include "G4Box.hh"
#include "G4SDManager.hh"
#include "G4Material.hh"
#include "G4VisAttributes.hh"
#include "G4SubtractionSolid.hh"
#include "G4SystemOfUnits.hh"
#include "globals.hh"

// #include "PMTMessenger.hh"
// #include "PMTSD.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PMTDetector::PMTDetector(G4Material* material, G4LogicalVolume* motherVolume) : fMaterial(material), fMotherVolume(motherVolume)
{
	// fPMTMessenger = new PMTMessenger(this);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PMTDetector::~PMTDetector()
{
	// delete fPMTMessenger;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PMTDetector::CreateGeometry()
{
	G4Box* solidPMT = new G4Box("PMT", 5.0 * cm, 5.0 * cm, 5.0 * cm);
	fPMTVolume = new G4LogicalVolume(solidPMT, fMaterial, "PMT", 0, 0, 0);
	fPMTVolume->SetVisAttributes(G4VisAttributes(G4Colour::Green()));

	// G4double PMTx = 0.0 * m;
	// G4double PMTy = 0.0 * m;
	// G4double PMTz = 0.8 * m;
	// G4ThreeVector PMTPos = G4ThreeVector(PMTx, PMTy, PMTz);
	//
	// G4RotationMatrix* Rot = new G4RotationMatrix;
	// Rot->rotateY(0.1 * rad);
	// Rot->rotateX(0.1 * rad);
	//
	// // position PMT volume
	// new G4PVPlacement(Rot, PMTPos, fPMTVolume, "PMT", fMotherVolume, false, 0, false);

	G4Box* solidCell = new G4Box("PMTCell", 0.5 * cm, 0.5 * cm, 0.5 * cm);
	fCellVolume = new G4LogicalVolume(solidCell, G4Material::GetMaterial("PbF2"), "PMTCell", 0, 0, 0);
	fCellVolume->SetVisAttributes(G4VisAttributes(G4Colour::Red()));

	// G4double cellx = 0.0 * m;
	// G4double celly = 0.0 * m;
	// G4double cellz = 0.0 * m;
	// G4ThreeVector cellPos = G4ThreeVector(cellx, celly, cellz);
	//
	// position PMT cell inside PMT volume
	// new G4PVPlacement(0, cellPos, fCellVolume, "PMTCell", fPMTVolume, false, 0, false);
}
