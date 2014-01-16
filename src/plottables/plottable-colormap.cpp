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

QCPColorMapData::QCPColorMapData(int keySize, int valueSize, const QCPRange keyRange, const QCPRange valueRange) :
  mData(0),
  mKeyRange(keyRange),
  mValueRange(valueRange),
  mModified(true),
  mIsEmpty(true)
{
  setSize(keySize, valueSize);
  fill(0);
}

double QCPColorMapData::data(double key, double value)
{
  int keyCell = (key-mKeyRange.lower)/(mKeyRange.upper-mKeyRange.lower)*(mKeySize-1)+0.5;
  int valueCell = (1.0-(value-mValueRange.lower)/(mValueRange.upper-mValueRange.lower))*(mValueSize-1)+0.5;
  if (keyCell >= 0 && keyCell < mKeySize && valueCell >= 0 && valueCell < mValueSize)
    return mData[valueCell*mKeySize + keyCell];
  else
    return 0;
}

double QCPColorMapData::cell(int keyIndex, int valueIndex)
{
  if (keyIndex >= 0 && keyIndex < mKeySize && valueIndex >= 0 && valueIndex < mValueSize)
    return mData[valueIndex*mKeySize + keyIndex];
  else
    return 0;
}

void QCPColorMapData::coordToCell(double key, double value, int *keyIndex, int *valueIndex) const
{
  if (keyIndex)
    *keyIndex = (key-mKeyRange.lower)/(mKeyRange.upper-mKeyRange.lower)*(mKeySize-1)+0.5;
  if (valueIndex)
    *valueIndex = (1.0-(value-mValueRange.lower)/(mValueRange.upper-mValueRange.lower))*(mValueSize-1)+0.5;
}

void QCPColorMapData::cellToCoord(int keyIndex, int valueIndex, double *key, double *value) const
{
  if (key)
    *key = keyIndex/(double)(mKeySize-1)*(mKeyRange.upper-mKeyRange.lower)+mKeyRange.lower;
  if (value)
    *value = (1.0-valueIndex/(double)(mValueSize-1))*(mValueRange.upper-mValueRange.lower)+mValueRange.lower;
}

void QCPColorMapData::setSize(int keySize, int valueSize)
{
  if (keySize != mKeySize || valueSize != mValueSize)
  {
    mKeySize = keySize;
    mValueSize = valueSize;
    if (mData)
      delete mData;
    mIsEmpty = mKeySize == 0 || mValueSize == 0;
    if (!mIsEmpty)
    {
      mData = new double[mKeySize*mValueSize];
      if (!mData)
        qDebug() << Q_FUNC_INFO << "out of memory for data dimensions "<< mKeySize << "*" << mValueSize;
      else
        fill(0);
    } else
      mData = 0;
    mModified = true;
  }
}

void QCPColorMapData::setKeySize(int keySize)
{
  setSize(keySize, mValueSize);
}

void QCPColorMapData::setValueSize(int valueSize)
{
  setSize(mKeySize, valueSize);
}

void QCPColorMapData::setRange(const QCPRange &keyRange, const QCPRange &valueRange)
{
  setKeyRange(keyRange);
  setValueRange(valueRange);
}

void QCPColorMapData::setValueRange(const QCPRange &keyRange)
{
  mKeyRange = keyRange;
}

void QCPColorMapData::setKeyRange(const QCPRange &valueRange)
{
  mValueRange = valueRange;
}

void QCPColorMapData::setData(double key, double value, double z)
{
  int keyCell = (key-mKeyRange.lower)/(mKeyRange.upper-mKeyRange.lower)*(mKeySize-1)+0.5;
  int valueCell = (1.0-(value-mValueRange.lower)/(mValueRange.upper-mValueRange.lower))*(mValueSize-1)+0.5;
  if (keyCell >= 0 && keyCell < mKeySize && valueCell >= 0 && valueCell < mValueSize)
  {
    mData[valueCell*mKeySize + keyCell] = z;
    if (z < mDataBounds.lower)
      mDataBounds.lower = z;
    if (z > mDataBounds.upper)
      mDataBounds.upper = z;
     mModified = true;
  }
}

void QCPColorMapData::setCell(int keyIndex, int valueIndex, double z)
{
  if (keyIndex >= 0 && keyIndex < mKeySize && valueIndex >= 0 && valueIndex < mValueSize)
  {
    mData[valueIndex*mKeySize + keyIndex] = z;
    if (z < mDataBounds.lower)
      mDataBounds.lower = z;
    if (z > mDataBounds.upper)
      mDataBounds.upper = z;
     mModified = true;
  }
}

void QCPColorMapData::recalculateDataBounds()
{
  if (mKeySize > 0 && mValueSize > 0)
  {
    double minHeight = mData[0];
    double maxHeight = mData[0];
    const int dataCount = mValueSize*mKeySize;
    for (int i=0; i<dataCount; ++i)
    {
      if (mData[i] > maxHeight)
        maxHeight = mData[i];
      if (mData[i] < minHeight)
        minHeight = mData[i];
    }
    mDataBounds.lower = minHeight;
    mDataBounds.upper = maxHeight;
    mModified = true;
  }
}

void QCPColorMapData::clear()
{
  setSize(0, 0);
}

void QCPColorMapData::fill(double z)
{
  const int dataCount = mValueSize*mKeySize;
  for (int i=0; i<dataCount; ++i)
    mData[i] = z;
  mDataBounds = QCPRange(z, z);
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
  newMap->setName("Temperature Map");
  newMap->setData();\endcode // TODO
*/

QCPColorMap::QCPColorMap(QCPAxis *keyAxis, QCPAxis *valueAxis) :
  QCPAbstractPlottable(keyAxis, valueAxis),
  mMapData(new QCPColorMapData(10, 10, QCPRange(0, 5), QCPRange(0, 5))),
  mInterpolate(true),
  mTightBoundary(false),
  mMapImageInvalidated(true)
{
}

QCPColorMap::~QCPColorMap()
{
  delete mMapData;
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
    *mMapData = *data;
  } else
  {
    delete mMapData;
    mMapData = data;
  }
  mMapImageInvalidated = true;
}

void QCPColorMap::setDataRange(const QCPRange &dataRange)
{
  if (!QCPRange::validRange(dataRange)) return;
  if (mDataRange.lower != dataRange.lower || mDataRange.upper != dataRange.upper)
  {
    if (mDataScaleType == QCPAxis::stLogarithmic)
      mDataRange = dataRange.sanitizedForLogScale();
    else
      mDataRange = dataRange.sanitizedForLinScale();
    mMapImageInvalidated = true;
    emit dataRangeChanged(mDataRange);
  }
}

void QCPColorMap::setDataScaleType(QCPAxis::ScaleType scaleType)
{
  if (mDataScaleType != scaleType)
  {
    mDataScaleType = scaleType;
    mMapImageInvalidated = true;
    emit dataScaleTypeChanged(mDataScaleType);
    if (mDataScaleType == QCPAxis::stLogarithmic)
      setDataRange(mDataRange.sanitizedForLogScale());
  }
}

void QCPColorMap::setGradient(const QCPColorGradient &gradient)
{
  if (mGradient != gradient)
  {
    mGradient = gradient;
    mMapImageInvalidated = true;
    emit gradientChanged(mGradient);
  }
}

void QCPColorMap::setInterpolate(bool enabled)
{
  mInterpolate = enabled;
}

void QCPColorMap::setTightBoundary(bool enabled)
{
  mTightBoundary = enabled;
}

void QCPColorMap::setColorScale(QCPColorScale *colorScale)
{
  if (mColorScale) // unconnect signals from old color scale
  {
    disconnect(this, SIGNAL(dataRangeChanged(QCPRange)), mColorScale.data(), SLOT(setDataRange(QCPRange)));
    disconnect(this, SIGNAL(dataScaleTypeChanged(QCPAxis::ScaleType)), mColorScale.data(), SLOT(setDataScaleType(QCPAxis::ScaleType)));
    disconnect(this, SIGNAL(gradientChanged(QCPColorGradient)), mColorScale.data(), SLOT(setGradient(QCPColorGradient)));
    disconnect(mColorScale.data(), SIGNAL(dataRangeChanged(QCPRange)), this, SLOT(setDataRange(QCPRange)));
    disconnect(mColorScale.data(), SIGNAL(gradientChanged(QCPColorGradient)), this, SLOT(setGradient(QCPColorGradient)));
    disconnect(mColorScale.data(), SIGNAL(dataScaleTypeChanged(QCPAxis::ScaleType)), this, SLOT(setDataScaleType(QCPAxis::ScaleType)));
  }
  mColorScale = colorScale;
  if (mColorScale) // connect signals to new color scale
  {
    connect(this, SIGNAL(dataRangeChanged(QCPRange)), mColorScale.data(), SLOT(setDataRange(QCPRange)));
    connect(this, SIGNAL(dataScaleTypeChanged(QCPAxis::ScaleType)), mColorScale.data(), SLOT(setDataScaleType(QCPAxis::ScaleType)));
    connect(this, SIGNAL(gradientChanged(QCPColorGradient)), mColorScale.data(), SLOT(setGradient(QCPColorGradient)));
    connect(mColorScale.data(), SIGNAL(dataRangeChanged(QCPRange)), this, SLOT(setDataRange(QCPRange)));
    connect(mColorScale.data(), SIGNAL(gradientChanged(QCPColorGradient)), this, SLOT(setGradient(QCPColorGradient)));
    connect(mColorScale.data(), SIGNAL(dataScaleTypeChanged(QCPAxis::ScaleType)), this, SLOT(setDataScaleType(QCPAxis::ScaleType)));
  }
}

void QCPColorMap::rescaleDataRange(bool recalculateDataBounds)
{
  if (recalculateDataBounds)
    mMapData->recalculateDataBounds();
  setDataRange(mMapData->dataBounds());
}

void QCPColorMap::updateLegendIcon(Qt::TransformationMode transformMode, const QSize &thumbSize)
{
  if (!mMapImage.isNull())
  {
    QRectF imageRect(coordsToPixels(mMapData->keyRange().lower, mMapData->valueRange().upper),
                     coordsToPixels(mMapData->keyRange().upper, mMapData->valueRange().lower));
    mLegendIcon = QPixmap::fromImage(mMapImage.mirrored(imageRect.width() < 0, imageRect.height() < 0)).scaled(thumbSize, Qt::KeepAspectRatio, transformMode);
  }
}

/*!
  Clears the colormap data by calling \ref QCPColorMapData::clear() on the internal data. This also
  resizes the map to 0x0 cells.
*/
void QCPColorMap::clearData()
{
  mMapData->clear();
}

/* inherits documentation from base class */
double QCPColorMap::selectTest(const QPointF &pos, bool onlySelectable, QVariant *details) const
{
  Q_UNUSED(details)
  if (onlySelectable && !mSelectable)
    return -1;
  
  double posKey, posValue;
  pixelsToCoords(pos, posKey, posValue);
  if (mMapData->keyRange().contains(posKey) && mMapData->valueRange().contains(posValue))
    return mParentPlot->selectionTolerance()*0.99;
  else
    return -1;
}

void QCPColorMap::updateMapImage()
{
  QCPAxis *keyAxis = mKeyAxis.data();
  if (!keyAxis) return;
  
  // resize mMapImage to correct dimensions, according to key/value axes orientation:
  if (keyAxis->orientation() == Qt::Horizontal && (mMapImage.size().width() != mMapData->keySize() || mMapImage.size().height() != mMapData->valueSize()))
    mMapImage = QImage(QSize(mMapData->keySize(), mMapData->valueSize()), QImage::Format_RGB32);
  else if (keyAxis->orientation() == Qt::Vertical && (mMapImage.size().width() != mMapData->valueSize() || mMapImage.size().height() != mMapData->keySize()))
    mMapImage = QImage(QSize(mMapData->valueSize(), mMapData->keySize()), QImage::Format_RGB32);
  
  const int keySize = mMapData->keySize();
  const int valueSize = mMapData->valueSize();
  const double *rawData = mMapData->mData;
  
  if (keyAxis->orientation() == Qt::Horizontal)
  {
    const int lineCount = valueSize;
    const int rowCount = keySize;
    for (int line=0; line<lineCount; ++line)
    {
      QRgb* pixels = reinterpret_cast<QRgb*>(mMapImage.scanLine(line));
      mGradient.colorize(rawData+line*rowCount, mDataRange, pixels, rowCount, 1, mDataScaleType==QCPAxis::stLogarithmic);
    }
  } else // keyAxis->orientation() == Qt::Vertical
  {
    const int lineCount = keySize;
    const int rowCount = valueSize;
    for (int line=0; line<lineCount; ++line)
    {
      QRgb* pixels = reinterpret_cast<QRgb*>(mMapImage.scanLine(line));
      mGradient.colorize(rawData+line, mDataRange, pixels, rowCount, lineCount, mDataScaleType==QCPAxis::stLogarithmic);
    }
  }
  
  mMapData->mModified = false;
  mMapImageInvalidated = false;
}

/* inherits documentation from base class */
void QCPColorMap::draw(QCPPainter *painter)
{
  if (mMapData->isEmpty()) return;
  if (!mKeyAxis || !mValueAxis) return;
  applyDefaultAntialiasingHint(painter);
  
  if (mMapData->mModified || mMapImageInvalidated)
    updateMapImage();
  
  double halfSampleKey = 0;
  double halfSampleValue = 0;
  if (mMapData->keySize() > 1)
    halfSampleKey = 0.5*mMapData->keyRange().size()/(double)(mMapData->keySize()-1);
  if (mMapData->valueSize() > 1)
    halfSampleValue = 0.5*mMapData->valueRange().size()/(double)(mMapData->valueSize()-1);
  QRectF imageRect(coordsToPixels(mMapData->keyRange().lower-halfSampleKey, mMapData->valueRange().upper+halfSampleValue),
                   coordsToPixels(mMapData->keyRange().upper+halfSampleKey, mMapData->valueRange().lower-halfSampleValue));
  bool smoothBackup = painter->renderHints().testFlag(QPainter::SmoothPixmapTransform);
  painter->setRenderHint(QPainter::SmoothPixmapTransform, mInterpolate);
  QRegion clipBackup;
  if (mTightBoundary)
  { 
    clipBackup = painter->clipRegion();
    painter->setClipRect(QRectF(coordsToPixels(mMapData->keyRange().lower, mMapData->valueRange().upper),
                                coordsToPixels(mMapData->keyRange().upper, mMapData->valueRange().lower)).normalized(), Qt::IntersectClip);
  }
  painter->drawImage(imageRect.normalized(), mMapImage.mirrored(imageRect.width() < 0, imageRect.height() < 0));
  if (mTightBoundary)
    painter->setClipRegion(clipBackup);
  painter->setRenderHint(QPainter::SmoothPixmapTransform, smoothBackup);
}

/* inherits documentation from base class */
void QCPColorMap::drawLegendIcon(QCPPainter *painter, const QRectF &rect) const
{
  applyDefaultAntialiasingHint(painter);
  // draw map thumbnail:
  if (!mLegendIcon.isNull())
  {
    QPixmap scaledIcon = mLegendIcon.scaled(rect.size().toSize(), Qt::KeepAspectRatio, Qt::FastTransformation);
    QRectF iconRect = QRectF(0, 0, scaledIcon.width(), scaledIcon.height());
    iconRect.moveCenter(rect.center());
    painter->drawPixmap(iconRect.topLeft(), scaledIcon);
  }
  /*
  // draw frame:
  painter->setBrush(Qt::NoBrush);
  painter->setPen(Qt::black);
  painter->drawRect(rect.adjusted(1, 1, 0, 0));
  */
}

/* inherits documentation from base class */
QCPRange QCPColorMap::getKeyRange(bool &foundRange, SignDomain inSignDomain) const
{
  foundRange = true;
  QCPRange result = mMapData->keyRange();
  result.normalize();
  if (inSignDomain == QCPAbstractPlottable::sdPositive)
  {
    if (result.lower <= 0 && result.upper > 0)
      result.lower = result.upper*1e-3;
    else if (result.lower <= 0 && result.upper <= 0)
      foundRange = false;
  } else if (inSignDomain == QCPAbstractPlottable::sdNegative)
  {
    if (result.upper >= 0 && result.lower < 0)
      result.upper = result.lower*1e-3;
    else if (result.upper >= 0 && result.lower >= 0)
      foundRange = false;
  }
  return result;
}

/* inherits documentation from base class */
QCPRange QCPColorMap::getValueRange(bool &foundRange, SignDomain inSignDomain) const
{
  foundRange = true;
  QCPRange result = mMapData->valueRange();
  result.normalize();
  if (inSignDomain == QCPAbstractPlottable::sdPositive)
  {
    if (result.lower <= 0 && result.upper > 0)
      result.lower = result.upper*1e-3;
    else if (result.lower <= 0 && result.upper <= 0)
      foundRange = false;
  } else if (inSignDomain == QCPAbstractPlottable::sdNegative)
  {
    if (result.upper >= 0 && result.lower < 0)
      result.upper = result.lower*1e-3;
    else if (result.upper >= 0 && result.lower >= 0)
      foundRange = false;
  }
  return result;
}

