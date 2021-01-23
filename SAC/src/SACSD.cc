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
#include "G4VProcess.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"

// #include "G4UnitsTable.hh"
// #include <map>
// #include "SACGeometry.hh"
// #include "Analysis.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SACSD::SACSD(G4String name) : G4VSensitiveDetector(name)
{
	G4String HCname;
	collectionName.insert(HCname = name); // create the collection name
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
	G4StepPoint* preStepPoint = aStep->GetPreStepPoint();
	G4TouchableHandle touchable = preStepPoint->GetTouchableHandle();
	G4Track* track = aStep->GetTrack();

	G4double eDep = aStep->GetTotalEnergyDeposit();
	if(eDep == 0.0) return false;

	G4ThreeVector worldPosPre = preStepPoint->GetPosition();
	G4ThreeVector localPosPre = touchable->GetHistory()->GetTopTransform().TransformPoint(worldPosPre);

	SACHit* newHit = new SACHit();
	newHit->SetCellID(touchable->GetCopyNumber(1)); // copy number of cell, not crystal
	newHit->SetTrackID(track->GetTrackID());
	newHit->SetParticleName(track->GetDefinition()->GetParticleName());
	newHit->SetVolumeName(touchable->GetVolume()->GetName());
	newHit->SetSDName(preStepPoint->GetSensitiveDetector()->GetName());
	newHit->SetCreatorProcessName(track->GetCreatorProcess()->GetProcessName());
	newHit->SetEnergyDeposition(eDep);
	newHit->SetTime(preStepPoint->GetGlobalTime());
	newHit->SetTrackLength(track->GetTrackLength());
	newHit->SetPosition(worldPosPre);
	newHit->SetLocalPosition(localPosPre);
	fSACCollection->insert(newHit);

	return true;
}

// void SACSD::EndOfEvent(G4HCofThisEvent*)
// {
// 	if(verboseLevel > 0)
// 	{
// 		G4int NbHits = fSACCollection->entries();
// 		G4cout << "\n-- SAC Hits Collection: " << NbHits << " hits --" << G4endl;
// 		for(G4int i = 0; i < NbHits; i++) (*fSACCollection)[i]->Print();
// 	}
//
// 	G4AnalysisManager* fAnalysisManager = G4AnalysisManager::Instance();
//
// 	G4int nHitEntries = fSACCollection->entries();
// 	std::map<G4int, G4bool> trackedHits;
//
// 	SACGeometry* geo = SACGeometry::GetInstance();
// 	G4double incidentE = geo->GetIncidentE();
//
// 	G4int nParticles = 11;
// 	G4int NPerEvent[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
// 	G4double EPerEvent[] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
// 	G4double TotalEDep = 0.0;
// 	G4double UntrackedE = 0.0;
//
// 	for(G4int i = 0; i < nHitEntries; i++)
// 	{
// 		SACHit* currentHit = (*fSACCollection)[i];
//
// 		// print hit
// 		currentHit->Print();
//
// 		G4int CellID = currentHit->GetCellID();
// 		G4double time = currentHit->GetTime();
// 		G4double initE = currentHit->GetInitialEnergy();
// 		G4double eDep = currentHit->GetEnergyDep();
// 		G4int trackID = currentHit->GetTrackId();
// 		G4int partType = currentHit->GetPType();
// 		G4double trLen = currentHit->GetTrackLen();
//
// 		// G4cout << "time: " << G4BestUnit(currentHit->GetTime(), "Time") << G4endl;
//
// 		if(partType == -1)
// 		{
// 			G4cout << "untracked energy of " << G4BestUnit(eDep, "Energy")
// 				<< " added to untracked energy in event" << G4endl;
// 			UntrackedE += eDep;
// 			continue;
// 		}
//
// 		// fill energy deposition per hit
// 		fAnalysisManager->FillH1(partType + 0 * nParticles, eDep, 1.0);
// 		fAnalysisManager->FillH1(partType + 1 * nParticles, eDep, eDep);
//
// 		// fill track length per hit
// 		fAnalysisManager->FillH1(partType + 2 * nParticles, trLen, 1.0);
//
// 		G4int x = CellID % 10;		// x = x-coord in layer (0-9)
// 		G4int y = CellID / 10 % 10;	// y = y-coord in layer (0-9)
// 		G4int z = CellID / 100;		// z = which layer (0, 1, 2, 3)
//
// 		// only do this if the particle hasn't been seen before
// 		if(trackedHits[trackID] == false)
// 		{
// 			trackedHits[trackID] = true;
//
// 			// fill initial energy per particle
// 			fAnalysisManager->FillH1(partType + 6 * nParticles, initE, 1.0);
// 			fAnalysisManager->FillH1(partType + 7 * nParticles, initE, initE);
//
// 			// fill number of particle hits in SAC by layer
// 			fAnalysisManager->FillH1(partType + 8 * nParticles, z, 1.0);
//
// 			// fill x-y plane of SAC hits
// 			fAnalysisManager->FillH2(partType + (z + 1) * nParticles, x, y, 1.0);
// 			fAnalysisManager->FillH2(z + 5 * nParticles, x, y, 1.0);
//
// 			// fill SAC hits
// 			fAnalysisManager->FillH3(partType, x, y, z, 1.0);
// 			fAnalysisManager->FillH3(11, x, y, z, 1.0);
//
// 			// increment multiplicity of particle
// 			NPerEvent[partType]++;
//
// 			// TEMPORARY, UNTIL BACK OF SAC IS MADE NON-REFLECTIVE
// 			if(partType == 10)
// 				fAnalysisManager->FillH1(101 + x * 10 + y + z * 100, time, 1.0);
// 		}
//
// 		// increment energy deposition of particle
// 		EPerEvent[partType] += eDep;
//
// 		// fill energy deposition vs. track length
// 		fAnalysisManager->FillH2(partType + 0 * nParticles, eDep, trLen, 1.0);
// 	}
//
// 	for(G4int partType = 0; partType < nParticles; partType++)
// 	{
// 		// fill multiplicity of each particle per event
// 		if(NPerEvent[partType] > 0)
// 		{
// 			fAnalysisManager->FillH1(partType + 3 * nParticles, NPerEvent[partType] / incidentE, 1.0);
// 			fAnalysisManager->FillH1(partType + 4 * nParticles, NPerEvent[partType] / incidentE, 1.0);
// 		}
//
// 		// fill total energy deposition for each particle per event
// 		if(EPerEvent[partType] > 0)
// 		{
// 			fAnalysisManager->FillH1(partType + 5 * nParticles, EPerEvent[partType] / incidentE, 1.0);
// 			TotalEDep += EPerEvent[partType];
// 		}
// 	}
//
// 	// fill total energy deposition per event
// 	if(TotalEDep > 0.0) fAnalysisManager->FillH1(99, TotalEDep, 1.0);
//
// 	// fill untracked energy from other particles per event
// 	if(UntrackedE > 0.0) fAnalysisManager->FillH1(100, UntrackedE, 1.0);
// }
