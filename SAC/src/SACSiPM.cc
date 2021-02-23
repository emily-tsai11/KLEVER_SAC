// SACSiPM.cc
// --------------------------------------------------------------
// History:
//
// Created by Emily Tsai (emily.tsai11@gmail.com) 2021-1-11
// --------------------------------------------------------------

#include "SACSiPM.hh"

#include "G4Material.hh"
#include "G4Box.hh"
#include "G4VisAttributes.hh"
#include "G4RotationMatrix.hh"
#include "G4ThreeVector.hh"
#include "G4PVPlacement.hh"
#include "G4SDManager.hh"

#include "SACGeometry.hh"
#include "SACSD.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SACSiPM::SACSiPM()
{
	SACGeometry* Geo = SACGeometry::GetInstance();

	fSiPMSize = Geo->GetSiPMSize();
	printf("[SACSiPM::SACSiPM] SiPM size is %f\n", fSiPMSize);

	fSiPMThickness = Geo->GetSiPMThickness();
	printf("[SACSiPM::SACSiPM] SiPM thickness is %f\n", fSiPMThickness);

	fSACSDName = Geo->GetSACSensitiveDetectorName();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SACSiPM::~SACSiPM() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SACSiPM::CreateGeometry()
{
	G4Box* fSiPMSolid = new G4Box(
		"SACSiPM",
		0.5 * fSiPMSize,
		0.5 * fSiPMSize,
		0.5 * fSiPMThickness);

	fSiPMVolume = new G4LogicalVolume(
		fSiPMSolid,
		G4Material::GetMaterial("PbF2"),
		"SACSiPM",
		0, 0, 0);
	fSiPMVolume->SetVisAttributes(G4VisAttributes(G4Colour::Green()));

	// Make SiPM a sensitive detector
	G4SDManager* SDMan = G4SDManager::GetSDMpointer();
	SACSD* fSACSD = static_cast<SACSD*>(SDMan->FindSensitiveDetector(fSACSDName));
	if(!fSACSD)
	{
		printf("[SACSiPM::CreateGeometry] SAC SD %s not found, registering it now\n", fSACSDName.c_str());
		fSACSD = new SACSD(fSACSDName);
		SDMan->AddNewDetector(fSACSD);
	}
	fSiPMVolume->SetSensitiveDetector(fSACSD);
	printf("[SACSiPM::CreateGeometry] Added SAC SiPM as sensitive detector\n");
}
