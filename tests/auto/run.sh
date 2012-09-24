#!/bin/bash

qmake -project "QT += testlib"
qmake
make 
./auto

