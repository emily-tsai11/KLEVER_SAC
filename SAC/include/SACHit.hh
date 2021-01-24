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

		void SetCellID(G4int id) { fCellID = id; }
		G4int GetCellID() { return fCellID; }

		void SetTrackID(G4int id) { fTrackID = id; }
		G4int GetTrackID() { return fTrackID; }

		void SetParticleName(G4String pn) { fParticleName = pn; }
		G4String GetParticleName() { return fParticleName; }

		void SetVolumeName(G4String vn) { fVolumeName = vn; }
		G4String GetVolumeName() { return fVolumeName; }

		void SetSDName(G4String sdn) { fSDName = sdn; }
		G4String GetSDName() { return fSDName; }

		void SetCreatorProcessName(G4String cpn) { fCreatorProcessName = cpn; }
		G4String GetCreatorProcessName() { return fCreatorProcessName; }

		void SetEnergyDeposition(G4double e) { fEnergyDeposition = e; }
		G4double GetEnergyDeposition() { return fEnergyDeposition; }

		void SetTime(G4double t) { fTime = t; };
		G4double GetTime() { return fTime; };

		void SetTrackLength(G4double tl) { fTrackLength = tl; }
		G4double GetTrackLength() { return fTrackLength; }

		void SetPosition(G4ThreeVector p) { fPosition = p; }
		G4ThreeVector GetPosition() { return fPosition; }
		G4double GetPositionX() { return fPosition.x(); }
		G4double GetPositionY() { return fPosition.y(); }
		G4double GetPositionZ() { return fPosition.z(); }

		void SetLocalPosition(G4ThreeVector p) { fLocalPosition = p; }
		G4ThreeVector GetLocalPosition() { return fLocalPosition; }
		G4double GetLocalPositionX() { return fLocalPosition.x(); }
		G4double GetLocalPositionY() { return fLocalPosition.y(); }
		G4double GetLocalPositionZ() { return fLocalPosition.z(); }

	private:

		G4int fCellID;
		G4int fTrackID;
		G4String fParticleName;
		G4String fVolumeName;
		G4String fSDName;
		G4String fCreatorProcessName;
		G4double fEnergyDeposition;
		G4double fTime;
		G4double fTrackLength;
		G4ThreeVector fPosition;
		G4ThreeVector fLocalPosition;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

typedef G4THitsCollection<SACHit> SACHitsCollection;

extern G4Allocator<SACHit>* SACHitAllocator;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void* SACHit::operator new(size_t)
{
	if(!SACHitAllocator) SACHitAllocator = new G4Allocator<SACHit>;
	return (void*) SACHitAllocator->MallocSingle();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void SACHit::operator delete(void* aHit)
{
	SACHitAllocator->FreeSingle((SACHit*) aHit);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
