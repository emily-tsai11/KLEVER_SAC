// EventAction.cc
// --------------------------------------------------------------
// History:
//
// Created by Emily Tsai (emily.tsai11@gmail.com) 2020-7-8
// --------------------------------------------------------------

#include "EventAction.hh"

#include "G4EventManager.hh"
#include "G4VVisManager.hh"
#include "G4SDManager.hh"
#include "G4VAnalysisManager.hh"

#include "G4Event.hh"
#include "G4TrajectoryContainer.hh"
#include "G4Trajectory.hh"
#include "G4Timer.hh"

#include "G4ios.hh"
#include "g4root.hh"

#include "PrimaryGeneratorAction.hh"
#include "Analysis.hh"
#include "RandomGenerator.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EventAction::EventAction(int seedNum)
{
	fTimer = new G4Timer;
	fCurrentEventCount = 0;
	// fRandomEngineStateFileExist = true;
	CLHEP::HepRandom::setTheSeed(seedNum);
	fCommandLineSeed = seedNum;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EventAction::~EventAction()
{
	delete fTimer;
	// if(fRandomEngineStateFileExist)
	// {
	// 	delete fRandomDecayState;
	// 	fRandomEngineStateFile->Close();
	// 	delete fRandomEngineStateFile;
	// }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::BeginOfEventAction(const G4Event* evt)
{
	fTimer->Start();
	fEventID = evt->GetEventID();
	// CLHEP::HepRandom::showEngineStatus();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::EndOfEventAction(const G4Event* evt)
{
	// stop event timer
	fTimer->Stop();

	// get number of stored trajectories
	G4TrajectoryContainer* trajectoryContainer = evt->GetTrajectoryContainer();
	G4int nTrajectories = 0;
	if(trajectoryContainer) nTrajectories = trajectoryContainer->entries();

	G4bool vis = false;
	if(G4VVisManager::GetConcreteInstance()) vis = true;

	for(G4int i = 0; i < nTrajectories; i++)
	{
		// draw all trajectories
		if(vis)
		{
			G4Trajectory* trj = (G4Trajectory*) ((*trajectoryContainer)[i]);
			trj->DrawTrajectory();
		}
		// if(i % 10000 == 0) G4cout << "EventAction::EndOfEventAction() -> trajectory " << i << " processed" << G4endl;
	}

	// periodic printing
	G4int eventID = evt->GetEventID();
	if((eventID < 10) || (eventID < 100 && !(eventID % 10)) || (eventID < 1000 && !(eventID % 100)) || !(eventID % 1000))
	{
		G4cout << "Event " << eventID << " processed" << G4endl;
	}
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::FillRandomEnginesStates()
{
	if(fCurrentEventCount == 0) fRandomDecayState = new TRandom3();

	// if((fRandomEngineStateFileExist) && (fRandomEngineStateTree->GetEntries() == fCurrentEventCount)) G4cout << "###### ##### ##### Processing of the randomstate.root finished. Switching to pseudorandom event generation. ########## ######### #######" << G4endl;

	// if((!fRandomEngineStateFileExist) || (fRandomEngineStateFileExist && (fRandomEngineStateTree->GetEntries() <= fCurrentEventCount)))
	// {
		if(fCurrentEventCount == 0)
		{
			// unsigned int iSeed = DatacardManager::GetInstance()->GetRandDecaySeed();
			unsigned int iSeed = 10;
			if(fCommandLineSeed != -1) iSeed = fCommandLineSeed; // not sure why fCommandLineSeed = seedNum would be -1, but that's a problem for future me
			G4cout << "[EventAction::FillRandomEnginesStates] Random seed = " << iSeed << G4endl;
			RandomGenerator::GetInstance()->Init(iSeed);
			// set the seeds for GPS particles to generate different events from run to run
			// PrimaryGeneratorAction *PGA = (PrimaryGeneratorAction*) G4RunManager::GetRunManager()->GetUserPrimaryGeneratorAction();
			// G4int BeamType = PGA->GetBeamType();
			// if(BeamType == 1)
			// {
			// 	G4cout << "===> GPS beam is used!" << G4endl;
			// 	long gps_seeds[2];
			// 	gps_seeds[0] = iSeed;
			// 	gps_seeds[1] = iSeed + 62;
			// 	CLHEP::HepRandom::setTheSeeds(gps_seeds);
			// }
		}

		*fRandomDecayState = *RandomGenerator::GetInstance()->GetRandomDecay();
		const long *table = CLHEP::HepRandom::getTheSeeds();
		fRanecuState[0] = table[0];
		fRanecuState[1] = table[1];
	// }
	// else
	// {
	// 	if(fRandomEngineStateTree->GetEntries() > fCurrentEventCount)
	// 	{
	// 		fRandomEngineStateTree->GetEntry(fCurrentEventCount);
	// 		fRanecuState[0] = fRandEvent->GetRanecuState()[0];
	// 		fRanecuState[1] = fRandEvent->GetRanecuState()[1];
	// 		CLHEP::HepRandom::setTheSeeds(fRanecuState);
	// 		*fRandomDecayState = *fRandEvent->GetRandomDecayState();
	// 		RandomGenerator::GetInstance()->Init(fRandomDecayState);
	// 	}
	// 	else G4RunManager::GetRunManager()->AbortRun();
	// }
	fCurrentEventCount++;
}
