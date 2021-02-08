// SACPMT.cc
// --------------------------------------------------------------
// History:
//
// Created by Emily Tsai (emily.tsai11@gmail.com) 2021-1-1
// --------------------------------------------------------------

#include "SACPMT.hh"

#include "G4Material.hh"
#include "G4Tubs.hh"
#include "G4VisAttributes.hh"
#include "G4RotationMatrix.hh"
#include "G4ThreeVector.hh"
#include "G4PVPlacement.hh"
#include "G4SDManager.hh"

#include "SACGeometry.hh"
#include "SACSD.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SACPMT::SACPMT()
{
	SACGeometry* Geo = SACGeometry::GetInstance();

	fPMTDiameter = Geo->GetPMTDiameter();
	printf("[SACPMT::SACPMT] PMT diameter is %f\n", fPMTDiameter);

	fPMTThickness = Geo->GetPMTThickness();
	printf("[SACPMT::SACPMT] PMT thickness is %f\n", fPMTThickness);

	fPMTRound = Geo->GetPMTRound();
	fSACSDName = Geo->GetSACSensitiveDetectorName();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SACPMT::CreateGeometry()
{
	G4Tubs* fPMTSolid = new G4Tubs(
		"SACPMT",
		0.0,
		0.5 * fPMTDiameter,
		0.5 * fPMTThickness,
		0.0,
		fPMTRound);

	fPMTVolume = new G4LogicalVolume(
		fPMTSolid,
		G4Material::GetMaterial("PbF2"),
		"SACPMT",
		0, 0, 0);
	fPMTVolume->SetVisAttributes(G4VisAttributes(G4Colour::Green()));

	// Make PMT a sensitive detector
	G4SDManager* SDMan = G4SDManager::GetSDMpointer();
	SACSD* fSACSD = static_cast<SACSD*>(SDMan->FindSensitiveDetector(fSACSDName));
	if(!fSACSD)
	{
		printf("[SACPMT::CreateGeometry] SAC SD %s not found, registering it now\n", fSACSDName.c_str());
		fSACSD = new SACSD(fSACSDName);
		SDMan->AddNewDetector(fSACSD);
	}
	fPMTVolume->SetSensitiveDetector(fSACSD);
	printf("[SACPMT::CreateGeometry] Added SAC PMT as sensitive detector\n");
}
