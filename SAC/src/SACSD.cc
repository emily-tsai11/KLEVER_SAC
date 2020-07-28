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
#include "G4UnitsTable.hh"

#include "G4Gamma.hh"
#include "G4Electron.hh"
#include "G4Positron.hh"
#include "G4Proton.hh"
#include "G4Neutron.hh"
#include "G4PionPlus.hh"
#include "G4PionMinus.hh"
#include "G4PionZero.hh"
#include "G4MuonPlus.hh"
#include "G4MuonMinus.hh"
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

	G4Track* track = aStep->GetTrack();
	newHit->SetPType(ClassifyTrack(track));

	newHit->SetTrackLen(track->GetTrackLength());

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

	G4AnalysisManager* fAnalysisManager = G4AnalysisManager::Instance();

	G4int nHitEntries = fSACCollection->entries();
	G4int nParticles = 11;
	G4int NPerEvent[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	G4double EPerEvent[] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
	G4double TotalEPerEvent = 0.0;

	for(int i = 0; i < nHitEntries; i++)
	{
		SACHit* currentHit = (*fSACCollection)[i];
		G4double eDep = currentHit->GetEnergy();
		G4double trLen = currentHit->GetTrackLen();
		G4int partType = currentHit->GetPType();

		if(partType == -1)
		{
			G4cout << "untracked energy of " << G4BestUnit(eDep, "Energy") << " added to total energy per event" << G4endl;
			TotalEPerEvent += eDep;
			continue;
		}

		fAnalysisManager->FillH1(partType + 0 * nParticles, eDep, 1.0);
		fAnalysisManager->FillH1(partType + 1 * nParticles, trLen, 1.0);

		fAnalysisManager->FillH2(partType + 0 * nParticles, eDep, trLen, 1.0);

		NPerEvent[partType]++;
		EPerEvent[partType] += eDep;
	}

	for(int partType = 0; partType < nParticles; partType++)
	{
		fAnalysisManager->FillH1(partType + 2 * nParticles, NPerEvent[partType], 1.0);
		fAnalysisManager->FillH1(partType + 3 * nParticles, EPerEvent[partType], 1.0);
		TotalEPerEvent += EPerEvent[partType];
	}

	fAnalysisManager->FillH1(44, TotalEPerEvent, 1.0);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4int SACSD::ClassifyTrack(G4Track* track)
{
	G4ParticleDefinition* particleType = track->GetDefinition();
	if(particleType == G4Gamma::GammaDefinition()) { return 0; }
	else if(particleType == G4Positron::PositronDefinition()) { return 1; }
	else if(particleType == G4Electron::ElectronDefinition()) { return 2; }
	else if(particleType == G4Proton::ProtonDefinition()) { return 3; }
	else if(particleType == G4Neutron::NeutronDefinition()) { return 4; }
	else if(particleType == G4PionPlus::PionPlusDefinition()) { return 5; }
	else if(particleType == G4PionMinus::PionMinusDefinition()) { return 6; }
	else if(particleType == G4PionZero::PionZeroDefinition()) { return 7; }
	else if(particleType == G4MuonPlus::MuonPlusDefinition()) { return 8; }
	else if(particleType == G4MuonMinus::MuonMinusDefinition()) { return 9; }
	else if(particleType == G4OpticalPhoton::OpticalPhotonDefinition()) { return 10; }
	else
	{
		G4cout << "untracked energy deposition!! particle name: " << particleType->GetParticleName() << G4endl;
		return -1;
	}
}
