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

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EventAction::EventAction(int seedNum)
{
	timer = new G4Timer;
	fCurrentEventCount = 0;
	CLHEP::HepRandom::setTheSeed(seedNum);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EventAction::~EventAction()
{
	delete timer;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::BeginOfEventAction(const G4Event* evt)
{
	timer->Start();
	fEventID = evt->GetEventID();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EndOfEventAction(const G4Event* evt)
{
	// SAVE EVENT TO ROOT FILE


	// stop event timer
	timer->Stop();

	// get number of stored trajectories
	G4TrajectoryContainer* trajectoryContainer = evt->GetTrajectoryContainer();
	G4int nTrajectories = 0; nOptical = 0;
	if(trajectoryContainer) nTrajectories = trajectoryContainer->entries();

	G4bool vis = false;
	if(G4VVisManager::GetConcreteInstance()) vis = true;

	for(G4int i = 0; i < nTrajectories; i++)
	{
		// get name of particle being drawn
		G4Trajectory* trj = (G4Trajectory*) ((*trajectoryContainer)[i]);
		G4String name = trj->GetParticleName();
		G4cout << "EventAction::EndOfEventAction -> particle is a " << name << G4endl;

		// draw all trajectories
		if(vis) trj->DrawTrajectory(0);

		// save count of optical photons
		if(name == "opticalphoton") nOptical++;
	}

	// periodic printing
	G4int eventID = evt->GetEventID();
	if((eventID < 10) || (eventID < 100 && !(eventID % 10)) || (eventID < 1000 && !(eventID % 100)) || !(eventID % 1000))
	{
		G4cout << "Event " << eventID << " processed" << G4endl;
	}
}
