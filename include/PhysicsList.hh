// PhysicsList.hh
// --------------------------------------------------------------
// Based on GEANT 4 - PhysicsList class from Hadr01 example
// --------------------------------------------------------------
// History:
//
// Antonino Sergi (Antonino.Sergi@cern.ch)
// Sergey Podolsky (siarhei.padolski@cern.ch) 03-09-2012
// Adapted from KLMC by Emily Tsai (emily.tsai11@gmail.com) 2020-7-9
// --------------------------------------------------------------

#ifndef PhysicsList_H
#define PhysicsList_H 1

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "G4VModularPhysicsList.hh"
#include "G4SystemOfUnits.hh"
#include "globals.hh"

// optical processes
#include "G4Cerenkov.hh"
#include "G4Scintillation.hh"
#include "G4OpAbsorption.hh"
#include "G4OpRayleigh.hh"
#include "G4OpBoundaryProcess.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class G4OpWLS;
class G4VPhysicsConstructor;
class PhysicsListMessenger;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class PhysicsList : public G4VModularPhysicsList
{
	public:

		PhysicsList();
		virtual ~PhysicsList();
		static PhysicsList *GetInstance();

		void ConstructParticle();
		void ConstructProcess();

		void SetCuts();
		void SetCutForGamma(G4double);
		void SetCutForElectron(G4double);
		void SetCutForPositron(G4double);
		void SetCutForProton(G4double);

		void AddPhysicsList(const G4String& name);
		void OpticalPhysics();
		void List();
		void AddParameterisation();

		void SetLambdaDecaySpecial(G4bool);

		// void SetExoticParticleMass(G4double, G4int);
		// void SetExoticParticleDecayMode(G4int, G4int);
		// void SetExoticParticleLifetime(G4double, G4int);
		// G4int GetExoticParticleDecayMode() const { return fExoticParticleDecayMode; }
		// void SetExoticParticleNumberOfGeneratedParticles(G4int);
		// G4int GetExoticParticleNumberOfGeneratedParticles() { return fNumberOfGeneratedParticles; }
		// void SetExoticParticleMassStep(G4double);
		// G4double GetExoticParticleMassStep() { return fExoticParticleMassStep; }
		void SetBrPie2(G4double); // set the branching ratio of the pi+- --> e+- nu decay
		void SetMuonDecay(G4int); // set muon decay mode

	private:

		void SetBuilderList0(G4bool flagHP = false);
		void SetBuilderList1(G4bool flagHP = false);
		void SetBuilderList2();

		static PhysicsList* fgInstance;
		static int fNumberOfGeneratedParticles;
		// static double fExoticParticleMassStep;

		G4double fCutForGamma;
		G4double fCutForElectron;
		G4double fCutForPositron;
		G4double fCutForProton;

		// optical processes
		G4Cerenkov* fCerenkovProcess;
		G4Scintillation* fScintillationProcess;
		G4OpAbsorption* fAbsorptionProcess;
		G4OpRayleigh* fRayleighScatteringProcess;
		G4OpBoundaryProcess* fBoundaryProcess;
		G4OpWLS* fWLSProcess;

		G4VPhysicsConstructor* fEmPhysicsList;
		G4VPhysicsConstructor* fParticleList;
		// G4VPhysicsConstructor* fExoticParticle;
		// G4int fExoticParticleDecayMode;
		std::vector<G4VPhysicsConstructor*> fHadronPhys;

		// PhysicsListMessenger* fMessenger;
		G4bool dump;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
