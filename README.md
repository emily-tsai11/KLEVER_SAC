# PbF2 SAC Monte Carlo Geant4 simulation

Based off of https://github.com/dariosol/padme-fw. 

### Beam Types:

- `1`: K-longs with Atherton momentum
- `2`: K-longs with user-defined energies
- `3`: neutrons with user-defined energies
- anything else: default to gammas with user-defined energies

### Running KLMC_SAC:

- `cd KLMC_SAC`
- `mkdir build`
- `cd build`
- `cmake ..`
- `make`
- `./KLMC_SAC <macro file>`

If no macro file is specified, `vis.mac` is used with the visualizer.
Otherwise, all macro files are run in batch mode. 

### Running SACAnalysis.cc:

- `cd KLMC_SAC/analysis`
- `root -l -b "SACAnalysis.cc(<BeamType>, <NEvents>)"`
