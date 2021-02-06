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

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class DetectorConstruction;
class EventAction;
class G4ParticleTable;
class G4ParticleGun;
class G4Event;
class PrimaryGeneratorActionMessenger;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
	public:

		PrimaryGeneratorAction(DetectorConstruction*, EventAction*);
		~PrimaryGeneratorAction();

		void GeneratePrimaries(G4Event* anEvent); // called at the beginning of each event
		void GenerateAthertonMomentum();
		G4LorentzVector GenerateKaonPositionTime();

	private:

		G4int fBeamType;
		G4String fParticleName;
		G4int fBeamEnergy;
		G4double fTime;
		G4ThreeVector fPosition;
		G4ThreeVector fMomentum;

		G4double fKaonPrimaryMomentum;
		G4double fKaonProductionAngle;
		G4double fKaonProductionAzimuth;
		G4double fKaonOpeningAngle;
		G4double fDecayZMin;

		G4LorentzVector f4Momentum;

		G4ParticleTable* fParticleTable;
		G4ParticleGun* fParticleGun;

		DetectorConstruction* fDetector;
		EventAction* fEventAction;
		PrimaryGeneratorActionMessenger* fMessenger;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
