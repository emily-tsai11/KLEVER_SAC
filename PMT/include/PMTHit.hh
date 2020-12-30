// PMTHit.hh
// --------------------------------------------------------------
// History:
//
// Created by Emily Tsai (emily.tsai11@gmail.com) 2020-12-30
// --------------------------------------------------------------

#ifndef PMTHit_H
#define PMTHit_H 1

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class PMTHit : public G4VHit
{
	public:

		PMTHit();
		// PMTHit(const PMTHit&);
		~PMTHit();

		void SetChannelId(G4int c) { fChannelId = c; }
		G4int GetChannelId() { return fChannelId; }

	private:

		G4int fChannelId;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

typedef G4THitsCollection<PMTHit> PMTHitsCollection;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
