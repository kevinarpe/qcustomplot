#include <QtGui>
#include <QtCore>
#include "test-qcustomplot/test-qcustomplot.h"
#include "test-qcpgraph/test-qcpgraph.h"
#include "test-qcplayout/test-qcplayout.h"
#include "test-qcpaxisrect/test-qcpaxisrect.h"

#define QCPTEST(t) t t##instance; QTest::qExec(&t##instance)

int main(int argc, char **argv)
{
  QApplication app(argc, argv);
  
  QCPTEST(TestQCustomPlot);
  QCPTEST(TestQCPGraph);
  QCPTEST(TestQCPLayout);
  QCPTEST(TestQCPAxisRect);
  
  return 0;
}
