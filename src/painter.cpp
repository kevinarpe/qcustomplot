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

#include "painter.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// QCPPainter
////////////////////////////////////////////////////////////////////////////////////////////////////

/*! \class QCPPainter
  \brief QPainter subclass used internally
  
  This internal class is used to provide some extended functionality e.g. for tweaking position
  consistency between antialiased and non-antialiased painting and drawing common shapes (like
  scatter symbols). Further it provides workarounds for QPainter quirks.
  
  \warning This class intentionally hides non-virtual functions of QPainter, e.g. setPen, save and
  restore. So while it is possible to pass a QCPPainter instance to a function that expects a
  QPainter pointer, some of the workarounds and tweaks will be unavailable to the function (because
  it will call the base class implementations of the functions actually hidden by QCPPainter).
*/

/*!
  Creates a new QCPPainter instance and sets default values
*/
QCPPainter::QCPPainter() :
  QPainter(),
  mModes(pmDefault),
  mIsAntialiasing(false)
{
}

/*!
  Creates a new QCPPainter instance on the specified paint \a device and sets default values. Just
  like the analogous QPainter constructor, begins painting on \a device immediately.
*/
QCPPainter::QCPPainter(QPaintDevice *device) :
  QPainter(device),
  mModes(pmDefault),
  mIsAntialiasing(false)
{
}

QCPPainter::~QCPPainter()
{
}

/*!
  Sets the pen of the painter and applies certain fixes to it, depending on the mode of this
  QCPPainter.
  
  \note this function hides the non-virtual base class implementation.
*/
void QCPPainter::setPen(const QPen &pen)
{
  QPainter::setPen(pen);
  if (mModes.testFlag(pmScaledPen))
    fixScaledPen();
}

/*! \overload
  
  Sets the pen (by color) of the painter and applies certain fixes to it, depending on the mode of
  this QCPPainter.
  
  \note this function hides the non-virtual base class implementation.
*/
void QCPPainter::setPen(const QColor &color)
{
  QPainter::setPen(color);
  if (mModes.testFlag(pmScaledPen))
    fixScaledPen();
}

/*! \overload
  
  Sets the pen (by style) of the painter and applies certain fixes to it, depending on the mode of
  this QCPPainter.
  
  \note this function hides the non-virtual base class implementation.
*/
void QCPPainter::setPen(Qt::PenStyle penStyle)
{
  QPainter::setPen(penStyle);
  if (mModes.testFlag(pmScaledPen))
    fixScaledPen();
}

/*! \overload
  
  Works around a Qt bug introduced with Qt 4.8 which makes drawing QLineF unpredictable when
  antialiasing is disabled.
  
  \note this function hides the non-virtual base class implementation.
*/
void QCPPainter::drawLine(const QLineF &line)
{
  if (mIsAntialiasing)
    QPainter::drawLine(line);
  else
    QPainter::drawLine(line.toLine());
}

/*! 
  Sets whether painting uses antialiasing or not. Use this method instead of using setRenderHint
  with QPainter::Antialiasing directly, as it allows QCPPainter to regain pixel exactness between
  antialiased and non-antialiased painting (Since Qt uses slightly different coordinate systems for
  AA/Non-AA painting).
*/
void QCPPainter::setAntialiasing(bool enabled)
{
  if (mModes.testFlag(pmVectorized))
    return;
  
  setRenderHint(QPainter::Antialiasing, enabled);
  if (mIsAntialiasing != enabled)
  {
    if (mIsAntialiasing)
      translate(-0.5, -0.5);
    else
      translate(0.5, 0.5);
    mIsAntialiasing = enabled;
  }
}

/*!
  Sets the mode of the painter. This controls whether the painter shall adjust its
  fixes/workarounds optimized for certain output devices.
*/
void QCPPainter::setModes(QCPPainter::PainterModes modes)
{
  mModes = modes;
}

/*! \overload
  Sets the mode of the painter. This controls whether the painter shall adjust its
  fixes/workarounds optimized for certain output devices.
*/
void QCPPainter::setMode(QCPPainter::PainterMode mode, bool enabled)
{
  if (!enabled && mModes.testFlag(mode))
    mModes &= ~mode;
  else if (enabled && !mModes.testFlag(mode))
    mModes |= mode;
}

/*!
  Saves the painter (see QPainter::save). Since QCPPainter adds some new internal state to
  QPainter, the save/restore functions are reimplemented to also save/restore those members.
  
  \note this function hides the non-virtual base class implementation.
  
  \see restore
*/
void QCPPainter::save()
{
  mAntialiasingStack.push(mIsAntialiasing);
  QPainter::save();
}

/*!
  Restores the painter (see QPainter::restore). Since QCPPainter adds some new internal state to
  QPainter, the save/restore functions are reimplemented to also save/restore those members.
  
  \note this function hides the non-virtual base class implementation.
  
  \see save
*/
void QCPPainter::restore()
{
  if (!mAntialiasingStack.isEmpty())
    mIsAntialiasing = mAntialiasingStack.pop();
  else
    qDebug() << Q_FUNC_INFO << "Unbalanced save/restore";
  QPainter::restore();
}

/*!
  Provides a workaround for a QPainter bug that prevents scaling of pen widths for pens with width
  0, although the QPainter::NonCosmeticDefaultPen render hint is set.
  
  Changes the pen width from 0 to 1, if appropriate.
  
  Does nothing if the QCPPainter is not in scaled pen mode (\ref setMode).
*/
void QCPPainter::fixScaledPen()
{
  if (mModes.testFlag(pmScaledPen) && pen().isCosmetic() && qFuzzyIsNull(pen().widthF()))
  {
    QPen p = pen();
    p.setWidth(1);
    QPainter::setPen(p);
  }
}


////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// QCPScatterStyle
////////////////////////////////////////////////////////////////////////////////////////////////////


QCPScatterStyle::QCPScatterStyle() :
  mPenDefined(false),
  mSize(6),
  mShape(ssNone),
  mPen(Qt::NoPen),
  mBrush(Qt::NoBrush)
{
}

QCPScatterStyle::QCPScatterStyle(ScatterShape shape, double size) :
  mPenDefined(false),
  mSize(size),
  mShape(shape),
  mPen(Qt::NoPen),
  mBrush(Qt::NoBrush)
{
}

QCPScatterStyle::QCPScatterStyle(ScatterShape shape, const QColor &color, double size) :
  mPenDefined(true),
  mSize(size),
  mShape(shape),
  mPen(QPen(color)),
  mBrush(Qt::NoBrush)
{
}

QCPScatterStyle::QCPScatterStyle(ScatterShape shape, const QColor &color, const QColor &fill, double size) :
  mPenDefined(true),
  mSize(size),
  mShape(shape),
  mPen(QPen(color)),
  mBrush(QBrush(fill))
{
}

// doc: maybe warn that ..., Qt::NoPen, Qt::blue,... will call other overload, use QBrush(Qt::blue)
QCPScatterStyle::QCPScatterStyle(ScatterShape shape, const QPen &pen, const QBrush &brush, double size) :
  mPenDefined(pen.style() != Qt::NoPen),
  mSize(size),
  mShape(shape),
  mPen(pen),
  mBrush(brush)
{
}

QCPScatterStyle::QCPScatterStyle(const QPixmap &pixmap) :
  mPenDefined(false),
  mSize(5),
  mShape(ssPixmap),
  mPen(Qt::NoPen),
  mBrush(Qt::NoBrush),
  mPixmap(pixmap)
{
}

QCPScatterStyle::QCPScatterStyle(const QPainterPath &customPath, const QPen &pen, const QBrush &brush, double size) :
  mPenDefined(false),
  mSize(size),
  mShape(ssCustom),
  mPen(pen),
  mBrush(brush),
  mCustomPath(customPath)
{
}

void QCPScatterStyle::setSize(double size)
{
  mSize = size;
}

void QCPScatterStyle::setShape(QCPScatterStyle::ScatterShape shape)
{
  mShape = shape;
}

void QCPScatterStyle::setPen(const QPen &pen)
{
  mPen = pen;
}

void QCPScatterStyle::setBrush(const QBrush &brush)
{
  mBrush = brush;
}

void QCPScatterStyle::setPixmap(const QPixmap &pixmap)
{
  mPixmap = pixmap;
}

void QCPScatterStyle::setCustomPath(const QPainterPath &customPath)
{
  mCustomPath = customPath;
}

void QCPScatterStyle::applyTo(QCPPainter *painter, const QPen &defaultPen) const
{
  painter->setPen(mPenDefined ? mPen : defaultPen);
  painter->setBrush(mBrush);
}

void QCPScatterStyle::drawShape(QCPPainter *painter, QPointF pos) const
{
  drawShape(painter, pos.x(), pos.y());
}

void QCPScatterStyle::drawShape(QCPPainter *painter, double x, double y) const
{
  double w = mSize/2.0;
  switch (mShape)
  {
    case ssNone: break;
    case ssDot:
    {
      painter->drawLine(QPointF(x, y), QPointF(x+0.0001, y));
      break;
    }
    case ssCross:
    {
      painter->drawLine(QLineF(x-w, y-w, x+w, y+w));
      painter->drawLine(QLineF(x-w, y+w, x+w, y-w));
      break;
    }
    case ssPlus:
    {
      painter->drawLine(QLineF(x-w,   y, x+w,   y));
      painter->drawLine(QLineF(  x, y+w,   x, y-w));
      break;
    }
    case ssCircle:
    {
      painter->drawEllipse(QPointF(x , y), w, w);
      break;
    }
    case ssDisc:
    {
      QBrush b = painter->brush();
      painter->setBrush(painter->pen().color());
      painter->drawEllipse(QPointF(x , y), w, w);
      painter->setBrush(b);
      break;
    }
    case ssSquare:
    {
      painter->drawRect(QRectF(x-w, y-w, mSize, mSize));
      break;
    }
    case ssDiamond:
    {
      painter->drawLine(QLineF(x-w,   y,   x, y-w));
      painter->drawLine(QLineF(  x, y-w, x+w,   y));
      painter->drawLine(QLineF(x+w,   y,   x, y+w));
      painter->drawLine(QLineF(  x, y+w, x-w,   y));
      break;
    }
    case ssStar:
    {
      painter->drawLine(QLineF(x-w,   y, x+w,   y));
      painter->drawLine(QLineF(  x, y+w,   x, y-w));
      painter->drawLine(QLineF(x-w*0.707, y-w*0.707, x+w*0.707, y+w*0.707));
      painter->drawLine(QLineF(x-w*0.707, y+w*0.707, x+w*0.707, y-w*0.707));
      break;
    }
    case ssTriangle:
    {
       painter->drawLine(QLineF(x-w, y+0.755*w, x+w, y+0.755*w));
       painter->drawLine(QLineF(x+w, y+0.755*w,   x, y-0.977*w));
       painter->drawLine(QLineF(  x, y-0.977*w, x-w, y+0.755*w));
      break;
    }
    case ssTriangleInverted:
    {
       painter->drawLine(QLineF(x-w, y-0.755*w, x+w, y-0.755*w));
       painter->drawLine(QLineF(x+w, y-0.755*w,   x, y+0.977*w));
       painter->drawLine(QLineF(  x, y+0.977*w, x-w, y-0.755*w));
      break;
    }
    case ssCrossSquare:
    {
       painter->drawLine(QLineF(x-w, y-w, x+w*0.95, y+w*0.95));
       painter->drawLine(QLineF(x-w, y+w*0.95, x+w*0.95, y-w));
       painter->drawRect(QRectF(x-w, y-w, mSize, mSize));
      break;
    }
    case ssPlusSquare:
    {
       painter->drawLine(QLineF(x-w,   y, x+w*0.95,   y));
       painter->drawLine(QLineF(  x, y+w,        x, y-w));
       painter->drawRect(QRectF(x-w, y-w, mSize, mSize));
      break;
    }
    case ssCrossCircle:
    {
       painter->drawLine(QLineF(x-w*0.707, y-w*0.707, x+w*0.670, y+w*0.670));
       painter->drawLine(QLineF(x-w*0.707, y+w*0.670, x+w*0.670, y-w*0.707));
       painter->drawEllipse(QPointF(x, y), w, w);
      break;
    }
    case ssPlusCircle:
    {
       painter->drawLine(QLineF(x-w,   y, x+w,   y));
       painter->drawLine(QLineF(  x, y+w,   x, y-w));
       painter->drawEllipse(QPointF(x, y), w, w);
      break;
    }
    case ssPeace:
    {
       painter->drawLine(QLineF(x, y-w,         x,       y+w));
       painter->drawLine(QLineF(x,   y, x-w*0.707, y+w*0.707));
       painter->drawLine(QLineF(x,   y, x+w*0.707, y+w*0.707));
       painter->drawEllipse(QPointF(x, y), w, w);
      break;
    }
    case ssPixmap:
    {
      painter->drawPixmap(x-mPixmap.width()*0.5, y-mPixmap.height()*0.5, mPixmap);
      break;
    }
    case ssCustom:
    {
      QTransform t = painter->transform();
      painter->translate(x, y);
      painter->scale(mSize/6.0, mSize/6.0);
      painter->drawPath(mCustomPath);
      painter->setTransform(t);
      break;
    }
  }
}

/* at 0, 0
void QCPScatterStyle::drawShape(QCPPainter *painter) const
{
  double w = mSize/2.0;
  switch (mShape)
  {
    case ssNone: break;
    case ssDot:
    {
      painter->drawLine(QPointF(0, 0), QPointF(0.0001, 0));
      break;
    }
    case ssCross:
    {
      painter->drawLine(QLineF(-w, -w, +w, +w));
      painter->drawLine(QLineF(-w, +w, +w, -w));
      break;
    }
    case ssPlus:
    {
      painter->drawLine(QLineF(-w,  0, +w,  0));
      painter->drawLine(QLineF( 0, +w,  0, -w));
      break;
    }
    case ssCircle:
    {
      painter->drawEllipse(QPointF(0 , 0), w, w);
      break;
    }
    case ssSquare:
    {
      painter->drawRect(QRectF(-w, -w, mSize, mSize));
      break;
    }
    case ssDiamond:
    {
      painter->drawLine(QLineF(-w,  0,  0, -w));
      painter->drawLine(QLineF( 0, -w, +w,  0));
      painter->drawLine(QLineF(+w,  0,  0, +w));
      painter->drawLine(QLineF( 0, +w, -w,  0));
      break;
    }
    case ssStar:
    {
      painter->drawLine(QLineF(-w,  0, +w,  0));
      painter->drawLine(QLineF( 0, +w,  0, -w));
      painter->drawLine(QLineF(-w*0.707, -w*0.707, +w*0.707, +w*0.707));
      painter->drawLine(QLineF(-w*0.707, +w*0.707, +w*0.707, -w*0.707));
      break;
    }
    case ssTriangle:
    {
       painter->drawLine(QLineF(-w, +0.755*w, +w, +0.755*w));
       painter->drawLine(QLineF(+w, +0.755*w,  0, -0.977*w));
       painter->drawLine(QLineF( 0, -0.977*w, -w, +0.755*w));
      break;
    }
    case ssTriangleInverted:
    {
       painter->drawLine(QLineF(-w, -0.755*w, +w, -0.755*w));
       painter->drawLine(QLineF(+w, -0.755*w,  0, +0.977*w));
       painter->drawLine(QLineF( 0, +0.977*w, -w, -0.755*w));
      break;
    }
    case ssCrossSquare:
    {
       painter->drawLine(QLineF(-w, -w, +w*0.95, +w*0.95));
       painter->drawLine(QLineF(-w, +w*0.95, +w*0.95, -w));
       painter->drawRect(QRectF(-w, -w, mSize, mSize));
      break;
    }
    case ssPlusSquare:
    {
       painter->drawLine(QLineF(-w,  0, +w*0.95,  0));
       painter->drawLine(QLineF( 0, +w,  0, -w));
       painter->drawRect(QRectF(-w, -w, mSize, mSize));
      break;
    }
    case ssCrossCircle:
    {
       painter->drawLine(QLineF(-w*0.707, -w*0.707, +w*0.670, +w*0.670));
       painter->drawLine(QLineF(-w*0.707, +w*0.670, +w*0.670, -w*0.707));
       painter->drawEllipse(QPointF(0, 0), w, w);
      break;
    }
    case ssPlusCircle:
    {
       painter->drawLine(QLineF(-w,  0, +w,  0));
       painter->drawLine(QLineF( 0, +w,  0, -w));
       painter->drawEllipse(QPointF(0, 0), w, w);
      break;
    }
    case ssPeace:
    {
       painter->drawLine(QLineF(0, -w,  0, +w));
       painter->drawLine(QLineF(0,  0, -w*0.707, +w*0.707));
       painter->drawLine(QLineF(0,  0, +w*0.707, +w*0.707));
       painter->drawEllipse(QPointF(0, 0), w, w);
      break;
    }
    case ssPixmap:
    {
      painter->drawPixmap(-mPixmap.width()*0.5, -mPixmap.height()*0.5, mPixmap);
      break;
    }
    case ssCustom:
    {
      painter->drawPath(mCustomPath);
      break;
    }
  }
}
*/





