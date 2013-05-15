#!/usr/bin/env python
import os, sys, subprocess, shutil, distutils.dir_util

def printinfo(message):
  print "\033[1;36m"+message+"\033[1;m"

def printerror(message):
  print "\033[1;31m"+message+"\033[1;m"

tarcommand = "GZIP=\"-9\" tar -caf" # -a means determine compressor by archive suffix
tarsuffix = ".tar.gz"

os.chdir(sys.path[0]) # change current working dir to script dir
os.mkdir("./temp")
os.chdir("./temp")

# source only:
printinfo("Building QCustomPlot-source package")
for f in ["../qcustomplot.h", "../qcustomplot.cpp", "../GPL.txt", "../changenotes.txt"]:
  shutil.copy2(f, "./")
subprocess.call("find . -name .gitignore -exec rm -f \"{}\" \;", shell=True)
subprocess.call(tarcommand+" QCustomPlot-source"+tarsuffix+" *", shell=True)
shutil.move("QCustomPlot-source"+tarsuffix, "../")
subprocess.call("rm -r *", shell=True)

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


