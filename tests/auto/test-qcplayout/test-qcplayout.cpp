#include "test-qcplayout.h"

void TestQCPLayout::init()
{
  mPlot = new QCustomPlot(0);
  mPlot->show();
  QTest::qWaitForWindowShown(mPlot);
}


void TestQCPLayout::cleanup()
{
  delete mPlot;
}

void TestQCPLayout::layoutGridElementManagement()
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

void TestQCPLayout::layoutGridLayout()
{
  mPlot->setGeometry(50, 50, 500, 500);
  QCPLayoutGrid *mainLayout = qobject_cast<QCPLayoutGrid*>(mPlot->plotLayout());
  delete mainLayout->takeAt(0); // remove initial axis rect
  // create 3x3 grid:
  mainLayout->addElement(new QCPAxisRect(mPlot), 0, 0);
  mainLayout->addElement(new QCPAxisRect(mPlot), 0, 1);
  mainLayout->addElement(new QCPAxisRect(mPlot), 0, 2);
  mainLayout->addElement(new QCPAxisRect(mPlot), 1, 0);
  mainLayout->addElement(new QCPAxisRect(mPlot), 1, 1);
  mainLayout->addElement(new QCPAxisRect(mPlot), 1, 2);
  mainLayout->addElement(new QCPAxisRect(mPlot), 2, 0);
  mainLayout->addElement(new QCPAxisRect(mPlot), 2, 1);
  mainLayout->addElement(new QCPAxisRect(mPlot), 2, 2);
  QList<QCPAxisRect*> rlist;
  for (int i=0; i<mainLayout->elementCount(); ++i)
  {
    rlist << qobject_cast<QCPAxisRect*>(mainLayout->elementAt(i));
    rlist.last()->addAxes(QCPAxis::atLeft|QCPAxis::atRight|QCPAxis::atTop|QCPAxis::atBottom);
  }
  
  // disable spacing for testing of layout functions:
  mainLayout->setRowSpacing(0);
  mainLayout->setColumnSpacing(0);
  mPlot->replot();
  foreach (QCPAxisRect *r, rlist)
  {
    QCOMPARE(r->outerRect().width(), qRound(500/3.0));
    QCOMPARE(r->outerRect().height(), qRound(500/3.0));
  }
  
  // test stretch factors:
  mainLayout->setColumnStretchFactors(QList<double>() << 1 << 2 << 1);
  mainLayout->setRowStretchFactors(QList<double>() << 1 << 2 << 3);
  mPlot->replot();
  QCOMPARE(mainLayout->element(0, 0)->outerRect().width(), qRound(500/4.0*1));
  QCOMPARE(mainLayout->element(0, 1)->outerRect().width(), qRound(500/4.0*2));
  QCOMPARE(mainLayout->element(0, 2)->outerRect().width(), qRound(500/4.0*1));
  QCOMPARE(mainLayout->element(0, 0)->outerRect().height(), qRound(500/6.0*1));
  QCOMPARE(mainLayout->element(1, 0)->outerRect().height(), qRound(500/6.0*2));
  QCOMPARE(mainLayout->element(2, 0)->outerRect().height(), qRound(500/6.0*3));
  QCOMPARE(mainLayout->element(2, 2)->outerRect().width(), qRound(500/4.0*1));
  QCOMPARE(mainLayout->element(2, 2)->outerRect().height(), qRound(500/6.0*3));
  
  // test maximum size:
  mainLayout->element(0, 0)->setMaximumSize(100, QWIDGETSIZE_MAX);
  mPlot->replot();
  QCOMPARE(mainLayout->element(0, 0)->outerRect().width(), 100);
  QCOMPARE(mainLayout->element(0, 1)->outerRect().width(), qRound(400/3.0*2));
  QCOMPARE(mainLayout->element(0, 2)->outerRect().width(), qRound(400/3.0*1));
  
  // test minimum size:
  mainLayout->element(0, 0)->setMinimumSize(100, 200);
  mPlot->replot();
  QCOMPARE(mainLayout->element(0, 0)->outerRect().width(), 100);
  QCOMPARE(mainLayout->element(0, 0)->outerRect().height(), 200);
  QCOMPARE(mainLayout->element(1, 0)->outerRect().height(), qRound(300/5.0*2));
  QCOMPARE(mainLayout->element(2, 0)->outerRect().height(), qRound(300/5.0*3));
  
  // test minimum size hint on parent layout:
  QCOMPARE(mainLayout->minimumSizeHint(), QSize(100+50+50, 200+50+50));
  
  // add spacing:
  mainLayout->setRowSpacing(10);
  mainLayout->setColumnSpacing(15);
  // test minimum size hint on parent layout with spacing:
  QCOMPARE(mainLayout->minimumSizeHint(), QSize(100+15+50+15+50, 200+10+50+10+50));
}
