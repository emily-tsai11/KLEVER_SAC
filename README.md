# PbF2 SAC Monte Carlo Geant4 simulation

Based off of https://github.com/dariosol/padme-fw.
Currently works with Geant4.10.4 on CERN lxplus. 

### Setting up the CERN lxplus environment

- `source /cvmfs/sft.cern.ch/lcg/contrib/gcc/6.3/x86_64-slc6/setup.sh`
- `source /cvmfs/geant4.cern.ch/geant4/10.5/x86_64-slc6-gcc63-opt-MT/CMake-setup.sh`
- `export CXX=$(which g++)`
- `export CC=$(which gcc)`
- `source /afs/cern.ch/eng/tracking-tools/python_installations/activate_default_python`

### Beam types

- `1`: K-longs with Atherton momentum
- `2`: K-longs with user-defined energies
- `3`: neutrons with user-defined energies
- anything else: default to gammas with user-defined energies

### Running KLMC_SAC

- `cd KLMC_SAC`
- `mkdir build`
- `cd build`
- `cmake ..`
- `make`
- `./KLMC_SAC <macro file>`

If no macro file is specified, `vis.mac` is used with the visualizer.
Otherwise, all macro files are run in batch mode. 

### Running SACAnalysis.cc

- `cd KLMC_SAC/analysis`
- `root -l -b "SACAnalysis.cc(<BeamType>, <NEvents>)"`
