// PMTMessenger.hh
// --------------------------------------------------------------
// History:
//
// Created by Emily Tsai (emily.tsai11@gmail.com) 2020-12-30
// --------------------------------------------------------------

#ifndef PMTMessenger_H
#define PMTMessenger_H 1

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "G4UImessenger.hh"
#include "globals.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class G4UIdirectory;
class G4UIcmdWithAnInteger;
class G4UIcmdWithADoubleAndUnit;

class PMTDetector;
class PMTSD;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class PMTMessenger : public G4UImessenger
{
	public:

		PMTMessenger(PMTDetector*);
		~PMTMessenger();

	private:

		PMTDetector* fPMTDetector;
		PMTSD* fPMTSD;

		G4UIdirectory* fPMTDetectorDir;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
