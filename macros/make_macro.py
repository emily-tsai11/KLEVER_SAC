import os, argparse

# define parser
parser = argparse.ArgumentParser(description = "variables to generate KLMC_SAC macro files with")
parser.add_argument("incidentP", type = str, help = "type of incident particle")
parser.add_argument("incidentE", type = float, help = "energy of incident particle")
parser.add_argument("incidentEUnit", type = str, help = "unit of energy of incident particle")
parser.add_argument("numEvents", type = int, help = "number of events")
parser.add_argument("-r", type = int, help = "run number")

# parse the arguments and save values
args = parser.parse_args()
incidentP = args.incidentP
incidentE = args.incidentE
incidentEUnit = args.incidentEUnit
numEvents = args.numEvents
runNum = args.r

# write macro file
filename = str(int(incidentE)) + incidentEUnit + "_" + incidentP + "_n" + str(numEvents)
if args.r != None: filename += "_r" + str(runNum)
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
	f.write("/gun/particle " + incidentP + "\n")
	f.write("/gun/energy " + str(incidentE) + " " + incidentEUnit + "\n")
	f.write("\n")
	f.write("/Detector/SAC/IncidentE " + str(incidentE) + " " + incidentEUnit + "\n")
	f.write("\n")
	if args.r == None:
		f.write("/RunAction/FileName SAC_" + str(int(incidentE)) + incidentEUnit + "_" + incidentP + "_n" + str(numEvents) + "\n")
	else:
		f.write("/RunAction/FileName SAC_" + str(int(incidentE)) + incidentEUnit + "_" + incidentP + "_n" + str(numEvents) + "_r" + str(runNum) + "\n")
	f.write("\n")
	f.write("/RunAction/GammaPerEventInitE " + str(incidentE) + " " + incidentEUnit + "\n")
	f.write("/RunAction/PositronPerEventInitE " + str(incidentE) + " " + incidentEUnit + "\n")
	f.write("/RunAction/ElectronPerEventInitE " + str(incidentE) + " " + incidentEUnit + "\n")
	f.write("/RunAction/ProtonPerEventInitE " + str(incidentE) + " " + incidentEUnit + "\n")
	f.write("/RunAction/NeutronPerEventInitE " + str(incidentE) + " " + incidentEUnit + "\n")
	f.write("/RunAction/PionPlusPerEventInitE " + str(incidentE) + " " + incidentEUnit + "\n")
	f.write("/RunAction/PionMinusPerEventInitE " + str(incidentE) + " " + incidentEUnit + "\n")
	f.write("/RunAction/PionZeroPerEventInitE " + str(incidentE) + " " + incidentEUnit + "\n")
	f.write("/RunAction/MuonPlusPerEventInitE " + str(incidentE) + " " + incidentEUnit + "\n")
	f.write("/RunAction/MuonMinusPerEventInitE " + str(incidentE) + " " + incidentEUnit + "\n")
	f.write("/RunAction/OptPhotPerEventInitE " + str(incidentE) + " " + incidentEUnit + "\n")
	f.write("\n")
	f.write("/RunAction/PerEventEDep " + str(incidentE) + " " + incidentEUnit + "\n")
	f.write("/RunAction/PerEventUntrackedE " + str(incidentE) + " " + incidentEUnit + "\n")
	f.write("\n")
	f.write("/run/beamOn " + str(numEvents) + "\n")
