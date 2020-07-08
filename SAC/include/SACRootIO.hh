// SACRootIO.hh
// --------------------------------------------------------------
// History:
//
// Created by Emanuele Leonardi (emanuele.leonardi@roma1.infn.it) 2016-03-17
// Adapted from PADME experiment by Emily Tsai (emily.tsai11@gmail.com) 2020-7-8
// --------------------------------------------------------------

#ifndef SACRootIO_H
#define SACRootIO_H 1

#include "MCVRootIO.hh"

#include "TTree.h"
#include "TBranch.h"

#include "globals.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class TSACMCEvent;
class SACGeometry;
class TDetectorInfo;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class SACRootIO : public MCVRootIO
{
	public:

		SACRootIO();
		virtual ~SACRootIO();

		static SACRootIO* GetInstance();

		void NewRun(G4int, TDetectorInfo*);
		// void NewRun(G4int, TFile*);
		void EndRun();
		void SaveEvent(const G4Event*);
		void Close();

		// z-position where beam has t = 0
		// void SetBeamStartZ(Double_t z) { fEHistoBeamStartZ = z; }

		// time length of e+ bunch (40.0ns for the time being)
		void SetBeamBunchLengthT(Double_t t)
		{
			if (t > 0.0)
				fEHistoBeamBunchLengthT = t;
		}

	private:

		TTree* fEventTree;
		TBranch* fSACBranch;
		TSACMCEvent* fEvent;

		// parameters used for energy distribution histogram
		// Double_t fEHistoBeamStartZ;
		// Double_t fEHistoBeamBunchLengthT;

		// Int_t fEHistoNBins;
		// Double_t fEHistoTStart;
		// Double_t fEHistoTStep;
		// Double_t fEHistoTEnd;

		SACGeometry* fGeoPars;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif // SACRootIO_H
