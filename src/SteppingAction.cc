// SteppingAction.cc
// --------------------------------------------------------------
// History:
//
// Created by Emily Tsai (emily.tsai11@gmail.com) 2020-7-9
// --------------------------------------------------------------

#include "SteppingAction.hh"

#include "G4SteppingManager.hh"
#include "G4RunManager.hh"

#include "G4Track.hh"
#include "G4TrackVector.hh"
#include "G4VProcess.hh"

#include <stdio.h>

#include "EventAction.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SteppingAction::SteppingAction(EventAction* eventAction) : fEventAction(eventAction) {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SteppingAction::~SteppingAction() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SteppingAction::UserSteppingAction(const G4Step* aStep)
{
	// PrintStep(aStep);

	G4Track* Track = aStep->GetTrack();

	// from KLMC: suppress particles touching black hole volumes
	if(aStep->GetPostStepPoint()->GetPhysicalVolume())
	{
		if(aStep->GetPostStepPoint()->GetPhysicalVolume()->GetLogicalVolume()->GetMaterial()->GetName() == "KLBlackHole" &&
			Track->GetParticleDefinition()->GetParticleName() != "geantino" &&
			Track->GetParticleDefinition()->GetParticleName() != "Exotic")
		{
			Track->SetTrackStatus(fStopAndKill);
		}
	}

	// from KLMC: forbid Cherenkov light emission by secondaries in CEDAR lenses
	if(aStep->GetPreStepPoint()->GetPhysicalVolume())
	{
		if((aStep->GetPreStepPoint()->GetPhysicalVolume()->GetLogicalVolume()->GetMaterial()->GetName() == "Cedar_Quartz" ||
			aStep->GetPreStepPoint()->GetPhysicalVolume()->GetLogicalVolume()->GetMaterial()->GetName() == "Cedar_IdealQuartz") &&
			(Track->GetParticleDefinition()->GetParticleName() != "opticalphoton" &&
			Track->GetParticleDefinition()->GetParticleName() != "geantino" &&
			Track->GetParticleDefinition()->GetParticleName() != "Exotic"))
		{
			Track->SetTrackStatus(fKillTrackAndSecondaries);
		}
	}

	// save all steps [NOT SURE WHAT TO DO WITH THIS BUT I'LL FIGURE IT OUT LATER]
	// fFinalMomentum = G4LorentzVector(Track->GetMomentum(), Track->GetTotalEnergy());
	// fInitialMomentum = fFinalMomentum - G4LorentzVector(aStep->GetDeltaMomentum(), aStep->GetDeltaEnergy());
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SteppingAction::PrintStep(const G4Step* aStep, G4String ParticleName)
{
	G4Track *Track = aStep->GetTrack();

	if(!ParticleName.length() || (Track->GetParticleDefinition()->GetParticleName() == ParticleName))
	{
		if(aStep->GetPreStepPoint()->GetPhysicalVolume())
		{
			printf("PreStep: %7.1f %7.1f %8.1f | %f %f %f %f | %s %s\n",
				aStep->GetPreStepPoint()->GetPosition().x(),
				aStep->GetPreStepPoint()->GetPosition().y(),
				aStep->GetPreStepPoint()->GetPosition().z(),
				aStep->GetPreStepPoint()->GetMomentum().x(),
				aStep->GetPreStepPoint()->GetMomentum().y(),
				aStep->GetPreStepPoint()->GetMomentum().z(),
				aStep->GetPreStepPoint()->GetTotalEnergy(),
				Track->GetParticleDefinition()->GetParticleName().data(),
				aStep->GetPreStepPoint()->GetPhysicalVolume()->GetName().data());
		}
		if(aStep->GetPostStepPoint()->GetPhysicalVolume())
		{
			printf("PostStep: %7.1f %7.1f %8.1f | %f %f %f %f | %s %s\n",
				aStep->GetPostStepPoint()->GetPosition().x(),
				aStep->GetPostStepPoint()->GetPosition().y(),
				aStep->GetPostStepPoint()->GetPosition().z(),
				aStep->GetPostStepPoint()->GetMomentum().x(),
				aStep->GetPostStepPoint()->GetMomentum().y(),
				aStep->GetPostStepPoint()->GetMomentum().z(),
				aStep->GetPostStepPoint()->GetTotalEnergy(),
				Track->GetParticleDefinition()->GetParticleName().data(),
				aStep->GetPostStepPoint()->GetPhysicalVolume()->GetName().data());
		}
	}
}
