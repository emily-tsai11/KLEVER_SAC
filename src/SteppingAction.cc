// SteppingAction.cc
// --------------------------------------------------------------
// History:
//
// Created by Emily Tsai (emily.tsai11@gmail.com) 2020-7-9
// --------------------------------------------------------------

#include "SteppingAction.hh"
#include "G4Track.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SteppingAction::SteppingAction() : G4UserSteppingAction()
{
	fVerboseLevel = 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SteppingAction::~SteppingAction() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SteppingAction::UserSteppingAction(const G4Step* aStep)
{
	// if(fVerboseLevel > 5) PrintStep(aStep);
	PrintStep(aStep);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SteppingAction::PrintStep(const G4Step* aStep, G4String ParticleName)
{
	G4Track *aTrack = aStep->GetTrack();

	if(!ParticleName.length() || (aTrack->GetParticleDefinition()->GetParticleName() == ParticleName))
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
				aTrack->GetParticleDefinition()->GetParticleName().data(),
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
				aTrack->GetParticleDefinition()->GetParticleName().data(),
				aStep->GetPostStepPoint()->GetPhysicalVolume()->GetName().data());
		}
	}
}
