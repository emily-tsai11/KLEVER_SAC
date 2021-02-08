// RandomGenerator.cc
// --------------------------------------------------------------
// History:
//
// Taken from KLMC -- Emily Tsai (emily.tsai11@gmail.com) 2021-1-19
// --------------------------------------------------------------

#include "RandomGenerator.hh"
#include "Randomize.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RandomGenerator* RandomGenerator::fInstance = 0;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RandomGenerator* RandomGenerator::GetInstance()
{
	if(!fInstance) fInstance = new RandomGenerator();
	return fInstance;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RandomGenerator::RandomGenerator()
{
	CLHEP::HepRandom::setTheEngine(new CLHEP::RanecuEngine());
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RandomGenerator::~RandomGenerator() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RandomGenerator::SetSeed(long theSeed)
{
	fSeed = theSeed;
	CLHEP::HepRandom::setTheSeed(fSeed);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RandomGenerator::ShowStatus()
{
	CLHEP::HepRandom::showEngineStatus();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RandomGenerator::SaveStatus()
{
	CLHEP::HepRandom::getTheEngine()->saveStatus();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

double RandomGenerator::GetUniform()
{
	// Returns a pseudo random number between 0 and 1 (excluding end points)
	return G4UniformRand();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

double RandomGenerator::GetGauss(double m, double std)
{
	return G4RandGauss::shoot(m, std);
}
