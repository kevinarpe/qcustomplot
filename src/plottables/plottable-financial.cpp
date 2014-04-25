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

#include "plottable-financial.h"

#include "../painter.h"
#include "../core.h"
#include "../axis.h"
#include "../layoutelements/layoutelement-axisrect.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// QCPFinancialData
////////////////////////////////////////////////////////////////////////////////////////////////////

/*! \class QCPFinancialData
  \brief Holds the data of one single data point for QCPFinancial.
  
  The container for storing multiple data points is \ref QCPFinancialDataMap.
  
  The stored data is:
  \li \a key: coordinate on the key axis of this data point
  \li \a open: The opening value at the data point
  \li \a high: The high/maximum value at the data point
  \li \a low: The low/minimum value at the data point
  \li \a close: The closing value at the data point
  
  \see QCPFinancialDataMap
*/

/*!
  Constructs a data point with key and all values set to zero.
*/
QCPFinancialData::QCPFinancialData() :
  key(0),
  open(0),
  high(0),
  low(0),
  close(0)
{
}

/*!
  Constructs a data point with the specified \a key and values.
*/
QCPFinancialData::QCPFinancialData(double key, double open, double high, double low, double close) :
  key(key),
  open(open),
  high(high),
  low(low),
  close(close)
{
}


////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// QCPFinancial
////////////////////////////////////////////////////////////////////////////////////////////////////

/*! \class QCPFinancial
  \brief A plottable representing a financial stock chart

  \image html QCPFinancial.png
  
  
  
  \section appearance Changing the appearance
  
  
*/

/* start of documentation of inline functions */

/*! \fn QCPFinancialDataMap *QCPFinancial::data() const
  
  Returns a pointer to the internal data storage of type \ref QCPFinancialDataMap. You may use it to
  directly manipulate the data, which may be more convenient and faster than using the regular \ref
  setData or \ref addData methods, in certain situations.
*/

/* end of documentation of inline functions */

/*!
  Constructs a financial chart which uses \a keyAxis as its key axis ("x") and \a valueAxis as its value
  axis ("y"). \a keyAxis and \a valueAxis must reside in the same QCustomPlot instance and not have
  the same orientation. If either of these restrictions is violated, a corresponding message is
  printed to the debug output (qDebug), the construction is not aborted, though.
  
  The constructed QCPFinancial can be added to the plot with QCustomPlot::addPlottable, QCustomPlot
  then takes ownership of the financial chart.
*/
QCPFinancial::QCPFinancial(QCPAxis *keyAxis, QCPAxis *valueAxis) :
  QCPAbstractPlottable(keyAxis, valueAxis)
{
  mData = new QCPFinancialDataMap;
  
  /*
  setPen(QPen(Qt::blue, 0));
  setErrorPen(QPen(Qt::black));
  setBrush(Qt::NoBrush);
  setSelectedPen(QPen(QColor(80, 80, 255), 2.5));
  setSelectedBrush(Qt::NoBrush);
  
  setLineStyle(lsLine);
  setErrorType(etNone);
  setErrorBarSize(6);
  setErrorBarSkipSymbol(true);
  setChannelFillGraph(0);
  setAdaptiveSampling(true);
  */
}

QCPFinancial::~QCPFinancial()
{
  delete mData;
}

/*!
  Replaces the current data with the provided \a data.
  
  If \a copy is set to true, data points in \a data will only be copied. if false, the plottable
  takes ownership of the passed data and replaces the internal data pointer with it. This is
  significantly faster than copying for large datasets.
  
  Alternatively, you can also access and modify the plottable's data via the \ref data method, which
  returns a pointer to the internal \ref QCPFinancialDataMap.
*/
void QCPFinancial::setData(QCPFinancialDataMap *data, bool copy)
{
  if (copy)
  {
    *mData = *data;
  } else
  {
    delete mData;
    mData = data;
  }
}

/*! \overload
  
  Replaces the current data with the provided points in \a key and \a value pairs. The provided
  vectors should have equal length. Else, the number of added points will be the size of the
  smallest vector.
*/
void QCPFinancial::setData(const QVector<double> &key, const QVector<double> &value)
{
  mData->clear();
  int n = key.size();
  n = qMin(n, value.size());
  QCPFinancialData newData;
  for (int i=0; i<n; ++i)
  {
    newData.key = key[i];
    newData.value = value[i];
    mData->insertMulti(newData.key, newData);
  }
}
