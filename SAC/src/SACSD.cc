// SACSD.cc
// --------------------------------------------------------------
// History:
//
// Adapted from PADME experiment by Emily Tsai (emily.tsai11@gmail.com) 2020-7-8
// --------------------------------------------------------------

#include "SACSD.hh"

#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"

#include "G4Gamma.hh"
#include "G4Electron.hh"
#include "G4Positron.hh"
#include "G4Neutron.hh"
#include "G4PionPlus.hh"
#include "G4PionMinus.hh"
#include "G4MuonPlus.hh"
#include "G4MuonMinus.hh"
#include "G4PionZero.hh"
#include "G4OpticalPhoton.hh"

#include "Analysis.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SACSD::SACSD(G4String name) : G4VSensitiveDetector(name)
{
	G4String HCname;
	collectionName.insert(HCname = "SACCollection"); // create the collection name
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SACSD::~SACSD() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SACSD::Initialize(G4HCofThisEvent* HCE)
{
	fSACCollection = new SACHitsCollection(SensitiveDetectorName, collectionName[0]);
	static G4int HCID = -1;
	if(HCID < 0) HCID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
	HCE->AddHitsCollection(HCID, fSACCollection);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool SACSD::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{
	SACHit* newHit = new SACHit();

	G4StepPoint* preStepPoint = aStep->GetPreStepPoint();
	G4TouchableHandle touchHPre = preStepPoint->GetTouchableHandle();
	newHit->SetChannelId(touchHPre->GetCopyNumber(1)); // copy id is that of the cell, not of the crystal

	newHit->SetTime(preStepPoint->GetGlobalTime());

	G4double edep = aStep->GetTotalEnergyDeposit();
	if(edep == 0.0) return false;
	newHit->SetEnergy(edep);

	G4ThreeVector worldPosPre = preStepPoint->GetPosition();
	newHit->SetPosition(worldPosPre);

	G4ThreeVector localPosPre = touchHPre->GetHistory()->GetTopTransform().TransformPoint(worldPosPre);
	newHit->SetLocalPosition(localPosPre);

	G4int partType = ClassifyTrack(aStep->GetTrack(), edep);
	newHit->SetPType(partType);

	fSACCollection->insert(newHit);

	return true;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SACSD::EndOfEvent(G4HCofThisEvent*)
{
	if(verboseLevel > 0)
	{
		G4int NbHits = fSACCollection->entries();
		G4cout << "\n-- SAC Hits Collection: " << NbHits << " hits --" << G4endl;
		for(G4int i = 0; i < NbHits; i++) (*fSACCollection)[i]->Print();
	}

	// fill histograms
	G4AnalysisManager* fAnalysisManager = G4AnalysisManager::Instance();

	G4int nEntries = fSACCollection->entries();
	G4int nOptical = 0;
	G4double TotalOptEPerEvent = 0.0;

	for(int i = 0; i < nEntries; i++)
	{
		SACHit* currentHit = (*fSACCollection)[i];

		// energy deposition for each hit
		G4double eDep = currentHit->GetEnergy();
		G4int partType = currentHit->GetPType();
		switch(partType)
		{
			case 1: fAnalysisManager->FillH1(0, eDep, eDep); break;
			case 2: fAnalysisManager->FillH1(1, eDep, eDep); break;
			case 3: fAnalysisManager->FillH1(2, eDep, eDep); break;
			case 4: fAnalysisManager->FillH1(3, eDep, eDep); break;
			case 5: fAnalysisManager->FillH1(4, eDep, eDep); break;
			case 6: fAnalysisManager->FillH1(5, eDep, eDep); break;
			case 7: fAnalysisManager->FillH1(6, eDep, eDep); break;
			case 8:
				fAnalysisManager->FillH1(7, eDep, eDep);
				TotalOptEPerEvent += eDep;
				break;
			default: G4cout << "SWITCH CASE DEFAULT -- NOTHING HAPPENS" << G4endl;
		}

		if(partType == 8) nOptical++;
	}

	fAnalysisManager->FillH1(8, nOptical, 1.0);
	G4cout << "TotalOptEPerEvent: " << TotalOptEPerEvent << G4endl;
	fAnalysisManager->FillH1(9, TotalOptEPerEvent, 1.0);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4int SACSD::ClassifyTrack(G4Track* track, G4double edep)
{
	G4ParticleDefinition* particleType = track->GetDefinition();
	if(particleType == G4Gamma::GammaDefinition()) { return 1; }
	else if(particleType == G4Positron::PositronDefinition()) { return 2; }
	else if(particleType == G4Electron::ElectronDefinition()) { return 3; }
	else if(particleType == G4Neutron::NeutronDefinition()) { return 4; }
	else if(particleType == G4PionPlus::PionPlusDefinition() || particleType == G4PionMinus::PionMinusDefinition()) { return 5; }
	else if(particleType == G4PionZero::PionZeroDefinition()) { return 6; }
	else if(particleType == G4MuonPlus::MuonPlusDefinition() || particleType == G4MuonMinus::MuonMinusDefinition()) { return 7; }
	else if(particleType == G4OpticalPhoton::OpticalPhotonDefinition()) { return 8; }
	else
	{
		G4cout << "untracked energy deposition: " << edep << " | particleType: " << particleType << G4endl;
		return -1;
	}
}
