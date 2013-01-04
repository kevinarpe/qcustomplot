#include "test-qcpaxisrect.h"

void TestQCPAxisRect::init()
{
  mPlot = new QCustomPlot(0);
  mPlot->show();
  QTest::qWaitForWindowShown(mPlot);
}


void TestQCPAxisRect::cleanup()
{
  delete mPlot;
}

void TestQCPAxisRect::multiAxis()
{
  QCPAxisRect *ar = mPlot->axisRect(0);
  QCOMPARE(ar->axisCount(QCPAxis::atLeft), 1);
  QCOMPARE(ar->axisCount(QCPAxis::atRight), 1);
  QCOMPARE(ar->axisCount(QCPAxis::atBottom), 1);
  QCOMPARE(ar->axisCount(QCPAxis::atTop), 1);
  
  // test adding of axes:
  QList<QCPAxis*> newAxes = ar->addAxes(QCPAxis::atLeft | QCPAxis::atBottom);
  QCOMPARE(newAxes.size(), 2);
  QCOMPARE(ar->axisCount(QCPAxis::atLeft), 2);
  QCOMPARE(ar->axisCount(QCPAxis::atBottom), 2);
  
  // test removal of axes:
  ar->removeAxis(ar->axis(QCPAxis::atLeft, 0));
  QCOMPARE(mPlot->yAxis, (QCPAxis*)0);
  QCOMPARE(ar->axisCount(QCPAxis::atLeft), 1);
  QCOMPARE(ar->axis(QCPAxis::atLeft, 0), newAxes.at(0));
  QTest::ignoreMessage(QtDebugMsg, "bool QCPAxisRect::removeAxis(QCPAxis*) Axis isn't in axis rect: 1234 ");
  QVERIFY(!ar->removeAxis((QCPAxis*)1234));
}

void TestQCPAxisRect::multiAxisMargins()
{
  QCPAxisRect *ar = mPlot->axisRect(0);
  
  // test the increasing margin when axes are added:
  mPlot->replot();
  int leftMarginOneAxis = ar->margins().left();
  int bottomMarginOneAxis = ar->margins().bottom();
  ar->addAxes(QCPAxis::atLeft | QCPAxis::atBottom);
  mPlot->replot();
  int leftMarginTwoAxes = ar->margins().left();
  int bottomMarginTwoAxes = ar->margins().bottom();
  QCOMPARE(leftMarginOneAxis*2+ar->axis(QCPAxis::atLeft, 1)->tickLengthIn(), leftMarginTwoAxes);
  QCOMPARE(bottomMarginOneAxis*2+ar->axis(QCPAxis::atBottom, 1)->tickLengthIn(), bottomMarginTwoAxes);
  
  // test increasing margin when offset is added to first axis:
  int secondAxisOffsetBefore = ar->axis(QCPAxis::atLeft, 1)->offset();
  ar->axis(QCPAxis::atLeft, 0)->setOffset(5);
  mPlot->replot();
  QCOMPARE(leftMarginTwoAxes+5, ar->margins().left());
  QCOMPARE(secondAxisOffsetBefore+5, ar->axis(QCPAxis::atLeft, 1)->offset());
  
  // test increasing margin when padding is added to either axis:
  ar->axis(QCPAxis::atLeft, 0)->setOffset(0);
  ar->axis(QCPAxis::atLeft, 0)->setPadding(10);
  mPlot->replot();
  QCOMPARE(leftMarginTwoAxes+5, ar->margins().left()); // +5 because padding is 5 by default and thus included in leftMarginTwoAxes already
  ar->axis(QCPAxis::atLeft, 1)->setPadding(10);
  mPlot->replot();
  QCOMPARE(leftMarginTwoAxes+10, ar->margins().left()); // +10 because padding is 5 by default and thus included in leftMarginTwoAxes already
}

void TestQCPAxisRect::axisRemovalConsequencesToPlottables()
{
  // test consequences to plottables when one of their axes is removed:
  QCPGraph *graph = mPlot->addGraph(mPlot->xAxis, mPlot->yAxis);
  graph->setData(QVector<double>() << 1 << 2 << 3, QVector<double>() << 1 << 0 << -1);
  mPlot->replot();

  QVERIFY(mPlot->axisRect(0)->removeAxis(mPlot->xAxis));
  QTest::ignoreMessage(QtDebugMsg, "virtual void QCPGraph::draw(QCPPainter*) invalid key or value axis ");
  mPlot->replot();
  QTest::ignoreMessage(QtDebugMsg, "void QCPAbstractPlottable::rescaleKeyAxis(bool) const invalid key axis ");
  mPlot->rescaleAxes();
  QTest::ignoreMessage(QtDebugMsg, "void QCPAbstractPlottable::rescaleKeyAxis(bool) const invalid key axis ");
  graph->rescaleAxes();
  
  // test replacement of previously removed axis:
  QCPAxis *newAxis = mPlot->axisRect(0)->addAxis(QCPAxis::atBottom);
  graph->setKeyAxis(newAxis);
  mPlot->replot();
  mPlot->rescaleAxes();
  graph->rescaleAxes();
}

void TestQCPAxisRect::axisRemovalConsequencesToItems()
{
  // test consequences to items when their axes are removed:
  QCPItemLine *item = new QCPItemLine(mPlot);
  mPlot->addItem(item);
  mPlot->replot();

  QVERIFY(mPlot->axisRect(0)->removeAxis(mPlot->xAxis));
  mPlot->replot(); // currently, QCPItemPosition handles this gracefully by assuming pixels for the missing axis coordinate, so we expect no debug output
  QVERIFY(mPlot->axisRect(0)->removeAxis(mPlot->yAxis));
  QTest::ignoreMessage(QtDebugMsg, "virtual QPointF QCPItemPosition::pixelPoint() const No axes defined "); // for start position
  QTest::ignoreMessage(QtDebugMsg, "virtual QPointF QCPItemPosition::pixelPoint() const No axes defined "); // for end position
  mPlot->replot();
  
  
  QTest::ignoreMessage(QtDebugMsg, "void QCPItemPosition::setPixelPoint(const QPointF&) No axes defined ");
  item->start->setPixelPoint(QPointF(1, 2));
  
  // change type to axis-independent coordinates:
  QTest::ignoreMessage(QtDebugMsg, "virtual QPointF QCPItemPosition::pixelPoint() const No axes defined ");
  QTest::ignoreMessage(QtDebugMsg, "virtual QPointF QCPItemPosition::pixelPoint() const No axes defined ");
  item->start->setType(QCPItemPosition::ptAxisRectRatio);
  item->end->setType(QCPItemPosition::ptAxisRectRatio);
  mPlot->replot(); // shouldn't output any debug messages now because we've changed the type to be independent of axes
  // change back to axis-dependent coordinates:
  QTest::ignoreMessage(QtDebugMsg, "void QCPItemPosition::setPixelPoint(const QPointF&) No axes defined ");
  QTest::ignoreMessage(QtDebugMsg, "void QCPItemPosition::setPixelPoint(const QPointF&) No axes defined ");
  item->start->setType(QCPItemPosition::ptPlotCoords);
  item->end->setType(QCPItemPosition::ptPlotCoords);
  
  // test replacement of previously removed axis:
  QCPAxis *newAxis = mPlot->axisRect(0)->addAxis(QCPAxis::atBottom);
  item->start->setAxes(newAxis, item->start->valueAxis());
  item->end->setAxes(newAxis, item->start->valueAxis());
  mPlot->replot();
}













