
QT += core gui testlib
TEMPLATE = app
DEPENDPATH = .
INCLUDEPATH = .

HEADERS += ../../qcustomplot.h \
    test-qcpgraph/test-qcpgraph.h \
    test-qcplayout/test-qcplayout.h \
    test-qcpaxisrect/test-qcpaxisrect.h

SOURCES += ../../qcustomplot.cpp \
           autotest.cpp \
    test-qcpgraph/test-qcpgraph.cpp \
    test-qcplayout/test-qcplayout.cpp \
    test-qcpaxisrect/test-qcpaxisrect.cpp
    
