// PrimaryGeneratorAction.cc
// --------------------------------------------------------------
// History:
//
// Created by Emily Tsai (emily.tsai11@gmail.com) 2020-7-9
// --------------------------------------------------------------

#include "PrimaryGeneratorAction.hh"

#include "globals.hh"
#include "TRandom3.h"
#include "G4SystemOfUnits.hh"
#include "G4RunManager.hh"
#include "G4GeometryManager.hh"
#include "G4ParticleGun.hh"
#include "G4DecayTable.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4PhysicalConstants.hh"
#include "RandomGenerator.hh"

#include "PhysicsList.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorAction::PrimaryGeneratorAction(DetectorConstruction* detector, EventAction& eventAction) : fDetector(detector)
{
	fEventAction = &eventAction;
	fParticleTable = G4ParticleTable::GetParticleTable();

	fBeamType = 0; // 1: KL

	// Atherton momentum constants
	fKaonPrimaryMomentum = 400.0 * GeV;
	fKaonProductionAngle = 8.0 * mrad;
	fKaonProductionAzimuth = -0.5 * pi * radian;
	fKaonOpeningAngle = 0.4 * mrad;
	fDecayZMin = 102425.0;

	// default particle type
	fParticleName = "gamma";

	G4int nParticles = 1;
	fParticleGun = new G4ParticleGun(nParticles);
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
	switch(fBeamType)
	{
		case 1: // KL
		{
			fParticleName = "kaon0L";
			fEventAction->FillRandomEnginesStates();
			GenerateAthertonMomentum(); // fills f4Momentum
			G4LorentzVector PosTime = GenerateKaonPositionTime(); // gets values from f4Momentum

			fParticleGun->SetParticleDefinition(fParticleTable->FindParticle(fParticleName));
			fParticleGun->SetParticleEnergy(f4Momentum.e());
			G4cout << "PARTICLE ENERGY: " << fParticleGun->GetParticleEnergy() << G4endl;
			fParticleGun->SetParticleTime(PosTime.t());
			G4cout << "PARTICLE TIME: " << fParticleGun->GetParticleTime() << G4endl;
			fParticleGun->SetParticlePosition(PosTime.vect());
			G4cout << "PARTICLE POSITION: " << fParticleGun->GetParticlePosition() << G4endl;
			fParticleGun->SetParticleMomentumDirection(G4ThreeVector(f4Momentum.px(), f4Momentum.py(), -f4Momentum.pz()));
			G4cout << "PARTICLE MOMENTUM DIRECTION: " << fParticleGun->GetParticleMomentumDirection() << G4endl;
			break;
		}
		default: // gamma
		{
			fParticleGun->SetParticleDefinition(fParticleTable->FindParticle(fParticleName));
			fParticleGun->SetParticleEnergy(100.0 * MeV);
			fParticleGun->SetParticleTime(0.0 * ns);
			fParticleGun->SetParticlePosition(G4ThreeVector(0.0 * m, 0.0 * m, 1.0 * m));
			fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0.0, 0.0, -1.0));
			break;
		}
	}

	fParticleGun->GeneratePrimaryVertex(anEvent);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PrimaryGeneratorAction::GenerateAthertonMomentum()
{
	if(fParticleName != "kaon0L")
	{
		G4cerr << "Invalid neutral beam particle specified for Atherton generator: " << fParticleName << G4endl;
		G4RunManager::GetRunManager()->AbortRun();
	}

	G4ThreeVector dirPrimary(sin(fKaonProductionAngle) * cos(fKaonProductionAzimuth),
		sin(fKaonProductionAngle) * sin(fKaonProductionAzimuth), cos(fKaonProductionAngle));
	G4double deltaCosThetaGen = 1 - cos(fKaonOpeningAngle);

	TRandom3* RandomDecay = (RandomGenerator::GetInstance())->GetRandomDecay();

	// Sample flat solid angle distribution about the netural beam axis (z-axis)
	G4ThreeVector dirBeam;
	G4double pA;

	while(true)
	{
		G4double cosThetaBeam = 1 - deltaCosThetaGen * RandomDecay->Uniform();
		G4double sinThetaBeam = sqrt(1 - cosThetaBeam * cosThetaBeam);
		G4double phiBeam = 2 * pi * RandomDecay->Uniform();
		dirBeam.set(sinThetaBeam * cos(phiBeam), sinThetaBeam * sin(phiBeam), cosThetaBeam);

		G4double theta = dirBeam.angle(dirPrimary);

		// d2N/dp*dOmega for KL is a linear combination of those for K+ and K-
		// (the so called Wachsmuth rule): KL = (1/4K+) + (3/4K-)

		// K+ Atherton spectrum parameters
		const G4double Ap = 0.16;
		const G4double Bp = 8.5;
		const G4double Cp = 3.0;

		// K- Atherton spectrum parameters
		const G4double Am = 0.1;
		const G4double Bm = 13.0;
		const G4double Cm = 3.5;

		// Flux distribution from Atherton et al.
		G4double p0 = fKaonPrimaryMomentum;
		G4double p0r = 0.001 * p0;
		const G4double Ath_max = 10.0;

		pA = RandomDecay->Uniform() * p0r;
		G4double f = RandomDecay->Uniform() * Ath_max;
		G4double fAth = 1.0 * Ap * Bp / p0r * exp(-Bp * pA / p0r) * Cp * pA * pA / M_PI * exp(-Cp * pA * pA * theta * theta)
			+ 3.0 * Am * Bm / p0r * exp(-Bm * pA / p0r) * Cm * pA * pA / M_PI * exp(-Cm * pA * pA * theta * theta);

		if (f < fAth) break;
	}

	pA *= 1000.0;
	f4Momentum.setVectM(dirBeam * pA, fParticleTable->FindParticle("kaon0L")->GetPDGMass());
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4LorentzVector PrimaryGeneratorAction::GenerateKaonPositionTime()
{
	TRandom3* RandomDecay = (RandomGenerator::GetInstance())->GetRandomDecay();

	// Kaon production position
	G4double sx = 0.2 * mm;
	G4double sy = 0.3 * mm;
	G4double x0 = RandomDecay->Gaus(0.0, sx);
	G4double y0 = RandomDecay->Gaus(0.0, sy);
	G4double z0 = RandomDecay->Uniform(-200.0 * mm, 200.0 * mm);

	// Position of kaon handover to Geant4 (bypass Cedar, GTK, CHANTI)
	G4double z = fDecayZMin;
	G4double x = x0 + (z - z0) * f4Momentum.px() / f4Momentum.pz();
	G4double y = y0 + (z - z0) * f4Momentum.py() / f4Momentum.pz();

	// Time of handing KL over to Geant4
	G4double path = sqrt((x - x0) * (x - x0) + (y - y0) * (y - y0) + (z - z0) * (z - z0));
	G4double time = path / (f4Momentum.beta() * c_light);

	return G4LorentzVector(x, y, z, time);
}
