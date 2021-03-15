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

#include "Analysis.hh"
#include "SACSD.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EventAction::EventAction() : G4UserEventAction()
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
	if((fEventID < 10) || (fEventID < 100 && !(fEventID % 10)) || (fEventID < 1000 && !(fEventID % 100)) || !(fEventID % 1000))
		G4cout << "[EventAction::BeginOfEventAction] Event " << fEventID << " starting!" << G4endl;
	fTimer->Start();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::EndOfEventAction(const G4Event* evt)
{
	fTimer->Stop();

	evt->Print();

	G4cout << "EventAction::EndOfEventAction] Primary Particle Track ID: " << evt->GetPrimaryVertex()->GetPrimary()->GetTrackID() << G4endl;
	G4cout << "EventAction::EndOfEventAction] Primary Particle PDG Code: " << evt->GetPrimaryVertex()->GetPrimary()->GetPDGcode() << G4endl;

	// G4cout << "EventAction::EndOfEventAction] Secondary Particle Track ID: " << evt->GetPrimaryVertex()->GetPrimary()->GetDaughter()->GetTrackID() << G4endl;
	// G4cout << "EventAction::EndOfEventAction] Secondary Particle PDG Code: " << evt->GetPrimaryVertex()->GetPrimary()->GetDaughter()->GetPDGcode() << G4endl;

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

	if(trajectoryContainer) G4cout << "[EventAction::EndOfEventAction] " << trajectoryContainer->entries() << " trajectories!!" << G4endl;
	else G4cout << "[EventAction::EndOfEventAction] No trajectories :'(" << G4endl;

	G4cout << "[EventAction::EndOfEventAction] Number of hits: " << ((SACHitsCollection*) evt->GetHCofThisEvent()->GetHC(0))->entries() << G4endl;
	Analysis::GetInstance()->FillHistograms(evt);

	// Periodic printing
	if((fEventID < 10) || (fEventID < 100 && !(fEventID % 10)) || (fEventID < 1000 && !(fEventID % 100)) || !(fEventID % 1000))
		G4cout << "[EventAction::EndOfEventAction] Event " << fEventID << " processed!" << G4endl;
}
