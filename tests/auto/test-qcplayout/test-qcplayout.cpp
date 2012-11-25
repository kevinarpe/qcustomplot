#include "test-qcplayout.h"

void TestQCPLayout::init()
{
  mPlot = new QCustomPlot(0);
}

void TestQCPLayout::cleanup()
{
  delete mPlot;
}

void TestQCPLayout::basicElementManagement()
{
  QCPLayoutGrid *mainLayout = qobject_cast<QCPLayoutGrid*>(mPlot->plotLayout());
  QVERIFY(mainLayout);
  QCOMPARE(mainLayout->elementCount(), 1);
  QCOMPARE(mainLayout->elementAt(0), mainLayout->element(0, 0));
  QCOMPARE(mainLayout->columnCount(), 1);
  QCOMPARE(mainLayout->rowCount(), 1);
  
  // test row/column expansion:
  QCPAxisRect *r1 = new QCPAxisRect(mPlot);
  QCPAxisRect *r2 = new QCPAxisRect(mPlot);
  QCPAxisRect *r3 = new QCPAxisRect(mPlot);
  
  QTest::ignoreMessage(QtDebugMsg, "bool QCPLayoutGrid::addElement(QCPLayoutElement*, int, int) There is already an element in the specified row/column: 0 0 ");
  QVERIFY(!mainLayout->addElement(r1, 0, 0));
  QVERIFY(mainLayout->addElement(r1, 0, 1));
  QCOMPARE(mainLayout->columnCount(), 2);
  QCOMPARE(mainLayout->rowCount(), 1);
  
  QTest::ignoreMessage(QtDebugMsg, "bool QCPLayoutGrid::addElement(QCPLayoutElement*, int, int) There is already an element in the specified row/column: 0 1 ");
  QVERIFY(!mainLayout->addElement(r2, 0, 1));
  QVERIFY(mainLayout->addElement(r2, 1, 0));
  QCOMPARE(mainLayout->columnCount(), 2);
  QCOMPARE(mainLayout->rowCount(), 2);
  
  QTest::ignoreMessage(QtDebugMsg, "bool QCPLayoutGrid::addElement(QCPLayoutElement*, int, int) There is already an element in the specified row/column: 1 0 ");
  QVERIFY(!mainLayout->addElement(r3, 1, 0));
  QVERIFY(mainLayout->addElement(r3, 2, 4));
  QCOMPARE(mainLayout->columnCount(), 5);
  QCOMPARE(mainLayout->rowCount(), 3);
  /*
   def  r1   0    0    0
   r2   0    0    0    0
   0    0    0    0    r3
  */     
  
  // simplify should remove columns 2 and 3
  mainLayout->simplify();
  /*
   def  r1   0
   r2   0    0
   0    0    r3
  */ 
  QCOMPARE(mainLayout->columnCount(), 3);
  QCOMPARE(mainLayout->rowCount(), 3);
  QCOMPARE(mainLayout->element(0, 0), mPlot->axisRect());
  QCOMPARE(mainLayout->element(0, 1), r1);
  QCOMPARE(mainLayout->element(0, 2), (QCPLayoutElement*)0);
  QCOMPARE(mainLayout->element(1, 0), r2);
  QCOMPARE(mainLayout->element(1, 1), (QCPLayoutElement*)0);
  QCOMPARE(mainLayout->element(1, 2), (QCPLayoutElement*)0);
  QCOMPARE(mainLayout->element(2, 0), (QCPLayoutElement*)0);
  QCOMPARE(mainLayout->element(2, 1), (QCPLayoutElement*)0);
  QCOMPARE(mainLayout->element(2, 2), r3);
      
  // remove outer right axis rect:
  QVERIFY(mainLayout->take(r3));
  delete r3;
  // now simplify should collapse to 2x2:
  mainLayout->simplify();
  QCOMPARE(mainLayout->columnCount(), 2);
  QCOMPARE(mainLayout->rowCount(), 2);
  QCOMPARE(mainLayout->element(0, 0), mPlot->axisRect());
  QCOMPARE(mainLayout->element(0, 1), r1);
  QCOMPARE(mainLayout->element(1, 0), r2);
  QCOMPARE(mainLayout->element(1, 1), (QCPLayoutElement*)0);
  
  // remove r1 axis rect:
  QCOMPARE(mainLayout->takeAt(1), r1);
  delete r1;
  // now simplify should collapse to 2x1:
  mainLayout->simplify();
  QCOMPARE(mainLayout->columnCount(), 1);
  QCOMPARE(mainLayout->rowCount(), 2);
  QCOMPARE(mainLayout->element(0, 0), mPlot->axisRect(0));
  QCOMPARE(mainLayout->element(1, 0), r2);
  
  // remove last axis rects:
  QCPAxisRect *mainAxisRect = mPlot->axisRect(0);
  QCOMPARE(mainLayout->takeAt(0), mainAxisRect);
  QCOMPARE(mainLayout->takeAt(1), r2);
  delete r2;
  delete mainAxisRect;
  QCOMPARE(mainLayout->element(0, 0), (QCPLayoutElement*)0);
  QCOMPARE(mainLayout->element(1, 0), (QCPLayoutElement*)0);
  // now simplify should collapse to 0x0:
  mainLayout->simplify();
  QVERIFY(!mainLayout->hasElement(0, 0));
  QCOMPARE(mainLayout->columnCount(), 0);
  QCOMPARE(mainLayout->rowCount(), 0);
  QTest::ignoreMessage(QtDebugMsg, "QCPAxisRect* QCustomPlot::axisRect(int) const invalid axis rect index 0 ");
  QVERIFY(!(bool)mPlot->axisRect());
  // repopulate:
  QCPAxisRect *r4 = new QCPAxisRect(mPlot);
  QVERIFY(mainLayout->addElement(r4, 0, 0));
  QCOMPARE(mPlot->axisRect(0), r4);
}
