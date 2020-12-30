// DetectorConstruction.hh
// --------------------------------------------------------------
// History:
//
// Created by Emily Tsai (emily.tsai11@gmail.com) 2020-7-8
// --------------------------------------------------------------

#ifndef DetectorConstruction_H
#define DetectorConstruction_H 1

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class G4LogicalVolume;
class G4VPhysicalVolume;
class G4VSolid;
class G4Box;

class SACDetector;
class PMTDetector;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class DetectorConstruction : public G4VUserDetectorConstruction
{
	public:

		DetectorConstruction();
		~DetectorConstruction();

		G4VPhysicalVolume* Construct();

		// G4VSolid* GetSolidWorld() { return solidWorld; };
	    // G4LogicalVolume* GetLogicalWorld() { return logicalWorld; };
	    // G4VPhysicalVolume* GetPhysicalWorld() { return physicalWorld; };

		// void SetSolidWorld(G4VSolid* v) { solidWorld = v; };
		// void SetLogicalWorld(G4LogicalVolume* v) { logicalWorld = v; };
	    // void SetPhysicalWorld(G4VPhysicalVolume* v) { physicalWorld = v; };

	    G4double GetWorldXLength() { return fWorldXLength; };
	    G4double GetWorldYLength() { return fWorldYLength; };
		G4double GetWorldZLength() { return fWorldZLength; };

		// void SetWorldXLength(G4double v) { fWorldXLength = v; };
		// void SetWorldYLength(G4double v) { fWorldYLength = v; };
		// void SetWorldZLength(G4double v) { fWorldZLength = v; };

	private:

		G4VSolid* solidWorld;
	    G4LogicalVolume* logicalWorld;
	    G4VPhysicalVolume* physicalWorld;

		G4double fWorldZLength;
	    G4double fWorldXLength;
	    G4double fWorldYLength;

		SACDetector* fSAC;
		G4LogicalVolume* fSACNonRefCellVolume;

		PMTDetector* fPMT;
		G4LogicalVolume* fPMTCellVolume;

		void DefineMaterials();
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
