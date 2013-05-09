#
# Project to generate various images for the QCustomPlot documentation
#

QT       += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = doc-image-generator
TEMPLATE = app

HEADERS  += mainwindow.h \
  ../../src/*.h

LIBS += -L../../src

CONFIG(debug, debug|release) {
  LIBS += -lqcustomplotd
} else {
  LIBS += -lqcustomplot
}

SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui
