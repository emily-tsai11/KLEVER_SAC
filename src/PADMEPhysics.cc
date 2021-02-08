// PADMEPhysics.cc
// --------------------------------------------------------------
// PADME modular physics list
// Based on QGSP_BERT + OpticalPhotons
// --------------------------------------------------------------
// History:
//
// Adapted from PADME by Emily Tsai (emily.tsai11@gmail.com) 2020-7-13
// --------------------------------------------------------------

#include "PADMEPhysics.hh"
#include "globals.hh"
#include "G4SystemOfUnits.hh"

#include "G4Material.hh"
#include "G4MaterialTable.hh"
#include "G4ProcessManager.hh"
#include "G4ProcessVector.hh"
#include "G4ParticleTypes.hh"
#include "G4ParticleTable.hh"
#include "G4DecayPhysics.hh"
#include "G4EmStandardPhysics.hh"
#include "G4EmExtraPhysics.hh"
#include "G4IonPhysics.hh"
#include "G4StoppingPhysics.hh"
#include "G4HadronElasticPhysics.hh"
#include "G4HadronElasticPhysicsHP.hh"
#include "G4NeutronTrackingCut.hh"
#include "G4OpticalPhysics.hh"

#include "G4HadronPhysicsQGSP_BERT.hh"
#include "G4HadronPhysicsQGSP_BERT_HP.hh"
#include "G4HadronPhysicsQGSP_BIC.hh"
#include "G4HadronPhysicsQGSP_BIC_HP.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PADMEPhysics::PADMEPhysics(G4int ver) : G4VModularPhysicsList()
{
	G4cout << "[PADMEPhysics::PADMEPhysics] Geant4 PhysicsList simulation engine is PADMEPhysics\n" << G4endl;

	const int IsNuclearON = 1;		// Std = 1
	const int NuclearBertini = 1;	// Bertini cascade
	const int NuclearBinary = 0;	// Binary cascade
	const int NuclearHP = 1;		// Nuclear high performance tracking (not speed!)

	this->defaultCutValue = 0.7 * mm;
	this->SetVerboseLevel(ver);

	// EM physics
	// Includes:
	//		Multiple scattering:
	//			< 100 * MeV: G4UrbanMscModel95
	//			> 100 * MeV: G4WentzelVIModel
	//		Coulomb scattering:
	//			> 100 * MeV:
	//		Ionisation
	//		Bremsstrahlung
	//		e+ annihilation
	this->RegisterPhysics(new G4EmStandardPhysics(ver));

	// Synchroton radiation & GN physics
	// Includes:
	//		BertiniElectroNuclearBuilder
	//			Gamma - Bertini Cascade
	//			e+/e- - the virtual photon is converted to a real photon; -> Bertini
	if(IsNuclearON == 1) this->RegisterPhysics(new G4EmExtraPhysics(ver));

	// Scintillation and Cerenkov radiation
	// Includes: optical photon propagation
	// Enable only if you really want to track optical photons
	this->RegisterPhysics(new G4OpticalPhysics(ver));

	// Decays
	this->RegisterPhysics(new G4DecayPhysics(ver));

	if(IsNuclearON == 1)
	{
		// Hadron elastic scattering
		if(NuclearHP == 1) this->RegisterPhysics(new G4HadronElasticPhysicsHP(ver));
		else this->RegisterPhysics(new G4HadronElasticPhysics(ver));

		// Hadron physics
		if(NuclearBertini == 1)
		{
			if(NuclearHP == 1) this->RegisterPhysics(new G4HadronPhysicsQGSP_BERT_HP(ver));
			else this->RegisterPhysics(new G4HadronPhysicsQGSP_BERT(ver));
		}
		else if(NuclearBinary == 1)
		{
			if(NuclearHP == 1) this->RegisterPhysics(new G4HadronPhysicsQGSP_BIC_HP(ver));
			else this->RegisterPhysics(new G4HadronPhysicsQGSP_BIC(ver));
		}

		// Ion physics
		this->RegisterPhysics(new G4IonPhysics(ver));
	}

	// Neutron tracking cut
	this->RegisterPhysics(new G4NeutronTrackingCut(ver));
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PADMEPhysics::~PADMEPhysics() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PADMEPhysics::SetCuts()
{
	if(this->verboseLevel > 1) G4cout << "[PADME_PHYS::SetCuts]" << G4endl;

	// Sets the default cut value for all particle types
	this->SetCutsWithDefault();

	// if(this->verboseLevel > 0) G4VUserPhysicsList::DumpCutValuesTable();
}
