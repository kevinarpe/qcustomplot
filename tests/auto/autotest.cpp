#include <QtGui>
#include <QtCore>
#include "test-qcpgraph/test-qcpgraph.h"
#include "test-qcplayout/test-qcplayout.h"

#define QCPTEST(t) t t##instance; QTest::qExec(&t##instance)

int main(int argc, char **argv)
{
  QApplication app(argc, argv);
  
  QCPTEST(TestQCPGraph);
  QCPTEST(TestQCPLayout);
  
  return 0;
}
