#include <QtTest/QtTest>
#include "../../../qcustomplot.h"

class TestQCPLayout : public QObject
{
  Q_OBJECT
private slots:
  void init();
  void cleanup();
  
  void basicElementManagement();
  
private:
  QCustomPlot *mPlot;
};
