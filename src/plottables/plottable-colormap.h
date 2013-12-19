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
  QCPColorMapData(int keySize, int valueSize, const QCPRange keyRange, const QCPRange valueRange);
  
  int keySize() const { return mKeySize; }
  int valueSize() const { return mValueSize; }
  double data(double key, double value);
  double cell(int keyIndex, int valueIndex);
  QCPRange keyRange() const { return mKeyRange; }
  QCPRange valueRange() const { return mValueRange; }
  QCPRange minMax() const { return mMinMax; }
  
  void setSize(int keySize, int valueSize);
  void setKeySize(int keySize);
  void setValueSize(int valueSize);
  void setData(double key, double value, double z);
  void setCell(int keyIndex, int valueIndex, double z);
  void setRange(const QCPRange keyRange, const QCPRange valueRange);
  void setMinMax(const QCPRange minMax);
  void recalculateMinMax();
  
  void clear();
  void fill(double z=0);
  bool isEmpty() const { return mIsEmpty; }
  
protected:
  double *mData;
  int mKeySize, mValueSize;
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
  QCPColorMapData *data() const { return mMapData; }
  bool interpolate() const { return mInterpolate; }
  bool tightBoundary() const { return mTightBoundary; }
  
  // setters:
  void setData(QCPColorMapData *data, bool copy=false);
  void setInterpolate(bool enabled);
  void setTightBoundary(bool enabled);
  
  // non-property methods:
  virtual void clearData();
  virtual double selectTest(const QPointF &pos, bool onlySelectable, QVariant *details=0) const;
  
protected:
  QCPColorMapData *mMapData;
  QImage mMapImage;
  QRgb *mColorGradient;
  int mColorGradientLevels;
  bool mInterpolate;
  bool mTightBoundary;
  
  virtual void updateGradient(int levels);
  virtual void updateMapImage();
  virtual void draw(QCPPainter *painter);
  virtual void drawLegendIcon(QCPPainter *painter, const QRectF &rect) const;
  
  QRgb wavelengthToRgb(double nm);
  
  virtual QCPRange getKeyRange(bool &foundRange, SignDomain inSignDomain=sdBoth) const;
  virtual QCPRange getValueRange(bool &foundRange, SignDomain inSignDomain=sdBoth) const;
  
  friend class QCustomPlot;
  friend class QCPLegend;
};

#endif // QCP_PLOTTABLE_COLORMAP_H
 
