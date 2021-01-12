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

#include "G4SystemOfUnits.hh"
#include "globals.hh"

#include <vector>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class G4LogicalVolume;

class SACGeometry
{
	public:

		~SACGeometry();
		static SACGeometry* GetInstance();
		std::vector<G4String> GetHashTable();

		// size of PbF2 crystal
		void SetCrystalSizeX(G4double SizeX) { fCrystalSizeX = SizeX; }
		G4double GetCrystalSizeX() { return fCrystalSizeX; }
		void SetCrystalSizeY(G4double SizeY) { fCrystalSizeY = SizeY; }
		G4double GetCrystalSizeY() { return fCrystalSizeY; }
		void SetCrystalSizeZ(G4double SizeZ) { fCrystalSizeZ = SizeZ; }
		G4double GetCrystalSizeZ() { return fCrystalSizeZ; }

		// thickness of paint coating around each crystal
		void SetCrystalCoating(G4double c) { fCrystalCoating = c; }
		G4double GetCrystalCoating() { return fCrystalCoating; }

		// size of SAC cell (crystal + coating)
		G4double GetCellSizeX() { return fCellSizeX; }
		G4double GetCellSizeY() { return fCellSizeY; }
		G4double GetCellSizeZ() { return fCellSizeZ; }

		// size of gap between cells
		void SetCellGap(G4double gap) { fCellGap = gap; }
		G4double GetCellGap() { return fCellGap; }

		// number of rows, columns, layers of cells in SAC
		void SetSACNRows(G4int r) { fSACNRows = r; }
		G4int GetSACNRows() { return fSACNRows; }
		void SetSACNCols(G4int c) { fSACNCols = c; }
		G4int GetSACNCols() { return fSACNCols; }
		void SetSACNLayers(G4int l) { fSACNLayers = l; }
		G4int GetSACNLayers() { return fSACNLayers; }

		// size of SAC box
		G4double GetSACSizeX() { return fSACSizeX; }
		G4double GetSACSizeY() { return fSACSizeY; }
		G4double GetSACSizeZ() { return fSACSizeZ; }

		// position of center of SAC box
		G4double GetSACPosX() { return fSACPosX; }
		G4double GetSACPosY() { return fSACPosY; }
		G4double GetSACPosZ() { return fSACPosZ; }

		// PMT parameters
		void SetEnablePMT(G4int b) { fEnablePMT = b; }
		G4bool GetEnablePMT() { return fEnablePMT; }
		void SetPMTDiameter(G4double d) { fPMTDiameter = d; }
		G4double GetPMTDiameter() { return fPMTDiameter; }
		void SetPMTThickness(G4double t) { fPMTThickness = t; }
		G4double GetPMTThickness() { return fPMTThickness; }
		G4double GetPMTRound() { return fPMTRound; }

		// SiPM parameters (square)
		void SetEnableSiPM(G4int b) { fEnableSiPM = b; }
		G4bool GetEnableSiPM() { return fEnableSiPM; }
		void SetSiPMSize(G4double d) { fSiPMSize = d; }
		G4double GetSiPMSize() { return fSiPMSize; }
		void SetSiPMThickness(G4double t) { fSiPMThickness = t; }
		G4double GetSiPMThickness() { return fSiPMThickness; }

		// --------------- DIGITIZATION PARAMETERS ---------------
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

		// verbose level
		void SetVerboseLevel(G4int v) { fVerbose = v; }
		G4int GetVerboseLevel() { return fVerbose; }

		// incident particle energy
		void SetIncidentE(G4double e) { fIncidentE = e; }
		G4double GetIncidentE() { return fIncidentE; }

		// set position along Z of SAC front face
		// void SetSACFrontFacePosZ(G4double z) { fSACFrontFacePosZ = z; }
		// G4double GetSACFrontFacePosZ() { return fSACFrontFacePosZ; }

	protected:

		SACGeometry();

	private:

		static SACGeometry* fInstance;

		// size of PbF2 crystal
		G4double fCrystalSizeX, fCrystalSizeY, fCrystalSizeZ;

		// thickness of paint coating around each crystal
		G4double fCrystalCoating;

		// size of SAC cell (crystal + coating)
		G4double fCellSizeX, fCellSizeY, fCellSizeZ;

		// size of gap between cells
		G4double fCellGap;

		// number of rows, columns, layers of cells in SAC
		G4int fSACNRows, fSACNCols, fSACNLayers;

		// size of SAC box
		G4double fSACSizeX, fSACSizeY, fSACSizeZ;

		// position of center of SAC box
		G4double fSACPosX, fSACPosY, fSACPosZ;

		// PMT parameters
		G4bool fEnablePMT;
		G4double fPMTDiameter, fPMTThickness, fPMTRound;

		// SiPM parameters (square)
		G4bool fEnableSiPM;
		G4double fSiPMSize, fSiPMThickness;

		// --------------- DIGITIZATION PARAMETERS ---------------
		// average light speed inside SAC crystal for Cherenkov spectrum
		G4double fDigiAvgLightSpeed;
		// number of p.e. produced by photocathode per MeV of hit energy
		G4double fDigiEtoNPEConversion;
		// contribution of 1 p.e. to integral ADC signal
		G4double fDigiPEtoSignalConversion;
		// relative collection efficiency along crystal
		std::vector<G4double> fDigiPECollectionMap;
		// PMT transit time from photocathode to anode
		G4double fDigiPMTTransitTime;
		// delay due to connection cables
		G4double fDigiPMTCableDelay;

		// SAC SD name
		G4String fSACSensitiveDetectorName;

		// verbose level
		G4int fVerbose;

		// incident particle energy
		G4double fIncidentE;

		// position along Z axis of SAC front face
		// G4double fSACFrontFacePosZ;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
