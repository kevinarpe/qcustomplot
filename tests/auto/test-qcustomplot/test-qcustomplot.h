#include <QtTest/QtTest>
#include "../../../qcustomplot.h"

class TestQCustomPlot : public QObject
{
  Q_OBJECT
private slots:
  void init();
  void cleanup();
  
  void rescaleAxes_GraphVisibility();
  void rescaleAxes_FlatGraph();
  
private:
  QCustomPlot *mPlot;
};
