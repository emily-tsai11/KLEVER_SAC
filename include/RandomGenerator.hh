// RandomGenerator.hh
// --------------------------------------------------------------
// History:
//
// Taken from KLMC -- Emily Tsai (emily.tsai11@gmail.com) 2021-1-19
// --------------------------------------------------------------

#ifndef RandomGenerator_H
#define RandomGenerator_H 1

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class RandomGenerator
{
	public:

		~RandomGenerator();
		static RandomGenerator* GetInstance();

		void SetSeed(long theSeed);
		long GetSeed() { return fSeed; }

		void ShowStatus();
		void SaveStatus();

		double GetUniform();
		double GetGauss(double m, double std);

	protected:

		RandomGenerator();

	private:

		static RandomGenerator* fInstance;
		long fSeed;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
