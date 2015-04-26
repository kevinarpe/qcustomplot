#!/usr/bin/env python3

"""
Borrowed from sharedlib/sharedlib-usage/main.cpp

#include <QApplication>
#include <QMainWindow>
#include "../../qcustomplot.h"

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  QMainWindow window;

  // setup customPlot as central widget of window:
  QCustomPlot customPlot;
  window.setCentralWidget(&customPlot);

  // create plot (from quadratic plot example):
  QVector<double> x(101), y(101);
  for (int i=0; i<101; ++i)
  {
    x[i] = i/50.0 - 1;
    y[i] = x[i]*x[i];
  }
  customPlot.addGraph();
  customPlot.graph(0)->setData(x, y);
  customPlot.xAxis->setLabel("x");
  customPlot.yAxis->setLabel("y");
  customPlot.rescaleAxes();

  window.setGeometry(100, 100, 500, 400);
  window.show();
  return a.exec();
}
"""
import sys

from PyQt5.QtWidgets import QApplication, QMainWindow
from QCustomPlot import QCustomPlot

def main():
    # Available later from global variable QtGui.qApp
    a = QApplication(sys.argv)
    window = QMainWindow()

    custom_plot = QCustomPlot()
    window.setCentralWidget(custom_plot)

    x = []
    y = []
    for i in range(0, 101):
        z = i/50.0 - 1
        x.append(z)
        y.append(z * z)

    custom_plot.addGraph()
    custom_plot.graph(0).setData(x, y)
    custom_plot.xAxis.setLabel("x")
    custom_plot.yAxis.setLabel("y")
    custom_plot.rescaleAxes()

    window.setGeometry(100, 100, 500, 400)
    window.show()
    status = a.exec_()
    sys.exit(status)

if __name__ == "__main__":
    main()

