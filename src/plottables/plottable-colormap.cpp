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
**             Date: 09.12.13                                             **
**          Version: 1.1.1                                                **
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

QCPColorMapData::QCPColorMapData(int keySize, int valueSize, const QCPRange &keyRange, const QCPRange &valueRange) :
  mData(0),
  mKeySize(0),
  mValueSize(0),
  mKeyRange(keyRange),
  mValueRange(valueRange),
  mDataModified(true),
  mIsEmpty(true)
{
  setSize(keySize, valueSize);
  fill(0);
}

QCPColorMapData::~QCPColorMapData()
{
  if (mData)
    delete[] mData;
}

QCPColorMapData::QCPColorMapData(const QCPColorMapData &other) :
  mData(0),
  mKeySize(0),
  mValueSize(0),
  mDataModified(true),
  mIsEmpty(true)
{
  *this = other;
}

QCPColorMapData &QCPColorMapData::operator=(const QCPColorMapData &other)
{
  if (&other != this)
  {
    const int keySize = other.keySize();
    const int valueSize = other.valueSize();
    setSize(keySize, valueSize);
    setRange(other.keyRange(), other.valueRange());
    if (!mIsEmpty)
      memcpy(mData, other.mData, sizeof(mData[0])*keySize*valueSize);
    mDataBounds = other.mDataBounds;
    mDataModified = true;
  }
  return *this;
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
      delete[] mData;
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
    mDataModified = true;
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

void QCPColorMapData::setKeyRange(const QCPRange &keyRange)
{
  mKeyRange = keyRange;
}

void QCPColorMapData::setValueRange(const QCPRange &valueRange)
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
     mDataModified = true;
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
     mDataModified = true;
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

  \image html QCPColorMap.png
  
  The data is stored in the class \ref QCPColorMapData, which can be accessed via the \ref data
  method.
  
  A color map has three dimensions to represent a data point: The \a key dimension, the \a value
  dimension and the \a data dimension. As with other plottables such as graphs, \a key and \a value
  correspond to two orthogonal axes on the QCustomPlot surface that you specify in the QColorMap
  constructor. The \a data dimension however is encoded as the color of the point at (\a key, \a
  value).

  Set the number of points (or \a cells) in the key/value dimension via \ref
  QCPColorMapData::setSize. The plot coordinate range over which these points will be displayed is
  specified via \ref QCPColorMapData::setRange. The first cell will be centered on the lower range
  boundary and the last cell will be centered on the upper range boundary. The data can be set by
  either accessing the cells directly with QCPColorMapData::setCell or by addressing the cells via
  their plot coordinates with \ref QCPColorMapData::setData. If possible, you should prefer
  setCell, since it doesn't need to do any coordinate transformation and thus performs a bit
  better.
  
  The cell with index (0, 0) is at the top left, if the color map uses normal (i.e. not reversed)
  key and value axes.
  
  To show the user which colors correspond to which \a data values, a \ref QCPColorScale is
  typically placed to the right of the axis rect. See the documentation there for details on how to
  add and use a color scale.
  
  \section appearance Changing the appearance
  
  The central part of the appearance is the color gradient, which can be specified via \ref
  setGradient. See the documentation of \ref QCPColorGradient for details on configuring a color
  gradient.
  
  The \a data range that is mapped to the colors of the gradient can be specified with \ref
  setDataRange. To make the data range encompass the whole data set minimum to maximum, call \ref
  rescaleDataRange.
  
  \section usage Usage
  
  Like all data representing objects in QCustomPlot, the QCPColorMap is a plottable
  (QCPAbstractPlottable). So the plottable-interface of QCustomPlot applies
  (QCustomPlot::plottable, QCustomPlot::addPlottable, QCustomPlot::removePlottable, etc.)
  
  Usually, you first create an instance:
  \code
  QCPColorMap *colorMap = new QCPColorMap(customPlot->xAxis, customPlot->yAxis);\endcode
  add it to the customPlot with QCustomPlot::addPlottable:
  \code
  customPlot->addPlottable(colorMap);\endcode
  and then modify the properties of the newly created color map, e.g.:
  \code
  colorMap->data()->setSize(50, 50);
  colorMap->data()->setRange(QCPRange(0, 2), QCPRange(0, 2));
  for (int x=0; x<50; ++x)
    for (int y=0; y<50; ++y)
      colorMap->data()->setCell(x, y, qCos(x/10.0)+qSin(y/10.0));
  colorMap->setGradient(QCPColorGradient::gpPolar);
  colorMap->rescaleDataRange(true);
  customPlot->rescaleAxes();
  customPlot->replot();
  \endcode
*/

/*!
  Constructs a color map with the specified \a keyAxis and \a valueAxis.
  
  The constructed QCPColorMap can be added to the plot with QCustomPlot::addPlottable, QCustomPlot
  then takes ownership of the graph.
*/
QCPColorMap::QCPColorMap(QCPAxis *keyAxis, QCPAxis *valueAxis) :
  QCPAbstractPlottable(keyAxis, valueAxis),
  mDataScaleType(QCPAxis::stLinear),
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
  
  If \a copy is set to true, the \a data object will only be copied. if false, the color map
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

/*!
  Sets the data range of this color map to \a dataRange. The data range defines which data values
  are mapped to the color gradient.
  
  To make the data range span the full range of the data set, use \ref rescaleDataRange.
  
  \see QCPColorScale::setDataRange
*/
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

/*!
  Sets whether the data is correlated with the color gradient linearly or logarithmically.
  
  \see QCPColorScale::setDataScaleType
*/
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

/*!
  Sets the color gradient that is used to represent the data. For more details on how to create an
  own gradient or use one of the preset gradients, see \ref QCPColorGradient.
  
  The colors defined by the gradient will be used to represent data values in the currently set
  data range, see \ref setDataRange. Data points that are outside this data range will either be
  colored uniformly with the respective gradient boundary color, or the gradient will repeat,
  depending on QCPColorGradient::setPeriodic.
  
  \see QCPColorScale::setGradient
*/
void QCPColorMap::setGradient(const QCPColorGradient &gradient)
{
  if (mGradient != gradient)
  {
    mGradient = gradient;
    mMapImageInvalidated = true;
    emit gradientChanged(mGradient);
  }
}

/*!
  Sets whether the color map image shall use bicubic interpolation when displaying the color map
  shrinked or expanded, and not at a 1:1 pixel-to-data scale.
  
  \image html QCPColorMap-interpolate.png "A 10*10 color map, with interpolation and without interpolation enabled"
*/
void QCPColorMap::setInterpolate(bool enabled)
{
  mInterpolate = enabled;
}

/*!
  Sets whether the outer most data rows and columns are clipped to the specified key and value
  range (see \ref setKeyRange, \ref setValueRange).
  
  if \a enabled is set to false, the data points at the border of the color map are drawn with the
  same width and height as all other data points. Since the data points are represented by
  rectangles of one color centered on the data coordinate, this means that the shown color map
  extends by half a data point over the specified key/value range in each direction.
  
  \image html QCPColorMap-tightboundary.png "A color map, with tight boundary enabled and disabled"
*/
void QCPColorMap::setTightBoundary(bool enabled)
{
  mTightBoundary = enabled;
}

/*!
  Associates the color scale \a colorScale with this color map.
  
  This means that both the color scale and the color map synchronize their gradient, data range and
  data scale type (\ref setGradient, \ref setDataRange, \ref setDataScaleType), if these properties
  change. Multiple color maps can be associated with one single color scale. This causes the color
  maps to also synchronize those properties, via the mutual color scale.
  
  This function causes the color map to adopt the current color gradient, data range and data scale
  type of \a colorScale. After this call, you may change these properties at either the color map
  or the color scale, and the setting will be applied to both.
  
  Pass 0 as \ref colorScale to disconnect the color scale from this color map again.
*/
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
    setGradient(mColorScale.data()->gradient());
    setDataRange(mColorScale.data()->dataRange());
    setDataScaleType(mColorScale.data()->dataScaleType());
    connect(this, SIGNAL(dataRangeChanged(QCPRange)), mColorScale.data(), SLOT(setDataRange(QCPRange)));
    connect(this, SIGNAL(dataScaleTypeChanged(QCPAxis::ScaleType)), mColorScale.data(), SLOT(setDataScaleType(QCPAxis::ScaleType)));
    connect(this, SIGNAL(gradientChanged(QCPColorGradient)), mColorScale.data(), SLOT(setGradient(QCPColorGradient)));
    connect(mColorScale.data(), SIGNAL(dataRangeChanged(QCPRange)), this, SLOT(setDataRange(QCPRange)));
    connect(mColorScale.data(), SIGNAL(gradientChanged(QCPColorGradient)), this, SLOT(setGradient(QCPColorGradient)));
    connect(mColorScale.data(), SIGNAL(dataScaleTypeChanged(QCPAxis::ScaleType)), this, SLOT(setDataScaleType(QCPAxis::ScaleType)));
  }
}

/*!
  Sets the data range (\ref setDataRange) to span the minimum and maximum values that occur in the
  current data set. This corresponds to the \ref rescaleKeyAxis or \ref rescaleValueAxis methods,
  only for the third data dimension of the color map.
  
  The minimum and maximum values of the data set are buffered in the internal QCPColorMapData
  instance (\ref data). As data is updated via its \ref QCPColorMapData::setCell or \ref
  QCPColorMapData::setData, the buffered minimum and maximum values are updated, too. For
  performance reasons, however, they are only updated in an expanding fashion. So the buffered
  maximum can only increase and the buffered minimum can only decrease. In consequence, changes to
  the data that actually lower the maximum of the data set (by overwriting the cell holding the
  current maximum with a smaller value), aren't recognized and the buffered maximum overestimates
  the true maximum of the data set. The same happens for the buffered minimum. To recalculate the
  true minimum and maximum by explicitly looking at each cell, the method
  QCPColorMapData::recalculateDataBounds can be used. For convenience, setting the parameter \a
  recalculateDataBounds calls this method before setting the data range to the buffered minimum and
  maximum.
  
  \see setDataRange
*/
void QCPColorMap::rescaleDataRange(bool recalculateDataBounds)
{
  if (recalculateDataBounds)
    mMapData->recalculateDataBounds();
  setDataRange(mMapData->dataBounds());
}

/*!
  Takes the current appearance of the color map and updates the legend icon that is used to
  represent this color map in the legend.
  
  The \a transformMode specifies whether the rescaling is done by a faster, low quality image
  scaling algorithm (Qt::FastTransformation) or by a slower, higher quality algorithm
  (Qt::SmoothTransformation).
  
  The current color map appearance is scaled down to \a thumbSize. Ideally, this should be equal to
  the size of the legend icon (see \ref QCPLegend::setIconSize). If it isn't exactly the configured
  legend icon size, it will be rescaled during drawing of the legend item.
  
  \see setDataRange
*/
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

/*! \internal
  
  Updates the internal map image buffer by going through the internal \ref QCPColorMapData and
  turning the data values into color pixels with \ref QCPColorGradient::colorize.
  
  This method is called by \ref QCPColorMap::draw if either the data has been modified or the map image
  has been invalidated for a different reason (e.g. a change of the data range with \ref
  setDataRange).
*/
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
  
  mMapData->mDataModified = false;
  mMapImageInvalidated = false;
}

/* inherits documentation from base class */
void QCPColorMap::draw(QCPPainter *painter)
{
  if (mMapData->isEmpty()) return;
  if (!mKeyAxis || !mValueAxis) return;
  applyDefaultAntialiasingHint(painter);
  
  if (mMapData->mDataModified || mMapImageInvalidated)
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

