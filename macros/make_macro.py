import argparse
import json

# define parser
parser = argparse.ArgumentParser(description = "variables to generate KLMC_SAC macro files with")
parser.add_argument("BeamType", type = int, help = "beam type, sets incident particle")
parser.add_argument("IncidentE", type = float, help = "energy of incident particle")
parser.add_argument("IncidentEUnit", type = str, help = "unit of energy of incident particle")
parser.add_argument("NEvents", type = int, help = "number of events")
parser.add_argument("-r", type = int, help = "run number")

# parse the arguments and save values
args = parser.parse_args()
BeamType = args.BeamType
IncidentE = args.IncidentE
IncidentEUnit = args.IncidentEUnit
NEvents = args.NEvents
RunNum = args.r

# name of file
filename = "SAC_" + str(BeamType) + "_" + str(int(IncidentE)) + IncidentEUnit + "_n" + str(NEvents)
if args.r != None: filename += "_r" + str(RunNum)

# read in config parameters
config = json.loads(open("config.json").read())
plist = config["particle list"]
current = config[str(IncidentE) + " " + str(IncidentEUnit)]

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
	f.write("/PrimaryGeneratorAction/BeamEnergy " + str(IncidentE) + " " + IncidentEUnit + "\n")
	f.write("\n")
	f.write("/HistManager/FileName " + filename + "\n")
	f.write("\n")
	for key in current:
		for j in range(len(plist)):
			f.write("/HistManager/HistToChange " + key + plist[j] + "\n")
			if key.find("EDep") != -1:
				f.write("/HistManager/LowXBoundWithUnit " + str(current[key]["lowX"][j]) + " " + IncidentEUnit + "\n")
				f.write("/HistManager/HighXBoundWithUnit " + str(current[key]["highX"][j]) + " " + IncidentEUnit + "\n")
			elif key.find("TrLen") != -1:
				f.write("/HistManager/LowXBoundWithUnit " + str(current[key]["lowX"][j]) + " cm\n")
				f.write("/HistManager/HighXBoundWithUnit " + str(current[key]["highX"][j]) + " cm\n")
			elif key.find("Mult") != -1:
				f.write("/HistManager/LowXBound " + str(current[key]["lowX"][j]) + "\n")
				f.write("/HistManager/HighXBound " + str(current[key]["highX"][j]) + "\n")
		f.write("\n")
	f.write("/run/beamOn " + str(NEvents) + "\n")
