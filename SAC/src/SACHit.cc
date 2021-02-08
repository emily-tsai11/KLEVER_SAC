// SACHit.cc
// --------------------------------------------------------------
// History:
//
// Created by Emanuele Leonardi (emanuele.leonardi@roma1.infn.it) 2016-03-17
// Adapted from PADME experiment by Emily Tsai (emily.tsai11@gmail.com) 2020-7-8
// --------------------------------------------------------------

#include "SACHit.hh"

#include "G4UnitsTable.hh"
#include "G4VVisManager.hh"
#include "G4Circle.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4Allocator<SACHit>* SACHitAllocator;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SACHit::SACHit() : G4VHit() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SACHit::SACHit(const SACHit& right) : G4VHit()
{
	fCellID = right.fCellID;
	fTrackID = right.fTrackID;
	fParticleName = right.fParticleName;
	fVolumeName = right.fVolumeName;
	fSDName = right.fSDName;
	fCreatorProcessName = right.fCreatorProcessName;
	fEnergyDeposition = right.fEnergyDeposition;
	fTime = right.fTime;
	fTrackLength = right.fTrackLength;
	fPosition = right.fPosition;
	fLocalPosition = right.fLocalPosition;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SACHit::~SACHit() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

const SACHit& SACHit::operator=(const SACHit& right)
{
	fCellID = right.fCellID;
	fTrackID = right.fTrackID;
	fParticleName = right.fParticleName;
	fVolumeName = right.fVolumeName;
	fSDName = right.fSDName;
	fCreatorProcessName = right.fCreatorProcessName;
	fEnergyDeposition = right.fEnergyDeposition;
	fTime = right.fTime;
	fTrackLength = right.fTrackLength;
	fPosition = right.fPosition;
	fLocalPosition = right.fLocalPosition;

	return *this;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4int SACHit::operator==(const SACHit& right) const
{
	return (this == &right) ? 1 : 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SACHit::Draw()
{
	G4VVisManager* pVVisManager = G4VVisManager::GetConcreteInstance();
	if(pVVisManager)
	{
		G4Circle circle(fPosition);
		circle.SetScreenSize(2.0);
		circle.SetFillStyle(G4Circle::filled);
		G4Colour colour(1.0, 0.0, 0.0);
		G4VisAttributes attribs(colour);
		circle.SetVisAttributes(attribs);
		pVVisManager->Draw(circle);
	}
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SACHit::Print()
{
	G4cout
	<< "--- Cell ID: " << fCellID
	<< "\t| Track ID: " << fTrackID
	<< "\t| Particle: " << fParticleName
	<< "\t| Volume: " << fVolumeName
	<< "\t| SD: " << fSDName
	<< "\t| Creator Process: " << fCreatorProcessName
	<< "\t| Energy Deposited: " << G4BestUnit(fEnergyDeposition, "Energy")
	// << "\t| Time: " << G4BestUnit(fTime, "Time")
	<< "\t| Track Length: " << G4BestUnit(fTrackLength, "Length")
	// << "\t| Position: " << G4BestUnit(fPosition, "Length")
	// << "\t| Local Position: " << G4BestUnit(fLocalPosition, "Length")
	<< " ---" << G4endl;
}
