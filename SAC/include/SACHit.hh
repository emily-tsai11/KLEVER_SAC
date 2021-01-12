// SACHit.hh
// --------------------------------------------------------------
// History:
//
// Created by Emanuele Leonardi (emanuele.leonardi@roma1.infn.it) 2016-03-17
// Adapted from PADME experiment by Emily Tsai (emily.tsai11@gmail.com) 2020-7-8
// --------------------------------------------------------------

#ifndef SACHit_H
#define SACHit_H 1

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"

// trial
#include "G4VPhysicalVolume.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class SACHit : public G4VHit
{
	public:

		SACHit();
		SACHit(const SACHit&);
		~SACHit();

		const SACHit& operator=(const SACHit&);
		G4int operator==(const SACHit&) const;

		inline void* operator new(size_t);
		inline void operator delete(void*);

		void Draw();
		void Print();

		void SetChannelId(G4int c) { fChannelId = c; }
		G4int GetChannelId() { return fChannelId; }

		void SetTime(G4double t) { fTime = t; };
		G4double GetTime() { return fTime; };

		// trial
		void SetVolume(G4VPhysicalVolume* v) { fVolume = v; }
		G4VPhysicalVolume* GetVolume() { return fVolume; }

		void SetInitialEnergy(G4double e) { fInitialEnergy = e; }
		G4double GetInitialEnergy() { return fInitialEnergy; }

		void SetFinalEnergy(G4double e) { fFinalEnergy = e; }
		G4double GetFinalEnergy() { return fFinalEnergy; }

		void SetEnergyDep(G4double e) { fEnergyDep = e; }
		G4double GetEnergyDep() { return fEnergyDep; }

		void SetPosition(G4ThreeVector p) { fPosition = p; }
		G4ThreeVector GetPosition() { return fPosition; }
		G4double GetPosX() { return fPosition.x(); };
		G4double GetPosY() { return fPosition.y(); };
		G4double GetPosZ() { return fPosition.z(); };

		void SetLocalPosition(G4ThreeVector p) { fLocalPosition = p; }
		G4ThreeVector GetLocalPosition() { return fLocalPosition; }
		G4double GetLocalPosX() { return fLocalPosition.x(); };
		G4double GetLocalPosY() { return fLocalPosition.y(); };
		G4double GetLocalPosZ() { return fLocalPosition.z(); };

		void SetTrackId(G4int id) { fTrackId = id; }
		G4int GetTrackId() { return fTrackId; }

		void SetPType(G4int typ) { fTrackType = typ; };
		G4int GetPType() { return fTrackType; };

		void SetTrackLen(G4double len) { fTrackLength = len; }
		G4double GetTrackLen() { return fTrackLength; }

	private:

		G4int fChannelId;
		G4double fTime;
		//trial
		G4VPhysicalVolume* fVolume;
		G4double fInitialEnergy;
		G4double fFinalEnergy;
		G4double fEnergyDep;
		G4ThreeVector fPosition;
		G4ThreeVector fLocalPosition;
		G4int fTrackId;
		G4int fTrackType;
		G4double fTrackLength;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

typedef G4THitsCollection<SACHit> SACHitsCollection;

extern G4Allocator<SACHit> SACHitAllocator;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void* SACHit::operator new(size_t)
{
	void *aHit;
	aHit = (void*) SACHitAllocator.MallocSingle();
	return aHit;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void SACHit::operator delete(void *aHit)
{
	SACHitAllocator.FreeSingle((SACHit*) aHit);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
