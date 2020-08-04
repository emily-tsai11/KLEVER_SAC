import os, argparse

# define parser
parser = argparse.ArgumentParser(description = "variables to generate KLMC_SAC condor submit files with")
parser.add_argument("incidentP", type = str, help = "type of incident particle")
parser.add_argument("incidentE", type = float, help = "energy of incident particle")
parser.add_argument("incidentEUnit", type = str, help = "unit of energy of incident particle")
parser.add_argument("numEvents", type = int, help = "number of events")
parser.add_argument("numRuns", type = int, help = "number of runs")

# parse the arguments and save values
args = parser.parse_args()
incidentP = args.incidentP
incidentE = args.incidentE
incidentEUnit = args.incidentEUnit
numEvents = args.numEvents
numRuns = args.numRuns

# filename
filename = str(int(incidentE)) + incidentEUnit + "_" + incidentP + "_n" + str(numEvents)
print("filename: " + filename + "_r" + str(numRuns) + "...")

# write in file
with open(filename + "_r" + str(numRuns) + ".in", "w") as f:
	for i in range(numRuns):
		f.write(filename + "_r" + str(i) + "\n")
print(".in file created!")

# add number of runs to filename
filename += "_r" + str(numRuns)

# generate macro files
os.system("rm -rf " + filename + "_macros")
os.mkdir(filename + "_macros")
os.chdir(filename + "_macros")
for i in range(numRuns):
	os.system("python ../../macros/make_macro.py " + incidentP + " " + str(incidentE) + " " + incidentEUnit + " " + str(numEvents) + " -r " + str(i))
os.chdir("..")
print("macro files created!")

# write condor file
with open(filename + ".condor", "w") as f:
	f.write("universe\t\t\t\t\t= vanilla\n")
	f.write("\n")
	f.write("error\t\t\t\t\t\t= err/$(macro).error\n")
	f.write("log\t\t\t\t\t\t\t= log/$(macro).log\n")
	f.write("output\t\t\t\t\t\t= out/$(macro).out\n")
	f.write("\n")
	f.write("should_transfer_files\t\t= YES\n")
	f.write("when_to_transfer_output\t\t= ON_EXIT\n")
	f.write("\n")
	f.write("environment\t\t\t\t\t= LD_LIBRARY_PATH=/cvmfs/sft.cern.ch/lcg/contrib/gcc/6.3/x86_64-slc6/lib64\n")
	f.write("executable\t\t\t\t\t= batch.sh\n")
	f.write("transfer_executable\t\t\t= TRUE\n")
	f.write("arguments\t\t\t\t\t= KLMC_SAC $(macro)\n")
	f.write("transfer_input_files\t\t= ../build/KLMC_SAC, " + filename + "_macros/$(macro).mac\n")
	f.write("\n")
	f.write("+JobFlavour\t\t\t\t\t= \"espresso\"\n")
	f.write("\n")
	f.write("queue macro from " + filename + ".in\n")
print(".condor file created!")
