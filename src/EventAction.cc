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

#include "RunAction.hh"
#include "HistManager.hh"
#include "RandomGenerator.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EventAction::EventAction(RunAction* runAction, int seedNum) : G4UserEventAction(),
 	fRunAction(runAction) // initialize defaults
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
	// if((fEventID < 10) || (fEventID < 100 && !(fEventID % 10)) || (fEventID < 1000 && !(fEventID % 100)) || !(fEventID % 1000))
	// 	G4cout << "EventAction::BeginOfEventAction(): Event " << fEventID << " starting!" << G4endl;
	// CLHEP::HepRandom::showEngineStatus();
	fTimer->Start();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::EndOfEventAction(const G4Event* evt)
{
	fTimer->Stop();

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
	if((fEventID < 10) || (fEventID < 100 && !(fEventID % 10)) || (fEventID < 1000 && !(fEventID % 100)) || !(fEventID % 1000))
		G4cout << "EventAction::EndOfEventAction(): Event " << fEventID << " processed!" << G4endl;
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
	// G4cout << primaryName << " incident energy: " << primaryInitE << G4endl;

	// get number of events in run
	G4double fTotalNEvents = fRunAction->GetNumEvents();

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
	G4double trackLength;
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
	G4int pIndex, x, y, z;
	G4double eNorm;

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
		// DON'T USE FOR NOW, CAUSING SEGFAULTS -- TRY AGAIN AFTER OPTICAL BOUNDARIES DEFINED
		// creatorProcessName = currentHit->GetCreatorProcessName();
		energyDeposition = currentHit->GetEnergyDeposition();
		// time = currentHit->GetTime();
		trackLength = currentHit->GetTrackLength();
		// position = currentHit->GetPosition();
		// localPosition = currentHit->GetLocalPosition();

		try { pIndex = fP.at(particleName); }
		catch(std::out_of_range)
		{
			// G4cout << "particle not yet added to list --- " << particleName << G4endl;
			pIndex = fP.at("other");
			particleName = "other";
		}

		x = cellID % 10;		// x = which row (0-9)
		y = cellID / 10 % 10;	// y = which column (0-9)
		z = cellID / 100;		// z = which layer (0 = front, 1, 2, 3 = back)

		eNorm = energyDeposition / primaryInitE / fTotalNEvents;

		// increment energy deposition in event
		fEDepPerEvent[fP.at("all")] += energyDeposition;
		fEDepPerEvent[pIndex] += energyDeposition;

		// fill "once per hit" histograms
		// energy deposition in SAC layers / incident energy
		fAnalysisManager->FillH1(
			f1DH.at("h1EDep_PerLayer_all").index,
			z, eNorm);
		fAnalysisManager->FillH1(
			f1DH.at("h1EDep_PerLayer_" + particleName).index,
			z, eNorm);

		// 2D energy deposition in SAC layer (z = <#>) / incident energy
		fAnalysisManager->FillH2(
			f2DH.at("h2EDepZ" + std::to_string(z) + "_PerLayer_all").index,
			x, y, eNorm);
		fAnalysisManager->FillH2(
			f2DH.at("h2EDepZ" + std::to_string(z) + "_PerLayer_" + particleName).index,
			x, y, eNorm);

		// 3D energy deposition in SAC / incident energy
		fAnalysisManager->FillH3(
			f3DH.at("h3EDep_SAC_all").index,
			x, y, z, eNorm);
		fAnalysisManager->FillH3(
			f3DH.at("h3EDep_SAC_" + particleName).index,
			x, y, z, eNorm);

		// track length per hit
		fAnalysisManager->FillH1(
			f1DH.at("h1TrLen_PerHit_all").index,
			trackLength, 1.0);
		fAnalysisManager->FillH1(
			f1DH.at("h1TrLen_PerHit_" + particleName).index,
			trackLength, 1.0);

		// increment mult in the event
		if(trackedHits[trackID] == false)
		{
			trackedHits[trackID] = true;
			fMultPerEvent[fP.at("all")]++;
			fMultPerEvent[pIndex]++;
		}
	}
	// take out optical photon energy, since they don't obey energy conservation
	// https://geant4-forum.web.cern.ch/t/energy-deposition-is-higher-than-the-primary-source-energy/2064/3
	fEDepPerEvent[fP.at("all")] -= fEDepPerEvent[fP.at("opticalphoton")];
	// get true untracked edep
	fEDepPerEvent[fP.at("untracked")] = primaryInitE - fEDepPerEvent[fP.at("all")];

	// fill "once per event" histograms
	std::map<G4String, G4int>::iterator iter;
	for(iter = fP.begin(); iter != fP.end(); iter++)
	{
		// energy deposition per event / incident energy
		fAnalysisManager->FillH1(
			f1DH.at("h1EDep_PerEvent_" + iter->first).index,
			fEDepPerEvent[iter->second] / primaryInitE, 1.0);

		// multiplicity per event / incident energy
		fAnalysisManager->FillH1(
			f1DH.at("h1Mult_PerEvent_" + iter->first).index,
			fMultPerEvent[iter->second] / primaryInitE, 1.0);

		// low range of multiplicity per event / incident energy
		fAnalysisManager->FillH1(
			f1DH.at("h1MultLow_PerEvent_" + iter->first).index,
			fMultPerEvent[iter->second] / primaryInitE, 1.0);
	}
}
