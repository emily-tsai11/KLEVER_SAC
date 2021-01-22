// PrimaryGeneratorAction.hh
// --------------------------------------------------------------
// History:
//
// Created by Emily Tsai (emily.tsai11@gmail.com) 2020-7-9
// --------------------------------------------------------------

#ifndef PrimaryGeneratorAction_H
#define PrimaryGeneratorAction_H 1

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleTable.hh"
#include "G4LorentzVector.hh"
#include "G4Event.hh"
#include "globals.hh"

#include "EventAction.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class DetectorConstruction;
class G4ParticleTable;
class G4ParticleDefinition;
class G4ParticleGun;
class G4Event;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
	public:

		PrimaryGeneratorAction(DetectorConstruction*, EventAction&);
		~PrimaryGeneratorAction();

		void GeneratePrimaries(G4Event* anEvent); // called at the beginning of each event
		void GenerateAthertonMomentum();
		G4LorentzVector GenerateNeutralPositionTime();

		// G4String GetParticleName() { return fParticleName; }
		// void SetParticleName(G4String v) { fParticleName = v; }

		G4String GetNeutralParticleName() { return fNeutralParticleName; }
		void SetNeutralParticleName(G4String v) { fNeutralParticleName = v; }

	private:

		G4int fBeamType;
		// G4String fParticleName;
		G4String fNeutralParticleName;

		G4double fNeutralPrimaryMomentum;
		G4double fNeutralProductionAngle;
		G4double fNeutralProductionAzimuth;
		G4double fNeutralOpeningAngle;
		G4double fDecayZMin;

		G4LorentzVector f4Momentum;

		G4ParticleTable* fParticleTable;
		G4ParticleDefinition* fParticleDef;
		G4ParticleGun* fParticleGun;
		DetectorConstruction* fDetector;
		EventAction* fEventAction;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
