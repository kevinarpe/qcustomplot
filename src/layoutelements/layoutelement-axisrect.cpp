/***************************************************************************
**                                                                        **
**  QCustomPlot, a simple to use, modern plotting widget for Qt           **
**  Copyright (C) 2011, 2012 Emanuel Eichhammer                           **
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
**  Website/Contact: http://www.WorksLikeClockwork.com/                   **
**             Date: 09.06.12                                             **
****************************************************************************/

#include "layoutelement-axisrect.h"

#include "../painter.h"
#include "../core.h"
#include "../plottable.h"
#include "../plottables/plottable-graph.h"
#include "../item.h"
#include "../axis.h"


////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// QCPAxisRect
////////////////////////////////////////////////////////////////////////////////////////////////////


QCPAxisRect::QCPAxisRect(QCustomPlot *parentPlot) :
  QCPLayoutElement(parentPlot)
{
  setMinimumSize(50, 50);
  setMinimumMargins(QMargins(15, 15, 15, 15));
  mAxes.insert(QCPAxis::atLeft, QList<QCPAxis*>());
  mAxes.insert(QCPAxis::atRight, QList<QCPAxis*>());
  mAxes.insert(QCPAxis::atTop, QList<QCPAxis*>());
  mAxes.insert(QCPAxis::atBottom, QList<QCPAxis*>());
}

QCPAxisRect::~QCPAxisRect()
{
  QList<QCPAxis*> axesList = axes();
  for (int i=0; i<axesList.size(); ++i)
    removeAxis(axesList.at(i));
}

int QCPAxisRect::axisCount(QCPAxis::AxisType type) const
{
  return mAxes.value(type).size();
}

QCPAxis *QCPAxisRect::axis(QCPAxis::AxisType type, int index) const
{
  QList<QCPAxis*> ax(mAxes.value(type));
  if (index >= 0 && index < ax.size())
  {
    return ax.at(index);
  } else
  {
    qDebug() << Q_FUNC_INFO << "Axis index out of bounds:" << index;
    return 0;
  }
}

QList<QCPAxis*> QCPAxisRect::axes(QCPAxis::AxisTypes types) const
{
  QList<QCPAxis*> result;
  if (types.testFlag(QCPAxis::atLeft))
    result << mAxes.value(QCPAxis::atLeft);
  if (types.testFlag(QCPAxis::atRight))
    result << mAxes.value(QCPAxis::atRight);
  if (types.testFlag(QCPAxis::atTop))
    result << mAxes.value(QCPAxis::atTop);
  if (types.testFlag(QCPAxis::atBottom))
    result << mAxes.value(QCPAxis::atBottom);
  return result;
}

QList<QCPAxis *> QCPAxisRect::axes() const
{
  QList<QCPAxis*> result;
  QHashIterator<QCPAxis::AxisType, QList<QCPAxis*> > it(mAxes);
  while (it.hasNext())
  {
    it.next();
    result << it.value();
  }
  return result;
}

QCPAxis *QCPAxisRect::addAxis(QCPAxis::AxisType type)
{
  QCPAxis *newAxis = new QCPAxis(this, type);
  mAxes[type].append(newAxis);
  
  return newAxis;
}

QList<QCPAxis*> QCPAxisRect::addAxes(QCPAxis::AxisTypes types)
{
  QList<QCPAxis*> result;
  if (types.testFlag(QCPAxis::atLeft))
    result << addAxis(QCPAxis::atLeft);
  if (types.testFlag(QCPAxis::atRight))
    result << addAxis(QCPAxis::atRight);
  if (types.testFlag(QCPAxis::atTop))
    result << addAxis(QCPAxis::atTop);
  if (types.testFlag(QCPAxis::atBottom))
    result << addAxis(QCPAxis::atBottom);
  return result;
}

bool QCPAxisRect::removeAxis(QCPAxis *axis)
{
  if (mAxes[axis->axisType()].contains(axis))
  {
    mAxes[axis->axisType()].removeOne(axis);
    parentPlot()->axisRemoved(axis);
    delete axis;
    return true;
  } else
  {
    qDebug() << Q_FUNC_INFO << "axis wasn't in expected hash bucket";
    return false;
  }
}

QList<QCPAbstractPlottable*> QCPAxisRect::plottables() const
{
  // Note: don't append all QCPAxis::plottables() into a list, because we might get duplicate entries
  QList<QCPAbstractPlottable*> result;
  for (int i=0; i<mParentPlot->mPlottables.size(); ++i)
  {
    if (mParentPlot->mPlottables.at(i)->keyAxis()->axisRect() == this ||mParentPlot->mPlottables.at(i)->valueAxis()->axisRect() == this)
      result.append(mParentPlot->mPlottables.at(i));
  }
  return result;
}

QList<QCPGraph*> QCPAxisRect::graphs() const
{
  // Note: don't append all QCPAxis::graphs() into a list, because we might get duplicate entries
  QList<QCPGraph*> result;
  for (int i=0; i<mParentPlot->mGraphs.size(); ++i)
  {
    if (mParentPlot->mGraphs.at(i)->keyAxis()->axisRect() == this || mParentPlot->mGraphs.at(i)->valueAxis()->axisRect() == this)
      result.append(mParentPlot->mGraphs.at(i));
  }
  return result;
}

QList<QCPAbstractItem *> QCPAxisRect::items() const
{
  // Note: don't append all QCPAxis::items() into a list, because we might get duplicate entries
  //       and miss those items that have this axis rect as clipAxisRect.
  QList<QCPAbstractItem*> result;
  for (int itemId=0; itemId<mParentPlot->mItems.size(); ++itemId)
  {
    if (mParentPlot->mItems.at(itemId)->clipAxisRect() == this)
    {
      result.append(mParentPlot->mItems.at(itemId));
      continue;
    }
    QList<QCPItemPosition*> positions = mParentPlot->mItems.at(itemId)->positions();
    for (int posId=0; posId<positions.size(); ++itemId)
    {
      if (positions.at(posId)->axisRect() == this ||
          positions.at(posId)->keyAxis()->axisRect() == this ||
          positions.at(posId)->valueAxis()->axisRect() == this)
      {
        result.append(mParentPlot->mItems.at(itemId));
        break;
      }
    }
  }
  return result;
}

/*!
  Sets \a pm as the axis background pixmap. The axis background pixmap will be drawn inside the
  axis rect, before anything else (e.g. the axes themselves, grids, graphs, etc.) is drawn. If the
  provided pixmap doesn't have the same size as the axis rect, scaling can be enabled with \ref
  setBackgroundScaled and the scaling mode (i.e. whether and how the aspect ratio is preserved) can
  be set with \ref setBackgroundScaledMode. To set all these options in one call, consider using
  the overloaded version of this function.

  \see setBackgroundScaled, setBackgroundScaledMode
*/
void QCPAxisRect::setBackground(const QPixmap &pm)
{
  mBackground = pm;
  mScaledBackground = QPixmap();
}

/*! \overload
  
  Allows setting the background pixmap of the axis rect, whether it shall be scaled and how it
  shall be scaled in one call.

  \see setBackground(const QPixmap &pm), setBackgroundScaled, setBackgroundScaledMode
*/
void QCPAxisRect::setBackground(const QPixmap &pm, bool scaled, Qt::AspectRatioMode mode)
{
  mBackground = pm;
  mScaledBackground = QPixmap();
  mBackgroundScaled = scaled;
  mBackgroundScaledMode = mode;
}

/*!
  Sets whether the axis background pixmap shall be scaled to fit the axis rect or not. If \a scaled
  is set to true, you may control whether and how the aspect ratio of the original pixmap is
  preserved with \ref setBackgroundScaledMode.
  
  Note that the scaled version of the original pixmap is buffered, so there is no performance
  penalty on replots. (Except when the axis rect is changed continuously.)
  
  \see setBackground, setBackgroundScaledMode
*/
void QCPAxisRect::setBackgroundScaled(bool scaled)
{
  mBackgroundScaled = scaled;
}

/*!
  If scaling of the axis background pixmap is enabled (\ref setBackgroundScaled), use this function to
  define whether and how the aspect ratio of the original pixmap passed to \ref setBackground is preserved.
  \see setBackground, setBackgroundScaled
*/
void QCPAxisRect::setBackgroundScaledMode(Qt::AspectRatioMode mode)
{
  mBackgroundScaledMode = mode;
}

/*! \internal

  If an axis background is provided via \ref setBackground, this function first buffers the scaled
  version depending on \ref setBackgroundScaled and \ref setBackgroundScaledMode and then draws it
  inside the current axisRect with the provided \a painter. The scaled version is buffered in
  mScaledBackground to prevent the need for rescaling at every redraw. It is only updated, when the
  axis rect has changed in a way that requires a rescale of the background pixmap (this is
  dependant on the \ref setBackgroundScaledMode), or when a differend axis backgroud was set.
  
  \see drawBackground, setBackground, setBackgroundScaled, setBackgroundScaledMode
*/
void QCPAxisRect::drawBackground(QCPPainter *painter)
{
  if (!mBackground.isNull())
  {
    if (mBackgroundScaled)
    {
      // check whether mScaledBackground needs to be updated:
      QSize scaledSize(mBackground.size());
      scaledSize.scale(mRect.size(), mBackgroundScaledMode);
      if (mScaledBackground.size() != scaledSize)
        mScaledBackground = mBackground.scaled(mRect.size(), mBackgroundScaledMode, Qt::SmoothTransformation);
      painter->drawPixmap(mRect.topLeft(), mScaledBackground, QRect(0, 0, mRect.width(), mRect.height()) & mScaledBackground.rect());
    } else
    {
      painter->drawPixmap(mRect.topLeft(), mBackground, QRect(0, 0, mRect.width(), mRect.height()));
    }
  }
}

void QCPAxisRect::updateAxisOffsets()
{
  QList<QCPAxis*> axesList;
  if (mAutoMargins.testFlag(QCP::msLeft))
  {
    axesList = mAxes.value(QCPAxis::atLeft);
    for (int i=1; i<axesList.size(); ++i)
      axesList.at(i)->setOffset(axesList.at(i-1)->offset() + axesList.at(i-1)->calculateMargin() + axesList.at(i)->tickLengthIn());
  }
  if (mAutoMargins.testFlag(QCP::msRight))
  {
    axesList = mAxes.value(QCPAxis::atRight);
    for (int i=1; i<axesList.size(); ++i)
      axesList.at(i)->setOffset(axesList.at(i-1)->offset() + axesList.at(i-1)->calculateMargin() + axesList.at(i)->tickLengthIn());
  }
  if (mAutoMargins.testFlag(QCP::msTop))
  {
    axesList = mAxes.value(QCPAxis::atTop);
    for (int i=1; i<axesList.size(); ++i)
      axesList.at(i)->setOffset(axesList.at(i-1)->offset() + axesList.at(i-1)->calculateMargin() + axesList.at(i)->tickLengthIn());
  }
  if (mAutoMargins.testFlag(QCP::msBottom))
  {
    axesList = mAxes.value(QCPAxis::atBottom);
    for (int i=1; i<axesList.size(); ++i)
      axesList.at(i)->setOffset(axesList.at(i-1)->offset() + axesList.at(i-1)->calculateMargin() + axesList.at(i)->tickLengthIn());
  }
}

void QCPAxisRect::update()
{
  updateAxisOffsets(); // call this before QCPLayoutElement::update(), because calculateAutoMargins() will use the offsets for total margin calculation
  QCPLayoutElement::update();
}

QMargins QCPAxisRect::calculateAutoMargins() const
{
  QMargins result;
  QList<QCPAxis*> axesList;
  
  // note: only need to look at the last (outer most) axis to determine the total margin
  if (mAutoMargins.testFlag(QCP::msLeft))
  {
    axesList = mAxes.value(QCPAxis::atLeft);
    if (axesList.size() > 0)
      result.setLeft(axesList.last()->offset() + axesList.last()->calculateMargin());
  }
  if (mAutoMargins.testFlag(QCP::msRight))
  {
    axesList = mAxes.value(QCPAxis::atRight);
    if (axesList.size() > 0)
      result.setRight(axesList.last()->offset() + axesList.last()->calculateMargin());
  }
  if (mAutoMargins.testFlag(QCP::msTop))
  {
    axesList = mAxes.value(QCPAxis::atTop);
    if (axesList.size() > 0)
      result.setTop(axesList.last()->offset() + axesList.last()->calculateMargin());
  }
  if (mAutoMargins.testFlag(QCP::msBottom))
  {
    axesList = mAxes.value(QCPAxis::atBottom);
    if (axesList.size() > 0)
      result.setBottom(axesList.last()->offset() + axesList.last()->calculateMargin());
  }
  return result;
}
