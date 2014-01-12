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

#include "layoutelement-colorscale.h"

#include "../painter.h"
#include "../core.h"
#include "../plottable.h"
#include "../axis.h"


////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// QCPColorScale
////////////////////////////////////////////////////////////////////////////////////////////////////

/*! \class QCPColorScale
  \brief 
  
*/

QCPColorScale::QCPColorScale(QCustomPlot *parentPlot) :
  QCPLayoutElement(parentPlot),
  mBarWidth(20),
  mAxisRect(new QCPColorScaleAxisRectPrivate(this))
{
  setMinimumMargins(QMargins(0, 6, 0, 6));
  setAxisType(QCPAxis::atRight);
  setDataRange(QCPRange(0, 6));
}

QCPColorScale::~QCPColorScale()
{
  
}

bool QCPColorScale::rangeDrag() const
{
  if (!mAxisRect)
  {
    qDebug() << Q_FUNC_INFO << "internal axis rect was deleted";
    return false;
  }
  if (!mColorAxis)
  {
    qDebug() << Q_FUNC_INFO << "internal color axis undefined";
    return false;
  }
  
  return mAxisRect.data()->rangeDrag().testFlag(QCPAxis::orientation(mAxisType)) &&
      mAxisRect.data()->rangeDragAxis(QCPAxis::orientation(mAxisType)) &&
      mAxisRect.data()->rangeDragAxis(QCPAxis::orientation(mAxisType))->orientation() == QCPAxis::orientation(mAxisType);
}

bool QCPColorScale::rangeZoom() const
{
  if (!mAxisRect)
  {
    qDebug() << Q_FUNC_INFO << "internal axis rect was deleted";
    return false;
  }
  if (!mColorAxis)
  {
    qDebug() << Q_FUNC_INFO << "internal color axis undefined";
    return false;
  }
  
  return mAxisRect.data()->rangeZoom().testFlag(QCPAxis::orientation(mAxisType)) &&
      mAxisRect.data()->rangeZoomAxis(QCPAxis::orientation(mAxisType)) &&
      mAxisRect.data()->rangeZoomAxis(QCPAxis::orientation(mAxisType))->orientation() == QCPAxis::orientation(mAxisType);
}

void QCPColorScale::setAxisType(QCPAxis::AxisType axisType)
{
  if (!mAxisRect)
  {
    qDebug() << Q_FUNC_INFO << "internal axis rect was deleted";
    return;
  }
  if (mAxisType != axisType)
  {
    mAxisType = axisType;
    QCPRange rangeTransfer(0, 6);
    QString labelTransfer;
    // revert some settings on old axis:
    if (mColorAxis)
    {
      rangeTransfer = mColorAxis.data()->range();
      labelTransfer = mColorAxis.data()->label();
      mColorAxis.data()->setLabel("");
      disconnect(mColorAxis.data(), SIGNAL(rangeChanged(QCPRange)), this, SLOT(setDataRange(QCPRange)));
    }
    foreach (QCPAxis::AxisType type, QList<QCPAxis::AxisType>() << QCPAxis::atLeft << QCPAxis::atRight << QCPAxis::atBottom << QCPAxis::atTop)
    {
      mAxisRect.data()->axis(type)->setTicks(type == mAxisType);
      mAxisRect.data()->axis(type)->setTickLabels(type== mAxisType);
    }
    // set new mColorAxis pointer:
    mColorAxis = mAxisRect.data()->axis(mAxisType);
    // transfer settings to new axis:
    mColorAxis.data()->setRange(rangeTransfer); // transfer range of old axis to new one (necessary if axis changes from vertical to horizontal or vice versa)
    mColorAxis.data()->setLabel(labelTransfer);
    connect(mColorAxis.data(), SIGNAL(rangeChanged(QCPRange)), this, SLOT(setDataRange(QCPRange)));
    mAxisRect.data()->setRangeDragAxes(QCPAxis::orientation(mAxisType) == Qt::Horizontal ? mColorAxis.data() : 0,
                                       QCPAxis::orientation(mAxisType) == Qt::Vertical ? mColorAxis.data() : 0);
  }
}

void QCPColorScale::setDataRange(const QCPRange &dataRange)
{
  if (mDataRange.lower != dataRange.lower || mDataRange.upper != dataRange.upper)
  {
    mDataRange = dataRange;
    if (mColorAxis)
      mColorAxis.data()->setRange(mDataRange);
    emit dataRangeChanged(mDataRange);
  }
}

void QCPColorScale::setGradient(const QCPColorGradient &gradient)
{
  if (mGradient != gradient)
  {
    mGradient = gradient;
    if (mAxisRect)
      mAxisRect.data()->mGradientImageInvalidated = true;
    emit gradientChanged(mGradient);
  }
}

void QCPColorScale::setBarWidth(int width)
{
  mBarWidth = width;
}

void QCPColorScale::setRangeDrag(bool enabled)
{
  if (!mAxisRect)
  {
    qDebug() << Q_FUNC_INFO << "internal axis rect was deleted";
    return;
  }
  
  if (enabled)
    mAxisRect.data()->setRangeDrag(QCPAxis::orientation(mAxisType));
  else
    mAxisRect.data()->setRangeDrag(0);
}

void QCPColorScale::setRangeZoom(bool enabled)
{
  if (!mAxisRect)
  {
    qDebug() << Q_FUNC_INFO << "internal axis rect was deleted";
    return;
  }
  
  if (enabled)
    mAxisRect.data()->setRangeZoom(QCPAxis::orientation(mAxisType));
  else
    mAxisRect.data()->setRangeZoom(0);
}

void QCPColorScale::update(UpdatePhase phase)
{
  QCPLayoutElement::update(phase);
  if (!mAxisRect)
  {
    qDebug() << Q_FUNC_INFO << "internal axis rect was deleted";
    return;
  }
  
  mAxisRect.data()->update(phase);
  
  switch (phase)
  {
    case upMargins:
    {
      if (mAxisType == QCPAxis::atBottom || mAxisType == QCPAxis::atTop)
      {
        setMaximumSize(QWIDGETSIZE_MAX, mBarWidth+mAxisRect.data()->margins().top()+mAxisRect.data()->margins().bottom()+margins().top()+margins().bottom());
        setMinimumSize(0,               mBarWidth+mAxisRect.data()->margins().top()+mAxisRect.data()->margins().bottom()+margins().top()+margins().bottom());
      } else
      {
        setMaximumSize(mBarWidth+mAxisRect.data()->margins().left()+mAxisRect.data()->margins().right()+margins().left()+margins().right(), QWIDGETSIZE_MAX);
        setMinimumSize(mBarWidth+mAxisRect.data()->margins().left()+mAxisRect.data()->margins().right()+margins().left()+margins().right(), 0);
      }
      break;
    }
    case upLayout:
    {
      mAxisRect.data()->setOuterRect(rect());
      break;
    }
    default: break;
  }
}

/* inherits documentation from base class */
void QCPColorScale::applyDefaultAntialiasingHint(QCPPainter *painter) const
{
  painter->setAntialiasing(false);
}

void QCPColorScale::mousePressEvent(QMouseEvent *event)
{
  if (!mAxisRect)
  {
    qDebug() << Q_FUNC_INFO << "internal axis rect was deleted";
    return;
  }
  mAxisRect.data()->mousePressEvent(event);
}

void QCPColorScale::mouseMoveEvent(QMouseEvent *event)
{
  if (!mAxisRect)
  {
    qDebug() << Q_FUNC_INFO << "internal axis rect was deleted";
    return;
  }
  mAxisRect.data()->mouseMoveEvent(event);
}

void QCPColorScale::mouseReleaseEvent(QMouseEvent *event)
{
  if (!mAxisRect)
  {
    qDebug() << Q_FUNC_INFO << "internal axis rect was deleted";
    return;
  }
  mAxisRect.data()->mouseReleaseEvent(event);
}

void QCPColorScale::wheelEvent(QWheelEvent *event)
{
  if (!mAxisRect)
  {
    qDebug() << Q_FUNC_INFO << "internal axis rect was deleted";
    return;
  }
  mAxisRect.data()->wheelEvent(event);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// QCPColorScaleAxisRectPrivate
////////////////////////////////////////////////////////////////////////////////////////////////////

/*! \class QCPColorScaleAxisRectPrivate \internal
  \brief (Private)
  
  This is a private class and not part of the public QCustomPlot interface.
  
*/

QCPColorScaleAxisRectPrivate::QCPColorScaleAxisRectPrivate(QCPColorScale *parentColorScale) :
  QCPAxisRect(parentColorScale->parentPlot(), true),
  mParentColorScale(parentColorScale),
  mGradientImageInvalidated(true)
{
  setMinimumMargins(QMargins(0, 0, 0, 0));
  foreach(QCPAxis::AxisType type, QList<QCPAxis::AxisType>() << QCPAxis::atBottom << QCPAxis::atTop << QCPAxis::atLeft << QCPAxis::atRight)
  {
    axis(type)->setVisible(true);
    axis(type)->grid()->setVisible(false);
    axis(type)->setPadding(0);
    connect(axis(type), SIGNAL(selectionChanged(QCPAxis::SelectableParts)), this, SLOT(axisSelectionChanged(QCPAxis::SelectableParts)));
  }

  connect(axis(QCPAxis::atLeft), SIGNAL(rangeChanged(QCPRange)), axis(QCPAxis::atRight), SLOT(setRange(QCPRange)));
  connect(axis(QCPAxis::atRight), SIGNAL(rangeChanged(QCPRange)), axis(QCPAxis::atLeft), SLOT(setRange(QCPRange)));
  connect(axis(QCPAxis::atBottom), SIGNAL(rangeChanged(QCPRange)), axis(QCPAxis::atTop), SLOT(setRange(QCPRange)));
  connect(axis(QCPAxis::atTop), SIGNAL(rangeChanged(QCPRange)), axis(QCPAxis::atBottom), SLOT(setRange(QCPRange)));
}

void QCPColorScaleAxisRectPrivate::draw(QCPPainter *painter)
{
  if (mGradientImageInvalidated)
    updateGradientImage();
  
  bool mirrorHorz = false;
  bool mirrorVert = false;
  if (mParentColorScale->mColorAxis)
  {
    mirrorHorz = mParentColorScale->mColorAxis.data()->rangeReversed() && (mParentColorScale->axisType() == QCPAxis::atBottom || mParentColorScale->axisType() == QCPAxis::atTop); 
    mirrorVert = mParentColorScale->mColorAxis.data()->rangeReversed() && (mParentColorScale->axisType() == QCPAxis::atLeft || mParentColorScale->axisType() == QCPAxis::atRight); 
  }
  
  painter->drawImage(rect(), mGradientImage.mirrored(mirrorHorz, mirrorVert));
  QCPAxisRect::draw(painter);
}

void QCPColorScaleAxisRectPrivate::updateGradientImage()
{
  if (!mParentColorScale->mAxisRect)
  {
    qDebug() << Q_FUNC_INFO << "internal axis rect was deleted";
    return;
  }
  if (rect().isEmpty())
    return;
  
  QImage *gradientImage = &(mParentColorScale->mAxisRect.data()->mGradientImage);
  int n = mParentColorScale->mGradient.levelCount();
  int w, h;
  double *data = new double[n];
  for (int i=0; i<n; ++i)
    data[i] = i;
  if (mParentColorScale->mAxisType == QCPAxis::atBottom || mParentColorScale->mAxisType == QCPAxis::atTop)
  {
    w = n;
    h = rect().height();
    *gradientImage = QImage(w, h, QImage::Format_RGB32);
    QVector<QRgb*> pixels;
    for (int y=0; y<h; ++y)
      pixels.append(reinterpret_cast<QRgb*>(gradientImage->scanLine(y)));
    mParentColorScale->mGradient.colorize(data, QCPRange(0, n-1), pixels.first(), n);
    for (int y=1; y<h; ++y)
      memcpy(pixels.at(y), pixels.first(), n*sizeof(QRgb));
  } else
  {
    w = rect().width();
    h = n;
    *gradientImage = QImage(w, h, QImage::Format_RGB32);
    for (int y=0; y<h; ++y)
    {
      QRgb *pixels = reinterpret_cast<QRgb*>(gradientImage->scanLine(y));
      const QRgb lineColor = mParentColorScale->mGradient.color(data[h-1-y], QCPRange(0, n-1));
      for (int x=0; x<w; ++x)
        pixels[x] = lineColor;
    }
  }
  mGradientImageInvalidated = false;
}

void QCPColorScaleAxisRectPrivate::axisSelectionChanged(QCPAxis::SelectableParts selectedParts)
{
  // axis bases of four axes shall always (de-)selected synchronously:
  foreach (QCPAxis::AxisType type, QList<QCPAxis::AxisType>() << QCPAxis::atBottom << QCPAxis::atTop << QCPAxis::atLeft << QCPAxis::atRight)
  {
    if (QCPAxis *senderAxis = qobject_cast<QCPAxis*>(sender()))
      if (senderAxis->axisType() == type)
        continue;
    
    if (axis(type)->selectableParts().testFlag(QCPAxis::spAxis))
    {
      if (selectedParts.testFlag(QCPAxis::spAxis))
        axis(type)->setSelectedParts(axis(type)->selectedParts() | QCPAxis::spAxis);
      else
        axis(type)->setSelectedParts(axis(type)->selectedParts() & ~QCPAxis::spAxis);
    }
  }
}
