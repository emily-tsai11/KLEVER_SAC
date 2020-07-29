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

G4Allocator<SACHit> SACHitAllocator;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SACHit::SACHit() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SACHit::~SACHit() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SACHit::SACHit(const SACHit& right) : G4VHit()
{
	fChannelId = right.fChannelId;
	fTime = right.fTime;
	fInitialEnergy = right.fInitialEnergy;
	fFinalEnergy = right.fFinalEnergy;
	fEnergyDep = right.fEnergyDep;
	fPosition = right.fPosition;
	fLocalPosition = right.fLocalPosition;
	fTrackId = right.fTrackId;
	fTrackType = right.fTrackType;
	fTrackLength = right.fTrackLength;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

const SACHit& SACHit::operator=(const SACHit& right)
{
	fChannelId = right.fChannelId;
	fTime = right.fTime;
	fInitialEnergy = right.fInitialEnergy;
	fFinalEnergy = right.fFinalEnergy;
	fEnergyDep = right.fEnergyDep;
	fPosition = right.fPosition;
	fLocalPosition = right.fLocalPosition;
	fTrackId = right.fTrackId;
	fTrackType = right.fTrackType;
	fTrackLength = right.fTrackLength;

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
	<< "- channel: " << fChannelId
	<< " time: " << G4BestUnit(fTime, "Time")
	<< " initial energy: " << G4BestUnit(fInitialEnergy, "Energy")
	<< " final energy: " << G4BestUnit(fFinalEnergy, "Energy")
	<< " energy deposit: " << G4BestUnit(fEnergyDep, "Energy")
	<< " global position: " << G4BestUnit(fPosition, "Length")
	<< " local position: " << G4BestUnit(fLocalPosition, "Length")
	<< " track: " << fTrackId
	<< " track type: " << fTrackType
	<< " track length: " << G4BestUnit(fTrackLength, "Length")
	<< G4endl;
}
