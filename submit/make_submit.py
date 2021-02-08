import os, argparse

# Define parser
parser = argparse.ArgumentParser(description = "Variables to generate KLMC_SAC condor submit files with")
parser.add_argument("BeamType", type = int, help = "Beam type -- Sets incident particle")
parser.add_argument("IncidentE", type = float, help = "Energy of incident particle")
parser.add_argument("IncidentEUnit", type = str, help = "Unit of energy of incident particle")
parser.add_argument("NEvents", type = int, help = "Number of events")
parser.add_argument("NRuns", type = int, help = "Number of runs")
parser.add_argument("JobFlav", type = str, help = "Job flavour")

# Parse the arguments and save values
args = parser.parse_args()
BeamType = args.BeamType
IncidentE = args.IncidentE
IncidentEUnit = args.IncidentEUnit
NEvents = args.NEvents
NRuns = args.NRuns
JobFlav = args.JobFlav

# Filename
filename = "SAC_" + str(BeamType) + "_" + str(int(IncidentE)) + IncidentEUnit + "_n" + str(NEvents) + "_r" + str(NRuns)
print("Filename: " + filename + "!")

# Delete and create new directory
os.system("rm -rf " + filename)
os.mkdir(filename)
os.chdir(filename)

# Create err, log, and out folders
os.mkdir("err")
os.mkdir("log")
os.mkdir("out")
os.mkdir("root_files")

# Write in file
with open(filename + ".in", "w") as f:
	for i in range(NRuns):
		f.write(filename[:-len(str(NRuns))] + str(i) + "\n") # FIX THIS!!!
print("In file created!")

# Write condor file
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
	f.write("executable\t\t\t\t\t= ../batch.sh\n")
	f.write("transfer_executable\t\t\t= TRUE\n")
	f.write("arguments\t\t\t\t\t= KLMC_SAC $(macro).mac\n")
	f.write("transfer_input_files\t\t= ../../build/KLMC_SAC, macros/$(macro).mac\n")
	f.write("\n")
	f.write("+JobFlavour\t\t\t\t\t= \"" + JobFlav + "\"\n")
	f.write("\n")
	f.write("queue macro from " + filename + ".in\n")
print("Condor file created!")

# Generate macro files
os.mkdir("macros")
os.chdir("macros")
for i in range(NRuns):
	os.system("python ../../../macros/make_macro.py " + str(BeamType) + " " + str(IncidentE) + " " + IncidentEUnit + " " + str(NEvents) + " -r " + str(i))
print("Macro files created!")
