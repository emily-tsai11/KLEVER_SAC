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

#include "DetectorConstruction.hh"
#include "EventAction.hh"
#include "PhysicsList.hh"
#include "PrimaryGeneratorAction.hh"
#include "RandomGenerator.hh"
#include "RunAction.hh"
#include "SteppingAction.hh"

#include <stdlib.h>
#include <time.h>

int main(int argc, char** argv)
{
	// Get macro file and seed number
	G4String fileName;
	srand(time(0));
	long seedNum = rand();

	if(argc == 1)
	{
		fileName = "macros/vis.mac"; // Default visualization for interactive mode
		G4cout << "No macro specified, using " << fileName << G4endl;
		G4cout << "No seed number specified, using " << seedNum << G4endl;
	}
	else if(argc == 2)
	{
		fileName = argv[1];
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

	// Set random engine
	RandomGenerator::GetInstance()->SetSeed(seedNum);

	// Construct default run manager
	G4RunManager* runManager = new G4RunManager;
	DetectorConstruction* detector = new DetectorConstruction();
	runManager->SetUserInitialization(detector);
	runManager->SetUserInitialization(new PhysicsList);

	// Set user actions
	RunAction* runAction = new RunAction();
	EventAction* eventAction = new EventAction(runAction);
	runManager->SetUserAction(runAction);
	runManager->SetUserAction(new PrimaryGeneratorAction());
	runManager->SetUserAction(eventAction);
	runManager->SetUserAction(new SteppingAction());

	// Define and start UI session
	G4UImanager* UImanager = G4UImanager::GetUIpointer();
	G4String command = "/control/execute ";

	if(argc == 1)
	{
		// Define and initialize visualization session
		G4VisManager* visManager = new G4VisExecutive;
		visManager->Initialize();

		// Default is interactive mode, if no macro specified
		G4UIExecutive* ui = new G4UIExecutive(argc, argv);
		UImanager->ApplyCommand(command + fileName);
		ui->SessionStart();
		delete ui;

		// Delete visualization manager
		delete visManager;
	}
	else
	{
		// Batch mode, if macro specified
		UImanager->ApplyCommand(command + fileName);
	}

	// Job termination
	delete runManager;

	G4cout << "KLMC_SAC finished successfully!" << G4endl;

	return 0;
}
