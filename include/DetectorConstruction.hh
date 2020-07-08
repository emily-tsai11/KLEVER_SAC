// DetectorConstruction.hh
// --------------------------------------------------------------
// History:
//
// Created by Emily Tsai (emily.tsai11@gmail.com) 2020-7-8
// --------------------------------------------------------------

#ifndef DetectorConstruction_H
#define DetectorConstruction_H 1

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class G4LogicalVolume;
class G4VPhysicalVolume;
class G4VSolid;

class SACDetector;

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

		// void SetSolidWorld(G4VSolid* value) { solidWorld = value; };
		// void SetLogicalWorld(G4LogicalVolume* value) { logicalWorld = value; };
	    // void SetPhysicalWorld(G4VPhysicalVolume* value) { physicalWorld = value; };

	    G4double GetWorldXLength() { return fWorldXLength; };
	    G4double GetWorldYLength() { return fWorldYLength; };
		G4double GetWorldZLength() { return fWorldZLength; };

		// void SetWorldXLength(G4double value) { fWorldXLength = value; };
		// void SetWorldYLength(G4double value) { fWorldYLength = value; };
		// void SetWorldZLength(G4double value) { fWorldZLength = value; };

	private:

		G4VSolid* solidWorld;
	    G4LogicalVolume* logicalWorld;
	    G4VPhysicalVolume* physicalWorld;

		G4double fWorldZLength;
	    G4double fWorldXLength;
	    G4double fWorldYLength;

		SACDetector* SAC;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
