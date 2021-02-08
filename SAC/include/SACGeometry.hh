// SACGeometry.hh
// --------------------------------------------------------------
// History:
//
// Created by Emanuele Leonardi (emanuele.leonardi@roma1.infn.it) 2015-12-14
// Adapted from PADME experiment by Emily Tsai (emily.tsai11@gmail.com) 2020-7-8
// --------------------------------------------------------------

#ifndef SACGeometry_H
#define SACGeometry_H 1

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "globals.hh"
#include <vector>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class G4LogicalVolume;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class SACGeometry
{
	public:

		~SACGeometry();
		static SACGeometry* GetInstance();
		std::vector<G4String> GetHashTable();

		// Size of PbF2 crystal
		void SetCrystalSizeX(G4double SizeX) { fCrystalSizeX = SizeX; }
		G4double GetCrystalSizeX() { return fCrystalSizeX; }
		void SetCrystalSizeY(G4double SizeY) { fCrystalSizeY = SizeY; }
		G4double GetCrystalSizeY() { return fCrystalSizeY; }
		void SetCrystalSizeZ(G4double SizeZ) { fCrystalSizeZ = SizeZ; }
		G4double GetCrystalSizeZ() { return fCrystalSizeZ; }

		// Thickness of paint coating around each crystal
		void SetCrystalCoating(G4double c) { fCrystalCoating = c; }
		G4double GetCrystalCoating() { return fCrystalCoating; }

		// Size of SAC cell (crystal + coating)
		G4double GetCellSizeX() { return fCellSizeX; }
		G4double GetCellSizeY() { return fCellSizeY; }
		G4double GetCellSizeZ() { return fCellSizeZ; }

		// Size of gap between cells
		void SetCellGap(G4double gap) { fCellGap = gap; }
		G4double GetCellGap() { return fCellGap; }

		// Size of gap between layers
		void SetLayerGap(G4double gap) { fLayerGap = gap; }
		G4double GetLayerGap() { return fLayerGap; }

		// Number of rows, columns, layers of cells in SAC
		void SetSACNRows(G4int r) { fSACNRows = r; }
		G4int GetSACNRows() { return fSACNRows; }
		void SetSACNCols(G4int c) { fSACNCols = c; }
		G4int GetSACNCols() { return fSACNCols; }
		void SetSACNLayers(G4int l) { fSACNLayers = l; }
		G4int GetSACNLayers() { return fSACNLayers; }

		// Size of SAC box
		G4double GetSACSizeX() { return fSACSizeX; }
		G4double GetSACSizeY() { return fSACSizeY; }
		G4double GetSACSizeZ() { return fSACSizeZ; }

		// Position of center of SAC box
		G4double GetSACPosX() { return fSACPosX; }
		G4double GetSACPosY() { return fSACPosY; }
		G4double GetSACPosZ() { return fSACPosZ; }

		// PMT parameters
		void SetEnablePMT(G4int b) { fEnablePMT = b; }
		G4int GetEnablePMT() { return fEnablePMT; }
		void SetPMTDiameter(G4double d) { fPMTDiameter = d; }
		G4double GetPMTDiameter() { return fPMTDiameter; }
		void SetPMTThickness(G4double t) { fPMTThickness = t; }
		G4double GetPMTThickness() { return fPMTThickness; }
		G4double GetPMTRound() { return fPMTRound; }

		// SiPM parameters (square)
		void SetEnableSiPM(G4int b) { fEnableSiPM = b; }
		G4int GetEnableSiPM() { return fEnableSiPM; }
		void SetSiPMSize(G4double d) { fSiPMSize = d; }
		G4double GetSiPMSize() { return fSiPMSize; }
		void SetSiPMThickness(G4double t) { fSiPMThickness = t; }
		G4double GetSiPMThickness() { return fSiPMThickness; }

		// Digitization parameters
		G4double GetSACLightPropagationSpeed() { return fDigiAvgLightSpeed; }
		G4double GetDigiEtoNPEConversion() { return fDigiEtoNPEConversion; }
		G4double GetDigiPEtoSignalConversion() { return fDigiPEtoSignalConversion; }
		G4int GetDigiPECollectionMapNBins() { return fDigiPECollectionMap.size(); }
		G4double GetDigiPECollectionMapBinLength() { return fCrystalSizeZ / fDigiPECollectionMap.size(); }
		std::vector<G4double> GetDigiPECollectionMap() { return fDigiPECollectionMap; }
		G4double GetDigiPMTTransitTime() { return fDigiPMTTransitTime; }
		G4double GetDigiPMTCableDelay() { return fDigiPMTCableDelay; }

		// SAC SD name
		G4String GetSACSensitiveDetectorName() { return fSACSensitiveDetectorName; }

		// Verbose level
		void SetVerboseLevel(G4int v) { fVerbose = v; }
		G4int GetVerboseLevel() { return fVerbose; }

	protected:

		SACGeometry();

	private:

		static SACGeometry* fInstance;

		// Size of PbF2 crystal
		G4double fCrystalSizeX, fCrystalSizeY, fCrystalSizeZ;

		// Thickness of paint coating around each crystal
		G4double fCrystalCoating;

		// Size of SAC cell (crystal + coating)
		G4double fCellSizeX, fCellSizeY, fCellSizeZ;

		// Size of gap between cells and layers
		G4double fCellGap, fLayerGap;

		// Number of rows, columns, layers of cells in SAC
		G4int fSACNRows, fSACNCols, fSACNLayers;

		// Size of SAC box
		G4double fSACSizeX, fSACSizeY, fSACSizeZ;

		// Position of center of SAC box
		G4double fSACPosX, fSACPosY, fSACPosZ;

		// PMT parameters
		G4int fEnablePMT;
		G4double fPMTDiameter, fPMTThickness, fPMTRound;

		// SiPM parameters (square)
		G4int fEnableSiPM;
		G4double fSiPMSize, fSiPMThickness;

		// Digitization parameters
		// Average light speed inside SAC crystal for Cherenkov spectrum
		G4double fDigiAvgLightSpeed;
		// Number of p.e. produced by photocathode per MeV of hit energy
		G4double fDigiEtoNPEConversion;
		// Contribution of 1 p.e. to integral ADC signal
		G4double fDigiPEtoSignalConversion;
		// Relative collection efficiency along crystal
		std::vector<G4double> fDigiPECollectionMap;
		// PMT transit time from photocathode to anode
		G4double fDigiPMTTransitTime;
		// Delay due to connection cables
		G4double fDigiPMTCableDelay;

		// SAC SD name
		G4String fSACSensitiveDetectorName;

		// Verbose level
		G4int fVerbose;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
