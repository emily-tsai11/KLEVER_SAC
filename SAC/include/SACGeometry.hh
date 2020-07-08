// SACGeometry.hh
// --------------------------------------------------------------
// History:
//
// Created by Emanuele Leonardi (emanuele.leonardi@roma1.infn.it) 2015-12-14
// Adapted from PADME experiment by Emily Tsai (emily.tsai11@gmail.com) 2020-7-8
// --------------------------------------------------------------

#ifndef SACGeometry_H
#define SACGeometry_H 1

#include "globals.hh"
#include "G4SystemOfUnits.hh"

#include <vector>

#define SACGEOMETRY_N_ROWS_MAX 10
#define SACGEOMETRY_N_COLS_MAX 10

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class G4LogicalVolume;

class SACGeometry
{

	public:

		~SACGeometry();
		static SACGeometry* GetInstance();
		std::vector<G4String> GetHashTable();

	private:

		static SACGeometry* fInstance;

	protected:

		SACGeometry();

	public:

		void SetVerboseLevel(G4int v) { fVerbose = v; }
		G4int GetVerboseLevel() { return fVerbose; }

		// position of center of SAC box
		G4double GetSACPosX() { return 0.0 * cm; }
		G4double GetSACPosY() { return 0.0 * cm; }
		G4double GetSACPosZ() { return fSACFrontFacePosZ + (GetCellSizeZ() * fSACNLayers) * 0.5; }

		// size of SAC box
		G4double GetSACSizeX() { return (GetCellSizeX() + fCrystalGap) * fSACNCols; }
		G4double GetSACSizeY() { return (GetCellSizeY() + fCrystalGap) * fSACNRows; }
		G4double GetSACSizeZ() { return (GetCellSizeZ() + fCrystalGap) * fSACNLayers; }

		// number of rows and columns of crystals in SAC
		G4int GetSACNRows() { return fSACNRows; }
		G4int GetSACNCols() { return fSACNCols; }
		G4int GetSACNLayers() { return fSACNLayers; }

		void  SetSACNRows(G4int r) { fSACNRows = r; }
		void  SetSACNCols(G4int c) { fSACNCols = c; }
		void  SetSACNLayers(G4int l) { fSACNLayers = l; }

		// check if crystal exists at given row/column
		G4int ExistsCrystalAt(G4int, G4int);

		// position of center of crystal at given row/column
		G4double GetCrystalPosX(G4int, G4int);
		G4double GetCrystalPosY(G4int, G4int);
		G4double GetCrystalPosZ(G4int, G4int);

		// size of SAC PbF2 crystal
		G4double GetCrystalSizeX() { return fCrystalSizeX; }
		G4double GetCrystalSizeY() { return fCrystalSizeY; }
		G4double GetCrystalSizeZ() { return fCrystalSizeZ; }

		// set size of BGO crystal
		void SetCrystalSizeX(G4double s) { fCrystalSizeX = s; }
		void SetCrystalSizeY(G4double s) { fCrystalSizeY = s; }
		void SetCrystalSizeZ(G4double s) { fCrystalSizeZ = s; }

		// size of SAC cell (crystal + coating)
		G4double GetCellSizeX() { return fCrystalSizeX + 2.0 * fCrystalCoating; }
		G4double GetCellSizeY() { return fCrystalSizeY + 2.0 * fCrystalCoating; }
		G4double GetCellSizeZ() { return fCrystalSizeZ + 2.0 * fCrystalCoating; }

		// size of gap between crystals
		G4double GetCrystalGap() { return fCrystalGap; }
		void SetCrystalGap(G4double g) { fCrystalGap = g; }

		// thickness of paint coating around each crystal
		G4double GetCrystalCoating() { return fCrystalCoating; }
		void SetCrystalCoating(G4double c) { fCrystalCoating = c; }

		// set position along Z of SAC front face
		G4double GetSACFrontFacePosZ() { return fSACFrontFacePosZ; }
		void SetSACFrontFacePosZ(G4double z) { fSACFrontFacePosZ = z; }

		// digitization parameters
		G4double GetSACLightPropagationSpeed() { return fDigiAvgLightSpeed; }
		G4double GetDigiEtoNPEConversion() { return fDigiEtoNPEConversion; }
		G4double GetDigiPEtoSignalConversion() { return fDigiPEtoSignalConversion; }

		std::vector<G4double> GetDigiPECollectionMap() { return fDigiPECollectionMap; }
		G4int GetDigiPECollectionMapNBins() { return fDigiPECollectionMap.size(); }
		G4double  GetDigiPECollectionMapBinLength() { return fCrystalSizeZ / fDigiPECollectionMap.size(); }

		G4double GetDigiPMTTransitTime() { return fDigiPMTTransitTime; }
		G4double GetDigiPMTCableDelay() { return fDigiPMTCableDelay; }

		// get name of SAC sensitive detector
		G4String GetSACSensitiveDetectorName() { return fSACSensitiveDetectorName; }

	private:

		// geometry parameters
		G4int fVerbose; // verbose level

		G4double fCrystalSizeX;
		G4double fCrystalSizeY;
		G4double fCrystalSizeZ;

		G4int fSACNRows;
		G4int fSACNCols;
		G4int fSACNLayers;

		G4double fCrystalGap; // air gap size between adjacent crystals

		G4double fCrystalCoating; // thickness of coating around crystals

		G4double fSACFrontFacePosZ; // position along Z axis of SAC front face

		// digitization parameters
		G4double fDigiAvgLightSpeed; // average light speed inside SAC crystal for Cherenkov spectrum

		G4double fDigiEtoNPEConversion; // number of p.e. produced by photocathode per MeV of hit energy
		G4double fDigiPEtoSignalConversion; // contribution of 1 p.e. to integral ADC signal

		std::vector<G4double> fDigiPECollectionMap; // relative collection efficiency along crystal

		G4double fDigiPMTTransitTime; // PMT transit time from photocathode to anode
		G4double fDigiPMTCableDelay; // delay due to connection cables

		// other parameters
		G4String fSACSensitiveDetectorName;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
