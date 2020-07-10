// PrimaryGeneratorAction.cc
// --------------------------------------------------------------
// History:
//
// Created by Emily Tsai (emily.tsai11@gmail.com) 2020-7-9
// --------------------------------------------------------------

#include "PrimaryGeneratorAction.hh"

#include "G4RunManager.hh"
#include "G4GeometryManager.hh"
// #include "RootIOManager.hh"

#include "G4GeneralParticleSource.hh"
#include "G4ParticleGun.hh"
#include "G4DecayTable.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4PhysicalConstants.hh"

#include "PhysicsList.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorAction::PrimaryGeneratorAction(DetectorConstruction* detector, EventAction& eventAction) : fDetector(detector)
{
	fEventAction = &eventAction;
	fParticleTable = G4ParticleTable::GetParticleTable();

	// default settings
	fParticleGun = new G4GeneralParticleSource();
	fParticleGun->SetParticleDefinition(fParticleTable->FindParticle("opticalphoton"));
	fParticleGun->SetParticleTime(0.0 * ns);
	fParticleGun->SetParticlePosition(G4ThreeVector(0.0 * m, 0.0 * m, 0.0 * m));
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
	delete fParticleGun;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

// called at the beginning of each event
void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
	fParticleGun->GeneratePrimaryVertex(anEvent);
}
