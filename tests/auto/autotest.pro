
QT += core gui testlib
TEMPLATE = app
TARGET =
DEPENDPATH = .
INCLUDEPATH = .

HEADERS += ../../qcustomplot.h \
    test-qcpgraph/test-qcpgraph.h \
    test-qcplayout/test-qcplayout.h

SOURCES += ../../qcustomplot.cpp \
           autotest.cpp \
    test-qcpgraph/test-qcpgraph.cpp \
    test-qcplayout/test-qcplayout.cpp
    
