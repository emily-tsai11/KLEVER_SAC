# PbF2 SAC Monte Carlo Geant4 simulation

Based off of https://github.com/dariosol/padme-fw.
Currently works with Geant4 10.6.2 on CERN lxplus.

### Running `KLMC_SAC`

- `cd KLMC_SAC`
- `mkdir build`
- `cd build`
- `cmake ..`
- `make`
- `./KLMC_SAC <macro file>`

If no macro file is specified, `build/macros/vis.mac` is used with the visualizer.
Otherwise, all macro files are run in batch mode.

### Running `SACAnalysis.cc`

- `cd KLMC_SAC/analysis`
- `root -l -b "SACAnalysis.cc(<BeamType>, <NEvents>)"`

### Beam types

- `1`: K-longs with Atherton momentum
- `2`: K-longs with user-defined energies
- `3`: neutrons with user-defined energies
- anything else: default to gammas with user-defined energies

### Setting up Geant4 on the CERN lxplus environment

- `source /cvmfs/sft.cern.ch/lcg/views/LCG_98/x86_64-centos7-gcc8-opt/setup.sh`
- To set up CMake:
	- `source /cvmfs/sft.cern.ch/lcg/releases/LCG_98/CMake/3.17.3/x86_64-centos7-gcc8-opt/CMake-env.sh`
	- `PATH=/cvmfs/sft.cern.ch/lcg/releases/LCG_98/CMake/3.17.3/x86_64-centos7-gcc8-opt/bin/:$PATH`
