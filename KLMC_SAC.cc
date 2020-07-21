// KLMC_SAC.cc
// --------------------------------------------------------------
// History:
//
// Created by Emily Tsai (emily.tsai11@gmail.com) 2020-7-7
// --------------------------------------------------------------

#include "G4RunManager.hh"
#include "G4VisExecutive.hh"
#include "G4VisManager.hh"
#include "G4UIExecutive.hh"
#include "G4UImanager.hh"
#include "Randomize.hh"

#include "DetectorConstruction.hh"
#include "EventAction.hh"
#include "PhysicsList.hh"
#include "PrimaryGeneratorAction.hh"
#include "RunAction.hh"
#include "SteppingAction.hh"

#include <stdlib.h>

int main(int argc, char** argv)
{
	// get macro file and seed number
	G4String fileName;
	G4int seedNum;
	if(argc == 1)
	{
		fileName = "vis.mac"; // default interactive mode
		seedNum = rand();
		G4cout << "No macro specified, using " << fileName << G4endl;
		G4cout << "No seed number specified, using " << seedNum << G4endl;
	}
	else if(argc == 2)
	{
		fileName = argv[1];
		seedNum = rand();
		G4cout << "Using macro " << fileName << G4endl;
		G4cout << "No seed number specified, using " << seedNum << G4endl;
	}
	else
	{
		fileName = argv[1];
		seedNum = atoi(argv[2]);
		G4cout << "Using macro " << fileName << G4endl;
		G4cout << "Using seed number " << seedNum << G4endl;
	}

	// set Random engine
	CLHEP::HepRandom::setTheEngine(new CLHEP::RanecuEngine());

	// construct default run manager
	G4RunManager* runManager = new G4RunManager;
	DetectorConstruction* detector = new DetectorConstruction();
	runManager->SetUserInitialization(detector);
	runManager->SetUserInitialization(new PhysicsList);

	// set user actions
	runManager->SetUserAction(new RunAction);
	EventAction* eventAction = new EventAction(seedNum);
	runManager->SetUserAction(new PrimaryGeneratorAction(detector, *eventAction));
	runManager->SetUserAction(eventAction);
	runManager->SetUserAction(new SteppingAction(eventAction));

	// define and initialize visualization session
	G4VisManager* visManager = new G4VisExecutive;
	visManager->Initialize();

	// define and start UI session
	G4UImanager* UImanager = G4UImanager::GetUIpointer();
	G4String command = "/control/execute ";
	UImanager->ApplyCommand(command + fileName);

	// default interactive mode, if no macro specified
	if(argc == 1)
	{
		G4UIExecutive* ui = new G4UIExecutive(argc, argv);
		ui->SessionStart();
		delete ui;
	}

	// job termination
	delete visManager;
	delete runManager;

	G4cout << "KLMC_SAC finished successfully!" << G4endl;

	return 0;
}
