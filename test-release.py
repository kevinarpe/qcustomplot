#!/usr/bin/env python
import os, sys, subprocess, shutil, distutils.dir_util

def printinfo(message):
  print "\033[1;36m"+message+"\033[1;m"

def printerror(message):
  print "\033[1;31m"+message+"\033[1;m"
  
def runQmakeMake(qmakecommand):
  if subprocess.call(qmakecommand, shell=True) != 0:
    printerror("qmake failed"); sys.exit(1)
  if subprocess.call("make -j4", shell=True) != 0:
    printerror("make failed"); sys.exit(1)

qmakeVersions = ["qmake474", "qmake483", "qmake502"]
for qmakecommand in qmakeVersions:

  qmakeproc = subprocess.Popen([qmakecommand, "-v"], stdout=subprocess.PIPE)
  printinfo(qmakeproc.stdout.readlines()[1].rstrip()); # print used Qt version in this run
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

  # interactions example:
  printinfo("examples/interactions")
  os.chdir("examples/interactions")
  runQmakeMake(qmakecommand)
  if subprocess.call("./interaction-example", shell=True) != 0:
    printerror("Execution unsuccessful")
  os.chdir("../..")
  # plots examples:
  printinfo("examples/plots")
  os.chdir("examples/plots")
  runQmakeMake(qmakecommand)
  if subprocess.call("./plot-examples", shell=True) != 0:
    printerror("Execution unsuccessful")
  os.chdir("../..")

  os.chdir("..")

  # ================== QCustomPlot-sharedlib and -source package ==================
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
  if subprocess.call("export LD_LIBRARY_PATH=.; ./sharedlib-usage", shell=True) != 0:
    printerror("Execution unsuccessful")
  os.chdir("..")

  os.chdir("..")

  # ================== Cleanup ==================
  if qmakecommand == qmakeVersions[len(qmakeVersions)-1]:
    printinfo("Press any key to remove temp directory and finish release test.")
  else:
    printinfo("Press any key to remove temp directory and continue with next Qt version...")
  raw_input()
  os.chdir("..")
  shutil.rmtree("./temp")
  printinfo("cleaned up")

