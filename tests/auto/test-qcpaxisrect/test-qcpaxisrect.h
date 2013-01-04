#include <QtTest/QtTest>
#include "../../../qcustomplot.h"

class TestQCPAxisRect : public QObject
{
  Q_OBJECT
private slots:
  void init();
  void cleanup();
  
  void multiAxis();
  void multiAxisMargins();
  void axisRemovalConsequencesToPlottables();
  void axisRemovalConsequencesToItems();
  
private:
  QCustomPlot *mPlot;
};
