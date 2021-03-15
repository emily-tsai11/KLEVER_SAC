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

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class G4LogicalVolume;
class G4VPhysicalVolume;
class G4VSolid;
class G4Box;

class DetectorConstructionMessenger;
class SACDetector;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class DetectorConstruction : public G4VUserDetectorConstruction
{
	public:

		DetectorConstruction();
		~DetectorConstruction();

		G4VPhysicalVolume* Construct();
		void UpdateGeometry();


		void SetSolidWorld(G4VSolid* v) { fSolidWorld = v; }
		G4VSolid* GetSolidWorld() { return fSolidWorld; }

		void SetLogicalWorld(G4LogicalVolume* v) { fLogicalWorld = v; }
	    G4LogicalVolume* GetLogicalWorld() { return fLogicalWorld; }

		void SetPhysicalWorld(G4VPhysicalVolume* v) { fPhysicalWorld = v; }
	    G4VPhysicalVolume* GetPhysicalWorld() { return fPhysicalWorld; }


		void SetWorldLengthX(G4double v) { fWorldLengthX = v; }
	    G4double GetWorldLengthX() { return fWorldLengthX; }

		void SetWorldLengthY(G4double v) { fWorldLengthY = v; }
	    G4double GetWorldLengthY() { return fWorldLengthY; }

		void SetWorldLengthZ(G4double v) { fWorldLengthZ = v; }
		G4double GetWorldLengthZ() { return fWorldLengthZ; }

	private:

		G4VSolid* fSolidWorld;
	    G4LogicalVolume* fLogicalWorld;
	    G4VPhysicalVolume* fPhysicalWorld;

		G4double fWorldLengthX;
	    G4double fWorldLengthY;
	    G4double fWorldLengthZ;

		SACDetector* fSACDetector;
		DetectorConstructionMessenger* fMessenger;

		void DefineMaterials();
		void CleanGeometry();
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
