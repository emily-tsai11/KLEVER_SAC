#include "G4RunManager.hh"
#include "G4UImanager.hh"

#ifdef G4UI_USE
#include "G4VisExecutive.hh"
#endif

#include "SACDetector.hh"
#include "SACGeometry.hh"

int main()
{
	// construct default run manager
	G4RunManager* runManager = new G4RunManager;



	// job termination
	delete runManager;
	return 0;
}
