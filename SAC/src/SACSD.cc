// SACSD.cc
// --------------------------------------------------------------
// History:
//
// Adapted from PADME experiment by Emily Tsai (emily.tsai11@gmail.com) 2020-7-8
// --------------------------------------------------------------

#include "SACSD.hh"
#include "globals.hh"

#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4SDManager.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SACSD::SACSD(G4String name) : G4VSensitiveDetector(name), fSACCollection(nullptr), fHCID(-1)
{
	G4String HCname;
	collectionName.insert(HCname = name); // create the collection name
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SACSD::~SACSD() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SACSD::Initialize(G4HCofThisEvent* fHCE)
{
	fSACCollection = new SACHitsCollection(SensitiveDetectorName, collectionName[0]);
	if(fHCID < 0) fHCID = G4SDManager::GetSDMpointer()->GetCollectionID(fSACCollection);
	fHCE->AddHitsCollection(fHCID, fSACCollection);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool SACSD::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{
	G4StepPoint* preStepPoint = aStep->GetPreStepPoint();
	G4TouchableHandle touchable = preStepPoint->GetTouchableHandle();
	G4Track* track = aStep->GetTrack();

	G4double eDep = aStep->GetTotalEnergyDeposit();
	if(eDep == 0.0) return false;
	G4ThreeVector worldPosPre = preStepPoint->GetPosition();
	G4ThreeVector localPosPre = touchable->GetHistory()->GetTopTransform().TransformPoint(worldPosPre);

	SACHit* newHit = new SACHit();
	newHit->SetCellID(touchable->GetCopyNumber(1)); // Copy number of cell, not crystal
	newHit->SetTrackID(track->GetTrackID());
	newHit->SetParticleName(track->GetDefinition()->GetParticleName());
	newHit->SetVolumeName(touchable->GetVolume()->GetName());
	newHit->SetSDName(preStepPoint->GetSensitiveDetector()->GetName());
	// newHit->SetCreatorProcessName(track->GetCreatorProcess()->GetProcessName());
	newHit->SetEnergyDeposition(eDep);
	newHit->SetTime(preStepPoint->GetGlobalTime());
	newHit->SetTrackLength(track->GetTrackLength());
	newHit->SetPosition(worldPosPre);
	newHit->SetLocalPosition(localPosPre);
	fSACCollection->insert(newHit);

	return true;
}
