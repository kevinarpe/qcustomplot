#!/usr/bin/env python
import os, sys, subprocess, shutil, distutils.dir_util, argparse

# Define command line interface:
argparser = argparse.ArgumentParser()
argparser.add_argument("-qt", type=int, default=0,
                    help="Specify a specific qt version (e.g. 474 or 502) to test.")
argparser.add_argument("-p", "--pause", action="store_true",
                    help="Pause after every qt version.")
argparser.add_argument("-s", "--short", action="store_true",
                    help="Only test the plots example.")
argparser.add_argument("-i", "--interactive", action="store_true",
                    help="Keep tests open for user interaction and continue test only after user quits them.")
config = argparser.parse_args()

execTestSuffix = "& sleep 1; kill $!"; # appended to test execution command line. Starts test executable in background and quits it after one second (if executable fails, kill will fail and thus propagate !=0 return value)
if config.interactive:
  execTestSuffix = "";

# define print functions:
def printinfo(message):
  print "\033[1;36m"+message+"\033[1;m"

def printerror(message):
  print "\033[1;31m"+message+"\033[1;m"
  
def runQmakeMake(qmakecommand):
  if subprocess.call(qmakecommand, shell=True) != 0:
    printerror("qmake failed"); sys.exit(1)
  if subprocess.call("make -j4", shell=True) != 0:
    printerror("make failed"); sys.exit(1)

# main test loop:
qmakeVersions = ["qmake464", "qmake474", "qmake480", "qmake481", "qmake483", "qmake501", "qmake502"]
if (config.qt > 0):
  qmakeVersions = ["qmake"+str(config.qt)];
for qmakecommand in qmakeVersions:

  try:
    qmakeproc = subprocess.Popen([qmakecommand, "-v"], stdout=subprocess.PIPE)
    printinfo(qmakeproc.stdout.readlines()[1].rstrip()); # print used Qt version in this run
  except:
    printinfo("Qt version of '"+qmakecommand+"' not found, skipping.");
    continue
  os.chdir(sys.path[0]) # change current working dir to script dir
  os.mkdir("./temp")
  os.chdir("./temp")

  # ================== QCustomPlot full ==================
  printinfo("QCustomPlot full:")
  os.mkdir("QCustomPlot")
  os.chdir("QCustomPlot")
  shutil.copy2("../../QCustomPlot.tar.gz", "./")
  if subprocess.call("tar -xf QCustomPlot.tar.gz", shell=True) != 0:
    printerror("Failed to untar QCustomPlot.tar.gz"); sys.exit(1)

  # plots examples:
  printinfo("examples/plots")
  os.chdir("examples/plots")
  runQmakeMake(qmakecommand)
  if subprocess.call("./plot-examples"+execTestSuffix, shell=True) != 0:
    printerror("Execution unsuccessful")
  os.chdir("../..")
  if not config.short:
    # interactions example:
    printinfo("examples/interactions")
    os.chdir("examples/interactions")
    runQmakeMake(qmakecommand)
    if subprocess.call("./interaction-example"+execTestSuffix, shell=True) != 0:
      printerror("Execution unsuccessful")
    os.chdir("../..")
    # text-document-integration example:
    printinfo("examples/text-document-integration")
    os.chdir("examples/text-document-integration")
    runQmakeMake(qmakecommand)
    if subprocess.call("./text-document-integration"+execTestSuffix, shell=True) != 0:
      printerror("Execution unsuccessful")
    os.chdir("../..")
    # scrollbar-axis-range-control example:
    printinfo("examples/scrollbar-axis-range-control")
    os.chdir("examples/scrollbar-axis-range-control")
    runQmakeMake(qmakecommand)
    if subprocess.call("./scrollbar-axis-range-control"+execTestSuffix, shell=True) != 0:
      printerror("Execution unsuccessful")
    os.chdir("../..")
  
  os.chdir("..")

  # ================== QCustomPlot-sharedlib and -source package ==================
  if not config.short:
    printinfo("QCustomPlot sharedlib and source:")
    os.mkdir("QCustomPlot-sharedlib")
    os.chdir("QCustomPlot-sharedlib")
    shutil.copy2("../../QCustomPlot-sharedlib.tar.gz", "./")
    if subprocess.call("tar -xf QCustomPlot-sharedlib.tar.gz", shell=True) != 0:
      printerror("Failed to untar QCustomPlot-sharedlib.tar.gz"); sys.exit(1)
    os.chdir("..") # untar qcustomplot source directly in "temp/" because sharedlib expects it two dirs above pro file
    shutil.copy2("../QCustomPlot-source.tar.gz", "./")
    if subprocess.call("tar -xf QCustomPlot-source.tar.gz", shell=True) != 0:
      printerror("Failed to untar QCustomPlot-source.tar.gz"); sys.exit(1)
    os.chdir("QCustomPlot-sharedlib")
    # sharedlib compile:
    printinfo("sharedlib-compilation")
    os.chdir("sharedlib-compilation")
    runQmakeMake(qmakecommand)
    subprocess.call("cp libqcustomplot* ../sharedlib-usage", shell=True)
    os.chdir("..");
    # sharedlib use:
    printinfo("sharedlib-usage")
    os.chdir("sharedlib-usage");
    runQmakeMake(qmakecommand)
    if subprocess.call("export LD_LIBRARY_PATH=.; ./sharedlib-usage"+execTestSuffix, shell=True) != 0:
      printerror("Execution unsuccessful")
    os.chdir("..")

    os.chdir("..")

  # ================== Cleanup ==================
  answer = ""
  if config.pause:
    if qmakecommand == qmakeVersions[len(qmakeVersions)-1]:
      printinfo("Press any key to finish release test.")
    else:
      printinfo("Enter 'q' and hit return to stop release test, enter nothing to continue with next qt version...")
    answer = raw_input()
  os.chdir("..")
  shutil.rmtree("./temp")
  printinfo("cleaned up")
  if (answer == "q"):
    break
  

