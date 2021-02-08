// PhysicsList.cc
// --------------------------------------------------------------
// Based on GEANT 4 - PhysicsList class from Hadr01 example
// --------------------------------------------------------------
// History:
//
// Antonino Sergi (Antonino.Sergi@cern.ch)
// Sergey Podolsky (siarhei.padolski@cern.ch) 03-09-2012
// Roberto Piandani (roberto.piandani@cern.ch) 30-05-2013
// Michal Koval (michal.koval@cern.ch) 21-08-2013
// Karim Massri (karim.massri@cern.ch) 27-03-2014
// - Updated physics list for Geant 4 10 compatibility (from Hadr01 example)
//		- deprecated lists removed:
//			- G4HadronDElasticPhysics
//			- G4HadronQElasticPhysics
//			- G4HadronHElasticPhysics
//			- G4QStoppingPhysics
//			- G4LHEPStoppingPhysics
//			- HadronPhysicsLHEP
//			- HadronPhysicsQGSC_BERT
//			- HadronPhysicsQGSC_CHIPS
//			- HadronPhysicsFTFP_BERT_TRV
//			- HadronPhysicsLHEP
//			- HadronPhysicsLHEP_EMV
//			- HadronPhysicsQGSP
//		- change of names:
//			- HadronPhysicsFTFP_BERT -> G4HadronPhysicsFTFP_BERT
//			- HadronPhysicsFTF_BIC -> G4HadronPhysicsFTF_BIC
//			- HadronPhysicsQGSP_BERT -> G4HadronPhysicsQGSP_BERT
//			- HadronPhysicsQGSP_BERT_HP -> G4HadronPhysicsQGSP_BERT_HP
//			- HadronPhysicsQGSP_BIC -> G4HadronPhysicsQGSP_BIC
//			- HadronPhysicsQGSP_BIC_HP -> G4HadronPhysicsQGSP_BIC_HP
//			- HadronPhysicsQGSP_FTFP_BERT -> G4HadronPhysicsQGSP_FTFP_BERT
//			- HadronPhysicsQGS_BIC -> G4HadronPhysicsQGS_BIC
//		- new lists added:
//			- G4EmStandardPhysics_option4
//			- G4HadronElasticPhysicsXS
//			- G4HadronElasticPhysicsHP
//			- G4NeutronCrossSectionXS
//			- G4StoppingPhysics
//			- G4HadronPhysicsFTFP_BERT_HP
// Evgueni Goudzovski (eg@hep.ph.bham.ac.uk) Autumn 2015
// - Exotic particle and pion decays
// Viacheslav Duk (Viacheslav.Duk@cern.ch) August 2017
// - Exotic particle update
// Karim Massri (karim.massri@cern.ch) 09-10-2017
// - Custom muon decay class introduced
// Adapted from PADME by Emily Tsai (emily.tsai11@gmail.com) 2020-7-13
// --------------------------------------------------------------

#include "PhysicsList.hh"
#include "globals.hh"
#include "G4SystemOfUnits.hh"

#include "G4EmExtraPhysics.hh"
#include "G4EmLivermorePhysics.hh"
#include "G4EmPenelopePhysics.hh"
#include "G4EmProcessOptions.hh"
#include "G4EmSaturation.hh"
#include "G4EmStandardPhysics.hh"
#include "G4EmStandardPhysics_option4.hh"

#include "G4HadronicProcessStore.hh"
#include "G4HadronElasticPhysics.hh"
#include "G4HadronElasticPhysicsHP.hh"
#include "G4HadronElasticPhysicsXS.hh"
#include "G4HadronPhysicsFTFP_BERT.hh"
#include "G4HadronPhysicsQGSP_BERT.hh"
#include "G4HadronPhysicsQGSP_FTFP_BERT.hh"

#include "G4Decay.hh"
#include "G4DecayPhysics.hh"
#include "G4DecayTable.hh"
#include "G4DecayWithSpin.hh"

#include "G4eIonisation.hh"
#include "G4eBremsstrahlung.hh"
#include "G4eplusAnnihilation.hh"

#include "G4ParticleTypes.hh"
#include "G4ParticleTable.hh"
#include "G4ParticlePropertyTable.hh"

#include "G4NeutronTrackingCut.hh"
#include "G4NeutronCrossSectionXS.hh"

#include "G4MuonDecayChannelWithSpin.hh"
#include "G4MuonRadiativeDecayChannelWithSpin.hh"

#include "G4ChargeExchangePhysics.hh"
#include "G4StoppingPhysics.hh"
#include "G4IonPhysics.hh"

#include "G4RunManager.hh"
#include "G4LossTableManager.hh"
#include "G4ProcessManager.hh"

#include "G4ProductionCuts.hh"
#include "G4FastSimulationManagerProcess.hh"
#include "G4ProcessTable.hh"
#include "G4PhaseSpaceDecayChannel.hh"
#include "G4VMultipleScattering.hh"

// Optical processes
#include "G4OpticalPhysics.hh"
#include "G4Cerenkov.hh"
#include "G4Scintillation.hh"
#include "G4OpAbsorption.hh"
#include "G4OpRayleigh.hh"
#include "G4OpBoundaryProcess.hh"
#include "G4OpWLS.hh"

#include "PhysicsListMessenger.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PhysicsList* PhysicsList::fInstance = 0;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PhysicsList* PhysicsList::GetInstance()
{
	if(!fInstance) fInstance = new PhysicsList();
	return fInstance;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PhysicsList::PhysicsList() : G4VModularPhysicsList(), fCerenkovProcess(nullptr),
	fScintillationProcess(nullptr), fAbsorptionProcess(nullptr),
	fRayleighScatteringProcess(nullptr), fBoundaryProcess(nullptr),
	fWLSProcess(nullptr), fEmPhysicsList(nullptr), fParticleList(nullptr),
	fMessenger(nullptr), fBrPie2(0.0)
{
	G4LossTableManager::Instance();
	defaultCutValue = 0.7 * mm;
	fCutForGamma = defaultCutValue;
	fCutForElectron = defaultCutValue;
	fCutForPositron = defaultCutValue;
	fCutForProton = defaultCutValue;
	verboseLevel = 0;

	// Particles
	fParticleList = new G4DecayPhysics("decays");

	// EM physics
	fEmPhysicsList = new G4EmStandardPhysics();

	// HNL mode
	fMDS = 1968.47;

	// Add hadronic and optical photon physics list
	AddPhysicsList("FTFP_BERT_EMZ");

	fMessenger = new PhysicsListMessenger();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PhysicsList::~PhysicsList()
{
	delete fParticleList;
	delete fEmPhysicsList;
	for(size_t i = 0; i < fHadronPhys.size(); i++) delete fHadronPhys[i];
	delete fMessenger;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PhysicsList::SetMessengerParam()
{
	fMessenger = PhysicsListMessenger::GetInstance();
	if(!fMessenger)
	{
		G4cout << "[PhysicsList::SetMessengerParam] ERORR: no messenger available!!! Exiting!" << G4endl;
		return;
	}

	// ConstructParticle();
	fPhysicsListName = fMessenger->GetPhysicsList();
	AddPhysicsList(fPhysicsListName);

	if(fMessenger->GetListPhysList()) this->List();

	if(fMessenger->GetCutForGamma() > 0.0) fCutForGamma = fMessenger->GetCutForGamma();
	if(fMessenger->GetCutForElectron() > 0.0) fCutForElectron = fMessenger->GetCutForElectron();
	if(fMessenger->GetCutForPositron() > 0.0) fCutForPositron = fMessenger->GetCutForPositron();
	if(fMessenger->GetCutForProton() > 0.0) fCutForProton = fMessenger->GetCutForProton();

	verboseLevel = 1;
	fBrPie2 = fMessenger->GetBrPie2();

	if(fBrPie2 > 0.0) SetBrPie2(fBrPie2);
	if(fMessenger->GetMuonDecay() > 0)
	{
		G4cout << "[PhysicsList::SetMessengerParam] SetMuonDecay " << fMessenger->GetMuonDecay() << G4endl;
		SetMuonDecay(fMessenger->GetMuonDecay());
	}
	if(fMessenger->GetAddParameterisation())
	{
		G4RunManager::GetRunManager()->GeometryHasBeenModified();
		AddParameterisation();
		G4RunManager::GetRunManager()->PhysicsHasBeenModified();
	}
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PhysicsList::ConstructParticle()
{
	// Standard Geant4 particles
	fParticleList->ConstructParticle();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

// Set BR(pi->enu) and BR(pi-->munu)
void PhysicsList::SetBrPie2(G4double val)
{
	G4double BrPie2 = val;
	G4double BrPimu2 = 1.0 - BrPie2;
	fBrPie2 = BrPie2;

	if(BrPie2 < 0.0 || BrPie2 > 1.0)
	{
		G4cout << "[PhysicsList::SetBrPie2] ERROR: invalid Br(pi-->enu) = " << BrPie2 << G4endl;
		return;
	}

	G4DecayTable* PionPlusDecayTable = new G4DecayTable();
	PionPlusDecayTable->Insert(new G4PhaseSpaceDecayChannel("pi+", BrPimu2, 2, "mu+", "nu_mu"));
	PionPlusDecayTable->Insert(new G4PhaseSpaceDecayChannel("pi+", BrPie2, 2, "e+", "nu_e"));
	G4PionPlus::PionPlusDefinition()->SetDecayTable(PionPlusDecayTable);

	G4DecayTable* PionMinusDecayTable = new G4DecayTable();
	PionMinusDecayTable->Insert(new G4PhaseSpaceDecayChannel("pi-", BrPimu2, 2, "mu-", "anti_nu_mu"));
	PionMinusDecayTable->Insert(new G4PhaseSpaceDecayChannel("pi-", BrPie2, 2, "e-", "anti_nu_e"));
	G4PionMinus::PionMinusDefinition()->SetDecayTable(PionMinusDecayTable);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

// Polarized muon decay tables
void PhysicsList::SetMuonDecay(G4int MuonDecayMode)
{
	if(MuonDecayMode < 0 || MuonDecayMode > 2)
	{
		G4cout << "[PhysicsList::SetMuonDecay] ERROR: invalid MuonDecayMode = " << MuonDecayMode << G4endl;
		return;
	}
	// G4cout << "[PhysicsList::SetMuonDecay] Setting MuonDecayMode = " << MuonDecayMode << G4endl;

	G4DecayTable* MuonPlusDecayTable = new G4DecayTable();
	G4DecayTable* MuonMinusDecayTable = new G4DecayTable();
	if(MuonDecayMode == 0) // Geant4 mu decay (polarization + 1st order radiative corrections)
	{
		MuonPlusDecayTable->Insert(new G4MuonDecayChannelWithSpin("mu+", 1.0));
		MuonMinusDecayTable->Insert(new G4MuonDecayChannelWithSpin("mu-", 1.0));
	}
	else if(MuonDecayMode == 1) // Geant4 mu decay (polarization + 1st order radiative corrections + radiative mu->enunug decay, Eg > 0.5 MeV)
	{
		// The weights are such that the ratio of radiative muon decays with Eg > 10 MeV is 0.014, in agreement with the experimental results
		MuonPlusDecayTable->Insert(new G4MuonDecayChannelWithSpin("mu+", 0.932));
		MuonMinusDecayTable->Insert(new G4MuonDecayChannelWithSpin("mu-", 0.932));
	}
	G4MuonPlus::MuonPlusDefinition()->SetDecayTable(MuonPlusDecayTable);
	G4MuonMinus::MuonMinusDefinition()->SetDecayTable(MuonMinusDecayTable);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PhysicsList::ConstructProcess()
{
	AddTransportation();
	fEmPhysicsList->ConstructProcess();
	OpticalPhysics();
	fParticleList->ConstructProcess();
	for(size_t i = 0; i < fHadronPhys.size(); i++) fHadronPhys[i]->ConstructProcess();

	G4HadronicProcessStore::Instance()->SetVerbose(0);
	// G4HadronicProcessStore::Instance()->Dump(1);

	// Muon decay with spin
	G4DecayWithSpin* decayWithSpin = new G4DecayWithSpin();
	G4ProcessTable* processTable = G4ProcessTable::GetProcessTable();
	G4VProcess* Decay = processTable->FindProcess("Decay", G4MuonPlus::MuonPlus());
	G4ProcessManager* Manager = G4MuonPlus::MuonPlus()->GetProcessManager();

	if(Manager)
	{
		if(Decay) Manager->RemoveProcess(Decay);
		Manager->AddProcess(decayWithSpin);
		Manager->SetProcessOrdering(decayWithSpin, idxPostStep);
		Manager->SetProcessOrdering(decayWithSpin, idxAtRest);
	}

	Decay = processTable->FindProcess("Decay", G4MuonMinus::MuonMinus());
	Manager = G4MuonMinus::MuonMinus()->GetProcessManager();

	if(Manager)
	{
		if(Decay) Manager->RemoveProcess(Decay);
		Manager->AddProcess(decayWithSpin);
		Manager->SetProcessOrdering(decayWithSpin, idxPostStep);
		Manager->SetProcessOrdering(decayWithSpin, idxAtRest);
	}
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PhysicsList::OpticalPhysics()
{
	fCerenkovProcess = new G4Cerenkov("Cerenkov");
	fScintillationProcess = new G4Scintillation("Scintillation");
	fAbsorptionProcess = new G4OpAbsorption();
	fRayleighScatteringProcess = new G4OpRayleigh();
	fBoundaryProcess = new G4OpBoundaryProcess();
	fWLSProcess = new G4OpWLS();

	// fCerenkovProcess->DumpPhysicsTable();
	// fScintillationProcess->DumpPhysicsTable();
	// fAbsorptionProcess->DumpPhysicsTable();
	// fRayleighScatteringProcess->DumpPhysicsTable();
	// SetVerbose(1);
	// fBoundaryProcess->SetVerboseLevel(1);

	fCerenkovProcess->SetMaxNumPhotonsPerStep(30);
	fCerenkovProcess->SetTrackSecondariesFirst(false);

	fScintillationProcess->SetScintillationYieldFactor(1.0);
	fScintillationProcess->SetTrackSecondariesFirst(false);

	// Use Birks correction in the scintillation process
	G4EmSaturation* emSaturation = G4LossTableManager::Instance()->EmSaturation();
	fScintillationProcess->AddSaturation(emSaturation);

#ifndef G4SLC6
	auto theParticleIterator = GetParticleIterator();
#endif
	theParticleIterator->reset();
	while((*theParticleIterator)())
	{
		G4ParticleDefinition* particle = theParticleIterator->value();
		G4ProcessManager* pmanager = particle->GetProcessManager();
		G4String particleName = particle->GetParticleName();
		if(particleName == "opticalphoton")
		{
			G4cout << "[PhysicsList::OpticalPhysics] Add discrete process to opticalphoton " << G4endl;
			pmanager->AddDiscreteProcess(fAbsorptionProcess);
			// pmanager->AddDiscreteProcess(fRayleighScatteringProcess);
			pmanager->AddDiscreteProcess(fBoundaryProcess);
			pmanager->AddDiscreteProcess(fWLSProcess);
		}
		if(particleName == "e+")
		{
			// pmanager->AddProcess(new G4MultipleScattering, -1, 1, 1); // DOESN'T WORK BUT IDK HOW TO FIX
			pmanager->AddProcess(new G4eIonisation, -1, 2, 2);
			pmanager->AddProcess(new G4eBremsstrahlung, -1, 3, 3);
			pmanager->AddProcess(new G4eplusAnnihilation, 0, -1, 4);
		}
		if(fScintillationProcess->IsApplicable(*particle))
		{
			pmanager->AddProcess(fScintillationProcess);
			pmanager->SetProcessOrderingToLast(fScintillationProcess, idxAtRest);
			pmanager->SetProcessOrderingToLast(fScintillationProcess, idxPostStep);
		}
		if(fCerenkovProcess->IsApplicable(*particle))
		{
			pmanager->AddProcess(fCerenkovProcess);
			pmanager->SetProcessOrdering(fCerenkovProcess, idxPostStep);
		}
	}
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....

void PhysicsList::AddPhysicsList(const G4String& name)
{
	G4cout << "[PhysicsList::AddPhysicsList] Adding list " << name << G4endl;
	if(name == "FTFP_BERT")
	{
		SetBuilderList1();
		fHadronPhys.push_back(new G4HadronPhysicsFTFP_BERT());
	}
	else if(name == "QGSP_BERT")
	{
		SetBuilderList1();
		fHadronPhys.push_back(new G4HadronPhysicsQGSP_BERT());
	}
	else if(name == "QGSP_FTFP_BERT")
	{
		SetBuilderList1();
		fHadronPhys.push_back(new G4HadronPhysicsQGSP_FTFP_BERT());
	}
	else if(name == "FTFP_BERT_EMZ")
	{
		delete fEmPhysicsList;
		fEmPhysicsList = new G4EmStandardPhysics_option4();
		AddPhysicsList("FTFP_BERT");
	}
	else if(name == "QGSP_BERT_EMZ")
	{
		delete fEmPhysicsList;
		fEmPhysicsList = new G4EmStandardPhysics_option4();
		AddPhysicsList("QGSP_BERT");
	}
	else if(name == "QGSP_FTFP_BERT_EMZ")
	{
		delete fEmPhysicsList;
		fEmPhysicsList = new G4EmStandardPhysics_option4();
		AddPhysicsList("QGSP_FTFP_BERT");
	}
	else
	{
		G4cout << "[PhysicsList::AddPhysicsList] Invalid list " << name << G4endl;
		return;
	}
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PhysicsList::SetBuilderList1(G4bool flagHP)
{
	fHadronPhys.push_back(new G4EmExtraPhysics(verboseLevel));
	if(flagHP) fHadronPhys.push_back(new G4HadronElasticPhysicsHP(verboseLevel));
	else fHadronPhys.push_back(new G4HadronElasticPhysics(verboseLevel));
	fHadronPhys.push_back(new G4StoppingPhysics(verboseLevel));
	fHadronPhys.push_back(new G4IonPhysics(verboseLevel));
	fHadronPhys.push_back(new G4NeutronTrackingCut(verboseLevel));
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PhysicsList::SetBuilderList2()
{
	fHadronPhys.push_back(new G4EmExtraPhysics(verboseLevel));
	fHadronPhys.push_back(new G4HadronElasticPhysicsXS(verboseLevel));
	fHadronPhys.push_back(new G4StoppingPhysics(verboseLevel));
	fHadronPhys.push_back(new G4IonPhysics(verboseLevel));
	fHadronPhys.push_back(new G4NeutronTrackingCut(verboseLevel));
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PhysicsList::SetCuts()
{
	if(verboseLevel > 0)
	{
		G4cout << "[PhysicsList::SetCuts] CutLength " << G4BestUnit(fCutForGamma, "Length") << " for gamma" << G4endl;
		G4cout << "[PhysicsList::SetCuts] CutLength " << G4BestUnit(fCutForElectron, "Length") << " for e-" << G4endl;
		G4cout << "[PhysicsList::SetCuts] CutLength " << G4BestUnit(fCutForPositron, "Length") << " for e+" << G4endl;
		G4cout << "[PhysicsList::SetCuts] CutLength " << G4BestUnit(fCutForProton, "Length") << " for proton" << G4endl;
	}

	// Set cut values for gamma at first and for e- second and next for e+
	// because some processes for e+/e- need cut values for gamma
	SetCutValue(fCutForGamma, "gamma");
	SetCutValue(fCutForElectron, "e-");
	SetCutValue(fCutForPositron, "e+");
	SetCutValue(fCutForProton, "proton");

	// G4ProductionCutsTable::GetProductionCutsTable()->SetEnergyRange(100.0 * MeV, 200.0 * GeV);
	// if(verboseLevel > 0) DumpCutValuesTable();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PhysicsList::SetCutForGamma(G4double cut)
{
	fCutForGamma = cut;
	SetParticleCuts(fCutForGamma, G4Gamma::Gamma());
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PhysicsList::SetCutForElectron(G4double cut)
{
	fCutForElectron = cut;
	SetParticleCuts(fCutForElectron, G4Electron::Electron());
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PhysicsList::SetCutForPositron(G4double cut)
{
	fCutForPositron = cut;
	SetParticleCuts(fCutForPositron, G4Positron::Positron());
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PhysicsList::SetCutForProton(G4double cut)
{
	fCutForProton = cut;
	SetParticleCuts(fCutForProton, G4Proton::Proton());
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PhysicsList::AddParameterisation()
{
	G4FastSimulationManagerProcess* fastSimProcess_massGeom = new G4FastSimulationManagerProcess("G4FSMP_massGeom", "World");

#ifndef G4SLC6
	auto theParticleIterator=GetParticleIterator();
#endif

	theParticleIterator->reset();
	while((*theParticleIterator)())
	{
		G4ParticleDefinition* particle = theParticleIterator->value();
		G4ProcessManager* pmanager = particle->GetProcessManager();
		if(particle->GetParticleName() == "e+" || particle->GetParticleName() == "e-")
		{
			pmanager->AddDiscreteProcess(fastSimProcess_massGeom);
			pmanager->SetProcessOrdering(fastSimProcess_massGeom, idxAlongStep, 10);
			pmanager->SetProcessOrdering(fastSimProcess_massGeom, idxPostStep, 10);
		}
	}
}
