// SACDigitizer.hh
// --------------------------------------------------------------
// History:
//
// Created by Emanuele Leonardi (emanuele.leonardi@roma1.infn.it) 2016-07-22
// Adapted from PADME experiment by Emily Tsai (emily.tsai11@gmail.com) 2020-7-7
// --------------------------------------------------------------

#ifndef SACDigitizer_H
#define SACDigitizer_H 1

#include "G4VDigitizerModule.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class SACHit;

class SACDigitizer : public G4VDigitizerModule
{
	public:

		SACDigitizer(G4String);
		~SACDigitizer();

		void Digitize();
		void DigitizeSinglHit();

	private:

		// compute number of photoelectrons produced by hit and time of first photoelectron
		void ComputeNpe(SACHit*, G4double*, G4double*); // hit, signal, time

		// digitization parameters
		G4double fCrystalLength; // length of SAC crystal along Z
		G4double fPropagationSpeed; // speed of light in SAC crystal
		G4double fEHtoNPEConversion; // converts hit energy to # of photoelectrons produced by photocathode
		G4double fPEtoSConversion; // signal produced by 1 photoelectron

		// collection probability map along Z
		G4int fCollMapNBins;
		G4double fCollMapBinLen;
		std::vector<G4double> fCollectionMap;

		// PMT parameters
		G4double fPMTTransitTime;
		G4double fPMTCableDelay;

		G4double fMinimumSignalTime; // minimum time for SAC signal output
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
