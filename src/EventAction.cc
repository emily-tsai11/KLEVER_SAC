// EventAction.cc
// --------------------------------------------------------------
// History:
//
// Created by Emily Tsai (emily.tsai11@gmail.com) 2020-7-8
// --------------------------------------------------------------

#include "EventAction.hh"

#include "G4VVisManager.hh"
#include "G4Event.hh"
#include "G4TrajectoryContainer.hh"
#include "G4Trajectory.hh"
#include "G4Timer.hh"
#include "G4PrimaryParticle.hh"

#include "RunAction.hh"
#include "HistManager.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EventAction::EventAction(RunAction* runAction) : G4UserEventAction(),
 	fRunAction(runAction)
{
	fTimer = new G4Timer;
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
	// 	G4cout << "[EventAction::BeginOfEventAction] Event " << fEventID << " starting!" << G4endl;
	fTimer->Start();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::EndOfEventAction(const G4Event* evt)
{
	fTimer->Stop();

	// Draw all trajectories
	G4TrajectoryContainer* trajectoryContainer = evt->GetTrajectoryContainer();
	if(G4VVisManager::GetConcreteInstance() && trajectoryContainer)
	{
		G4int nTrajectories = trajectoryContainer->entries();
		for(G4int i = 0; i < nTrajectories; i++)
		{
			G4Trajectory* trj = (G4Trajectory*) ((*trajectoryContainer)[i]);
			trj->DrawTrajectory();
			if(i % 10000 == 0) G4cout << "[EventAction::EndOfEventAction] Trajectory " << i << " processed!" << G4endl;
		}
	}

	FillHistograms(evt);

	// Periodic printing
	if((fEventID < 10) || (fEventID < 100 && !(fEventID % 10)) || (fEventID < 1000 && !(fEventID % 100)) || !(fEventID % 1000))
		G4cout << "[EventAction::EndOfEventAction] Event " << fEventID << " processed!" << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::FillHistograms(const G4Event* evt)
{
	// Get histogram parameters
	HistManager* fHistManager = HistManager::GetInstance();

	std::map<G4String, G4int> fP = fHistManager->GetParticleNames();
	std::map<G4String, f1DHistInfo> f1DH = fHistManager->Get1DHistInfo();
	std::map<G4String, f2DHistInfo> f2DH = fHistManager->Get2DHistInfo();
	std::map<G4String, f3DHistInfo> f3DH = fHistManager->Get3DHistInfo();

	// Get analysis manager
	G4AnalysisManager* fAnalysisManager = G4AnalysisManager::Instance();

	// Get incident particle name & energy
	G4PrimaryParticle* primary = evt->GetPrimaryVertex()->GetPrimary();
	G4String primaryName = primary->GetParticleDefinition()->GetParticleName();
	G4double primaryInitE = primary->GetKineticEnergy();

	// Get number of events in run
	G4double fTotalNEvents = fRunAction->GetNumEvents();

	// Get hits collection
	fSACCollection = (SACHitsCollection*) evt->GetHCofThisEvent()->GetHC(0);
	G4int nHits = fSACCollection->entries();

	// Hit attributes
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

	// Helpers to fill histograms, will need to scale # entries w/# groups
	std::map<G4int, G4bool> trackedHits;
	G4double fEDepPerEvent[] = {
		0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
		0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
	G4double fMultPerEvent[] = {
		0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
		0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
	G4int pIndex, x, y, z;
	G4double eNorm;

	// Filling histograms
	for(G4int i = 0; i < nHits; i++)
	{
		// Get current hit and attributes
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
			// G4cout << "[EventAction::FillHistograms] Particle not yet added to list --- " << particleName << G4endl;
			pIndex = fP.at("other");
			particleName = "other";
		}

		x = cellID % 10;		// x = which row (0-9)
		y = cellID / 10 % 10;	// y = which column (0-9)
		z = cellID / 100;		// z = which layer (0 = front, 1, 2, 3 = back)

		eNorm = energyDeposition / primaryInitE / fTotalNEvents;

		// Increment energy deposition in event
		fEDepPerEvent[fP.at("all")] += energyDeposition;
		fEDepPerEvent[pIndex] += energyDeposition;

		// Fill "once per hit" histograms
		// Energy deposition in SAC layers / incident energy
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

		// Track length per hit
		fAnalysisManager->FillH1(
			f1DH.at("h1TrLen_PerHit_all").index,
			trackLength, 1.0);
		fAnalysisManager->FillH1(
			f1DH.at("h1TrLen_PerHit_" + particleName).index,
			trackLength, 1.0);

		// Increment multiplicity in the event
		if(trackedHits[trackID] == false)
		{
			trackedHits[trackID] = true;
			fMultPerEvent[fP.at("all")]++;
			fMultPerEvent[pIndex]++;
		}
	}
	// Take out optical photon energy, since they don't obey energy conservation
	// https://geant4-forum.web.cern.ch/t/energy-deposition-is-higher-than-the-primary-source-energy/2064/3
	fEDepPerEvent[fP.at("all")] -= fEDepPerEvent[fP.at("opticalphoton")];
	// Get true untracked energy deposition
	fEDepPerEvent[fP.at("untracked")] = primaryInitE - fEDepPerEvent[fP.at("all")];

	// Fill "once per event" histograms
	std::map<G4String, G4int>::iterator iter;
	for(iter = fP.begin(); iter != fP.end(); iter++)
	{
		// Energy deposition per event / incident energy
		fAnalysisManager->FillH1(
			f1DH.at("h1EDep_PerEvent_" + iter->first).index,
			fEDepPerEvent[iter->second] / primaryInitE, 1.0);

		// Multiplicity per event / incident energy
		fAnalysisManager->FillH1(
			f1DH.at("h1Mult_PerEvent_" + iter->first).index,
			fMultPerEvent[iter->second] / primaryInitE, 1.0);

		// Low range of multiplicity per event / incident energy
		fAnalysisManager->FillH1(
			f1DH.at("h1MultLow_PerEvent_" + iter->first).index,
			fMultPerEvent[iter->second] / primaryInitE, 1.0);
	}
}
