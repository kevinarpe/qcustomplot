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

tarcommand = "GZIP=\"-9\" tar -caf" # -a means determine compressor by archive suffix
tarsuffix = ".tar.gz"


if raw_input("\033[1;31m"+"This will call 'git clean -dxf' which will completely reset the current working directory.\n(Call 'git clean -dxn' to see what will be deleted.)\nContinue? (y/n): "+"\033[1;m").lower() != 'y':
  printinfo("Aborted.");
  sys.exit(1);

os.chdir(sys.path[0]) # change current working dir to script dir

# clean working dir:
printinfo("Cleaning working directory...")
if subprocess.call("git clean -dxf", shell=True) != 0:
  printerror("Failed to clean working directory with git."); sys.exit(1)
# amalgamate sources:
printinfo("Amalgamating sources...")
subprocess.call("./run-amalgamate.sh", shell=True)
# generate documentation images:
printinfo("Generating documentation images...")
os.chdir("./other/doc-image-generator")
runQmakeMake("qmake474")
if subprocess.call("./doc-image-generator", shell=True) != 0:
  printerror("Failed to generate documentation images."); sys.exit(1)
os.chdir("../..")
# generate documentation:
printinfo("Compiling documentation...")
subprocess.call("./run-doxygen.sh", shell=True)

# build release packages in temp directory:
print ""
os.mkdir("./temp")
os.chdir("./temp")

# full:
printinfo("Building full QCustomPlot package")
distutils.dir_util.copy_tree("../documentation/html", "./documentation/html")
shutil.copy2("../documentation/qthelp/qcustomplot.qch", "./documentation/")
for f in ["../qcustomplot.h", "../qcustomplot.cpp", "../GPL.txt", "../changenotes.txt"]:
  shutil.copy2(f, "./")
distutils.dir_util.copy_tree("../examples", "./examples")
os.chdir("./examples/plots");
shutil.rmtree("./screenshots")
os.chdir("../../");
subprocess.call("find . -name .gitignore -exec rm -f \"{}\" \;", shell=True)
subprocess.call(tarcommand+" QCustomPlot"+tarsuffix+" *", shell=True)
shutil.move("QCustomPlot"+tarsuffix, "../")
subprocess.call("rm -r *", shell=True)

# source only:
printinfo("Building QCustomPlot-source package")
for f in ["../qcustomplot.h", "../qcustomplot.cpp", "../GPL.txt", "../changenotes.txt"]:
  shutil.copy2(f, "./")
subprocess.call("find . -name .gitignore -exec rm -f \"{}\" \;", shell=True)
subprocess.call(tarcommand+" QCustomPlot-source"+tarsuffix+" *", shell=True)
shutil.move("QCustomPlot-source"+tarsuffix, "../")
subprocess.call("rm -r *", shell=True)

# shared lib:
printinfo("Building QCustomPlot-sharedlib package")
distutils.dir_util.copy_tree("../sharedlib/", "./")
subprocess.call("find . -name .gitignore -exec rm -f \"{}\" \;", shell=True)
subprocess.call(tarcommand+" QCustomPlot-sharedlib"+tarsuffix+" *", shell=True)
shutil.move("QCustomPlot-sharedlib"+tarsuffix, "../")
subprocess.call("rm -r *", shell=True)

# clean up:
os.chdir("../")
os.rmdir("./temp")
printinfo("done")


