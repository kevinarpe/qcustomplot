#include <QtTest/QtTest>
#include "../../../qcustomplot.h"

class TestQCustomPlot : public QObject
{
  Q_OBJECT
private slots:
  void init();
  void cleanup();
  
  void rescaleAxes();
  
private:
  QCustomPlot *mPlot;
};
