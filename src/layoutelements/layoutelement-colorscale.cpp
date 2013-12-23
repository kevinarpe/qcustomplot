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
  QCPLayoutElement(parentPlot)
{
}

QCPColorScale::~QCPColorScale()
{

}

void QCPColorScale::update()
{
  QCPLayoutElement::update();
}

/* inherits documentation from base class */
void QCPColorScale::applyDefaultAntialiasingHint(QCPPainter *painter) const
{
  painter->setAntialiasing(false);
}

/* inherits documentation from base class */
void QCPColorScale::draw(QCPPainter *painter)
{
  
}

/* inherits documentation from base class */
int QCPColorScale::calculateAutoMargin(QCP::MarginSide side)
{

}

/*! \internal
  
  Event handler for when a mouse button is pressed on the axis rect. If the left mouse button is
  pressed, the range dragging interaction is initialized (the actual range manipulation happens in
  the \ref mouseMoveEvent).

  The mDragging flag is set to true and some anchor points are set that are needed to determine the
  distance the mouse was dragged in the mouse move/release events later.
  
  \see mouseMoveEvent, mouseReleaseEvent
*/
void QCPColorScale::mousePressEvent(QMouseEvent *event)
{
  /*
  mDragStart = event->pos(); // need this even when not LeftButton is pressed, to determine in releaseEvent whether it was a full click (no position change between press and release)
  if (event->buttons() & Qt::LeftButton)
  {
    mDragging = true;
    // Mouse range dragging interaction:
    if (mParentPlot->interactions().testFlag(QCP::iRangeDrag))
    {
      if (mRangeDragHorzAxis)
        mDragStartHorzRange = mRangeDragHorzAxis.data()->range();
      if (mRangeDragVertAxis)
        mDragStartVertRange = mRangeDragVertAxis.data()->range();
    }
  }
  */
}

/*! \internal
  
  Event handler for when the mouse is moved on the axis rect. If range dragging was activated in a
  preceding \ref mousePressEvent, the range is moved accordingly.
  
  \see mousePressEvent, mouseReleaseEvent
*/
void QCPColorScale::mouseMoveEvent(QMouseEvent *event)
{
  /*
  // Mouse range dragging interaction:
  if (mDragging && mParentPlot->interactions().testFlag(QCP::iRangeDrag))
  {
    if (mRangeDrag.testFlag(Qt::Horizontal))
    {
      if (QCPAxis *rangeDragHorzAxis = mRangeDragHorzAxis.data())
      {
        if (rangeDragHorzAxis->mScaleType == QCPAxis::stLinear)
        {
          double diff = rangeDragHorzAxis->pixelToCoord(mDragStart.x()) - rangeDragHorzAxis->pixelToCoord(event->pos().x());
          rangeDragHorzAxis->setRange(mDragStartHorzRange.lower+diff, mDragStartHorzRange.upper+diff);
        } else if (rangeDragHorzAxis->mScaleType == QCPAxis::stLogarithmic)
        {
          double diff = rangeDragHorzAxis->pixelToCoord(mDragStart.x()) / rangeDragHorzAxis->pixelToCoord(event->pos().x());
          rangeDragHorzAxis->setRange(mDragStartHorzRange.lower*diff, mDragStartHorzRange.upper*diff);
        }
      }
    }
    if (mRangeDrag.testFlag(Qt::Vertical))
    {
      if (QCPAxis *rangeDragVertAxis = mRangeDragVertAxis.data())
      {
        if (rangeDragVertAxis->mScaleType == QCPAxis::stLinear)
        {
          double diff = rangeDragVertAxis->pixelToCoord(mDragStart.y()) - rangeDragVertAxis->pixelToCoord(event->pos().y());
          rangeDragVertAxis->setRange(mDragStartVertRange.lower+diff, mDragStartVertRange.upper+diff);
        } else if (rangeDragVertAxis->mScaleType == QCPAxis::stLogarithmic)
        {
          double diff = rangeDragVertAxis->pixelToCoord(mDragStart.y()) / rangeDragVertAxis->pixelToCoord(event->pos().y());
          rangeDragVertAxis->setRange(mDragStartVertRange.lower*diff, mDragStartVertRange.upper*diff);
        }
      }
    }
    if (mRangeDrag != 0) // if either vertical or horizontal drag was enabled, do a replot
    {
      if (mParentPlot->noAntialiasingOnDrag())
        mParentPlot->setNotAntialiasedElements(QCP::aeAll);
      mParentPlot->replot();
    }
  }
  */
}

/* inherits documentation from base class */
void QCPColorScale::mouseReleaseEvent(QMouseEvent *event)
{
  Q_UNUSED(event)
  /*
  mDragging = false;
  if (mParentPlot->noAntialiasingOnDrag())
  {
    mParentPlot->setAntialiasedElements(mAADragBackup);
    mParentPlot->setNotAntialiasedElements(mNotAADragBackup);
  }
  */
}

/*! \internal
  
  Event handler for mouse wheel events. If rangeZoom is Qt::Horizontal, Qt::Vertical or both, the
  ranges of the axes defined as rangeZoomHorzAxis and rangeZoomVertAxis are scaled. The center of
  the scaling operation is the current cursor position inside the axis rect. The scaling factor is
  dependant on the mouse wheel delta (which direction the wheel was rotated) to provide a natural
  zooming feel. The Strength of the zoom can be controlled via \ref setRangeZoomFactor.
  
  Note, that event->delta() is usually +/-120 for single rotation steps. However, if the mouse
  wheel is turned rapidly, many steps may bunch up to one event, so the event->delta() may then be
  multiples of 120. This is taken into account here, by calculating \a wheelSteps and using it as
  exponent of the range zoom factor. This takes care of the wheel direction automatically, by
  inverting the factor, when the wheel step is negative (f^-1 = 1/f).
*/
void QCPColorScale::wheelEvent(QWheelEvent *event)
{
  /*
  // Mouse range zooming interaction:
  if (mParentPlot->interactions().testFlag(QCP::iRangeZoom))
  {
    if (mRangeZoom != 0)
    {
      double factor;
      double wheelSteps = event->delta()/120.0; // a single step delta is +/-120 usually
      if (mRangeZoom.testFlag(Qt::Horizontal))
      {
        factor = pow(mRangeZoomFactorHorz, wheelSteps);
        if (mRangeZoomHorzAxis.data())
          mRangeZoomHorzAxis.data()->scaleRange(factor, mRangeZoomHorzAxis.data()->pixelToCoord(event->pos().x()));
      }
      if (mRangeZoom.testFlag(Qt::Vertical))
      {
        factor = pow(mRangeZoomFactorVert, wheelSteps);
        if (mRangeZoomVertAxis.data())
          mRangeZoomVertAxis.data()->scaleRange(factor, mRangeZoomVertAxis.data()->pixelToCoord(event->pos().y()));
      }
      mParentPlot->replot();
    }
  }
  */
}










