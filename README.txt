QCustomPlot for Py3Qt5
======================

Forked from two other Git repos:

    Original C++ source:
        https://gitorious.org/qcustomplot
        Many thanks to the original author for impressive features and regular updates!

    Original Python2 bindings for PyQt4:
        https://github.com/mds5000/qcustomplot-python
        Many thanks to the original author for teaching me SIP (for PyQt bindings)!

All (copy)rights retained by original authors.

I have not changed the C++ code; only added Py3Qt5 bindings.
Ideally, each time qcustomplot has a new release this repo will be updated.

$ git clone git@github.com:kevinarpe/qcustomplot.git kevinarpe-qcustomplot
$ cd kevinarpe-qcustomplot
$ git checkout v1.3.1-Py3Qt5
$ sharedlib/build.bash

If the build fails, it is 99% due to missing headers, libraries, SIP files, or config here:
    -> sharedlib/sharedlib-Py3Qt5/Py3Qt5/pyqtconfig_nd4.py

Kevin Connor ARPE
kevinarpe@gmail.com

