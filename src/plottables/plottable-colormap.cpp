/***************************************************************************
**                                                                        **
**  QCustomPlot, an easy to use, modern plotting widget for Qt            **
**  Copyright (C) 2011, 2012, 2013 Emanuel Eichhammer                     **
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
**             Date: 01.08.13                                             **
**          Version: 1.0.0                                                **
****************************************************************************/

#include "plottable-colormap.h"

//#include <QRgb>
#include "../painter.h"
#include "../core.h"
#include "../axis.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// QCPColorMapData
////////////////////////////////////////////////////////////////////////////////////////////////////

/*! \class QCPColorMapData
  \brief Holds the two-dimensional data of a QCPColorMap plottable.
  
*/

QCPColorMapData::QCPColorMapData(const QSize &size, const QCPRange keyRange, const QCPRange valueRange) :
  mData(0),
  mKeyRange(keyRange),
  mValueRange(valueRange),
  mModified(true),
  mIsEmpty(true)
{
  setSize(size);
  reset();
}

double QCPColorMapData::value(double key, double value)
{
  if (key >= mKeyRange.lower && key < mKeyRange.upper && value >= mValueRange.lower && value < mValueRange.upper)
  {
    int keyCell = (key-mKeyRange.lower)/(mKeyRange.upper-mKeyRange.lower)*mSize.width()+0.5;
    int valueCell = (1.0-(value-mValueRange.lower)/(mValueRange.upper-mValueRange.lower))*mSize.height()-0.5; // -0.5 since we inverted the scale
    return mData[valueCell*mSize.width() + keyCell];
  } else
    return 0;
}

double QCPColorMapData::cell(int key, int value)
{
  if (key >= 0 && key < mSize.width() && value >= 0 && value < mSize.height())
    return mData[value*mSize.width() + key];
  else
    return 0;
}

void QCPColorMapData::setSize(const QSize &size)
{
  if (size != mSize)
  {
    mSize = size;
    if (mData)
      delete mData;
    mData = new double[(mSize.width()+1)*(mSize.height()+1)];
    if (!mData)
      qDebug() << Q_FUNC_INFO << "out of memory for data dimensions "<< size.width() << "*" << size.height();
    mModified = true;
    mIsEmpty = mSize == QSize(1, 1);
  }
}

void QCPColorMapData::setValue(double key, double value, double z)
{
  if (key >= mKeyRange.lower && key < mKeyRange.upper && value >= mValueRange.lower && value < mValueRange.upper)
  {
    int keyCell = (key-mKeyRange.lower)/(mKeyRange.upper-mKeyRange.lower)*mSize.width()+0.5;
    int valueCell = (1.0-(value-mValueRange.lower)/(mValueRange.upper-mValueRange.lower))*mSize.height()-0.5; // -0.5 since we inverted the scale
    mData[valueCell*mSize.width() + keyCell] = z;
    if (z < mMinMax.lower)
      mMinMax.lower = z;
    if (z > mMinMax.upper)
      mMinMax.upper = z;
     mModified = true;
  }
}

void QCPColorMapData::setCell(int key, int value, double z)
{
  if (key >= 0 && key < mSize.width() && value >= 0 && value < mSize.height())
  {
    mData[value*mSize.width() + key] = z;
    if (z < mMinMax.lower)
      mMinMax.lower = z;
    if (z > mMinMax.upper)
      mMinMax.upper = z;
     mModified = true;
  }
}

void QCPColorMapData::setRange(const QCPRange keyRange, const QCPRange valueRange)
{
  mKeyRange = keyRange;
  mValueRange = valueRange;
}

void QCPColorMapData::setMinMax(const QCPRange minMax)
{
  if (!qFuzzyCompare(mMinMax.lower, minMax.lower) || !qFuzzyCompare(minMax.upper, minMax.upper))
  {
    mMinMax = minMax;
    mModified = true;
  }
}

void QCPColorMapData::recalculateMinMax()
{
  int maxValue = mSize.height();
  int maxKey = mSize.width();
  if (maxValue > 0 && maxKey > 0)
  {
    double minHeight = mData[0];
    double maxHeight = mData[0];
    for (int value=0; value<maxValue; ++value)
    {
      for (int key=0; key<maxKey; ++key)
      {
        if (mData[value*maxKey + key] > maxHeight)
          maxHeight = mData[value*maxKey + key];
        if (mData[value*maxKey + key] < minHeight)
          minHeight = mData[value*maxKey + key];
      }
    }
    mMinMax.lower = minHeight;
    mMinMax.upper = maxHeight;
    mModified = true;
  }
}

void QCPColorMapData::clear()
{
  setSize(QSize(1, 1));
}

void QCPColorMapData::reset(double z)
{
  int maxValue = mSize.height();
  int maxKey = mSize.width();
  for (int key=0; key<maxKey; ++key)
  {
    for (int value=0; value<maxValue; ++value)
    {
      mData[value*maxKey + key] = z;
    }
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// QCPColorMap
////////////////////////////////////////////////////////////////////////////////////////////////////

/*! \class QCPColorMap
  \brief A plottable representing a two-dimensional color map in a plot.

  To plot data, assign it with the \ref setData function.
  
  \section appearance Changing the appearance
  
  
  \section usage Usage
  
  Like all data representing objects in QCustomPlot, the QCPColorMap is a plottable
  (QCPAbstractPlottable). So the plottable-interface of QCustomPlot applies
  (QCustomPlot::plottable, QCustomPlot::addPlottable, QCustomPlot::removePlottable, etc.)
  
  Usually, you first create an instance:
  \code
  QCPColorMap *newMap = new QCPColorMap(customPlot->xAxis, customPlot->yAxis);\endcode
  add it to the customPlot with QCustomPlot::addPlottable:
  \code
  customPlot->addPlottable(newMap);\endcode
  and then modify the properties of the newly created plottable, e.g.:
  \code
  newBars->setName("Temperature Map");
  newBars->setData();\endcode // DBG
*/

QCPColorMap::QCPColorMap(QCPAxis *keyAxis, QCPAxis *valueAxis) :
  QCPAbstractPlottable(keyAxis, valueAxis),
  mColorGradient(0),
  mColorGradientLevels(0)
{
  updateGradient(500);
  mData = new QCPColorMapData(QSize(10, 10), QCPRange(0, 5), QCPRange(0, 5));
}

QCPColorMap::~QCPColorMap()
{
  delete mData;
}

/*!
  Replaces the current data with the provided \a data.
  
  If \a copy is set to true, data points in \a data will only be copied. if false, the plottable
  takes ownership of the passed data and replaces the internal data pointer with it. This is
  significantly faster than copying for large datasets.
*/
void QCPColorMap::setData(QCPColorMapData *data, bool copy)
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

/*!
  Removes all data points.
*/
void QCPColorMap::clearData()
{
  //mData->clear(); // DBG
}

/* inherits documentation from base class */
double QCPColorMap::selectTest(const QPointF &pos, bool onlySelectable, QVariant *details) const
{
  return -1; // DBG
}

void QCPColorMap::updateGradient(int levels)
{
  if (mColorGradient)
    delete mColorGradient;
  mColorGradient = new QRgb[levels+1];
  mColorGradientLevels = levels;
  
  for (int i=0; i<mColorGradientLevels+1; ++i)
    mColorGradient[i] = wavelengthToRgb(i/double(mColorGradientLevels) * 140 + 370);
}

void QCPColorMap::updateMapImage()
{
  if (mMapImage.size() != mData->size())
    mMapImage = QImage(mData->size(), QImage::Format_RGB32);
  int maxValue = mData->size().height();
  int maxKey = mData->size().width();
  double *rawData = mData->mData;
  double dataMin = mData->minMax().lower;
  double dataMax = mData->minMax().upper;
  double dataMaxMinNormalization = mColorGradientLevels/(dataMax-dataMin);
  for (int value=0; value<maxValue; ++value)
  {
    QRgb* bits = reinterpret_cast<QRgb*>(mMapImage.scanLine(value));
    for (int key=0; key<maxKey; ++key)
    {
      int v = (rawData[value*maxKey + key]-dataMin)*dataMaxMinNormalization;
      bits[key] = mColorGradient[v];
    }
  }
}

/* inherits documentation from base class */
void QCPColorMap::draw(QCPPainter *painter)
{
  if (mData->isEmpty()) return;
  applyDefaultAntialiasingHint(painter);
  
  if (mData->mModified)
    updateMapImage();
  
  // TODO: fix this for reversed axes
  QRectF imageRect(coordsToPixels(mData->keyRange().lower, mData->valueRange().upper), coordsToPixels(mData->keyRange().upper, mData->valueRange().lower));
  bool smoothBackup = painter->renderHints().testFlag(QPainter::SmoothPixmapTransform);
  painter->setRenderHint(QPainter::SmoothPixmapTransform, true);
  painter->drawImage(imageRect, mMapImage);
  painter->setRenderHint(QPainter::SmoothPixmapTransform, smoothBackup);
}

/* inherits documentation from base class */
void QCPColorMap::drawLegendIcon(QCPPainter *painter, const QRectF &rect) const
{
  // draw filled rect:
  applyDefaultAntialiasingHint(painter);
  painter->setBrush(mBrush);
  painter->setPen(mPen);
  QRectF r = QRectF(0, 0, rect.width()*0.67, rect.height()*0.67);
  r.moveCenter(rect.center());
  painter->drawRect(r);
}

QRgb QCPColorMap::wavelengthToRgb(double nm)
{
  double gamma = 0.8;
  double factor = 0;
  double r = 0;
  double g = 0;
  double b = 0;
  
  // Chromaticity:
  if (nm <= 380)
  {
    r = 1.0;
    b = 1.0;
  } else if (nm > 380 && nm <= 440)
  {
    r = -(nm-440.0)/(440.0-380.0);
    b = 1.0;
  } else if (nm > 440 && nm <= 490)
  {
    g = (nm-440.0)/(490.0-440.0);
    b = 1.0;
  } else if (nm > 490 && nm <= 510)
  {
    g = 1.0;
    b = -(nm-510.0)/(510.0-490.0);
  } else if (nm > 510 && nm <= 580)
  {
    r = (nm-510.0)/(580.0-510.0);
    g = 1.0;
  } else if (nm > 580 && nm <= 645)
  {
    r = 1.0;
    g = -(nm-645.0)/(645.0-580.0);
  } else if (nm > 645 && nm <= 780)
  {
    r = 1.0;
  } else if (nm > 780)
  {
    r = 1.0;
  }
  // Visibility:
  if (nm > 360 && nm <= 420)
    factor = (nm-360)/(420.0-360.0);
  else if (nm > 420 && nm <= 700)
    factor = 1.0;
  else if (nm > 700 && nm <= 790)
    factor = (790-nm)/(790.0-700.0);
  if (r > 0)
    r = 255*qPow(r*factor, gamma);
  if (g > 0)
    g = 255*qPow(g*factor, gamma);
  if (b > 0)
    b = 255*qPow(b*factor, gamma);
  
  return qRgb(qBound(0, (int)r, 255), qBound(0, (int)g, 255), qBound(0, (int)b, 255));
}

/* inherits documentation from base class */
QCPRange QCPColorMap::getKeyRange(bool &validRange, SignDomain inSignDomain) const
{
  if (mData->keyRange().size() > 0)
    validRange = true;
  return mData->keyRange();
  // TODO: limit depending on sign domain
}

/* inherits documentation from base class */
QCPRange QCPColorMap::getValueRange(bool &validRange, SignDomain inSignDomain) const
{
  if (mData->valueRange().size() > 0)
    validRange = true;
  return mData->valueRange();
  // TODO: limit depending on sign domain
}
