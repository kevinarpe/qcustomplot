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

#include "layout.h"
#include "core.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// QCPLayoutElement
////////////////////////////////////////////////////////////////////////////////////////////////////

QCPLayoutElement::QCPLayoutElement(QCustomPlot *parentPlot) :
  mParentPlot(parentPlot),
  mParentLayout(0),
  mMinimumSize(0, 0),
  mMaximumSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX),
  mRect(0, 0, 0, 0),
  mOuterRect(0, 0, 0, 0),
  mMargins(0, 0, 0, 0),
  mMinimumMargins(0, 0, 0, 0),
  mAutoMargins(QCP::msAll)
{
}

void QCPLayoutElement::setOuterRect(const QRect &rect)
{
  if (mOuterRect != rect)
  {
    mOuterRect = rect;
    mRect = mOuterRect.adjusted(mMargins.left(), mMargins.top(), -mMargins.right(), -mMargins.bottom());
  }
}

void QCPLayoutElement::setMargins(const QMargins &margins)
{
  if (mMargins != margins)
  {
    mMargins = margins;
    mRect = mOuterRect.adjusted(mMargins.left(), mMargins.top(), -mMargins.right(), -mMargins.bottom());
  }
}

void QCPLayoutElement::setMinimumMargins(const QMargins &margins)
{
  mMinimumMargins = margins;
}

void QCPLayoutElement::setAutoMargins(QCP::MarginSides sides)
{
  mAutoMargins = sides;
}

void QCPLayoutElement::setMinimumSize(const QSize &size)
{
  if (mMinimumSize != size)
  {
    mMinimumSize = size;
    mParentPlot->updateGeometry();
  }
}

void QCPLayoutElement::setMaximumSize(const QSize &size)
{
  if (mMaximumSize != size)
  {
    mMaximumSize = size;
    mParentPlot->updateGeometry();
  }
}

void QCPLayoutElement::update()
{
  if (mAutoMargins != QCP::msNone)
  {
    QMargins autoMargins = calculateAutoMargins();
    QMargins newMargins = mMargins;
    if (mAutoMargins.testFlag(QCP::msLeft))
      newMargins.setLeft(qMax(mMinimumMargins.left(), autoMargins.left()));
    if (mAutoMargins.testFlag(QCP::msRight))
      newMargins.setRight(qMax(mMinimumMargins.right(), autoMargins.right()));
    if (mAutoMargins.testFlag(QCP::msTop))
      newMargins.setTop(qMax(mMinimumMargins.top(), autoMargins.top()));
    if (mAutoMargins.testFlag(QCP::msBottom))
      newMargins.setBottom(qMax(mMinimumMargins.bottom(), autoMargins.bottom()));
    setMargins(newMargins);
  }
}

QSize QCPLayoutElement::minimumSizeHint() const
{
  return mMinimumSize;
}

QSize QCPLayoutElement::maximumSizeHint() const
{
  return mMaximumSize;
}

QMargins QCPLayoutElement::calculateAutoMargins() const
{
  return mMargins;
}


////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// QCPLayout
////////////////////////////////////////////////////////////////////////////////////////////////////

QCPLayout::QCPLayout(QCustomPlot *parentPlot) :
  QCPLayoutElement(parentPlot)
{
}

void QCPLayout::update()
{
  QCPLayoutElement::update();
  
  // set child element rects according to layout:
  layoutElements();
  
  // propagate update call to child elements:
  for (int i=0; i<elementCount(); ++i)
  {
    if (QCPLayoutElement *el = elementAt(i))
      el->update();
  }
}

void QCPLayout::layoutElements()
{
}

void QCPLayout::adoptChild(QCPLayoutElement *el)
{
  if (el)
    el->mParentLayout = this;
  else
    qDebug() << Q_FUNC_INFO << "Null element passed";
}

void QCPLayout::releaseChild(QCPLayoutElement *el)
{
  if (el)
    el->mParentLayout = 0;
  else
    qDebug() << Q_FUNC_INFO << "Null element passed";
}


////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// QCPLayoutGrid
////////////////////////////////////////////////////////////////////////////////////////////////////


QCPLayoutGrid::QCPLayoutGrid(QCustomPlot *parentPlot) :
  QCPLayout(parentPlot)
{
}

QCPLayoutElement *QCPLayoutGrid::element(int row, int column) const
{
  if (row >= 0 && row < mElements.size())
  {
    if (column >= 0 && column < mElements.first().size())
    {
      return mElements.at(row).at(column);
    } else
      qDebug() << Q_FUNC_INFO << "Invalid column. Row:" << row << "Column:" << column;
  } else
    qDebug() << Q_FUNC_INFO << "Invalid row. Row:" << row << "Column:" << column;
  return 0;
}

int QCPLayoutGrid::rows() const
{
  return mElements.size();
}

int QCPLayoutGrid::columns() const
{
  if (mElements.size() > 0)
    return mElements.first().size();
  else
    return 0;
}

bool QCPLayoutGrid::addElement(QCPLayoutElement *element, int row, int column)
{
  if (element)
  {
    if (!hasElement(row, column))
    {
      if (element->layout()) // remove from old layout first
        element->layout()->take(element);
      expandTo(row+1, column+1);
      mElements[row][column] = element;
      adoptChild(element);
      return true;
    } else
      qDebug() << Q_FUNC_INFO << "The specified cell is already occupied by an element:" << row << column; 
  } else
    qDebug() << Q_FUNC_INFO << "Can't add null element to row/column:" << row << column;
  return false;
}

bool QCPLayoutGrid::hasElement(int row, int column)
{
  if (row >= 0 && row < rows() && column >= 0 && column < columns())
    return mElements.at(row).at(column);
  else
    return false;
}

void QCPLayoutGrid::setColumnStretchFactor(int column, double factor)
{
  if (column >= 0 && column < columns())
  {
    if (factor >= 0)
    {
      mColumnStretchFactors[column] = factor;
    } else
    {
      mColumnStretchFactors[column] = 0;
      qDebug() << Q_FUNC_INFO << "Invalid stretch factor, must be positive or zero:" << factor;
    }
  } else
    qDebug() << Q_FUNC_INFO << "Invalid column:" << column;
}

void QCPLayoutGrid::setColumnStretchFactors(const QList<double> &factors)
{
  if (factors.size() == mColumnStretchFactors.size())
  {
    mColumnStretchFactors = factors;
    for (int i=0; i<mColumnStretchFactors.size(); ++i)
    {
      if (mColumnStretchFactors.at(i) < 0)
      {
        qDebug() << Q_FUNC_INFO << "Invalid stretch factor, must be positive or zero:" << mColumnStretchFactors.at(i);
        mColumnStretchFactors[i] = 0;
      }
    }
  } else
    qDebug() << Q_FUNC_INFO << "More stretch factors passed than there are columns:" << factors;
}

void QCPLayoutGrid::setRowStretchFactor(int row, double factor)
{
  if (row >= 0 && row < rows())
  {
    if (factor >= 0)
    {
      mRowStretchFactors[row] = factor;
    } else
    {
      mRowStretchFactors[row] = 0;
      qDebug() << Q_FUNC_INFO << "Invalid stretch factor, must be positive or zero:" << factor;
    }
  } else
    qDebug() << Q_FUNC_INFO << "Invalid row:" << row;
}

void QCPLayoutGrid::setRowStretchFactors(const QList<double> &factors)
{
  if (factors.size() == mRowStretchFactors.size())
  {
    mRowStretchFactors = factors;
    for (int i=0; i<mRowStretchFactors.size(); ++i)
    {
      if (mRowStretchFactors.at(i) < 0)
      {
        qDebug() << Q_FUNC_INFO << "Invalid stretch factor, must be positive or zero:" << mRowStretchFactors.at(i);
        mRowStretchFactors[i] = 0;
      }
    }
  } else
    qDebug() << Q_FUNC_INFO << "More stretch factors passed than there are rows:" << factors;
}

void QCPLayoutGrid::expandTo(int rowCount, int columnCount)
{
  // add rows as necessary:
  while (rows() < rowCount)
  {
    mElements.append(QList<QCPLayoutElement*>());
    mRowStretchFactors.append(1);
  }
  // go through rows and expand columns as necessary:
  int newColCount = qMax(columns(), columnCount);
  for (int i=0; i<rows(); ++i)
  {
    while (mElements.at(i).size() < newColCount)
    {
      mElements[i].append(0);
      mColumnStretchFactors.append(1);
    }
  }
}

void QCPLayoutGrid::layoutElements()
{
  QVector<int> minColWidths, minRowHeights, maxColWidths, maxRowHeights;
  getMinimumRowColSizes(&minColWidths, &minRowHeights);
  getMaximumRowColSizes(&maxColWidths, &maxRowHeights);
  QList<double> colStretchFactors = mColumnStretchFactors;
  QList<double> rowStretchFactors = mRowStretchFactors;
  
  // if provided width is forced smaller than total minimum column widths, ignore minimum widths (squeeze columns):
  int minColWidthsSum = 0;
  for (int i=0; i<minColWidths.size(); ++i)
    minColWidthsSum += minColWidths.at(i);
  if (mRect.width() < minColWidthsSum)
  {
    // new stretch factors are minimum widths and minimum widths are removed:
    for (int i=0; i<colStretchFactors.size(); ++i)
    {
      colStretchFactors[i] = minColWidths.at(i);
      minColWidths[i] = 0;
    }
  }
  
  // calculate column widths:
  int freeWidth = mRect.width(); // width pixels that are available for stretching unhandled columns
  double factorSum = 0;
  for (int i=0; i<columns(); ++i)
    factorSum += colStretchFactors.at(i);
  QVector<int> columnWidths(columns(), 0);
  QMultiMap<double, int> prioritizedColumns; // double is minWidth/StretchFactor, int is column index. Columns which will reach their minimum first are first in this map.
  for (int i=0; i<columns(); ++i)
    prioritizedColumns.insert(-minColWidths.at(i)/colStretchFactors.at(i), i);
  
  QMapIterator<double, int> columnIterator(prioritizedColumns);
  while (columnIterator.hasNext())
  {
    columnIterator.next();
    int colId = columnIterator.value();
    double stretchedColWidth = colStretchFactors.at(colId)/factorSum*freeWidth;
    if (stretchedColWidth < minColWidths.at(colId))
    {
      // column would become less than its minimum -> set to minimum and distribute rest of space on rest of columns
      columnWidths[colId] = minColWidths.at(colId);
      freeWidth = qMax(0, freeWidth - columnWidths[colId]);
      factorSum -= colStretchFactors.at(colId);
    } else if (stretchedColWidth > maxColWidths.at(colId))
    {
      columnWidths[colId] = maxColWidths.at(colId);
      freeWidth = qMax(0, freeWidth - columnWidths[colId]);
      factorSum -= colStretchFactors.at(colId);
    } else
    {
      columnWidths[colId] = stretchedColWidth;
    }
  }
  
  // if provided height is forced smaller than total minimum row heights, ignore minimum heights (squeeze rows):
  int minRowHeightsSum = 0;
  for (int i=0; i<minRowHeights.size(); ++i)
    minRowHeightsSum += minRowHeights.at(i);
  if (mRect.height() < minRowHeightsSum)
  {
    // new stretch factors are minimum heights and minimum heights are removed:
    for (int i=0; i<rowStretchFactors.size(); ++i)
    {
      rowStretchFactors[i] = minRowHeights.at(i);
      minRowHeights[i] = 0;
    }
  }
  
  // calculate row heights:
  int freeHeight = mRect.height(); // height pixels that are available for stretching unhandled rows
  factorSum = 0;
  for (int i=0; i<rows(); ++i)
    factorSum += rowStretchFactors.at(i);
  QVector<int> rowHeights(rows(), 0);
  QMultiMap<double, int> prioritizedRows; // double is -minHeight/StretchFactor, int is row index. Rows which will reach their minimum first are first in this map.
  for (int i=0; i<rows(); ++i)
    prioritizedRows.insert(-minRowHeights.at(i)/rowStretchFactors.at(i), i);
  
  QMapIterator<double, int> rowIterator(prioritizedRows);
  while (rowIterator.hasNext())
  {
    rowIterator.next();
    int rowId = rowIterator.value();
    double stretchedRowHeight = rowStretchFactors.at(rowId)/factorSum*freeHeight;
    if (stretchedRowHeight < minRowHeights.at(rowId))
    {
      // row would become less than its minimum -> set to minimum and distribute rest of space on rest of rows
      rowHeights[rowId] = minRowHeights.at(rowId);
      freeHeight = qMax(0, freeHeight - rowHeights[rowId]);
      factorSum -= rowStretchFactors.at(rowId);
    } else if (stretchedRowHeight > maxRowHeights.at(rowId))
    {
      // row would become less than its minimum -> set to minimum and distribute rest of space on rest of rows
      rowHeights[rowId] = maxRowHeights.at(rowId);
      freeHeight = qMax(0, freeHeight - rowHeights[rowId]);
      factorSum -= rowStretchFactors.at(rowId);
    } else
    {
      rowHeights[rowId] = stretchedRowHeight;
    }
  }
  
  // go through cells and set rects accordingly:
  int yOffset = mRect.top();
  for (int row=0; row<rows(); ++row)
  {
    if (row > 0)
      yOffset += rowHeights.at(row-1);
    int xOffset = mRect.left();
    for (int col=0; col<columns(); ++col)
    {
      if (col > 0)
        xOffset += columnWidths.at(col-1);
      if (mElements.at(row).at(col))
        mElements.at(row).at(col)->setOuterRect(QRect(xOffset, yOffset, columnWidths.at(col), rowHeights.at(row)));
    }
  }
}

int QCPLayoutGrid::elementCount() const
{
  return rows()*columns();
}

QCPLayoutElement *QCPLayoutGrid::elementAt(int index) const
{
  if (index >= 0 && index < elementCount())
    return mElements.at(index / columns()).at(index % columns());
  else
    return 0;
}

QCPLayoutElement *QCPLayoutGrid::takeAt(int index)
{
  if (QCPLayoutElement *el = elementAt(index))
  {
    releaseChild(el);
    mElements[index / columns()][index % columns()] = 0;
    return el;
  } else
  {
    qDebug() << Q_FUNC_INFO << "Attempt to take invalid index:" << index;
    return 0;
  }
}

bool QCPLayoutGrid::take(QCPLayoutElement *element)
{
  if (element)
  {
    for (int i=0; i<elementCount(); ++i)
    {
      if (elementAt(i) == element)
      {
        takeAt(i);
        return true;
      }
    }
  } else
    qDebug() << Q_FUNC_INFO << "Can't take null element";
  return false;
}

void QCPLayoutGrid::simplify()
{
  // remove rows with only empty cells:
  for (int row=rows()-1; row>=0; --row)
  {
    bool hasElements = false;
    for (int col=0; col<columns(); ++col)
    {
      if (mElements.at(row).at(col))
      {
        hasElements = true;
        break;
      }
    }
    if (!hasElements)
    {
      mRowStretchFactors.removeAt(row);
      mElements.removeAt(row);
    }
  }
  
  // remove columns with only empty cells:
  for (int col=columns()-1; col>=0; --col)
  {
    bool hasElements = false;
    for (int row=0; row<rows(); ++row)
    {
      if (mElements.at(row).at(col))
      {
        hasElements = true;
        break;
      }
    }
    if (!hasElements)
    {
      mColumnStretchFactors.removeAt(col);
      for (int row=0; row<rows(); ++row)
        mElements[row].removeAt(col);
    }
  }
}

QSize QCPLayoutGrid::minimumSizeHint() const
{
  QVector<int> minColWidths, minRowHeights;
  getMinimumRowColSizes(&minColWidths, &minRowHeights);
  
  QSize result(0, 0);
  for (int i=0; i<minColWidths.size(); ++i)
    result.rwidth() += minColWidths.at(i);
  for (int i=0; i<minRowHeights.size(); ++i)
    result.rheight() += minRowHeights.at(i);
  
  return result;
}

QSize QCPLayoutGrid::maximumSizeHint() const
{
  QVector<int> maxColWidths, maxRowHeights;
  getMaximumRowColSizes(&maxColWidths, &maxRowHeights);
  
  QSize result(0, 0);
  for (int i=0; i<maxColWidths.size(); ++i)
    result.setWidth(qMin(result.width()+maxColWidths.at(i), QWIDGETSIZE_MAX));
  for (int i=0; i<maxRowHeights.size(); ++i)
    result.setHeight(qMin(result.height()+maxRowHeights.at(i), QWIDGETSIZE_MAX));
  
  return result;
}

void QCPLayoutGrid::getMinimumRowColSizes(QVector<int> *minColWidths, QVector<int> *minRowHeights) const
{
  *minColWidths = QVector<int> (columns(), 0);
  *minRowHeights = QVector<int> (rows(), 0);
  for (int row=0; row<rows(); ++row)
  {
    for (int col=0; col<columns(); ++col)
    {
      if (mElements.at(row).at(col))
      {
        QSize minHint = mElements.at(row).at(col)->minimumSizeHint();
        QSize min = mElements.at(row).at(col)->minimumSize();
        QSize final(min.width() > 0 ? min.width() : minHint.width(), min.height() > 0 ? min.height() : minHint.height());
        if (minColWidths->at(col) < final.width())
          (*minColWidths)[col] = final.width();
        if (minRowHeights->at(row) < final.height())
          (*minRowHeights)[row] = final.height();
      }
    }
  }
}

void QCPLayoutGrid::getMaximumRowColSizes(QVector<int> *maxColWidths, QVector<int> *maxRowHeights) const
{
  *maxColWidths = QVector<int> (columns(), QWIDGETSIZE_MAX);
  *maxRowHeights = QVector<int> (rows(), QWIDGETSIZE_MAX);
  for (int row=0; row<rows(); ++row)
  {
    for (int col=0; col<columns(); ++col)
    {
      if (mElements.at(row).at(col))
      {
        QSize maxHint = mElements.at(row).at(col)->maximumSizeHint();
        QSize max = mElements.at(row).at(col)->maximumSize();
        QSize final(max.width() < QWIDGETSIZE_MAX ? max.width() : maxHint.width(), max.height() < QWIDGETSIZE_MAX ? max.height() : maxHint.height());
        if (maxColWidths->at(col) > final.width())
          (*maxColWidths)[col] = final.width();
        if (maxRowHeights->at(row) > final.height())
          (*maxRowHeights)[row] = final.height();
      }
    }
  }
}


