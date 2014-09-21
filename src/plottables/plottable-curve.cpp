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

#include "plottable-curve.h"

#include "../painter.h"
#include "../core.h"
#include "../axis.h"
#include "../layoutelements/layoutelement-axisrect.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// QCPCurveData
////////////////////////////////////////////////////////////////////////////////////////////////////

/*! \class QCPCurveData
  \brief Holds the data of one single data point for QCPCurve.
  
  The container for storing multiple data points is \ref QCPCurveDataMap.
  
  The stored data is:
  \li \a t: the free parameter of the curve at this curve point (cp. the mathematical vector <em>(x(t), y(t))</em>)
  \li \a key: coordinate on the key axis of this curve point
  \li \a value: coordinate on the value axis of this curve point
  
  \see QCPCurveDataMap
*/

/*!
  Constructs a curve data point with t, key and value set to zero.
*/
QCPCurveData::QCPCurveData() :
  t(0),
  key(0),
  value(0)
{
}

/*!
  Constructs a curve data point with the specified \a t, \a key and \a value.
*/
QCPCurveData::QCPCurveData(double t, double key, double value) :
  t(t),
  key(key),
  value(value)
{
}


////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// QCPCurve
////////////////////////////////////////////////////////////////////////////////////////////////////

/*! \class QCPCurve
  \brief A plottable representing a parametric curve in a plot.
  
  \image html QCPCurve.png
  
  Unlike QCPGraph, plottables of this type may have multiple points with the same key coordinate,
  so their visual representation can have \a loops. This is realized by introducing a third
  coordinate \a t, which defines the order of the points described by the other two coordinates \a
  x and \a y.

  To plot data, assign it with the \ref setData or \ref addData functions.
  
  \section appearance Changing the appearance
  
  The appearance of the curve is determined by the pen and the brush (\ref setPen, \ref setBrush).
  \section usage Usage
  
  Like all data representing objects in QCustomPlot, the QCPCurve is a plottable (QCPAbstractPlottable). So
  the plottable-interface of QCustomPlot applies (QCustomPlot::plottable, QCustomPlot::addPlottable, QCustomPlot::removePlottable, etc.)
  
  Usually, you first create an instance:
  \code
  QCPCurve *newCurve = new QCPCurve(customPlot->xAxis, customPlot->yAxis);\endcode
  add it to the customPlot with QCustomPlot::addPlottable:
  \code
  customPlot->addPlottable(newCurve);\endcode
  and then modify the properties of the newly created plottable, e.g.:
  \code
  newCurve->setName("Fermat's Spiral");
  newCurve->setData(tData, xData, yData);\endcode
*/

/*!
  Constructs a curve which uses \a keyAxis as its key axis ("x") and \a valueAxis as its value
  axis ("y"). \a keyAxis and \a valueAxis must reside in the same QCustomPlot instance and not have
  the same orientation. If either of these restrictions is violated, a corresponding message is
  printed to the debug output (qDebug), the construction is not aborted, though.
  
  The constructed QCPCurve can be added to the plot with QCustomPlot::addPlottable, QCustomPlot
  then takes ownership of the graph.
*/
QCPCurve::QCPCurve(QCPAxis *keyAxis, QCPAxis *valueAxis) :
  QCPAbstractPlottable(keyAxis, valueAxis)
{
  mData = new QCPCurveDataMap;
  mPen.setColor(Qt::blue);
  mPen.setStyle(Qt::SolidLine);
  mBrush.setColor(Qt::blue);
  mBrush.setStyle(Qt::NoBrush);
  mSelectedPen = mPen;
  mSelectedPen.setWidthF(2.5);
  mSelectedPen.setColor(QColor(80, 80, 255)); // lighter than Qt::blue of mPen
  mSelectedBrush = mBrush;
  
  setScatterStyle(QCPScatterStyle());
  setLineStyle(lsLine);
}

QCPCurve::~QCPCurve()
{
  delete mData;
}

/*!
  Replaces the current data with the provided \a data.
  
  If \a copy is set to true, data points in \a data will only be copied. if false, the plottable
  takes ownership of the passed data and replaces the internal data pointer with it. This is
  significantly faster than copying for large datasets.
*/
void QCPCurve::setData(QCPCurveDataMap *data, bool copy)
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
  
  Replaces the current data with the provided points in \a t, \a key and \a value tuples. The
  provided vectors should have equal length. Else, the number of added points will be the size of
  the smallest vector.
*/
void QCPCurve::setData(const QVector<double> &t, const QVector<double> &key, const QVector<double> &value)
{
  mData->clear();
  int n = t.size();
  n = qMin(n, key.size());
  n = qMin(n, value.size());
  QCPCurveData newData;
  for (int i=0; i<n; ++i)
  {
    newData.t = t[i];
    newData.key = key[i];
    newData.value = value[i];
    mData->insertMulti(newData.t, newData);
  }
}

/*! \overload
  
  Replaces the current data with the provided \a key and \a value pairs. The t parameter
  of each data point will be set to the integer index of the respective key/value pair.
*/
void QCPCurve::setData(const QVector<double> &key, const QVector<double> &value)
{
  mData->clear();
  int n = key.size();
  n = qMin(n, value.size());
  QCPCurveData newData;
  for (int i=0; i<n; ++i)
  {
    newData.t = i; // no t vector given, so we assign t the index of the key/value pair
    newData.key = key[i];
    newData.value = value[i];
    mData->insertMulti(newData.t, newData);
  }
}

/*!
  Sets the visual appearance of single data points in the plot. If set to \ref
  QCPScatterStyle::ssNone, no scatter points are drawn (e.g. for line-only plots with appropriate
  line style).
  
  \see QCPScatterStyle, setLineStyle
*/
void QCPCurve::setScatterStyle(const QCPScatterStyle &style)
{
  mScatterStyle = style;
}

/*!
  Sets how the single data points are connected in the plot or how they are represented visually
  apart from the scatter symbol. For scatter-only plots, set \a style to \ref lsNone and \ref
  setScatterStyle to the desired scatter style.
  
  \see setScatterStyle
*/
void QCPCurve::setLineStyle(QCPCurve::LineStyle style)
{
  mLineStyle = style;
}

/*!
  Adds the provided data points in \a dataMap to the current data.
  \see removeData
*/
void QCPCurve::addData(const QCPCurveDataMap &dataMap)
{
  mData->unite(dataMap);
}

/*! \overload
  Adds the provided single data point in \a data to the current data.
  \see removeData
*/
void QCPCurve::addData(const QCPCurveData &data)
{
  mData->insertMulti(data.t, data);
}

/*! \overload
  Adds the provided single data point as \a t, \a key and \a value tuple to the current data
  \see removeData
*/
void QCPCurve::addData(double t, double key, double value)
{
  QCPCurveData newData;
  newData.t = t;
  newData.key = key;
  newData.value = value;
  mData->insertMulti(newData.t, newData);
}

/*! \overload
  
  Adds the provided single data point as \a key and \a value pair to the current data The t
  parameter of the data point is set to the t of the last data point plus 1. If there is no last
  data point, t will be set to 0.
  
  \see removeData
*/
void QCPCurve::addData(double key, double value)
{
  QCPCurveData newData;
  if (!mData->isEmpty())
    newData.t = (mData->constEnd()-1).key()+1;
  else
    newData.t = 0;
  newData.key = key;
  newData.value = value;
  mData->insertMulti(newData.t, newData);
}

/*! \overload
  Adds the provided data points as \a t, \a key and \a value tuples to the current data.
  \see removeData
*/
void QCPCurve::addData(const QVector<double> &ts, const QVector<double> &keys, const QVector<double> &values)
{
  int n = ts.size();
  n = qMin(n, keys.size());
  n = qMin(n, values.size());
  QCPCurveData newData;
  for (int i=0; i<n; ++i)
  {
    newData.t = ts[i];
    newData.key = keys[i];
    newData.value = values[i];
    mData->insertMulti(newData.t, newData);
  }
}

/*!
  Removes all data points with curve parameter t smaller than \a t.
  \see addData, clearData
*/
void QCPCurve::removeDataBefore(double t)
{
  QCPCurveDataMap::iterator it = mData->begin();
  while (it != mData->end() && it.key() < t)
    it = mData->erase(it);
}

/*!
  Removes all data points with curve parameter t greater than \a t.
  \see addData, clearData
*/
void QCPCurve::removeDataAfter(double t)
{
  if (mData->isEmpty()) return;
  QCPCurveDataMap::iterator it = mData->upperBound(t);
  while (it != mData->end())
    it = mData->erase(it);
}

/*!
  Removes all data points with curve parameter t between \a fromt and \a tot. if \a fromt is
  greater or equal to \a tot, the function does nothing. To remove a single data point with known
  t, use \ref removeData(double t).
  
  \see addData, clearData
*/
void QCPCurve::removeData(double fromt, double tot)
{
  if (fromt >= tot || mData->isEmpty()) return;
  QCPCurveDataMap::iterator it = mData->upperBound(fromt);
  QCPCurveDataMap::iterator itEnd = mData->upperBound(tot);
  while (it != itEnd)
    it = mData->erase(it);
}

/*! \overload
  
  Removes a single data point at curve parameter \a t. If the position is not known with absolute
  precision, consider using \ref removeData(double fromt, double tot) with a small fuzziness
  interval around the suspected position, depeding on the precision with which the curve parameter
  is known.
  
  \see addData, clearData
*/
void QCPCurve::removeData(double t)
{
  mData->remove(t);
}

/*!
  Removes all data points.
  \see removeData, removeDataAfter, removeDataBefore
*/
void QCPCurve::clearData()
{
  mData->clear();
}

/* inherits documentation from base class */
double QCPCurve::selectTest(const QPointF &pos, bool onlySelectable, QVariant *details) const
{
  Q_UNUSED(details)
  if ((onlySelectable && !mSelectable) || mData->isEmpty())
    return -1;
  if (!mKeyAxis || !mValueAxis) { qDebug() << Q_FUNC_INFO << "invalid key or value axis"; return -1; }
  
  if (mKeyAxis.data()->axisRect()->rect().contains(pos.toPoint()))
    return pointDistance(pos);
  else
    return -1;
}

/* inherits documentation from base class */
void QCPCurve::draw(QCPPainter *painter)
{
  if (mData->isEmpty()) return;
  
  // allocate line vector:
  QVector<QPointF> *lineData = new QVector<QPointF>;
  
  // fill with curve data:
  getCurveData(lineData);
  
  // check data validity if flag set:
#ifdef QCUSTOMPLOT_CHECK_DATA
  QCPCurveDataMap::const_iterator it;
  for (it = mData->constBegin(); it != mData->constEnd(); ++it)
  {
    if (QCP::isInvalidData(it.value().t) ||
        QCP::isInvalidData(it.value().key, it.value().value))
      qDebug() << Q_FUNC_INFO << "Data point at" << it.key() << "invalid." << "Plottable name:" << name();
  }
#endif
  
  // draw curve fill:
  if (mainBrush().style() != Qt::NoBrush && mainBrush().color().alpha() != 0)
  {
    applyFillAntialiasingHint(painter);
    painter->setPen(Qt::NoPen);
    painter->setBrush(mainBrush());
    painter->drawPolygon(QPolygonF(*lineData));
  }
  
  // draw curve line:
  if (mLineStyle != lsNone && mainPen().style() != Qt::NoPen && mainPen().color().alpha() != 0)
  {
    applyDefaultAntialiasingHint(painter);
    painter->setPen(mainPen());
    painter->setBrush(Qt::NoBrush);
    // if drawing solid line and not in PDF, use much faster line drawing instead of polyline:
    if (mParentPlot->plottingHints().testFlag(QCP::phFastPolylines) &&
        painter->pen().style() == Qt::SolidLine &&
        !painter->modes().testFlag(QCPPainter::pmVectorized) &&
        !painter->modes().testFlag(QCPPainter::pmNoCaching))
    {
      for (int i=1; i<lineData->size(); ++i)
        painter->drawLine(lineData->at(i-1), lineData->at(i));
    } else
    {
      painter->drawPolyline(QPolygonF(*lineData));
    }
  }
  
  // draw scatters:
  if (!mScatterStyle.isNone())
    drawScatterPlot(painter, lineData);
  
  // free allocated line data:
  delete lineData;
}

/* inherits documentation from base class */
void QCPCurve::drawLegendIcon(QCPPainter *painter, const QRectF &rect) const
{
  // draw fill:
  if (mBrush.style() != Qt::NoBrush)
  {
    applyFillAntialiasingHint(painter);
    painter->fillRect(QRectF(rect.left(), rect.top()+rect.height()/2.0, rect.width(), rect.height()/3.0), mBrush);
  }
  // draw line vertically centered:
  if (mLineStyle != lsNone)
  {
    applyDefaultAntialiasingHint(painter);
    painter->setPen(mPen);
    painter->drawLine(QLineF(rect.left(), rect.top()+rect.height()/2.0, rect.right()+5, rect.top()+rect.height()/2.0)); // +5 on x2 else last segment is missing from dashed/dotted pens
  }
  // draw scatter symbol:
  if (!mScatterStyle.isNone())
  {
    applyScattersAntialiasingHint(painter);
    // scale scatter pixmap if it's too large to fit in legend icon rect:
    if (mScatterStyle.shape() == QCPScatterStyle::ssPixmap && (mScatterStyle.pixmap().size().width() > rect.width() || mScatterStyle.pixmap().size().height() > rect.height()))
    {
      QCPScatterStyle scaledStyle(mScatterStyle);
      scaledStyle.setPixmap(scaledStyle.pixmap().scaled(rect.size().toSize(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
      scaledStyle.applyTo(painter, mPen);
      scaledStyle.drawShape(painter, QRectF(rect).center());
    } else
    {
      mScatterStyle.applyTo(painter, mPen);
      mScatterStyle.drawShape(painter, QRectF(rect).center());
    }
  }
}

/*! \internal
  
  Draws scatter symbols at every data point passed in \a pointData. scatter symbols are independent of
  the line style and are always drawn if scatter shape is not \ref QCPScatterStyle::ssNone.
*/
void QCPCurve::drawScatterPlot(QCPPainter *painter, const QVector<QPointF> *pointData) const
{
  // draw scatter point symbols:
  applyScattersAntialiasingHint(painter);
  mScatterStyle.applyTo(painter, mPen);
  for (int i=0; i<pointData->size(); ++i)
    mScatterStyle.drawShape(painter,  pointData->at(i));
}

/*! \internal
  
  called by QCPCurve::draw to generate a point vector (pixels) which represents the line of the
  curve. Line segments that aren't visible in the current axis rect are handled in an optimized
  way.
*/
void QCPCurve::getCurveData(QVector<QPointF> *lineData) const
{
  QCPAxis *keyAxis = mKeyAxis.data();
  QCPAxis *valueAxis = mValueAxis.data();
  if (!keyAxis || !valueAxis) { qDebug() << Q_FUNC_INFO << "invalid key or value axis"; return; }
  
  // add margins to rect to compensate for stroke width
  // TODO: Properly apply strokeMargin. Currently only added, but must be subtracted on certain axes/sides
  double strokeMargin = mainPen().widthF()*0.55; // stroke radius + 10% safety
  double rectLeft = keyAxis->pixelToCoord(keyAxis->coordToPixel(keyAxis->range().lower)+strokeMargin);
  double rectRight = keyAxis->pixelToCoord(keyAxis->coordToPixel(keyAxis->range().upper)+strokeMargin);
  double rectBottom = valueAxis->pixelToCoord(valueAxis->coordToPixel(valueAxis->range().lower)+strokeMargin);
  double rectTop = valueAxis->pixelToCoord(valueAxis->coordToPixel(valueAxis->range().upper)+strokeMargin);
  lineData->reserve(mData->size()); // TODO: is this reasonable?
  int currentRegion;
  QCPCurveDataMap::const_iterator it = mData->constBegin();
  QCPCurveDataMap::const_iterator prevIt = mData->constEnd()-1;
  int prevRegion = getRegion(prevIt.value().key, prevIt.value().value, rectLeft, rectTop, rectRight, rectBottom);
  QVector<QPointF> trailingPoints; // points that must be applied after all other points (are generated only when handling first point to get virtual segment between last and first point right)
  while (it != mData->constEnd())
  {
    currentRegion = getRegion(it.value().key, it.value().value, rectLeft, rectTop, rectRight, rectBottom);
    if (currentRegion != prevRegion) // changed region, possibly need to add some optimized edge points or original points if entering R
    {
      if (currentRegion != 5) // segment doesn't end in R, so it's a candidate for removal
      {
        QPointF crossA, crossB;
        if (prevRegion == 5) // we're coming from R, so add this point optimized
        {
          lineData->append(getOptimizedPoint(currentRegion, it.value().key, it.value().value, prevIt.value().key, prevIt.value().value, rectLeft, rectTop, rectRight, rectBottom));
          // in the situations 5->1/7/9/3 the segment may leave R and directly cross through two outer regions. In these cases we need to add an additional corner point
          *lineData << getOptimizedCornerPoints(prevRegion, currentRegion, prevIt.value().key, prevIt.value().value, it.value().key, it.value().value, rectLeft, rectTop, rectRight, rectBottom);
        } else if (mayTraverse(prevRegion, currentRegion) &&
                   getTraverse(prevIt.value().key, prevIt.value().value, it.value().key, it.value().value, rectLeft, rectTop, rectRight, rectBottom, crossA, crossB))
        {
          // add the two cross points optimized if segment crosses R and if segment isn't virtual zeroth segment between last and first curve point:
          QVector<QPointF> beforeTraverseCornerPoints, afterTraverseCornerPoints;
          getTraverseCornerPoints(prevRegion, currentRegion, rectLeft, rectTop, rectRight, rectBottom, beforeTraverseCornerPoints, afterTraverseCornerPoints);
          if (it != mData->constBegin())
          {
            *lineData << beforeTraverseCornerPoints;
            lineData->append(crossA);
            lineData->append(crossB);
            *lineData << afterTraverseCornerPoints;
          } else
          {
            lineData->append(crossB);
            *lineData << afterTraverseCornerPoints;
            trailingPoints << beforeTraverseCornerPoints << crossA ;
          }
        } else // doesn't cross R, line is just moving around in outside regions, so only need to add optimized point(s) at the boundary corner(s)
        {
          *lineData << getOptimizedCornerPoints(prevRegion, currentRegion, prevIt.value().key, prevIt.value().value, it.value().key, it.value().value, rectLeft, rectTop, rectRight, rectBottom);
          // TODO: Boundary cross id prevRegion*10+currentRegion. check that we didn't just cross in the other direction. If not, add to cross id stack
        }
      } else // segment does end in R, so we add previous point optimized and this point at original position
      {
        if (it == mData->constBegin()) // it is first point in curve and prevIt is last one. So save optimized point for adding it to the lineData in the end
          trailingPoints << getOptimizedPoint(prevRegion, prevIt.value().key, prevIt.value().value, it.value().key, it.value().value, rectLeft, rectTop, rectRight, rectBottom);
        else
          lineData->append(getOptimizedPoint(prevRegion, prevIt.value().key, prevIt.value().value, it.value().key, it.value().value, rectLeft, rectTop, rectRight, rectBottom));
        lineData->append(coordsToPixels(it.value().key, it.value().value));
      }
    } else // region didn't change
    {
      if (currentRegion == 5) // still in R, keep adding original points
      {
        lineData->append(coordsToPixels(it.value().key, it.value().value));
      } else // still outside R, no need to add anything
      {
        // see how this is not doing anything? That's the main optimization...
      }
    }
    prevIt = it;
    prevRegion = currentRegion;
    ++it;
  }
  *lineData << trailingPoints;
}

void QCPCurve::getTraverseCornerPoints(int prevRegion, int currentRegion, double rectLeft, double rectTop, double rectRight, double rectBottom, QVector<QPointF> &beforeTraverse, QVector<QPointF> &afterTraverse) const
{
  // This method assumes that upon going from prevRegion to currentRegion, R is crossed
  switch (prevRegion)
  {
    case 1:
    {
      switch (currentRegion)
      {
        case 6: { beforeTraverse << coordsToPixels(rectLeft, rectTop); break; }
        case 9: { beforeTraverse << coordsToPixels(rectLeft, rectTop); afterTraverse << coordsToPixels(rectRight, rectBottom); break; }
        case 8: { beforeTraverse << coordsToPixels(rectLeft, rectTop); break; }
      }
      break;
    }
    case 2:
    {
      switch (currentRegion)
      {
        case 7: { afterTraverse << coordsToPixels(rectRight, rectTop); break; }
        case 9: { afterTraverse << coordsToPixels(rectRight, rectBottom); break; }
      }
      break;
    }
    case 3:
    {
      switch (currentRegion)
      {
        case 4: { beforeTraverse << coordsToPixels(rectLeft, rectBottom); break; }
        case 7: { beforeTraverse << coordsToPixels(rectLeft, rectBottom); afterTraverse << coordsToPixels(rectRight, rectTop); break; }
        case 8: { beforeTraverse << coordsToPixels(rectLeft, rectBottom); break; }
      }
      break;
    }
    case 4:
    {
      switch (currentRegion)
      {
        case 3: { afterTraverse << coordsToPixels(rectLeft, rectBottom); break; }
        case 9: { afterTraverse << coordsToPixels(rectRight, rectBottom); break; }
      }
      break;
    }
    case 5: { break; } // shouldn't happen because this method only handles full traverses
    case 6:
    {
      switch (currentRegion)
      {
        case 1: { afterTraverse << coordsToPixels(rectLeft, rectTop); break; }
        case 7: { afterTraverse << coordsToPixels(rectRight, rectTop); break; }
      }
      break;
    }
    case 7:
    {
      switch (currentRegion)
      {
        case 2: { beforeTraverse << coordsToPixels(rectRight, rectTop); break; }
        case 3: { beforeTraverse << coordsToPixels(rectRight, rectTop); afterTraverse << coordsToPixels(rectLeft, rectBottom); break; }
        case 6: { beforeTraverse << coordsToPixels(rectRight, rectTop); break; }
      }
      break;
    }
    case 8:
    {
      switch (currentRegion)
      {
        case 1: { afterTraverse << coordsToPixels(rectLeft, rectTop); break; }
        case 3: { afterTraverse << coordsToPixels(rectLeft, rectBottom); break; }
      }
      break;
    }
    case 9:
    {
      switch (currentRegion)
      {
        case 2: { beforeTraverse << coordsToPixels(rectRight, rectBottom); break; }
        case 1: { beforeTraverse << coordsToPixels(rectRight, rectBottom); afterTraverse << coordsToPixels(rectLeft, rectTop); break; }
        case 4: { beforeTraverse << coordsToPixels(rectRight, rectBottom); break; }
      }
      break;
    }
  }
}

QVector<QPointF> QCPCurve::getOptimizedCornerPoints(int prevRegion, int currentRegion, double prevKey, double prevValue, double key, double value, double rectLeft, double rectTop, double rectRight, double rectBottom) const
{
  QVector<QPointF> result;
  switch (prevRegion)
  {
    case 1:
    {
      switch (currentRegion)
      {
        case 2: { result << coordsToPixels(rectLeft, rectTop); break; }
        case 4: { result << coordsToPixels(rectLeft, rectTop); break; }
        case 3: { result << coordsToPixels(rectLeft, rectTop) << coordsToPixels(rectLeft, rectBottom); break; }
        case 7: { result << coordsToPixels(rectLeft, rectTop) << coordsToPixels(rectRight, rectTop); break; }
        case 6: { result << coordsToPixels(rectLeft, rectTop) << coordsToPixels(rectLeft, rectBottom); result.append(result.last()); break; }
        case 8: { result << coordsToPixels(rectLeft, rectTop) << coordsToPixels(rectRight, rectTop); result.append(result.last()); break; }
        case 9: { // in this case we need another distinction of cases: segment may pass below or above rect, requiring either bottom right or top left corner points
          if ((value-prevValue)/(key-prevKey)*(rectLeft-key)+value < rectBottom) // segment passes below R
          { result << coordsToPixels(rectLeft, rectTop) << coordsToPixels(rectLeft, rectBottom); result.append(result.last()); result << coordsToPixels(rectRight, rectBottom); }
          else
          { result << coordsToPixels(rectLeft, rectTop) << coordsToPixels(rectRight, rectTop); result.append(result.last()); result << coordsToPixels(rectRight, rectBottom); }
          break;
        }
      }
      break;
    }
    case 2:
    {
      switch (currentRegion)
      {
        case 1: { result << coordsToPixels(rectLeft, rectTop); break; }
        case 3: { result << coordsToPixels(rectLeft, rectBottom); break; }
        case 4: { result << coordsToPixels(rectLeft, rectTop); result.append(result.last()); break; }
        case 6: { result << coordsToPixels(rectLeft, rectBottom); result.append(result.last()); break; }
        case 7: { result << coordsToPixels(rectLeft, rectTop); result.append(result.last()); result << coordsToPixels(rectRight, rectTop); break; }
        case 9: { result << coordsToPixels(rectLeft, rectBottom); result.append(result.last()); result << coordsToPixels(rectRight, rectBottom); break; }
      }
      break;
    }
    case 3:
    {
      switch (currentRegion)
      {
        case 2: { result << coordsToPixels(rectLeft, rectBottom); break; }
        case 6: { result << coordsToPixels(rectLeft, rectBottom); break; }
        case 1: { result << coordsToPixels(rectLeft, rectBottom) << coordsToPixels(rectLeft, rectTop); break; }
        case 9: { result << coordsToPixels(rectLeft, rectBottom) << coordsToPixels(rectRight, rectBottom); break; }
        case 4: { result << coordsToPixels(rectLeft, rectBottom) << coordsToPixels(rectLeft, rectTop); result.append(result.last()); break; }
        case 8: { result << coordsToPixels(rectLeft, rectBottom) << coordsToPixels(rectRight, rectBottom); result.append(result.last()); break; }
        case 7: { // in this case we need another distinction of cases: segment may pass below or above rect, requiring either bottom right or top left corner points
          if ((value-prevValue)/(key-prevKey)*(rectRight-key)+value < rectBottom) // segment passes below R
          { result << coordsToPixels(rectLeft, rectBottom) << coordsToPixels(rectRight, rectBottom); result.append(result.last()); result << coordsToPixels(rectRight, rectTop); }
          else
          { result << coordsToPixels(rectLeft, rectBottom) << coordsToPixels(rectLeft, rectTop); result.append(result.last()); result << coordsToPixels(rectRight, rectTop); }
          break;
        }
      }
      break;
    }
    case 4:
    {
      switch (currentRegion)
      {
        case 1: { result << coordsToPixels(rectLeft, rectTop); break; }
        case 7: { result << coordsToPixels(rectRight, rectTop); break; }
        case 2: { result << coordsToPixels(rectLeft, rectTop); result.append(result.last()); break; }
        case 8: { result << coordsToPixels(rectRight, rectTop); result.append(result.last()); break; }
        case 3: { result << coordsToPixels(rectLeft, rectTop); result.append(result.last()); result << coordsToPixels(rectLeft, rectBottom); break; }
        case 9: { result << coordsToPixels(rectRight, rectTop); result.append(result.last()); result << coordsToPixels(rectRight, rectBottom); break; }
      }
      break;
    }
    case 5:
    {
      switch (currentRegion)
      {
        case 1: { result << coordsToPixels(rectLeft, rectTop); break; }
        case 7: { result << coordsToPixels(rectRight, rectTop); break; }
        case 9: { result << coordsToPixels(rectRight, rectBottom); break; }
        case 3: { result << coordsToPixels(rectLeft, rectBottom); break; }
      }
      break;
    }
    case 6:
    {
      switch (currentRegion)
      {
        case 3: { result << coordsToPixels(rectLeft, rectBottom); break; }
        case 9: { result << coordsToPixels(rectRight, rectBottom); break; }
        case 2: { result << coordsToPixels(rectLeft, rectBottom); result.append(result.last()); break; }
        case 8: { result << coordsToPixels(rectRight, rectBottom); result.append(result.last()); break; }
        case 1: { result << coordsToPixels(rectLeft, rectBottom); result.append(result.last()); result << coordsToPixels(rectLeft, rectTop); break; }
        case 7: { result << coordsToPixels(rectRight, rectBottom); result.append(result.last()); result << coordsToPixels(rectRight, rectTop); break; }
      }
      break;
    }
    case 7:
    {
      switch (currentRegion)
      {
        case 4: { result << coordsToPixels(rectRight, rectTop); break; }
        case 8: { result << coordsToPixels(rectRight, rectTop); break; }
        case 1: { result << coordsToPixels(rectRight, rectTop) << coordsToPixels(rectLeft, rectTop); break; }
        case 9: { result << coordsToPixels(rectRight, rectTop) << coordsToPixels(rectRight, rectBottom); break; }
        case 2: { result << coordsToPixels(rectRight, rectTop) << coordsToPixels(rectLeft, rectTop); result.append(result.last()); break; }
        case 6: { result << coordsToPixels(rectRight, rectTop) << coordsToPixels(rectRight, rectBottom); result.append(result.last()); break; }
        case 3: { // in this case we need another distinction of cases: segment may pass below or above rect, requiring either bottom right or top left corner points
          if ((value-prevValue)/(key-prevKey)*(rectRight-key)+value < rectBottom) // segment passes below R
          { result << coordsToPixels(rectRight, rectTop) << coordsToPixels(rectRight, rectBottom); result.append(result.last()); result << coordsToPixels(rectLeft, rectBottom); }
          else
          { result << coordsToPixels(rectRight, rectTop) << coordsToPixels(rectLeft, rectTop); result.append(result.last()); result << coordsToPixels(rectLeft, rectBottom); }
          break;
        }
      }
      break;
    }
    case 8:
    {
      switch (currentRegion)
      {
        case 7: { result << coordsToPixels(rectRight, rectTop); break; }
        case 9: { result << coordsToPixels(rectRight, rectBottom); break; }
        case 4: { result << coordsToPixels(rectRight, rectTop); result.append(result.last()); break; }
        case 6: { result << coordsToPixels(rectRight, rectBottom); result.append(result.last()); break; }
        case 1: { result << coordsToPixels(rectRight, rectTop); result.append(result.last()); result << coordsToPixels(rectLeft, rectTop); break; }
        case 3: { result << coordsToPixels(rectRight, rectBottom); result.append(result.last()); result << coordsToPixels(rectLeft, rectBottom); break; }
      }
      break;
    }
    case 9:
    {
      switch (currentRegion)
      {
        case 6: { result << coordsToPixels(rectRight, rectBottom); break; }
        case 8: { result << coordsToPixels(rectRight, rectBottom); break; }
        case 3: { result << coordsToPixels(rectRight, rectBottom) << coordsToPixels(rectLeft, rectBottom); break; }
        case 7: { result << coordsToPixels(rectRight, rectBottom) << coordsToPixels(rectRight, rectTop); break; }
        case 2: { result << coordsToPixels(rectRight, rectBottom) << coordsToPixels(rectLeft, rectBottom); result.append(result.last()); break; }
        case 4: { result << coordsToPixels(rectRight, rectBottom) << coordsToPixels(rectRight, rectTop); result.append(result.last()); break; }
        case 1: { // in this case we need another distinction of cases: segment may pass below or above rect, requiring either bottom right or top left corner points
          if ((value-prevValue)/(key-prevKey)*(rectLeft-key)+value < rectBottom) // segment passes below R
          { result << coordsToPixels(rectRight, rectBottom) << coordsToPixels(rectLeft, rectBottom); result.append(result.last()); result << coordsToPixels(rectLeft, rectTop); }
          else
          { result << coordsToPixels(rectRight, rectBottom) << coordsToPixels(rectRight, rectTop); result.append(result.last()); result << coordsToPixels(rectLeft, rectTop); }
          break;
        }
      }
      break;
    }
  }
  return result;
}

bool QCPCurve::getTraverse(double prevKey, double prevValue, double key, double value, double rectLeft, double rectTop, double rectRight, double rectBottom, QPointF &crossA, QPointF &crossB) const
{
  QList<QVector2D> intersections; // x of QVector2D corresponds to key and y to value
  if (qFuzzyIsNull(key-prevKey)) // line is parallel to value axis
  {
    // due to region filter in mayTraverseR(), if line is parallel to value or key axis, R is traversed here
    intersections.append(QVector2D(key, rectBottom)); // direction will be taken care of at end of method
    intersections.append(QVector2D(key, rectTop));
  } else if (qFuzzyIsNull(value-prevValue)) // line is parallel to key axis
  {
    // due to region filter in mayTraverseR(), if line is parallel to value or key axis, R is traversed here
    intersections.append(QVector2D(rectLeft, value)); // direction will be taken care of at end of method
    intersections.append(QVector2D(rectRight, value));
  } else // line is skewed
  {
    double gamma;
    double keyPerValue = (key-prevKey)/(value-prevValue);
    // check top of rect:
    gamma = prevKey + (rectTop-prevValue)*keyPerValue;
    if (gamma >= rectLeft && gamma <= rectRight)
      intersections.append(QVector2D(gamma, rectTop));
    // check bottom of rect:
    gamma = prevKey + (rectBottom-prevValue)*keyPerValue;
    if (gamma >= rectLeft && gamma <= rectRight)
      intersections.append(QVector2D(gamma, rectBottom));
    double valuePerKey = 1.0/keyPerValue;
    // check left of rect:
    gamma = prevValue + (rectLeft-prevKey)*valuePerKey;
    if (gamma >= rectBottom && gamma <= rectTop)
      intersections.append(QVector2D(rectLeft, gamma));
    // check right of rect:
    gamma = prevValue + (rectRight-prevKey)*valuePerKey;
    if (gamma >= rectBottom && gamma <= rectTop)
      intersections.append(QVector2D(rectRight, gamma));
  }
  
  // handle cases where found points isn't exactly 2:
  if (intersections.size() > 2)
  {
    // line probably goes through corner of rect, and we got duplicate points there. single out the point pair with greatest distance in between:
    double distSqrMax = 0;
    QVector2D pv1, pv2;
    for (int i=0; i<intersections.size()-1; ++i)
    {
      for (int k=i+1; k<intersections.size(); ++k)
      {
        double distSqr = (intersections.at(i)-intersections.at(k)).lengthSquared();
        if (distSqr > distSqrMax)
        {
          pv1 = intersections.at(i);
          pv2 = intersections.at(k);
          distSqrMax = distSqr;
        }
      }
    }
    intersections = QList<QVector2D>() << pv1 << pv2;
  } else if (intersections.size() != 2)
  {
    // one or even zero points found (shouldn't happen unless line perfectly tangent to corner), no need to draw segment
    return false;
  }
  
  // possibly re-sort points so optimized point segment has same direction as original segment:
  if ((key-prevKey)*(intersections.at(1).x()-intersections.at(0).x()) + (value-prevValue)*(intersections.at(1).y()-intersections.at(0).y()) < 0) // scalar product of both segments < 0 -> opposite direction
    intersections.swap(0, 1);
  crossA = coordsToPixels(intersections.at(0).x(), intersections.at(0).y());
  crossB = coordsToPixels(intersections.at(1).x(), intersections.at(1).y());
  return true;
}

bool QCPCurve::mayTraverse(int prevRegion, int currentRegion) const
{
  switch (prevRegion)
  {
    case 1:
    {
      switch (currentRegion)
      {
        case 4:
        case 7:
        case 2:
        case 3: return false;
        default: return true;
      }
    }
    case 2:
    {
      switch (currentRegion)
      {
        case 1:
        case 3: return false;
        default: return true;
      }
    }
    case 3:
    {
      switch (currentRegion)
      {
        case 1:
        case 2:
        case 6:
        case 9: return false;
        default: return true;
      }
    }
    case 4:
    {
      switch (currentRegion)
      {
        case 1:
        case 7: return false;
        default: return true;
      }
    }
    case 5: return false; // should never occur
    case 6:
    {
      switch (currentRegion)
      {
        case 3:
        case 9: return false;
        default: return true;
      }
    }
    case 7:
    {
      switch (currentRegion)
      {
        case 1:
        case 4:
        case 8:
        case 9: return false;
        default: return true;
      }
    }
    case 8:
    {
      switch (currentRegion)
      {
        case 7:
        case 9: return false;
        default: return true;
      }
    }
    case 9:
    {
      switch (currentRegion)
      {
        case 3:
        case 6:
        case 8:
        case 7: return false;
        default: return true;
      }
    }
    default: return true;
  }
}

QPointF QCPCurve::getOptimizedPoint(int otherRegion, double otherKey, double otherValue, double key, double value, double rectLeft, double rectTop, double rectRight, double rectBottom) const
{
  // this method assumes segment goes from R to otherRegion or from otherRegion to R
  double intersectKey = rectLeft; // initial value is just fail-safe
  double intersectValue = rectTop; // initial value is just fail-safe
  switch (otherRegion)
  {
    case 1: // top and left edge
    {
      intersectValue = rectTop;
      intersectKey = otherKey + (key-otherKey)/(value-otherValue)*(intersectValue-otherValue);
      if (intersectKey < rectLeft || intersectKey > rectRight) // doesn't intersect, so must intersect other:
      {
        intersectKey = rectLeft;
        intersectValue = otherValue + (value-otherValue)/(key-otherKey)*(intersectKey-otherKey);
      }
      break;
    }
    case 2: // left edge
    {
      intersectKey = rectLeft;
      intersectValue = otherValue + (value-otherValue)/(key-otherKey)*(intersectKey-otherKey);
      break;
    }
    case 3: // bottom and left edge
    {
      intersectValue = rectBottom;
      intersectKey = otherKey + (key-otherKey)/(value-otherValue)*(intersectValue-otherValue);
      if (intersectKey < rectLeft || intersectKey > rectRight) // doesn't intersect, so must intersect other:
      {
        intersectKey = rectLeft;
        intersectValue = otherValue + (value-otherValue)/(key-otherKey)*(intersectKey-otherKey);
      }
      break;
    }
    case 4: // top edge
    {
      intersectValue = rectTop;
      intersectKey = otherKey + (key-otherKey)/(value-otherValue)*(intersectValue-otherValue);
      break;
    }
    case 5:
    {
      break; // case 5 shouldn't happen for this function but we add it anyway to prevent potential discontinuity in branch table
    }
    case 6: // bottom edge
    {
      intersectValue = rectBottom;
      intersectKey = otherKey + (key-otherKey)/(value-otherValue)*(intersectValue-otherValue);
      break;
    }
    case 7: // top and right edge
    {
      intersectValue = rectTop;
      intersectKey = otherKey + (key-otherKey)/(value-otherValue)*(intersectValue-otherValue);
      if (intersectKey < rectLeft || intersectKey > rectRight) // doesn't intersect, so must intersect other:
      {
        intersectKey = rectRight;
        intersectValue = otherValue + (value-otherValue)/(key-otherKey)*(intersectKey-otherKey);
      }
      break;
    }
    case 8: // right edge
    {
      intersectKey = rectRight;
      intersectValue = otherValue + (value-otherValue)/(key-otherKey)*(intersectKey-otherKey);
      break;
    }
    case 9: // bottom and right edge
    {
      intersectValue = rectBottom;
      intersectKey = otherKey + (key-otherKey)/(value-otherValue)*(intersectValue-otherValue);
      if (intersectKey < rectLeft || intersectKey > rectRight) // doesn't intersect, so must intersect other:
      {
        intersectKey = rectRight;
        intersectValue = otherValue + (value-otherValue)/(key-otherKey)*(intersectKey-otherKey);
      }
      break;
    }
  }
  return coordsToPixels(intersectKey, intersectValue);
}

int QCPCurve::getRegion(double x, double y, double rectLeft, double rectTop, double rectRight, double rectBottom) const
{
  if (x < rectLeft) // region 123
  {
    if (y > rectTop)
      return 1;
    else if (y < rectBottom)
      return 3;
    else
      return 2;
  } else if (x > rectRight) // region 789
  {
    if (y > rectTop)
      return 7;
    else if (y < rectBottom)
      return 9;
    else
      return 8;
  } else // region 456
  {
    if (y > rectTop)
      return 4;
    else if (y < rectBottom)
      return 6;
    else
      return 5;
  }
}

/*! \internal
  
  Calculates the (minimum) distance (in pixels) the curve's representation has from the given \a
  pixelPoint in pixels. This is used to determine whether the curve was clicked or not, e.g. in
  \ref selectTest.
*/
double QCPCurve::pointDistance(const QPointF &pixelPoint) const
{
  if (mData->isEmpty())
  {
    qDebug() << Q_FUNC_INFO << "requested point distance on curve" << mName << "without data";
    return 500;
  }
  if (mData->size() == 1)
  {
    QPointF dataPoint = coordsToPixels(mData->constBegin().key(), mData->constBegin().value().value);
    return QVector2D(dataPoint-pixelPoint).length();
  }
  
  // calculate minimum distance to line segments:
  QVector<QPointF> *lineData = new QVector<QPointF>;
  getCurveData(lineData);
  double minDistSqr = std::numeric_limits<double>::max();
  for (int i=0; i<lineData->size()-1; ++i)
  {
    double currentDistSqr = distSqrToLine(lineData->at(i), lineData->at(i+1), pixelPoint);
    if (currentDistSqr < minDistSqr)
      minDistSqr = currentDistSqr;
  }
  delete lineData;
  return qSqrt(minDistSqr);
}

/*! \internal
  
  This is a specialized \ref coordsToPixels function for points that are outside the visible
  axisRect and just crossing a boundary (since \ref getCurveData reduces non-visible curve segments
  to those line segments that cross region boundaries, see documentation there). It only uses the
  coordinate parallel to the region boundary of the axisRect. The other coordinate is picked just
  outside the axisRect (how far is determined by the scatter size and the line width). Together
  with the optimization in \ref getCurveData this improves performance for large curves (or zoomed
  in ones) significantly while keeping the illusion the whole curve and its filling is still being
  drawn for the viewer.
*/
QPointF QCPCurve::outsideCoordsToPixels(double key, double value, int region, QRect axisRect) const
{
  int margin = qCeil(qMax(mScatterStyle.size(), (double)mPen.widthF())) + 2;
  QPointF result = coordsToPixels(key, value);
  switch (region)
  {
    case 2: result.setX(axisRect.left()-margin); break; // left
    case 8: result.setX(axisRect.right()+margin); break; // right
    case 4: result.setY(axisRect.top()-margin); break; // top
    case 6: result.setY(axisRect.bottom()+margin); break; // bottom
    case 1: result.setX(axisRect.left()-margin);
            result.setY(axisRect.top()-margin); break; // top left
    case 7: result.setX(axisRect.right()+margin);
            result.setY(axisRect.top()-margin); break; // top right
    case 9: result.setX(axisRect.right()+margin);
            result.setY(axisRect.bottom()+margin); break; // bottom right
    case 3: result.setX(axisRect.left()-margin);
            result.setY(axisRect.bottom()+margin); break; // bottom left
  }
  return result;
}

/* inherits documentation from base class */
QCPRange QCPCurve::getKeyRange(bool &foundRange, SignDomain inSignDomain) const
{
  QCPRange range;
  bool haveLower = false;
  bool haveUpper = false;
  
  double current;
  
  QCPCurveDataMap::const_iterator it = mData->constBegin();
  while (it != mData->constEnd())
  {
    current = it.value().key;
    if (inSignDomain == sdBoth || (inSignDomain == sdNegative && current < 0) || (inSignDomain == sdPositive && current > 0))
    {
      if (current < range.lower || !haveLower)
      {
        range.lower = current;
        haveLower = true;
      }
      if (current > range.upper || !haveUpper)
      {
        range.upper = current;
        haveUpper = true;
      }
    }
    ++it;
  }
  
  foundRange = haveLower && haveUpper;
  return range;
}

/* inherits documentation from base class */
QCPRange QCPCurve::getValueRange(bool &foundRange, SignDomain inSignDomain) const
{
  QCPRange range;
  bool haveLower = false;
  bool haveUpper = false;
  
  double current;
  
  QCPCurveDataMap::const_iterator it = mData->constBegin();
  while (it != mData->constEnd())
  {
    current = it.value().value;
    if (inSignDomain == sdBoth || (inSignDomain == sdNegative && current < 0) || (inSignDomain == sdPositive && current > 0))
    {
      if (current < range.lower || !haveLower)
      {
        range.lower = current;
        haveLower = true;
      }
      if (current > range.upper || !haveUpper)
      {
        range.upper = current;
        haveUpper = true;
      }
    }
    ++it;
  }
  
  foundRange = haveLower && haveUpper;
  return range;
}
