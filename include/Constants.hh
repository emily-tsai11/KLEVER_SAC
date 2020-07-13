// Constants.hh
// --------------------------------------------------------------
// History:
//
// Adapted from Padme by Emily Tsai (emily.tsai11@gmail.com) 2020-7-13
// --------------------------------------------------------------

// GENERAL FLAGS
// const int		NPrint				= 1000;
// const bool		fAutoSeed			= true;											// random seed with clock
// const bool		fAutoSeed			= false;										// fixed random seed

// DETECTORS FLAGS
// const int		IsCalibRun			= 0;											// 1 global calo calibration 2 calo energy resolution
// const double		EMinSaveNT			= 5.0;

// DETECTORS FLAGS
// const double		WorldLength			= 12.0;											// in meters

// const int		IsBFieldON			= 1;											// 1 uniform B field 2 non uniform
// const int		IsMagIronON			= 1;											// 1 CERN magnet
// const int		IsEcalON			= 1;
// const int		IsEcalRoundON		= 1;

// const int		IsTrackerON			= 0;											// cylindrical tracker obsolete set to 0
// const int		IsPlanarGEMON		= 0;											// 1 planar GEM spectrometer
// const int		IsPVetoON			= 1;											// 1 veto inside the magnet positron side
// const int		IsEleVetoON			= 1;											// veto inside the magnet electron side
// const int		IsEVetoON			= 1;											// veto for high momentum positron
// const int		IsSACON				= 1;											// small angle gamma veto
// const int		IsLAVON				= 0;											// large angle gamma veto

// const int		IsMonitorON			= 0;											// first station for P measurement
// const int		IsTargetON			= 0;											// is off if dump is on
// const int		IsTDumpON			= 0;

// const int		IsWallON			= 0;
// const int		IsTrackerRecoON		= 0;
// const int		IsSpecON			= 0;											// not used
// const int		IsPipeON			= 0;
// const int		IsSpecInside		= -1;											// if 0 spectrometer is outside vacuum 1 is inside any other no vac chamb

// beam parameter flags
// const int		NPrimaries			= 5000;											// maximum 10^5 on the macbook was 1000
// const int		BeamSpot			= 1;											// std 1
// const int		BeamESpread			= 1;											// std 1
// const int		IsBeamTimeON		= 1;											// std 1
// const int		BeamEmittance		= 1;											// std 1
// const int		IsResGasON			= 1;											// std 1, realistic vacuum
// const int		IsResGasON			= 0;											// std 1, galactic vacuum

// beam parameters
// const double		BeamEnergy			= 550.0;										// in MeV 550 standard
// const double		SigmaBeamE			= BeamEnergy * 0.01;							// in MeV
// const double		BunchLenght			= 40.0;											// at present from 3-40 ns
// const double		MicroBunchSpace		= 0.350;										// fixed to 350 ps
// const double		MicroBunchLength	= 0.150;										// fixed to 350 ps
// const double		SigmaBeamX			= 0.7;											// in mm
// const double		SigmaBeamY			= 0.7;											// in mm
// const double		BeamDivX			= 1.0e-3;										// in radians (still to be tested)
// const double		BeamDivY			= 1.0e-3;										// in radians (still to be tested)
// const double		position			= -80.0;										// in cm
// const double		VacuumP				= 1e-6;											// is in bar
// const double		VacuumDensity		= 1.290 * VacuumP;								// mg * cm3
// const double		MagneticField		= 0.55;											// 0.6

// physics list flags
// const int		UseNewPhysList		= 1;											// was 0
const int			IsSincrotronON		= 0;											// std = 1
const int			IsNuclearON			= 1;											// std = 1
const int			NuclearBertini		= 1;											// Bertini cascade
const int			NuclearBinary		= 0;											// binary cascade
const int			NuclearHP			= 1;											// nuclear high performance tracking (not speed!)

// turbo mode flags (kill e- e+ g with energy below thr in MeV)
// const int		IsTurboOn			= 0;
// const double		CutG4				= 0.1;											// in cm default is 0.1
// const double		epKillEnergy		= 10.0;											// kill e+
// const double		emKillEnergy		= 10.0;											// kill e-
// const double		gKillEnergy			= 1.0;											// kill gamma
// const double		nKillEnergy			= 10.0;											// kill neutrons

// other background events generation:
// const int		NThreePhoton		= 0;

// U boson parameters
// const int		NUBosons			= 0;											// works only with 0 and 1 for the moment...
// const double		UMass				= 22;											// in MeV
// const double		epsilon				= 1E-3;											// in 10-33
// const int		UDecType			= 1;											// with 0 e+e- and 1 for invisible DM
// const int		UGenType			= 0;											// with 0 for annihi and 1 for Bremsstrahlung not implemented

// monitor (second diamond station not used)
// const double		MonitorSizeX		= 4.0;											// in cm
// const double		MonitorSizeY		= 4.0;											// in cm
// const double		MonitorSizeZ		= 4.0;											// in cm

// const double		MonitorPosiX		= 0.0;											// in cm
// const double		MonitorPosiY		= 0.0;											// in cm
// const double		MonitorPosiZ		= -50.0;										// in cm

// target dimension
// const double		TargetSizeX			= 2.0;											// in cm
// const double		TargetSizeY			= 2.0;											// in cm
// const double		TargetSizeZ			= 0.01;											// in cm 100 micron was 0.005

// const double		TargetPosiX			= 0.0;											// in cm
// const double		TargetPosiY			= 0.0;											// in cm
// const double		TargetPosiZ			= -20.0;										// in cm
// const double		TargetPosiZ			= 0.0;											// in cm

// dump region parameters only
// const double		TDumpFiltThick		= 10.0;
// const double		TDumpFiltInnerRad1	= 3.5;
// const double		TDumpFiltOuterRad1	= TDumpFiltInnerRad1 + TDumpFiltThick;
// const double		TDumpFiltInnerRad2	= 5.5;
// const double		TDumpFiltOuterRad2	= TDumpFiltInnerRad2 + TDumpFiltThick;
// const double		TDumpFiltHz			= 5.0;											// questo per due

// const double		TDumpFiltPosiX		= 0.0;											// in cm
// const double		TDumpFiltPosiY		= 0.0;											// in cm
// const double		TDumpFiltPosiZ		= -10.0;										// in cm

// const double		TDumpInnerRad		= 0.0;
// const double		TDumpOuterRad		= 3.5;
// const double		TDumpHz				= 3.0;											// questo per due

// const double		TDumpPosiX			= 0.0;											// in cm
// const double		TDumpPosiY			= 0.0;											// in cm
// const double		TDumpPosiZ			= -65.0;										// in cm

// const double		LeadBrickSizeX		= 5.0;											// in cm
// const double		LeadBrickSizeY		= 10.0;											// in cm
// const double		LeadBrickSizeZ		= 20.0;											// in cm

// ECAL dimension
const double		ECalSizeX			= 58.0;											// in cm diamter
const double		ECalSizeY			= 58.0;											// in cm
const double		ECalSizeZ			= 22.0;											// in cm full scale length

// const double		ECalInnHole			= 4.0;											// this is radius
const double		ECalCellSize		= 2.0;

// const double		ECalPosiX			= 0.0;											// in cm
// const double		ECalPosiY			= 0.0;											// in cm
// const double		ECalPosiZ			= 180.0 + ECalSizeZ * 0.5;						// in cm; 200.0 + ECalSizeZ * 0.5; //in cm

const int			ECalNRow			= ECalSizeX / ECalCellSize;
const int			ECalNCol			= ECalSizeY / ECalCellSize;
const int			ECalNCells			= ECalNRow * ECalNCol;

// cluster reconstruction variables
const double		SeedE				= 20.0;											// in MeV
const double		CellsE				= 1.0;											// in MeV
// const double		RadiusCl			= 4.5;											// in cm 25 crystal 2x2 involved
// const double		IsCaloCalibON		= 0.0;											// calor calibration constants: 0 all 1; 1 gauss with sigma = 1%

// SAC geometry
// const double		SACX				= 12;											// 6x6 crystals 2x2 cm each
// const double		SACY				= 12;
// const double		SACLength			= 10;

// LAV geometry
// const double		LAVInnRad			= 4.0;
// const double		LAVOutRad			= 10.0;
// const double		LAVLength			= 20;											// in cm

// high energy positron veto outside the magnet dimension
// const double		EVetoPosiX			= 0.0;											// in cm
// const double		EVetoPosiY			= -75.0;										// in cm was 85 cm
// const double		EVetoPosiZ			= -0.5 * ECalSizeZ + 0.5 + ECalPosiZ + 20.0;	// in cm
// const double		EVetoPosiZ			= 223.0;										// in cm

// const double		EVetoSizeX			= 20.0;											// in cm
// const double		EVetoSizeY			= 50.0;											// in cm was 80
// const double		EVetoSizeZ			= 1.0;											// in cm

// const double		EVetoFingerSize		= 1.0;											// in cm

// positron veto inside the magnet dimension
// const double		PVetoPosiX			= 0.0;											// in cm
// const double		PVetoPosiY			= -20.0;										// in cm was 85 cm
// const double		PVetoPosiZ			= 0.0;											// in cm

// const double		PVetoSizeX			= 19.8;											// in cm
// const double		PVetoSizeY			= 1.0;											// in cm full detector size will decide finger number
// const double		PVetoSizeZ			= 100.0;										// in cm

// const double		PVetoFingerSize		= 1.0;											// in cm

// electron veto inside the magnet dimension
// const double		EleVetoPosiX		= 0.0;											// in cm
// const double		EleVetoPosiY		= 20;											// in cm was 85 cm
// const double		EleVetoPosiZ		= 0.0;											// in cm

// const double		EleVetoSizeX		= 19.8;											// in cm
// const double		EleVetoSizeY		= 1.0;											// in cm was 80
// const double		EleVetoSizeZ		= 100.0;										// in cm

// const double		EleVetoFingerSize	= 1.0;											// in cm

// tracker geometry same position of the magnet
// const double		TrackerPosiX		= 0.0;											// in cm
// const double		TrackerPosiY		= 0.0;											// in cm
// const double		TrackerPosiZ		= 0.0;											// in cm is now in the magnet
// const double		TrClsDist			= 0.3 * CLHEP::mm;

// **********************************
// TRACKER GEOMETRY
// **********************************

// cylindrical tracker
// const double		TrackerNLayers		= 5.0;
// const double		TrackerLayerTick	= 1.0;
// const double		TrackerNRings		= 1.0;
// const double		TrackerInnerRad		= 20.0;
// const double		TrackerOuterRad		= TrackerInnerRad + TrackerLayerTick;
// const double		TrackerHz			= 50.0;											// questo per due

// Planar GEM traker
// const int		NChambers			= 6;
// const double		PGEMSizeX			= 9.7;											// in cm
// const double		PGEMSizeY			= 0.5;											// for each of the NChambers
// const double		PGEMSizeZ			= 45.0;											// in cm half size

// const double		PGEMPosiX			= 0.0;
// const double		PGEMPosiY			= 20.0;
// const double		PGEMPosiZ			= 0.0;											// is inside the magnet volume

// const double		MagnetSizeX			= 20.0;											// magnetic field region
// const double		MagnetSizeY			= 52.0;
// const double		MagnetSizeZ			= 100.0;

// const double		MagnetPosiX			= 0.0;											// in cm
// const double		MagnetPosiY			= 0.0;											// in cm
// const double		MagnetPosiZ			= 20 + MagnetSizeZ * 0.5;						// in cm

// top joke of magnet
// const double		MagUpSizeX			= 77.0;
// const double		MagUpSizeY			= 200.0;										// was 116
// const double		MagUpSizeZ			= 100.0;

// const double		MagUpX				= 10.0 + MagUpSizeX / 2;
// const double		MagUpY				= 0.0;
// const double		MagUpZ				= MagnetPosiZ;									//in cm;

// const double		MagRgSizeX			= 20.0;
// const double		MagRgSizeY			= 42.0;
// const double		MagRgSizeZ			= 100.0;

// const double		MagRgX				= 0.0;
// const double		MagRgY				= 58.0 + MagRgSizeY / 2;						// was 26
// const double		MagRgZ				= MagnetPosiZ;									// in cm;

// const double		startAngle			= 0.0;
// const double		spanningAngle		= 180.0;
// const double		MagCoilRMin			= 15.0;											// numbers to be verified
// const double		MagCoilRMax			= 35.0;											// numbers to be verified
// const double		MagCoilDz			= 10.0;											// numbers to be verified

// const double		MagCoilPosX			= 10.0 + MagCoilDz;
// const double		MagCoilPosY			= 0.0;
// const double		MagCoilPosZ			= 20.0;											// in cm;

const int			MaxTracks			= 100;											// to be less than 50 check in EVENT ACTION histoManager.cc HistoManager.hh
// const int		MaxTrHits			= 500;

// beam dump wall dimension
// const double		WallSizeX			= 450.0;										// in cm
// const double		WallSizeY			= 200.0;										// in cm
// const double		WallSizeZ			= 50.0;											// in cm

// const double		WallPosiX			= 0.0;											// in cm
// const double		WallPosiY			= 0.0;											// in cm
// const double		WallPosiZ			= 500.0 + WallSizeZ / 2;						// in cm, reference point: center of magnet
