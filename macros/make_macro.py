import argparse

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

# get bound of lowMult histogram
EwithUnit = str(incidentE) + incidentEUnit
bound = ""
if EwithUnit == "100.0MeV": bound = "13050.0"
elif EwithUnit == "200.0MeV": bound = "6525.0"
elif EwithUnit == "500.0MeV": bound = "2610.0"
elif EwithUnit == "1.0GeV": bound = "1305.0"
elif EwithUnit == "2.0GeV": bound = "652.5"
elif EwithUnit == "5.0GeV": bound = "261.0"
elif EwithUnit == "10.0GeV": bound = "130.5"
elif EwithUnit == "20.0GeV": bound = "65.25"
elif EwithUnit == "50.0GeV": bound = "26.1"
elif EwithUnit == "100.0GeV": bound = "13.05"

# name of file
filename = str(int(incidentE)) + incidentEUnit + "_" + incidentP + "_n" + str(numEvents)
if args.r != None: filename += "_r" + str(runNum)

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
	f.write("/RunAction/GammaPerEventMultMin 0.0\n")
	f.write("/RunAction/PositronPerEventMultMin 0.0\n")
	f.write("/RunAction/ElectronPerEventMultMin 0.0\n")
	f.write("/RunAction/ProtonPerEventMultMin 0.0\n")
	f.write("/RunAction/NeutronPerEventMultMin 0.0\n")
	f.write("/RunAction/PionPlusPerEventMultMin 0.0\n")
	f.write("/RunAction/PionMinusPerEventMultMin 0.0\n")
	f.write("/RunAction/PionZeroPerEventMultMin 0.0\n")
	f.write("/RunAction/MuonPlusPerEventMultMin 0.0\n")
	f.write("/RunAction/MuonMinusPerEventMultMin 0.0\n")
	f.write("/RunAction/OptPhotPerEventMultMin 0.0\n")
	f.write("\n")
	f.write("/RunAction/GammaPerEventMultMax " + bound + "\n")
	f.write("/RunAction/PositronPerEventMultMax " + bound + "\n")
	f.write("/RunAction/ElectronPerEventMultMax " + bound + "\n")
	f.write("/RunAction/ProtonPerEventMultMax " + bound + "\n")
	f.write("/RunAction/NeutronPerEventMultMax " + bound + "\n")
	f.write("/RunAction/PionPlusPerEventMultMax " + bound + "\n")
	f.write("/RunAction/PionMinusPerEventMultMax " + bound + "\n")
	f.write("/RunAction/PionZeroPerEventMultMax " + bound + "\n")
	f.write("/RunAction/MuonPlusPerEventMultMax " + bound + "\n")
	f.write("/RunAction/MuonMinusPerEventMultMax " + bound + "\n")
	f.write("/RunAction/OptPhotPerEventMultMax " + bound + "\n")
	f.write("\n")
	f.write("/RunAction/PerEventEDep " + str(incidentE) + " " + incidentEUnit + "\n")
	f.write("/RunAction/PerEventUntrackedE " + str(incidentE) + " " + incidentEUnit + "\n")
	f.write("\n")
	f.write("/run/beamOn " + str(numEvents) + "\n")
