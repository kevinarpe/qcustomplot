/***************************************************************************
**                                                                        **
**  QCustomPlot, an easy to use, modern plotting widget for Qt            **
**  Copyright (C) 2011, 2012, 2013, 2014 Emanuel Eichhammer               **
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
**             Date: 07.04.14                                             **
**          Version: 1.2.1                                                **
****************************************************************************/
/*! \file */
#ifndef QCP_PLOTTABLE_FINANCIAL_H
#define QCP_PLOTTABLE_FINANCIAL_H

#include "../global.h"
#include "../range.h"
#include "../plottable.h"
#include "../painter.h"

class QCPPainter;
class QCPAxis;

class QCP_LIB_DECL QCPFinancialData
{
public:
  QCPFinancialData();
  QCPFinancialData(double key, double open, double high, double low, double close);
  double key, open, high, low, close;
};
Q_DECLARE_TYPEINFO(QCPFinancialData, Q_MOVABLE_TYPE);

/*! \typedef QCPFinancialData
  Container for storing QCPFinancialData items in a sorted fashion. The key of the map
  is the key member of the QCPFinancialData instance.
  
  This is the container in which QCPFinancial holds its data.
  \see QCPFinancial, QCPFinancial::setData
*/
typedef QMap<double, QCPFinancialData> QCPFinancialDataMap;
typedef QMapIterator<double, QCPFinancialData> QCPFinancialDataMapIterator;
typedef QMutableMapIterator<double, QCPFinancialData> QCPFinancialDataMutableMapIterator;


class QCP_LIB_DECL QCPFinancial : public QCPAbstractPlottable
{
  Q_OBJECT
  /// \cond INCLUDE_QPROPERTIES
  /*
  Q_PROPERTY(LineStyle lineStyle READ lineStyle WRITE setLineStyle)
  Q_PROPERTY(QCPScatterStyle scatterStyle READ scatterStyle WRITE setScatterStyle)
  Q_PROPERTY(ErrorType errorType READ errorType WRITE setErrorType)
  Q_PROPERTY(QPen errorPen READ errorPen WRITE setErrorPen)
  Q_PROPERTY(bool adaptiveSampling READ adaptiveSampling WRITE setAdaptiveSampling)
  */
  /// \endcond
public:
  /*!
    
  */
  enum ChartStyle { csOhlc         ///< Open-High-Low-Close representation
                   ,csCandlestick  ///< Candlestick representation
                  };
  Q_ENUMS(ChartStyle)
  
  explicit QCPFinancial(QCPAxis *keyAxis, QCPAxis *valueAxis);
  virtual ~QCPFinancial();
  
  // getters:
  QCPFinancialDataMap *data() const { return mData; }
  ChartStyle chartStyle() const { return mChartStyle; }
  double width() const { return mWidth; }
  bool twoColored() const { return mTwoColored; }
  QBrush brushPositive() const { return mBrushPositive; }
  QBrush brushNegative() const { return mBrushNegative; }
  QPen penPositive() const { return mPenPositive; }
  QPen penNegative() const { return mPenNegative; }
  
  
  // setters:
  void setData(QCPFinancialDataMap *data, bool copy=false);
  void setData(const QVector<double> &key, const QVector<double> &open, const QVector<double> &high, const QVector<double> &low, const QVector<double> &close);
  void setChartStyle(ChartStyle style);
  void setWidth(double width);
  void setTwoColored(bool twoColored);
  void setBrushPositive(const QBrush &brush);
  void setBrushNegative(const QBrush &brush);
  void setPenPositive(const QPen &pen);
  void setPenNegative(const QPen &pen);
  
  // non-property methods:
  void addData(const QCPFinancialDataMap &dataMap);
  void addData(const QCPFinancialData &data);
  void addData(double key, double open, double high, double low, double close);
  void addData(const QVector<double> &key, const QVector<double> &open, const QVector<double> &high, const QVector<double> &low, const QVector<double> &close);
  void removeDataBefore(double key);
  void removeDataAfter(double key);
  void removeData(double fromKey, double toKey);
  void removeData(double key);
  
  // reimplemented virtual methods:
  virtual void clearData();
  virtual double selectTest(const QPointF &pos, bool onlySelectable, QVariant *details=0) const;
  
protected:
  // property members:
  QCPFinancialDataMap *mData;
  ChartStyle mChartStyle;
  double mWidth;
  bool mTwoColored;
  QBrush mBrushPositive, mBrushNegative;
  QPen mPenPositive, mPenNegative;
  
  // reimplemented virtual methods:
  virtual void draw(QCPPainter *painter);
  virtual void drawLegendIcon(QCPPainter *painter, const QRectF &rect) const;
  virtual QCPRange getKeyRange(bool &foundRange, SignDomain inSignDomain=sdBoth) const;
  virtual QCPRange getValueRange(bool &foundRange, SignDomain inSignDomain=sdBoth) const;
  
  // introduced virtual methods:
  
  // non-virtual methods:
  void drawOhlcPlot(QCPPainter *painter, const QCPFinancialDataMap::const_iterator &begin, const QCPFinancialDataMap::const_iterator &end);
  void drawCandlestickPlot(QCPPainter *painter, const QCPFinancialDataMap::const_iterator &begin, const QCPFinancialDataMap::const_iterator &end);
  void getVisibleDataBounds(QCPFinancialDataMap::const_iterator &lower, QCPFinancialDataMap::const_iterator &upper) const;
  
  /*
  int countDataInBounds(const QCPDataMap::const_iterator &lower, const QCPDataMap::const_iterator &upper, int maxCount) const;
  double pointDistance(const QPointF &pixelPoint) const;
  */
  
  friend class QCustomPlot;
  friend class QCPLegend;
};

#endif // QCP_PLOTTABLE_FINANCIAL_H
