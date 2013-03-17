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

#include "layoutelement-plottitle.h"

#include "../painter.h"
#include "../core.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// QCPPlotTitle
////////////////////////////////////////////////////////////////////////////////////////////////////

QCPPlotTitle::QCPPlotTitle(QCustomPlot *parentPlot) : 
  QCPLayerable(parentPlot, "axes"),
  mFont(QFont(parentPlot->font().family(), parentPlot->font().pointSize()*1.5, QFont::Bold)),
  mTextColor(Qt::black),
  mSelectedFont(QFont(parentPlot->font().family(), parentPlot->font().pointSize()*1.6, QFont::Bold)),
  mSelectedTextColor(Qt::blue),
  mSelectable(false),
  mSelected(false)
{
  setMargins(QMargins(5, 5, 5, 0));
}

QCPPlotTitle::QCPPlotTitle(QCustomPlot *parentPlot, const QString &text) :
  QCPLayerable(parentPlot, "axes"),
  mText(text),
  mFont(QFont(parentPlot->font().family(), parentPlot->font().pointSize()*1.5, QFont::Bold)),
  mTextColor(Qt::black),
  mSelectedFont(QFont(parentPlot->font().family(), parentPlot->font().pointSize()*1.6, QFont::Bold)),
  mSelectedTextColor(Qt::blue),
  mSelectable(false),
  mSelected(false)
{
  setMargins(QMargins(5, 5, 5, 0));
}

void QCPPlotTitle::setText(const QString &text)
{
  mText = text;
}

void QCPPlotTitle::setFont(const QFont &font)
{
  mFont = font;
}

void QCPPlotTitle::setTextColor(const QColor &color)
{
  mTextColor = color;
}

void QCPPlotTitle::setSelectedFont(const QFont &font)
{
  mSelectedFont = font;
}

void QCPPlotTitle::setSelectedTextColor(const QColor &color)
{
  mSelectedTextColor = color;
}

void QCPPlotTitle::setSelectable(bool selectable)
{
  mSelectable = selectable;
}

void QCPPlotTitle::setSelected(bool selected)
{
  if (mSelected != selected)
  {
    mSelected = selected;
    emit selectionChanged(mSelected);
  }
}

void QCPPlotTitle::applyDefaultAntialiasingHint(QCPPainter *painter) const
{
  applyAntialiasingHint(painter, mAntialiased, QCP::aeNone);
}

void QCPPlotTitle::draw(QCPPainter *painter)
{
  painter->setFont(mainFont());
  painter->setPen(QPen(mainTextColor()));
  painter->drawText(mRect, Qt::AlignCenter, mText, &mTextBoundingRect);
}

QSize QCPPlotTitle::minimumSizeHint() const
{
  QFontMetrics metrics(mFont);
  QSize result = metrics.boundingRect(0, 0, 0, 0, Qt::AlignCenter, mText).size();
  result.rwidth() += mMargins.left() + mMargins.right();
  result.rheight() += mMargins.top() + mMargins.bottom();
  return result;
}

QSize QCPPlotTitle::maximumSizeHint() const
{
  QFontMetrics metrics(mFont);
  QSize result = metrics.boundingRect(0, 0, 0, 0, Qt::AlignCenter, mText).size();
  result.rheight() += mMargins.top() + mMargins.bottom();
  result.setWidth(QWIDGETSIZE_MAX);
  return result;
}

void QCPPlotTitle::selectEvent(QMouseEvent *event, bool additive, const QVariant &details, bool *selectionStateChanged)
{
  Q_UNUSED(event)
  Q_UNUSED(details)
  if (mSelectable)
  {
    bool selBefore = mSelected;
    setSelected(additive ? !mSelected : true);
    if (selectionStateChanged)
      *selectionStateChanged = mSelected != selBefore;
  }
}

void QCPPlotTitle::deselectEvent(bool *selectionStateChanged)
{
  if (mSelectable)
  {
    bool selBefore = mSelected;
    setSelected(false);
    if (selectionStateChanged)
      *selectionStateChanged = mSelected != selBefore;
  }
}

double QCPPlotTitle::selectTest(const QPointF &pos, bool onlySelectable, QVariant *details) const
{
  Q_UNUSED(details)
  if (onlySelectable && !mSelectable)
    return -1;
  
  if (mTextBoundingRect.contains(pos.toPoint()))
    return mParentPlot->selectionTolerance()*0.99;
  else
    return -1;
}

QFont QCPPlotTitle::mainFont() const
{
  return mSelected ? mSelectedFont : mFont;
}

QColor QCPPlotTitle::mainTextColor() const
{
  return mSelected ? mSelectedTextColor : mTextColor;
}

