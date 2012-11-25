#include "testrunner.h"
#include <QtTest/QtTest>
#include "test-qcpgraph/test-qcpgraph.h"
#include "test-qcplayout/test-qcplayout.h"

void TestRunner::run()
{
  QCPTEST(TestQCPGraph)
}
