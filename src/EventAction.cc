// EventAction.cc
// --------------------------------------------------------------
// History:
//
// Created by Emily Tsai (emily.tsai11@gmail.com) 2020-7-8
// --------------------------------------------------------------

#include "EventAction.hh"

#include "G4RunManager.hh"
#include "G4EventManager.hh"
#include "G4VVisManager.hh"
#include "G4SDManager.hh"
#include "G4VAnalysisManager.hh"
#include "G4Event.hh"
#include "G4TrajectoryContainer.hh"
#include "G4Trajectory.hh"
#include "G4Timer.hh"
#include "G4ios.hh"
#include "TRandom3.h"
#include "G4PrimaryParticle.hh"

#include "HistManager.hh"
#include "RandomGenerator.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EventAction::EventAction(int seedNum) : G4UserEventAction() // initialize defaults
{
	fTimer = new G4Timer;
	fCurrentEventCount = 0;
	CLHEP::HepRandom::setTheSeed(seedNum);
	fCommandLineSeed = seedNum;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EventAction::~EventAction()
{
	delete fTimer;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::BeginOfEventAction(const G4Event* evt)
{
	fEventID = evt->GetEventID();
	G4cout << "EventAction::BeginOfEventAction(): Event " << fEventID << " starting!" << G4endl;
	// CLHEP::HepRandom::showEngineStatus();
	fTimer->Start();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::EndOfEventAction(const G4Event* evt)
{
	fTimer->Stop();
	// G4cout << "EventAction::EndOfEventAction(): Event " << fEventID << " over, filling histograms now!" << G4endl;

	// draw all trajectories
	G4TrajectoryContainer* trajectoryContainer = evt->GetTrajectoryContainer();
	if(G4VVisManager::GetConcreteInstance() && trajectoryContainer)
	{
		G4int nTrajectories = trajectoryContainer->entries();
		for(G4int i = 0; i < nTrajectories; i++)
		{
			G4Trajectory* trj = (G4Trajectory*) ((*trajectoryContainer)[i]);
			trj->DrawTrajectory();
			if(i % 10000 == 0) G4cout << "EventAction::EndOfEventAction(): Trajectory " << i << " processed!" << G4endl;
		}
	}

	FillHistograms(evt);

	// periodic printing
	G4int eventID = evt->GetEventID();
	if((eventID < 10) || (eventID < 100 && !(eventID % 10)) || (eventID < 1000 && !(eventID % 100)) || !(eventID % 1000))
	{
		G4cout << "EventAction::EndOfEventAction(): Event " << eventID << " processed!" << G4endl;
	}
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::FillRandomEnginesStates()
{
	if(fCurrentEventCount == 0) fRandomDecayState = new TRandom3();

	if(fCurrentEventCount == 0)
	{
		unsigned int iSeed = 10;
		if(fCommandLineSeed != -1) iSeed = fCommandLineSeed;
		G4cout << "EventAction::FillRandomEnginesStates(): Random seed = " << iSeed << G4endl;
		RandomGenerator::GetInstance()->Init(iSeed);
	}

	*fRandomDecayState = *RandomGenerator::GetInstance()->GetRandomDecay();
	const long* table = CLHEP::HepRandom::getTheSeeds();
	fRanecuState[0] = table[0];
	fRanecuState[1] = table[1];
	fCurrentEventCount++;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::FillHistograms(const G4Event* evt)
{
	// get histogram parameters
	HistManager* fHistManager = HistManager::GetInstance();
	std::map<G4String, G4int> fP = fHistManager->GetParticleNames();
	std::map<G4String, f1DHistInfo> f1DH = fHistManager->Get1DHistInfo();
	std::map<G4String, f2DHistInfo> f2DH = fHistManager->Get2DHistInfo();
	std::map<G4String, f3DHistInfo> f3DH = fHistManager->Get3DHistInfo();
	// fHistManager->PrintHistNames();
	// fHistManager->Print1DHistInfo();
	// fHistManager->Print2DHistInfo();
	// fHistManager->Print3DHistInfo();

	// get analysis manager
	G4AnalysisManager* fAnalysisManager = G4AnalysisManager::Instance();

	// get incident particle name & energy
	G4PrimaryParticle* primary = evt->GetPrimaryVertex()->GetPrimary();
	G4String primaryName = primary->GetParticleDefinition()->GetParticleName();
	G4double primaryInitE = primary->GetKineticEnergy();
	G4cout << primaryName << " incident energy: " << primaryInitE << G4endl;

	// get hits collection
	fSACCollection = (SACHitsCollection*) evt->GetHCofThisEvent()->GetHC(0);
	G4int nHits = fSACCollection->entries();
	// G4cout << "[EventAction::FillHistograms()] There are " << nHits << " hits." << G4endl;
	// for(G4int i = 0; i < nHits; i++) (*fSACCollection)[i]->Print();

	// hit attributes
	G4int cellID;
	G4int trackID;
	G4String particleName;
	G4String volumeName;
	G4String sdName;
	G4String creatorProcessName;
	G4double energyDeposition;
	// G4double time;
	// G4double trackLength;
	// G4ThreeVector position;
	// G4ThreeVector localPosition;

	// helpers to fill histograms, will need to scale # entries w/# groups
	std::map<G4int, G4bool> trackedHits;
	G4double fEDepPerEvent[] = {
		0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
		0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
	G4double fMultPerEvent[] = {
		0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
		0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };

	// filling histograms
	for(G4int i = 0; i < nHits; i++)
	{
		// get current hit and attributes
		SACHit* currentHit = (*fSACCollection)[i];
		cellID = currentHit->GetCellID();
		trackID = currentHit->GetTrackID();
		particleName = currentHit->GetParticleName();
		volumeName = currentHit->GetVolumeName();
		sdName = currentHit->GetSDName();
		creatorProcessName = currentHit->GetCreatorProcessName();
		energyDeposition = currentHit->GetEnergyDeposition();
		// time = currentHit->GetTime();
		// trackLength = currentHit->GetTrackLength();
		// position = currentHit->GetPosition();
		// localPosition = currentHit->GetLocalPosition();

		// increment edep in the event
		fEDepPerEvent[fP.at("all")] += energyDeposition;
		try { fEDepPerEvent[fP.at(particleName)] += energyDeposition; }
		catch(std::out_of_range)
		{
			// G4cout << "particle not yet added to list --- " << particleName << G4endl;
			fEDepPerEvent[fP.at("other")] += energyDeposition;
		}

		// increment mult in the event
		if(trackedHits[trackID] == false)
		{
			trackedHits[trackID] = true;
			fMultPerEvent[fP.at("all")]++;
			try { fMultPerEvent[fP.at(particleName)]++; }
			catch(std::out_of_range) { fMultPerEvent[fP.at("other")]++; }
		}
	}
	// get true untracked edep
	fEDepPerEvent[fP.at("untracked")] = primaryInitE - fEDepPerEvent[fP.at("all")];

	// G4double eDepSum = 0.0;
	// for(G4int i = 0; i < 14; i++)
	// {
	// 	G4cout << fEDepPerEvent[i] << " ";
	// 	if(i > 0 && i < 13) eDepSum += fEDepPerEvent[i];
	// }
	// G4cout << G4endl;
	// G4cout << "total should be: " << eDepSum << G4endl;
	// G4double multSum = 0.0;
	// for(G4int i = 0; i < 14; i++)
	// {
	// 	G4cout << fMultPerEvent[i] << " ";
	// 	if(i > 0 && i < 13) multSum += fMultPerEvent[i];
	// }
	// G4cout << G4endl;
	// G4cout << "total should be: " << multSum << G4endl;

	// fill "per event" histograms
	std::map<G4String, G4int>::iterator iter;
	for(iter = fP.begin(); iter != fP.end(); iter++)
	{
		fAnalysisManager->FillH1(
			f1DH.at("h1EDepNorm_PerEvent_" + iter->first).index,
			fEDepPerEvent[iter->second] / primaryInitE, 1.0);
		fAnalysisManager->FillH1(
			f1DH.at("h1EDep_PerEvent_" + iter->first).index,
			fEDepPerEvent[iter->second], 1.0);
		fAnalysisManager->FillH1(
			f1DH.at("h1MultLow_PerEvent_" + iter->first).index,
			fMultPerEvent[iter->second], 1.0);
		fAnalysisManager->FillH1(
			f1DH.at("h1Mult_PerEvent_" + iter->first).index,
			fMultPerEvent[iter->second], 1.0);
	}
}

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
