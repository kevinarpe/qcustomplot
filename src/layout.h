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

#ifndef QCP_LAYOUT_H
#define QCP_LAYOUT_H

#include "global.h"

class QCPLayout;
class QCustomPlot;

class QCP_LIB_DECL QCPLayoutElement : public QObject
{
  Q_OBJECT
public:
  explicit QCPLayoutElement(QCustomPlot *parentPlot);
  virtual ~QCPLayoutElement() {}
  
  QCustomPlot *parentPlot() const { return mParentPlot; }
  QCPLayout *layout() const { return mParentLayout; }
  QRect rect() const { return mRect; }
  QRect outerRect() const { return mOuterRect; }
  QMargins margins() const { return mMargins; }
  QMargins minimumMargins() const { return mMinimumMargins; }
  QCP::MarginSides autoMargins() const { return mAutoMargins; }
  QSize minimumSize() const { return mMinimumSize; }
  QSize maximumSize() const { return mMaximumSize; }
  
  void setOuterRect(const QRect &rect);
  void setMargins(const QMargins &margins);
  void setMinimumMargins(const QMargins &margins);
  void setAutoMargins(QCP::MarginSides sides);
  void setMinimumSize(const QSize &size);
  void setMinimumSize(int width, int height);
  void setMaximumSize(const QSize &size);
  void setMaximumSize(int width, int height);
  
  virtual void update();
  virtual QSize minimumSizeHint() const;
  virtual QSize maximumSizeHint() const;
  
protected:
  QCustomPlot *mParentPlot;
  QCPLayout *mParentLayout;
  QSize mMinimumSize, mMaximumSize;
  QRect mRect, mOuterRect;
  QMargins mMargins, mMinimumMargins;
  QCP::MarginSides mAutoMargins;
  
  virtual QMargins calculateAutoMargins() const;
  
  friend class QCPLayout;
};


class QCP_LIB_DECL QCPLayout : public QCPLayoutElement
{
public:
  explicit QCPLayout(QCustomPlot *parentPlot);
  
  virtual void update();
  
  virtual int elementCount() const = 0;
  virtual QCPLayoutElement* elementAt(int index) const = 0;
  virtual QCPLayoutElement* takeAt(int index) = 0;
  virtual bool take(QCPLayoutElement* element) = 0;
  virtual void simplify() = 0;
  
protected:
  virtual void layoutElements();
  void adoptChild(QCPLayoutElement *el);
  void releaseChild(QCPLayoutElement *el);
  
  // layout helpers for subclasses:
  QVector<int> getSectionSizes(QVector<int> maxSizes, QVector<int> minSizes, QVector<double> stretchFactors, int totalSize) const;
};


class QCP_LIB_DECL QCPLayoutGrid : public QCPLayout
{
public:
  explicit QCPLayoutGrid(QCustomPlot *parentPlot);
  
  int rows() const;
  int columns() const;
  QList<double> columnStretchFactors() const { return mColumnStretchFactors; }
  QList<double> rowStretchFactors() const { return mRowStretchFactors; }
  int columnSpacing() const { return mColumnSpacing; }
  int rowSpacing() const { return mRowSpacing; }
  
  void setColumnStretchFactor(int column, double factor);
  void setColumnStretchFactors(const QList<double> &factors);
  void setRowStretchFactor(int row, double factor);
  void setRowStretchFactors(const QList<double> &factors);
  void setColumnSpacing(int pixels);
  void setRowSpacing(int pixels);
  
  QCPLayoutElement *element(int row, int column) const;
  bool addElement(QCPLayoutElement *element, int row, int column);
  bool hasElement(int row, int column);
  void expandTo(int rowCount, int columnCount);
  
  virtual void layoutElements();
  virtual int elementCount() const;
  virtual QCPLayoutElement* elementAt(int index) const;
  virtual QCPLayoutElement* takeAt(int index);
  virtual bool take(QCPLayoutElement* element);
  virtual void simplify();
  
  virtual QSize minimumSizeHint() const;
  virtual QSize maximumSizeHint() const;
  
protected:
  QList<QList<QCPLayoutElement*> > mElements;
  QList<double> mColumnStretchFactors;
  QList<double> mRowStretchFactors;
  int mColumnSpacing, mRowSpacing;
  
  void getMinimumRowColSizes(QVector<int> *minColWidths, QVector<int> *minRowHeights) const;
  void getMaximumRowColSizes(QVector<int> *maxColWidths, QVector<int> *maxRowHeights) const;
};

#endif // QCP_LAYOUT_H
