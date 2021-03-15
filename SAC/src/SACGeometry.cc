// SACGeometry.cc
// --------------------------------------------------------------
// History:
//
// Created by Emanuele Leonardi (emanuele.leonardi@roma1.infn.it) 2015-12-11
// Adapted from PADME experiment by Emily Tsai (emily.tsai11@gmail.com) 2020-7-8
// --------------------------------------------------------------

#include "SACGeometry.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SACGeometry* SACGeometry::fInstance = nullptr;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SACGeometry* SACGeometry::GetInstance()
{
	if(!fInstance) fInstance = new SACGeometry();
	return fInstance;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SACGeometry::SACGeometry()
{
	// Size of PbF2 crystal
	fCrystalSizeX = 2.0 * cm;
	fCrystalSizeY = 2.0 * cm;
	fCrystalSizeZ = 3.75 * cm;

	// Thickness of paint coating around each crystal
	fCrystalCoating = 100.0 * um;

	// Size of SAC cell (crystal + coating)
	fCellSizeX = fCrystalSizeX + 2.0 * fCrystalCoating;
	fCellSizeY = fCrystalSizeY + 2.0 * fCrystalCoating;
	fCellSizeZ = fCrystalSizeZ + fCrystalCoating;

	// Size of gap between cells
	fCellGap = 50.0 * um;

	// Size of gap between layers
	fLayerGap = 2.0 * cm;

	// Number of rows, columns, layers of cells in SAC
	fSACNRows = 10;
	fSACNCols = 10;
	fSACNLayers = 4;

	// Size of SAC box
	fSACSizeX = (fCellSizeX + fCellGap) * fSACNCols - fCellGap;
	fSACSizeY = (fCellSizeY + fCellGap) * fSACNRows - fCellGap;
	fSACSizeZ = (fCellSizeZ + fLayerGap) * fSACNLayers - fLayerGap;

	// Position of center of SAC box
	fSACPosX = 1.0 * cm;
	fSACPosY = 1.0 * cm;
	fSACPosZ = 0.0 * cm; // debug

	// PMT parameters
	fEnablePMT = 0;
	fPMTDiameter = 8.0 * mm;
	fPMTThickness = 1.0 * mm;
	fPMTRound = 2.0 * M_PI * rad;

	// SiPM parameters (square)
	fEnableSiPM = 0;
	fSiPMSize = 7.08 * mm;
	fSiPMThickness = 0.3 * mm;

	// Digitization parameters
	// Average light speed inside SAC crystal for Cherenkov spectrum
	fDigiAvgLightSpeed = (2.998E8 * m/s) / 1.85;
	// Number of photoelectrons produced by photocathode per MeV of hit energy
	fDigiEtoNPEConversion = 0.5 / MeV; // wild guess: fix it!!!
	// Contribution of 1 p.e. to integral ADC signal
	fDigiPEtoSignalConversion = 1.0;
	// Relative collection efficiency as function of Z along the crystal (bin 0: front face, bin N: readout face)
	static const G4double cmap[] = {1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0};
	G4int nbins = 20;
	fDigiPECollectionMap.assign(cmap, cmap + nbins);
	// Hamamatsu R13478 PMT transit time from photocathode to anode
	fDigiPMTTransitTime = 9.1 * ns;
	// Delay due to connection cables
	fDigiPMTCableDelay = 0.0 * ns;

	// SAC SD name
	fSACSensitiveDetectorName = "SACSD";

	// Verbose level
	fVerbose = 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SACGeometry::~SACGeometry() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

std::vector<G4String> SACGeometry::GetHashTable()
{
	std::vector<G4String> hash;
	std::ostringstream buffer;

	buffer << "fCrystalSizeX " << fCrystalSizeX;
	hash.push_back(buffer.str());
	buffer.str("");

	buffer << "fCrystalSizeY " << fCrystalSizeY;
	hash.push_back(buffer.str());
	buffer.str("");

	buffer << "fCrystalSizeZ " << fCrystalSizeZ;
	hash.push_back(buffer.str());
	buffer.str("");

	buffer << "fCrystalCoating " << fCrystalCoating;
	hash.push_back(buffer.str());
	buffer.str("");

	buffer << "fCellGap " << fCellGap;
	hash.push_back(buffer.str());
	buffer.str("");

	buffer << "fLayerGap " << fLayerGap;
	hash.push_back(buffer.str());
	buffer.str("");

	buffer << "fSACNRows " << fSACNRows;
	hash.push_back(buffer.str());
	buffer.str("");

	buffer << "fSACNCols " << fSACNCols;
	hash.push_back(buffer.str());
	buffer.str("");

	buffer << "fSACNLayers " << fSACNLayers;
	hash.push_back(buffer.str());
	buffer.str("");

	buffer << "fDigiEtoNPEConversion " << fDigiEtoNPEConversion;
	hash.push_back(buffer.str());
	buffer.str("");

	buffer << "fDigiPEtoSignalConversion " << fDigiPEtoSignalConversion;
	hash.push_back(buffer.str());
	buffer.str("");

	buffer << "fDigiPECollectionMap ";
	for(G4int i = 0; i < (G4int) fDigiPECollectionMap.size(); i++) buffer << " " << fDigiPECollectionMap[i];
	hash.push_back(buffer.str());
	buffer.str("");

	buffer << "fDigiPMTTransitTime " << fDigiPMTTransitTime;
	hash.push_back(buffer.str());
	buffer.str("");

	buffer << "fDigiPMTCableDelay " << fDigiPMTCableDelay;
	hash.push_back(buffer.str());
	buffer.str("");

	return hash;
}
