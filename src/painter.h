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

#ifndef QCP_PAINTER_H
#define QCP_PAINTER_H

#include "global.h"

class QCPPainter;

class QCP_LIB_DECL QCPScatterPainter
{
public:
  QCPScatterPainter() {}
  virtual ~QCPScatterPainter() {}
  virtual void drawScatter(QCPPainter *painter, double x, double y, double size, QCP::ScatterStyle style);

protected:
  QPixmap mScatterPixmap;
};

class QCP_LIB_DECL QCPScatterPainter2 : public QCPScatterPainter
{
public:
  QCPScatterPainter2() {}
  virtual ~QCPScatterPainter2() {}
  virtual void drawScatter(QCPPainter *painter, double x, double y, double size, QCP::ScatterStyle style);
};

class QCP_LIB_DECL QCPPainter : public QPainter
{
Q_GADGET
public:
  /*!
    Defines special modes the painter can operate in. They disable or enable certain subsets of features/fixes/workarounds,
    depending on whether they are wanted on the respective output device.
  */
  enum PainterMode {pmDefault       = 0x00  ///< <tt>0x00</tt> Default mode for painting on screen devices
                    ,pmVectorized   = 0x01  ///< <tt>0x01</tt> Mode for vectorized painting (e.g. PDF export). For example, this prevents some antialiasing fixes.
                    ,pmNoCaching    = 0x02  ///< <tt>0x02</tt> Mode for all sorts of exports (e.g. PNG, PDF). For example, this prevents using cached pixmap labels
                    ,pmScaledPen    = 0x04  ///< <tt>0x04</tt> Enables a workaround for scaled pens, needed for scaled rastered export.
                   };
  Q_ENUMS(PainterMode)
  Q_DECLARE_FLAGS(PainterModes, PainterMode)
  
  QCPPainter();
  QCPPainter(QPaintDevice *device);
  ~QCPPainter();
  
  // getters:
  QPixmap scatterPixmap() const { return mScatterPixmap; }
  bool antialiasing() const { return testRenderHint(QPainter::Antialiasing); }
  PainterModes modes() const { return mModes; }
  
  // setters:
  void setScatterPixmap(const QPixmap pm);
  void setAntialiasing(bool enabled);
  void setMode(PainterMode mode, bool enabled=true);
  void setModes(PainterModes modes);
 
  // methods hiding non-virtual base class functions (QPainter bug workarounds):
  void setPen(const QPen &pen);
  void setPen(const QColor &color);
  void setPen(Qt::PenStyle penStyle);
  void drawLine(const QLineF &line);
  void drawLine(const QPointF &p1, const QPointF &p2) {drawLine(QLineF(p1, p2));}
  void save();
  void restore();

  // helpers:
  void fixScaledPen();
  void prepareScatter();
  void drawScatter(double x, double y, double size, QCP::ScatterStyle style);
  
protected:
  PainterModes mModes;
  QPixmap mScatterPixmap;
  bool mIsAntialiasing;
  QStack<bool> mAntialiasingStack;
  QCPScatterPainter *mScatterPainter;
};
Q_DECLARE_OPERATORS_FOR_FLAGS(QCPPainter::PainterModes)

#endif // QCP_PAINTER_H
