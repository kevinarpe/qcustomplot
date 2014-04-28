/***************************************************************************
**                                                                        **
**  QCustomPlot, an easy to use, modern plotting widget for Qt            **
**  Copyright (C) 2011, 2012, 2013, 2014 Emanuel Eichhammer               **
**                                                                        **
**  This program is free software: you can redistribute it and/or modify  **
**  it under the terms of the GNU General Public License as published by  **
**  the Free Software Foundation, either version 3 of the License, or     **
**  (at your option) any later version.                                   **
**                                                                        **
**  This program is distributed in the hope that it will be useful,       **
**  but WITHOUT ANY WARRANTY; without even the implied warranty of        **
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         **
**  GNU General Public License for more details.                          **
**                                                                        **
**  You should have received a copy of the GNU General Public License     **
**  along with this program.  If not, see http://www.gnu.org/licenses/.   **
**                                                                        **
****************************************************************************
**           Author: Emanuel Eichhammer                                   **
**  Website/Contact: http://www.qcustomplot.com/                          **
**             Date: 07.04.14                                             **
**          Version: 1.2.1                                                **
****************************************************************************/

#include "plottable-financial.h"

#include "../painter.h"
#include "../core.h"
#include "../axis.h"
#include "../layoutelements/layoutelement-axisrect.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// QCPFinancialData
////////////////////////////////////////////////////////////////////////////////////////////////////

/*! \class QCPFinancialData
  \brief Holds the data of one single data point for QCPFinancial.
  
  The container for storing multiple data points is \ref QCPFinancialDataMap.
  
  The stored data is:
  \li \a key: coordinate on the key axis of this data point
  \li \a open: The opening value at the data point
  \li \a high: The high/maximum value at the data point
  \li \a low: The low/minimum value at the data point
  \li \a close: The closing value at the data point
  
  \see QCPFinancialDataMap
*/

/*!
  Constructs a data point with key and all values set to zero.
*/
QCPFinancialData::QCPFinancialData() :
  key(0),
  open(0),
  high(0),
  low(0),
  close(0)
{
}

/*!
  Constructs a data point with the specified \a key and values.
*/
QCPFinancialData::QCPFinancialData(double key, double open, double high, double low, double close) :
  key(key),
  open(open),
  high(high),
  low(low),
  close(close)
{
}


////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// QCPFinancial
////////////////////////////////////////////////////////////////////////////////////////////////////

/*! \class QCPFinancial
  \brief A plottable representing a financial stock chart

  \image html QCPFinancial.png
  
  
  
  \section appearance Changing the appearance
  
  
*/

/* start of documentation of inline functions */

/*! \fn QCPFinancialDataMap *QCPFinancial::data() const
  
  Returns a pointer to the internal data storage of type \ref QCPFinancialDataMap. You may use it to
  directly manipulate the data, which may be more convenient and faster than using the regular \ref
  setData or \ref addData methods, in certain situations.
*/

/* end of documentation of inline functions */

/*!
  Constructs a financial chart which uses \a keyAxis as its key axis ("x") and \a valueAxis as its value
  axis ("y"). \a keyAxis and \a valueAxis must reside in the same QCustomPlot instance and not have
  the same orientation. If either of these restrictions is violated, a corresponding message is
  printed to the debug output (qDebug), the construction is not aborted, though.
  
  The constructed QCPFinancial can be added to the plot with QCustomPlot::addPlottable, QCustomPlot
  then takes ownership of the financial chart.
*/
QCPFinancial::QCPFinancial(QCPAxis *keyAxis, QCPAxis *valueAxis) :
  QCPAbstractPlottable(keyAxis, valueAxis),
  mData(0),
  mChartStyle(csOhlc),
  mWidth(0.5),
  mTwoColored(false),
  mBrushPositive(QBrush(QColor(200, 200, 255))),
  mBrushNegative(QBrush(QColor(255, 200, 200))),
  mPenPositive(QPen(QColor(0, 150, 255))),
  mPenNegative(QPen(QColor(255, 150, 0)))
{
  mData = new QCPFinancialDataMap;
  
}

QCPFinancial::~QCPFinancial()
{
  delete mData;
}

/*!
  Replaces the current data with the provided \a data.
  
  If \a copy is set to true, data points in \a data will only be copied. if false, the plottable
  takes ownership of the passed data and replaces the internal data pointer with it. This is
  significantly faster than copying for large datasets.
  
  Alternatively, you can also access and modify the plottable's data via the \ref data method, which
  returns a pointer to the internal \ref QCPFinancialDataMap.
*/
void QCPFinancial::setData(QCPFinancialDataMap *data, bool copy)
{
  if (copy)
  {
    *mData = *data;
  } else
  {
    delete mData;
    mData = data;
  }
}

/*! \overload
  
  Replaces the current data with the provided open/high/low/close data. The provided vectors should
  have equal length. Else, the number of added points will be the size of the smallest vector.
*/
void QCPFinancial::setData(const QVector<double> &key, const QVector<double> &open, const QVector<double> &high, const QVector<double> &low, const QVector<double> &close)
{
  mData->clear();
  int n = key.size();
  n = qMin(n, open.size());
  n = qMin(n, high.size());
  n = qMin(n, low.size());
  n = qMin(n, close.size());
  for (int i=0; i<n; ++i)
  {
    mData->insertMulti(key[i], QCPFinancialData(key[i], open[i], high[i], low[i], close[i]));
  }
}

void QCPFinancial::setChartStyle(QCPFinancial::ChartStyle style)
{
  mChartStyle = style;
}

void QCPFinancial::setWidth(double width)
{
  mWidth = width;
}

void QCPFinancial::setTwoColored(bool twoColored)
{
  mTwoColored = twoColored;
}

void QCPFinancial::setBrushPositive(const QBrush &brush)
{
  mBrushPositive = brush;
}

void QCPFinancial::setBrushNegative(const QBrush &brush)
{
  mBrushNegative = brush;
}

void QCPFinancial::setPenPositive(const QPen &pen)
{
  mPenPositive = pen;
}

void QCPFinancial::setPenNegative(const QPen &pen)
{
  mPenNegative = pen;
}

/*!
  Adds the provided data points in \a dataMap to the current data.
  
  Alternatively, you can also access and modify the data via the \ref data method, which returns a
  pointer to the internal \ref QCPFinancialDataMap.
  
  \see removeData
*/
void QCPFinancial::addData(const QCPFinancialDataMap &dataMap)
{
  mData->unite(dataMap);
}

/*! \overload
  
  Adds the provided single data point in \a data to the current data.
  
  Alternatively, you can also access and modify the data via the \ref data method, which returns a
  pointer to the internal \ref QCPFinancialData.
  
  \see removeData
*/
void QCPFinancial::addData(const QCPFinancialData &data)
{
  mData->insertMulti(data.key, data);
}

/*! \overload
  
  Adds the provided single data point given by \a key, \a open, \a high, \a low, and \a close to the current data.
  
  Alternatively, you can also access and modify the data via the \ref data method, which returns a
  pointer to the internal \ref QCPFinancialData.
  
  \see removeData
*/
void QCPFinancial::addData(double key, double open, double high, double low, double close)
{
  mData->insertMulti(key, QCPFinancialData(key, open, high, low, close));
}

/*! \overload
  
  Adds the provided open/high/low/close data to the current data.
  
  Alternatively, you can also access and modify the data via the \ref data method, which returns a
  pointer to the internal \ref QCPFinancialData.
  
  \see removeData
*/
void QCPFinancial::addData(const QVector<double> &key, const QVector<double> &open, const QVector<double> &high, const QVector<double> &low, const QVector<double> &close)
{
  int n = key.size();
  n = qMin(n, open.size());
  n = qMin(n, high.size());
  n = qMin(n, low.size());
  n = qMin(n, close.size());
  for (int i=0; i<n; ++i)
  {
    mData->insertMulti(key[i], QCPFinancialData(key[i], open[i], high[i], low[i], close[i]));
  }
}

/*!
  Removes all data points with keys smaller than \a key.
  
  \see addData, clearData
*/
void QCPFinancial::removeDataBefore(double key)
{
  QCPFinancialDataMap::iterator it = mData->begin();
  while (it != mData->end() && it.key() < key)
    it = mData->erase(it);
}

/*!
  Removes all data points with keys greater than \a key.
  
  \see addData, clearData
*/
void QCPFinancial::removeDataAfter(double key)
{
  if (mData->isEmpty()) return;
  QCPFinancialDataMap::iterator it = mData->upperBound(key);
  while (it != mData->end())
    it = mData->erase(it);
}

/*!
  Removes all data points with keys between \a fromKey and \a toKey. if \a fromKey is greater or
  equal to \a toKey, the function does nothing. To remove a single data point with known key, use
  \ref removeData(double key).
  
  \see addData, clearData
*/
void QCPFinancial::removeData(double fromKey, double toKey)
{
  if (fromKey >= toKey || mData->isEmpty()) return;
  QCPFinancialDataMap::iterator it = mData->upperBound(fromKey);
  QCPFinancialDataMap::iterator itEnd = mData->upperBound(toKey);
  while (it != itEnd)
    it = mData->erase(it);
}

/*! \overload
  
  Removes a single data point at \a key. If the position is not known with absolute precision,
  consider using \ref removeData(double fromKey, double toKey) with a small fuzziness interval
  around the suspected position, depeding on the precision with which the key is known.

  \see addData, clearData
*/
void QCPFinancial::removeData(double key)
{
  mData->remove(key);
}

/*!
  Removes all data points.
  
  \see removeData, removeDataAfter, removeDataBefore
*/
void QCPFinancial::clearData()
{
  mData->clear();
}

/* inherits documentation from base class */
double QCPFinancial::selectTest(const QPointF &pos, bool onlySelectable, QVariant *details) const
{
  return -1;
}

void QCPFinancial::draw(QCPPainter *painter)
{
  // get visible data range:
  QCPFinancialDataMap::const_iterator lower, upper; // note that upper is the actual upper point, and not 1 step after the upper point
  getVisibleDataBounds(lower, upper);
  if (lower == mData->constEnd() || upper == mData->constEnd())
    return;
  
  // draw visible data range according to configured style:
  switch (mChartStyle)
  {
    case QCPFinancial::csOhlc:
      drawOhlcPlot(painter, lower, upper+1); break;
    case QCPFinancial::csCandlestick:
      drawCandlestickPlot(painter, lower, upper+1); break;
  }
}

void QCPFinancial::drawLegendIcon(QCPPainter *painter, const QRectF &rect) const
{
  
}

QCPRange QCPFinancial::getKeyRange(bool &foundRange, QCPAbstractPlottable::SignDomain inSignDomain) const
{
  
}

QCPRange QCPFinancial::getValueRange(bool &foundRange, QCPAbstractPlottable::SignDomain inSignDomain) const
{
  
}

void QCPFinancial::drawOhlcPlot(QCPPainter *painter, const QCPFinancialDataMap::const_iterator &begin, const QCPFinancialDataMap::const_iterator &end)
{
  QCPAxis *keyAxis = mKeyAxis.data();
  QCPAxis *valueAxis = mValueAxis.data();
  if (!keyAxis || !valueAxis) { qDebug() << Q_FUNC_INFO << "invalid key or value axis"; return; }
  
  QPen linePen;
  
  if (keyAxis->orientation() == Qt::Horizontal)
  {
    for (QCPFinancialDataMap::const_iterator it = begin; it != end; ++it)
    {
      if (mSelected)
        linePen = mSelectedPen;
      else if (mTwoColored)
        linePen = it.value().close >= it.value().open ? mPenPositive : mPenNegative;
      else
        linePen = mPen;
      painter->setPen(linePen);
      double keyPixel = keyAxis->coordToPixel(it.value().key);
      double openPixel = valueAxis->coordToPixel(it.value().open);
      double closePixel = valueAxis->coordToPixel(it.value().close);
      // draw backbone:
      painter->drawLine(QPointF(keyPixel, valueAxis->coordToPixel(it.value().high)), QPointF(keyPixel, valueAxis->coordToPixel(it.value().low)));
      // draw open:
      double keyWidthPixels = keyPixel-keyAxis->coordToPixel(it.value().key-mWidth*0.5); // sign of this makes sure open/close are on correct sides
      painter->drawLine(QPointF(keyPixel-keyWidthPixels, openPixel), QPointF(keyPixel, openPixel));
      // draw close:
      painter->drawLine(QPointF(keyPixel, closePixel), QPointF(keyPixel+keyWidthPixels, closePixel));
    }
  } else
  {
    for (QCPFinancialDataMap::const_iterator it = begin; it != end; ++it)
    {
      if (mSelected)
        linePen = mSelectedPen;
      else if (mTwoColored)
        linePen = it.value().close >= it.value().open ? mPenPositive : mPenNegative;
      else
        linePen = mPen;
      painter->setPen(linePen);
      double keyPixel = keyAxis->coordToPixel(it.value().key);
      double openPixel = valueAxis->coordToPixel(it.value().open);
      double closePixel = valueAxis->coordToPixel(it.value().close);
      // draw backbone:
      painter->drawLine(QPointF(valueAxis->coordToPixel(it.value().high), keyPixel), QPointF(valueAxis->coordToPixel(it.value().low), keyPixel));
      // draw open:
      double keyWidthPixels = keyPixel-keyAxis->coordToPixel(it.value().key-mWidth*0.5); // sign of this makes sure open/close are on correct sides
      painter->drawLine(QPointF(openPixel, keyPixel-keyWidthPixels), QPointF(openPixel, keyPixel));
      // draw close:
      painter->drawLine(QPointF(closePixel, keyPixel), QPointF(closePixel, keyPixel+keyWidthPixels));
    }
  }
}

void QCPFinancial::drawCandlestickPlot(QCPPainter *painter, const QCPFinancialDataMap::const_iterator &begin, const QCPFinancialDataMap::const_iterator &end)
{
  
}

/*!  \internal
  
  called by the drawing methods to determine which data (key) range is visible at the current key
  axis range setting, so only that needs to be processed.
  
  \a lower returns an iterator to the lowest data point that needs to be taken into account when
  plotting. Note that in order to get a clean plot all the way to the edge of the axis rect, \a
  lower may still be just outside the visible range.
  
  \a upper returns an iterator to the highest data point. Same as before, \a upper may also lie
  just outside of the visible range.
  
  if the plottable contains no data, both \a lower and \a upper point to constEnd.
  
  \see QCPGraph::getVisibleDataBounds
*/
void QCPFinancial::getVisibleDataBounds(QCPFinancialDataMap::const_iterator &lower, QCPFinancialDataMap::const_iterator &upper) const
{
  if (!mKeyAxis) { qDebug() << Q_FUNC_INFO << "invalid key axis"; return; }
  if (mData->isEmpty())
  {
    lower = mData->constEnd();
    upper = mData->constEnd();
    return;
  }
  
  // get visible data range as QMap iterators
  QCPFinancialDataMap::const_iterator lbound = mData->lowerBound(mKeyAxis.data()->range().lower);
  QCPFinancialDataMap::const_iterator ubound = mData->upperBound(mKeyAxis.data()->range().upper);
  bool lowoutlier = lbound != mData->constBegin(); // indicates whether there exist points below axis range
  bool highoutlier = ubound != mData->constEnd(); // indicates whether there exist points above axis range
  
  lower = (lowoutlier ? lbound-1 : lbound); // data point range that will be actually drawn
  upper = (highoutlier ? ubound : ubound-1); // data point range that will be actually drawn
}
