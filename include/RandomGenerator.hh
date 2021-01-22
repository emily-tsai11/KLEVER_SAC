// RandomGenerator.hh
// --------------------------------------------------------------
// History:
//
// Taken from KLMC -- Emily Tsai (emily.tsai11@gmail.com) 2021-1-19
// --------------------------------------------------------------

#ifndef RandomGenerator_H
#define RandomGenerator_H 1

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "TROOT.h"
#include "TRandom3.h"
#include "TH2D.h"
#include "TMath.h"

#include <map>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class RandomGenerator
{
	public:

		virtual ~RandomGenerator();
		static RandomGenerator* GetInstance();
		void Init(unsigned int);
		void Init(TRandom3* RandDecay);
		void GetRandom2(TH2D*, Double_t&, Double_t&);

		TRandom3* GetRandomDecay() { /* G4cout << "Random3Return " << GetSeedDecay() << G4endl; */ return fRandDecay; }
		unsigned int GetSeedDecay() { return fRandDecay->GetSeed(); }
		double GetRndmDecay() { /* G4cout << "RandomNumberReturn " << GetSeedDecay() << G4endl; */ return fRandDecay->Rndm(); }
		void GetArrayDecay(int n, double* numbers) { /* G4cout << "RandomArrayReturn " << GetSeedDecay() << G4endl; */ fRandDecay->RndmArray(n, numbers); }
		void WriteDecay(const char* filename) { fRandDecay->WriteRandom(filename); }
		void ReadDecay(const char* filename) { fRandDecay->ReadRandom(filename); }

	protected:

		RandomGenerator();

	private:

		static RandomGenerator* fInstance;
		TRandom3* fRandDecay;
		std::map<TH2D*, int> mapOfHistograms;
		bool fIntegralHasBeenCalculated;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
