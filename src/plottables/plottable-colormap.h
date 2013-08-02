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

#ifndef QCP_PLOTTABLE_COLORMAP_H
#define QCP_PLOTTABLE_COLORMAP_H

#include "../global.h"
#include "../range.h"
#include "../plottable.h"

class QCPPainter;
class QCPAxis;
class QCPColorMap;

class QCP_LIB_DECL QCPColorMapData
{
public:
  QCPColorMapData(const QSize &size, const QCPRange keyRange, const QCPRange valueRange);
  
  QSize size() const { return mSize; }
  double value(double key, double value);
  double cell(int key, int value);
  QCPRange keyRange() const { return mKeyRange; }
  QCPRange valueRange() const { return mValueRange; }
  QCPRange minMax() const { return mMinMax; }
  
  void setSize(const QSize &size);
  void setValue(double key, double value, double z);
  void setCell(int key, int value, double z);
  void setRange(const QCPRange keyRange, const QCPRange valueRange);
  void setMinMax(const QCPRange minMax);
  void recalculateMinMax();
  
  void clear();
  void reset(double z=0);
  bool isEmpty() const { return mIsEmpty; }
  
protected:
  double *mData;
  QSize mSize;
  QCPRange mKeyRange, mValueRange;
  QCPRange mMinMax;
  bool mModified;
  bool mIsEmpty;
  
  friend class QCPColorMap;
};

class QCP_LIB_DECL QCPColorMap : public QCPAbstractPlottable
{
  Q_OBJECT
public:
  explicit QCPColorMap(QCPAxis *keyAxis, QCPAxis *valueAxis);
  virtual ~QCPColorMap();
  
  // getters:
  QCPColorMapData *data() const { return mData; }
  
  // setters:
  void setData(QCPColorMapData *data, bool copy=false);
  
  // non-property methods:
  virtual void clearData();
  virtual double selectTest(const QPointF &pos, bool onlySelectable, QVariant *details=0) const;
  
protected:
  QCPColorMapData *mData;
  QImage mMapImage;
  QRgb *mColorGradient;
  int mColorGradientLevels;
  
  virtual void updateGradient(int levels);
  virtual void updateMapImage();
  virtual void draw(QCPPainter *painter);
  virtual void drawLegendIcon(QCPPainter *painter, const QRectF &rect) const;
  
  QRgb wavelengthToRgb(double nm);
  
  virtual QCPRange getKeyRange(bool &validRange, SignDomain inSignDomain=sdBoth) const;
  virtual QCPRange getValueRange(bool &validRange, SignDomain inSignDomain=sdBoth) const;
  
  friend class QCustomPlot;
  friend class QCPLegend;
};

#endif // QCP_PLOTTABLE_COLORMAP_H
 
