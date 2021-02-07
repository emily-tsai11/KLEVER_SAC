import argparse

# define parser
parser = argparse.ArgumentParser(description = "Variables to generate KLMC_SAC macro files with")
parser.add_argument("BeamType", type = int, help = "Beam type -- Sets incident particle")
parser.add_argument("IncidentE", type = float, help = "Energy of incident particle")
parser.add_argument("IncidentEUnit", type = str, help = "Unit of energy of incident particle")
parser.add_argument("NEvents", type = int, help = "Number of events")
parser.add_argument("-r", type = int, help = "Run number")

# parse the arguments and save values
args = parser.parse_args()
BeamType = args.BeamType
IncidentE = args.IncidentE
IncidentEUnit = args.IncidentEUnit
NEvents = args.NEvents
RunNum = args.r

# define helpers
key = str(IncidentE) + " " + IncidentEUnit
plist = ["all", "e+", "e-", "gamma", "mu+", "mu-", "neutron", "opticalphoton", "other", "pi+", "pi-", "pi0", "proton", "untracked"]
dim = ["X", "Y", "Z"]
NaN = float("NaN")

# name of file
filename = "SAC_" + str(BeamType) + "_" + str(int(IncidentE)) + IncidentEUnit + "_n" + str(NEvents)
if args.r != None: filename += "_r" + str(RunNum)

# write macro file
with open(filename + ".mac", "w") as f:
	f.write("/control/verbose 1\n")
	f.write("/run/verbose 1\n")
	f.write("/tracking/verbose 0\n")
	f.write("\n")
	f.write("# 0 -- don't store particle tracks, runs faster\n")
	f.write("# 1 -- stored particle tracks are drawn in viewer, runs very slow\n")
	f.write("/tracking/storeTrajectory 0\n")
	f.write("\n")
	f.write("/run/initialize\n")
	f.write("\n")
	f.write("/PrimaryGeneratorAction/BeamType " + str(BeamType) + "\n")
	f.write("/PrimaryGeneratorAction/BeamEnergy " + key + "\n")
	f.write("\n")
	f.write("/HistManager/FileName " + filename + "\n")
	f.write("\n")
	f.write("/run/beamOn " + str(NEvents) + "\n")
