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
#include "layer.h"

class QCPLayout;
class QCPLayoutElement;
class QCustomPlot;

class QCP_LIB_DECL QCPMarginGroup : public QObject
{
  Q_OBJECT
public:
  QCPMarginGroup(QCustomPlot *parentPlot);
  ~QCPMarginGroup();
  
  QList<QCPLayoutElement*> elements(QCP::MarginSide side) const { return mChildren.value(side); }
  bool isEmpty() const;
  void clear();
  
protected:
  QCustomPlot *mParentPlot;
  QHash<QCP::MarginSide, QList<QCPLayoutElement*> > mChildren;
  
  int commonMargin(QCP::MarginSide side) const;
  void addChild(QCP::MarginSide side, QCPLayoutElement *element);
  void removeChild(QCP::MarginSide side, QCPLayoutElement *element);
  
  friend class QCPLayoutElement;
};


class QCP_LIB_DECL QCPLayoutElement : public QCPLayerable
{
  Q_OBJECT
public:
  explicit QCPLayoutElement(QCustomPlot *parentPlot=0);
  ~QCPLayoutElement();
  
  QCPLayout *layout() const { return mParentLayout; }
  QRect rect() const { return mRect; }
  QRect outerRect() const { return mOuterRect; }
  QMargins margins() const { return mMargins; }
  QMargins minimumMargins() const { return mMinimumMargins; }
  QCP::MarginSides autoMargins() const { return mAutoMargins; }
  QSize minimumSize() const { return mMinimumSize; }
  QSize maximumSize() const { return mMaximumSize; }
  QCPMarginGroup *marginGroup(QCP::MarginSide side) const { return mMarginGroups.value(side, (QCPMarginGroup*)0); }
  QHash<QCP::MarginSide, QCPMarginGroup*> marginGroups() const { return mMarginGroups; }
  
  void setOuterRect(const QRect &rect);
  void setMargins(const QMargins &margins);
  void setMinimumMargins(const QMargins &margins);
  void setAutoMargins(QCP::MarginSides sides);
  void setMinimumSize(const QSize &size);
  void setMinimumSize(int width, int height);
  void setMaximumSize(const QSize &size);
  void setMaximumSize(int width, int height);
  void setMarginGroup(QCP::MarginSides sides, QCPMarginGroup *group);
  
  virtual void update();
  virtual QSize minimumSizeHint() const;
  virtual QSize maximumSizeHint() const;
  virtual QList<QCPLayoutElement*> elements() const;
  virtual double selectTest(const QPointF &pos, bool onlySelectable, QVariant *details=0) const;
  
protected:
  QCPLayout *mParentLayout;
  QSize mMinimumSize, mMaximumSize;
  QRect mRect, mOuterRect;
  QMargins mMargins, mMinimumMargins;
  QCP::MarginSides mAutoMargins;
  QHash<QCP::MarginSide, QCPMarginGroup*> mMarginGroups;
  
  virtual void applyDefaultAntialiasingHint(QCPPainter *painter) const { Q_UNUSED(painter) }
  virtual void draw(QCPPainter *painter) { Q_UNUSED(painter) }
  virtual void parentPlotInitialized(QCustomPlot *parentPlot);
  virtual int calculateAutoMargin(QCP::MarginSide side);
  virtual void mousePressEvent(QMouseEvent *event) {Q_UNUSED(event)}
  virtual void mouseMoveEvent(QMouseEvent *event) {Q_UNUSED(event)}
  virtual void mouseReleaseEvent(QMouseEvent *event) {Q_UNUSED(event)}
  virtual void mouseDoubleClickEvent(QMouseEvent *event) {Q_UNUSED(event)}
  virtual void wheelEvent(QWheelEvent *event) {Q_UNUSED(event)}

private:
  Q_DISABLE_COPY(QCPLayoutElement)
  
  friend class QCustomPlot;
  friend class QCPLayout;
  friend class QCPMarginGroup;
};


class QCP_LIB_DECL QCPLayout : public QCPLayoutElement
{
  Q_OBJECT
public:
  explicit QCPLayout();
  
  virtual void update();
  
  virtual int elementCount() const = 0;
  virtual QCPLayoutElement* elementAt(int index) const = 0;
  virtual QCPLayoutElement* takeAt(int index) = 0;
  virtual bool take(QCPLayoutElement* element) = 0;
  virtual QList<QCPLayoutElement*> elements() const;
  virtual void simplify();
  bool removeAt(int index);
  bool remove(QCPLayoutElement* element);
  void clear();
  
  void sizeConstraintsChanged() const;
  
protected:
  virtual void updateLayout();
  void adoptElement(QCPLayoutElement *el);
  void releaseElement(QCPLayoutElement *el);
  // layout helpers for subclasses:
  QVector<int> getSectionSizes(QVector<int> maxSizes, QVector<int> minSizes, QVector<double> stretchFactors, int totalSize) const;
  
private:
  Q_DISABLE_COPY(QCPLayout)
};


class QCP_LIB_DECL QCPLayoutGrid : public QCPLayout
{
  Q_OBJECT
public:
  explicit QCPLayoutGrid();
  
  int rowCount() const;
  int columnCount() const;
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
  bool addElement(int row, int column, QCPLayoutElement *element);
  bool hasElement(int row, int column);
  void expandTo(int newRowCount, int newColumnCount);
  void insertRow(int newIndex);
  void insertColumn(int newIndex);
  
  virtual void updateLayout();
  virtual int elementCount() const;
  virtual QCPLayoutElement* elementAt(int index) const;
  virtual QCPLayoutElement* takeAt(int index);
  virtual bool take(QCPLayoutElement* element);
  virtual QList<QCPLayoutElement*> elements() const;
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
  
private:
  Q_DISABLE_COPY(QCPLayoutGrid)
};


class QCP_LIB_DECL QCPLayoutInset : public QCPLayout
{
  Q_OBJECT
public:
  enum InsetPlacement {ipFree, ipBorderAligned};
  
  explicit QCPLayoutInset();
  
  void addElement(QCPLayoutElement *element, Qt::Alignment alignment);
  void addElement(QCPLayoutElement *element, const QRectF &rect);
  
  InsetPlacement insetPlacement(int index) const;
  Qt::Alignment insetAlignment(int index) const;
  QRectF insetRect(int index) const;
  
  void setInsetPlacement(int index, InsetPlacement placement);
  void setInsetAlignment(int index, Qt::Alignment alignment);
  void setInsetRect(int index, const QRectF &rect);
  
  virtual void updateLayout();
  virtual int elementCount() const;
  virtual QCPLayoutElement* elementAt(int index) const;
  virtual QCPLayoutElement* takeAt(int index);
  virtual bool take(QCPLayoutElement* element);
  
protected:
  QList<QCPLayoutElement*> mElements;
  QList<InsetPlacement> mInsetPlacement;
  QList<Qt::Alignment> mInsetAlignment;
  QList<QRectF> mInsetRect;
  
  void getMinimumRowColSizes(QVector<int> *minColWidths, QVector<int> *minRowHeights) const;
  void getMaximumRowColSizes(QVector<int> *maxColWidths, QVector<int> *maxRowHeights) const;
  
private:
  Q_DISABLE_COPY(QCPLayoutInset)
};

#endif // QCP_LAYOUT_H
