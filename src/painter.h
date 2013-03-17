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

class QCP_LIB_DECL QCPScatterStyle
{
public:
  /*!
    Defines the shape used for scatter points.

    On plottables/items that draw scatters, the sizes of these visualizations (with exception of
    \ref sDot and \ref sPixmap) can be controlled with the \a setSize function. Scatters are drawn
    with the pen and brush specified with \ref setPen and \ref setBrush.

    \see QCPGraph::setScatterStyle, QCPStatisticalBox::setOutlierStyle
  */
  enum ScatterShape { ssNone       ///< \enumimage{ssNone.png} no scatter symbols are drawn (e.g. in QCPGraph, data only represented with lines)
                      ,ssDot       ///< \enumimage{ssDot.png} a single pixel (use \ref ssDisc or \ref ssCircle if you want a round shape with a certain radius)
                      ,ssCross     ///< \enumimage{ssCross.png} a cross
                      ,ssPlus      ///< \enumimage{ssPlus.png} a plus
                      ,ssCircle    ///< \enumimage{ssCircle.png} a circle
                      ,ssDisc      ///< \enumimage{ssDisc.png} a circle which is filled with the pen (not the brush as ssCircle)
                      ,ssSquare    ///< \enumimage{ssSquare.png} a square
                      ,ssDiamond   ///< \enumimage{ssDiamond.png} a diamond
                      ,ssStar      ///< \enumimage{ssStar.png} a star with eight arms, i.e. a combination of cross and plus
                      ,ssTriangle  ///< \enumimage{ssTriangle.png} an equilateral triangle which is not filled, standing on baseline
                      ,ssTriangleInverted ///< \enumimage{ssTriangleInverted.png} an equilateral triangle which is not filled, standing on corner
                      ,ssCrossSquare      ///< \enumimage{ssCrossSquare.png} a square which is not filled, with a cross inside
                      ,ssPlusSquare       ///< \enumimage{ssPlusSquare.png} a square which is not filled, with a plus inside
                      ,ssCrossCircle      ///< \enumimage{ssCrossCircle.png} a circle which is not filled, with a cross inside
                      ,ssPlusCircle       ///< \enumimage{ssPlusCircle.png} a circle which is not filled, with a plus inside
                      ,ssPeace     ///< \enumimage{ssPeace.png} a circle which is not filled, with one vertical and two downward diagonal lines
                      ,ssPixmap    ///< \enumimage{ssPixmap.png} a custom pixmap specified by setScatterPixmap, centered on the data point coordinates
                      ,ssCustom    ///< \enumimage{ssCustom.png} custom painter operations are performed per scatter (As QPainterPath, see \ref setCustomPath)
                    };

  QCPScatterStyle();
  QCPScatterStyle(ScatterShape shape, double size=6);
  QCPScatterStyle(ScatterShape shape, const QColor &color, double size);
  QCPScatterStyle(ScatterShape shape, const QColor &color, const QColor &fill, double size);
  QCPScatterStyle(ScatterShape shape, const QPen &pen, const QBrush &brush, double size);
  QCPScatterStyle(const QPixmap &pixmap);
  QCPScatterStyle(const QPainterPath &customPath, const QPen &pen, const QBrush &brush=Qt::NoBrush, double size=6);

  // getters:
  double size() const { return mSize; }
  ScatterShape shape() const { return mShape; }
  QPen pen() const { return mPen; }
  QBrush brush() const { return mBrush; }
  QPixmap pixmap() const { return mPixmap; }
  QPainterPath customPath() const { return mCustomPath; }

  // setters:
  void setSize(double size);
  void setShape(ScatterShape shape);
  void setPen(const QPen &pen);
  void setBrush(const QBrush &brush);
  void setPixmap(const QPixmap &pixmap);
  void setCustomPath(const QPainterPath &customPath);

  // non-property methods:
  bool isNone() const { return mShape == ssNone; }
  bool isPenDefined() const { return mPenDefined; }
  void applyTo(QCPPainter *painter, const QPen &defaultPen) const;
  void drawShape(QCPPainter *painter, QPointF pos) const;
  void drawShape(QCPPainter *painter, double x, double y) const;

protected:
  bool mPenDefined;
  double mSize;
  ScatterShape mShape;
  QPen mPen;
  QBrush mBrush;
  QPixmap mPixmap;
  QPainterPath mCustomPath;
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
  bool antialiasing() const { return testRenderHint(QPainter::Antialiasing); }
  PainterModes modes() const { return mModes; }

  // setters:
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
  
protected:
  PainterModes mModes;
  bool mIsAntialiasing;
  QStack<bool> mAntialiasingStack;
};
Q_DECLARE_OPERATORS_FOR_FLAGS(QCPPainter::PainterModes)

#endif // QCP_PAINTER_H
