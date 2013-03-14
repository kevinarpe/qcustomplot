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
//////////////////// QCPMarginGroup
////////////////////////////////////////////////////////////////////////////////////////////////////


QCPMarginGroup::QCPMarginGroup(QCustomPlot *parentPlot) :
  QObject(parentPlot),
  mParentPlot(parentPlot)
{
  mChildren.insert(QCP::msLeft, QList<QCPLayoutElement*>());
  mChildren.insert(QCP::msRight, QList<QCPLayoutElement*>());
  mChildren.insert(QCP::msTop, QList<QCPLayoutElement*>());
  mChildren.insert(QCP::msBottom, QList<QCPLayoutElement*>());
}

QCPMarginGroup::~QCPMarginGroup()
{
  clear();
}

bool QCPMarginGroup::isEmpty() const
{
  QHashIterator<QCP::MarginSide, QList<QCPLayoutElement*> > it(mChildren);
  while (it.hasNext())
  {
    it.next();
    if (!it.value().isEmpty())
      return false;
  }
  return true;
}

void QCPMarginGroup::clear()
{
  // make all children remove themselves from this margin group:
  QHashIterator<QCP::MarginSide, QList<QCPLayoutElement*> > it(mChildren);
  while (it.hasNext())
  {
    it.next();
    const QList<QCPLayoutElement*> elements = it.value();
    for (int i=elements.size()-1; i>=0; --i)
      elements.at(i)->setMarginGroup(it.key(), 0); // removes itself from mChildren via removeChild
  }
}

int QCPMarginGroup::commonMargin(QCP::MarginSide side) const
{
  // query all automatic margins of the layout elements in this margin group side and find maximum:
  int result = 0;
  const QList<QCPLayoutElement*> elements = mChildren.value(side);
  for (int i=0; i<elements.size(); ++i)
  {
    if (!elements.at(i)->autoMargins().testFlag(side))
      continue;
    int m = qMax(elements.at(i)->calculateAutoMargin(side), QCP::getMarginValue(elements.at(i)->minimumMargins(), side));
    if (m > result)
      result = m;
  }
  return result;
}

void QCPMarginGroup::addChild(QCP::MarginSide side, QCPLayoutElement *element)
{
  if (!mChildren[side].contains(element))
    mChildren[side].append(element);
  else
    qDebug() << Q_FUNC_INFO << "element is already child of this margin group side" << reinterpret_cast<quintptr>(element);
}

void QCPMarginGroup::removeChild(QCP::MarginSide side, QCPLayoutElement *element)
{
  if (!mChildren[side].removeOne(element))
    qDebug() << Q_FUNC_INFO << "element is not child of this margin group side" << reinterpret_cast<quintptr>(element);
}


////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// QCPLayoutElement
////////////////////////////////////////////////////////////////////////////////////////////////////

QCPLayoutElement::QCPLayoutElement() :
  QObject(0), // parenthood is changed as soon as layout element gets inserted into a layout (except for top level layout)
  mParentLayout(0),
  mMinimumSize(),
  mMaximumSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX),
  mRect(0, 0, 0, 0),
  mOuterRect(0, 0, 0, 0),
  mMargins(0, 0, 0, 0),
  mMinimumMargins(0, 0, 0, 0),
  mAutoMargins(QCP::msAll)
{
}

QCPLayoutElement::~QCPLayoutElement()
{
  setMarginGroup(QCP::msAll, 0); // unregister at margin groups, if there are any
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

// doc: only has effect on auto margins
void QCPLayoutElement::setMinimumMargins(const QMargins &margins)
{
  if (mMinimumMargins != margins)
  {
    mMinimumMargins = margins;
  }
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
    if (mParentLayout)
      mParentLayout->sizeConstraintsChanged();
  }
}

void QCPLayoutElement::setMinimumSize(int width, int height)
{
  setMinimumSize(QSize(width, height));
}

void QCPLayoutElement::setMaximumSize(const QSize &size)
{
  if (mMaximumSize != size)
  {
    mMaximumSize = size;
    if (mParentLayout)
      mParentLayout->sizeConstraintsChanged();
  }
}

void QCPLayoutElement::setMaximumSize(int width, int height)
{
  setMaximumSize(QSize(width, height));
}

void QCPLayoutElement::setMarginGroup(QCP::MarginSides sides, QCPMarginGroup *group)
{
  QVector<QCP::MarginSide> sideVector;
  if (sides.testFlag(QCP::msLeft)) sideVector.append(QCP::msLeft);
  if (sides.testFlag(QCP::msRight)) sideVector.append(QCP::msRight);
  if (sides.testFlag(QCP::msTop)) sideVector.append(QCP::msTop);
  if (sides.testFlag(QCP::msBottom)) sideVector.append(QCP::msBottom);
  
  for (int i=0; i<sideVector.size(); ++i)
  {
    QCP::MarginSide side = sideVector.at(i);
    if (marginGroup(side) != group)
    {
      QCPMarginGroup *oldGroup = marginGroup(side);
      if (oldGroup) // unregister at old group
        oldGroup->removeChild(side, this);
      
      if (!group) // if setting to 0, remove hash entry. Else set hash entry to new group and register there
      {
        mMarginGroups.remove(side);
      } else // setting to a new group
      {
        mMarginGroups[side] = group;
        group->addChild(side, this);
      }
    }
  }
}

void QCPLayoutElement::update()
{
  if (mAutoMargins != QCP::msNone)
  {
    // set the margins of this layout element according to automatic margin calculation, either directly or via a margin group:
    QMargins newMargins = mMargins;
    QVector<QCP::MarginSide> marginSides = QVector<QCP::MarginSide>() << QCP::msLeft << QCP::msRight << QCP::msTop << QCP::msBottom;
    for (int i=0; i<marginSides.size(); ++i)
    {
      QCP::MarginSide side = marginSides.at(i);
      if (mAutoMargins.testFlag(side)) // this side's margin shall be calculated automatically
      {
        if (mMarginGroups.contains(side)) 
          QCP::setMarginValue(newMargins, side, mMarginGroups[side]->commonMargin(side)); // this side is part of a margin group, so get the margin value from that group
        else 
          QCP::setMarginValue(newMargins, side, calculateAutoMargin(side)); // this side is not part of a group, so calculate the value directly
        // apply minimum margin restrictions:
        if (QCP::getMarginValue(newMargins, side) < QCP::getMarginValue(mMinimumMargins, side))
          QCP::setMarginValue(newMargins, side, QCP::getMarginValue(mMinimumMargins, side));
      }
    }
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

QList<QCPLayoutElement*> QCPLayoutElement::elements() const
{
  return QList<QCPLayoutElement*>();
}

int QCPLayoutElement::calculateAutoMargin(QCP::MarginSide side)
{
  return QCP::getMarginValue(mMargins, side);
}

bool QCPLayoutElement::hitTest(const QPointF &pos) const
{
  return QRectF(mOuterRect).contains(pos);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// QCPLayout
////////////////////////////////////////////////////////////////////////////////////////////////////

QCPLayout::QCPLayout()
{
}

void QCPLayout::update()
{
  QCPLayoutElement::update(); // recalculates (auto-)margins
  
  // set child element rects according to layout:
  updateLayout();
  
  // propagate update call to child elements:
  for (int i=0; i<elementCount(); ++i)
  {
    if (QCPLayoutElement *el = elementAt(i))
      el->update();
  }
}

QList<QCPLayoutElement*> QCPLayout::elements() const
{
  int c = elementCount();
  QList<QCPLayoutElement*> result;
  result.reserve(c);
  for (int i=0; i<c; ++i)
    result.append(elementAt(i));
  return result;
}

void QCPLayout::simplify()
{
  qDebug() << Q_FUNC_INFO << "This layout does not support/need simplification";
}

bool QCPLayout::removeAt(int index)
{
  if (QCPLayoutElement *el = takeAt(index))
  {
    delete el;
    return true;
  } else
    return false;
}

bool QCPLayout::remove(QCPLayoutElement *element)
{
  if (take(element))
  {
    delete element;
    return true;
  } else
    return false;
}

void QCPLayout::clear()
{
  for (int i=elementCount()-1; i>=0; --i)
    removeAt(i);
}

void QCPLayout::sizeConstraintsChanged() const
{
  if (QWidget *w = qobject_cast<QWidget*>(parent()))
    w->updateGeometry();
  else if (QCPLayout *l = qobject_cast<QCPLayout*>(parent()))
    l->sizeConstraintsChanged();
// don't warn here because it's normal situation for QCPLayoutInset in QCPAxisRect:
//  else
//    qDebug() << Q_FUNC_INFO << "Layout has no parent QWidget or QCPLayout";
}

void QCPLayout::updateLayout()
{
}

void QCPLayout::adoptElement(QCPLayoutElement *el)
{
  if (el)
  {
    el->mParentLayout = this;
    el->setParent(this);
  } else
    qDebug() << Q_FUNC_INFO << "Null element passed";
}

void QCPLayout::releaseElement(QCPLayoutElement *el)
{
  if (el)
  {
    el->mParentLayout = 0;
    el->setParent(0);
  } else
    qDebug() << Q_FUNC_INFO << "Null element passed";
}

QVector<int> QCPLayout::getSectionSizes(QVector<int> maxSizes, QVector<int> minSizes, QVector<double> stretchFactors, int totalSize) const
{
  if (maxSizes.size() != minSizes.size() || minSizes.size() != stretchFactors.size())
  {
    qDebug() << Q_FUNC_INFO << "Passed vector sizes aren't equal:" << maxSizes << minSizes << stretchFactors;
    return QVector<int>();
  }
  if (stretchFactors.isEmpty())
    return QVector<int>();
  int sectionCount = stretchFactors.size();
  QVector<double> sectionSizes(sectionCount);
  // if provided total size is forced smaller than total minimum size, ignore minimum sizes (squeeze sections):
  int minSizeSum = 0;
  for (int i=0; i<sectionCount; ++i)
    minSizeSum += minSizes.at(i);
  if (totalSize < minSizeSum)
  {
    // new stretch factors are minimum sizes and minimum sizes are set to zero:
    for (int i=0; i<sectionCount; ++i)
    {
      stretchFactors[i] = minSizes.at(i);
      minSizes[i] = 0;
    }
  }
  
  QList<int> minimumLockedSections;
  QList<int> unfinishedSections;
  for (int i=0; i<sectionCount; ++i)
    unfinishedSections.append(i);
  double freeSize = totalSize;
  
  int outerIterations = 0;
  while (!unfinishedSections.isEmpty() && outerIterations < sectionCount*2) // the iteration check ist just a failsafe in case something really strange happens
  {
    ++outerIterations;
    int innerIterations = 0;
    while (!unfinishedSections.isEmpty() && innerIterations < sectionCount*2) // the iteration check ist just a failsafe in case something really strange happens
    {
      ++innerIterations;
      // find section that hits its maximum next:
      int nextId = -1;
      double nextMax = 1e12;
      for (int i=0; i<unfinishedSections.size(); ++i)
      {
        int secId = unfinishedSections.at(i);
        double hitsMaxAt = (maxSizes.at(secId)-sectionSizes.at(secId))/stretchFactors.at(secId);
        if (hitsMaxAt < nextMax)
        {
          nextMax = hitsMaxAt;
          nextId = secId;
        }
      }
      // check if that maximum is actually within the bounds of the total size (i.e. can we stretch all remaining sections so far that the found section
      // actually hits its maximum, without exceeding the total size when we add up all sections)
      double stretchFactorSum = 0;
      for (int i=0; i<unfinishedSections.size(); ++i)
        stretchFactorSum += stretchFactors.at(unfinishedSections.at(i));
      double nextMaxLimit = freeSize/stretchFactorSum;
      if (nextMax < nextMaxLimit) // next maximum is actually hit, move forward to that point and fix the size of that section
      {
        for (int i=0; i<unfinishedSections.size(); ++i)
        {
          sectionSizes[unfinishedSections.at(i)] += nextMax*stretchFactors.at(unfinishedSections.at(i)); // increment all sections
          freeSize -= nextMax*stretchFactors.at(unfinishedSections.at(i));
        }
        unfinishedSections.removeOne(nextId); // exclude the section that is now at maximum from further changes
      } else // next maximum isn't hit, just distribute rest of free space on remaining sections
      {
        for (int i=0; i<unfinishedSections.size(); ++i)
          sectionSizes[unfinishedSections.at(i)] += nextMaxLimit*stretchFactors.at(unfinishedSections.at(i)); // increment all sections
        unfinishedSections.clear();
      }
    }
    if (innerIterations == sectionCount*2)
      qDebug() << Q_FUNC_INFO << "Exceeded maximum expected inner iteration count, layouting aborted. Input was:" << maxSizes << minSizes << stretchFactors << totalSize;
    
    // now check whether the resulting section sizes violate minimum restrictions:
    bool foundMinimumViolation = false;
    for (int i=0; i<sectionSizes.size(); ++i)
    {
      if (minimumLockedSections.contains(i))
        continue;
      if (sectionSizes.at(i) < minSizes.at(i)) // section violates minimum
      {
        sectionSizes[i] = minSizes.at(i); // set it to minimum
        foundMinimumViolation = true; // make sure we repeat the whole optimization process
        minimumLockedSections.append(i);
      }
    }
    if (foundMinimumViolation)
    {
      freeSize = totalSize;
      for (int i=0; i<sectionCount; ++i)
      {
        if (!minimumLockedSections.contains(i)) // only put sections that haven't hit their minimum back into the pool
          unfinishedSections.append(i);
        else
          freeSize -= sectionSizes.at(i); // remove size of minimum locked sections from available space in next round
      }
      // reset all section sizes to zero that are in unfinished sections (all others have been set to their minimum):
      for (int i=0; i<unfinishedSections.size(); ++i)
        sectionSizes[unfinishedSections.at(i)] = 0;
    }
  }
  if (outerIterations == sectionCount*2)
    qDebug() << Q_FUNC_INFO << "Exceeded maximum expected outer iteration count, layouting aborted. Input was:" << maxSizes << minSizes << stretchFactors << totalSize;
  
  QVector<int> result(sectionCount);
  for (int i=0; i<sectionCount; ++i)
    result[i] = qRound(sectionSizes.at(i));
  return result;
}


////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// QCPLayoutGrid
////////////////////////////////////////////////////////////////////////////////////////////////////


QCPLayoutGrid::QCPLayoutGrid() :
  mColumnSpacing(5),
  mRowSpacing(5)
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

int QCPLayoutGrid::rowCount() const
{
  return mElements.size();
}

int QCPLayoutGrid::columnCount() const
{
  if (mElements.size() > 0)
    return mElements.first().size();
  else
    return 0;
}

bool QCPLayoutGrid::addElement(int row, int column, QCPLayoutElement *element)
{
  if (element)
  {
    if (!hasElement(row, column))
    {
      if (element->layout()) // remove from old layout first
        element->layout()->take(element);
      expandTo(row+1, column+1);
      mElements[row][column] = element;
      adoptElement(element);
      return true;
    } else
      qDebug() << Q_FUNC_INFO << "There is already an element in the specified row/column:" << row << column; 
  } else
    qDebug() << Q_FUNC_INFO << "Can't add null element to row/column:" << row << column;
  return false;
}

bool QCPLayoutGrid::hasElement(int row, int column)
{
  if (row >= 0 && row < rowCount() && column >= 0 && column < columnCount())
    return mElements.at(row).at(column);
  else
    return false;
}

void QCPLayoutGrid::setColumnStretchFactor(int column, double factor)
{
  if (column >= 0 && column < columnCount())
  {
    if (factor > 0)
      mColumnStretchFactors[column] = factor;
    else
      qDebug() << Q_FUNC_INFO << "Invalid stretch factor, must be positive:" << factor;
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
      if (mColumnStretchFactors.at(i) <= 0)
      {
        qDebug() << Q_FUNC_INFO << "Invalid stretch factor, must be positive:" << mColumnStretchFactors.at(i);
        mColumnStretchFactors[i] = 1;
      }
    }
  } else
    qDebug() << Q_FUNC_INFO << "Column count not equal to passed stretch factor count:" << factors;
}

void QCPLayoutGrid::setRowStretchFactor(int row, double factor)
{
  if (row >= 0 && row < rowCount())
  {
    if (factor > 0)
      mRowStretchFactors[row] = factor;
    else
      qDebug() << Q_FUNC_INFO << "Invalid stretch factor, must be positive:" << factor;
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
      if (mRowStretchFactors.at(i) <= 0)
      {
        qDebug() << Q_FUNC_INFO << "Invalid stretch factor, must be positive:" << mRowStretchFactors.at(i);
        mRowStretchFactors[i] = 1;
      }
    }
  } else
    qDebug() << Q_FUNC_INFO << "Row count not equal to passed stretch factor count:" << factors;
}

void QCPLayoutGrid::setColumnSpacing(int pixels)
{
  mColumnSpacing = pixels;
}

void QCPLayoutGrid::setRowSpacing(int pixels)
{
  mRowSpacing = pixels;
}

void QCPLayoutGrid::expandTo(int rows, int columns)
{
  // add rows as necessary:
  while (rowCount() < rows)
  {
    mElements.append(QList<QCPLayoutElement*>());
    mRowStretchFactors.append(1);
  }
  // go through rows and expand columns as necessary:
  int newColCount = qMax(columnCount(), columns);
  for (int i=0; i<rowCount(); ++i)
  {
    while (mElements.at(i).size() < newColCount)
      mElements[i].append(0);
  }
  while (mColumnStretchFactors.size() < newColCount)
    mColumnStretchFactors.append(1);
}

void QCPLayoutGrid::insertRow(int newIndex)
{
  if (newIndex < 0)
    newIndex = 0;
  if (newIndex > rowCount())
    newIndex = rowCount();
  
  mRowStretchFactors.insert(newIndex, 1);
  QList<QCPLayoutElement*> newRow;
  for (int col=0; col<columnCount(); ++col)
    newRow.append((QCPLayoutElement*)0);
  mElements.insert(newIndex, newRow);
}

void QCPLayoutGrid::insertColumn(int newIndex)
{
  if (rowCount() == 0)
    insertRow(0);
  
  if (newIndex < 0)
    newIndex = 0;
  if (newIndex > columnCount())
    newIndex = columnCount();
  
  mColumnStretchFactors.insert(newIndex, 1);
  for (int row=0; row<rowCount(); ++row)
    mElements[row].insert(newIndex, (QCPLayoutElement*)0);
}

void QCPLayoutGrid::updateLayout()
{
  QVector<int> minColWidths, minRowHeights, maxColWidths, maxRowHeights;
  getMinimumRowColSizes(&minColWidths, &minRowHeights);
  getMaximumRowColSizes(&maxColWidths, &maxRowHeights);
  
  int totalRowSpacing = (rowCount()-1) * mRowSpacing;
  int totalColSpacing = (columnCount()-1) * mColumnSpacing;
  QVector<int> colWidths = getSectionSizes(maxColWidths, minColWidths, mColumnStretchFactors.toVector(), mRect.width()-totalColSpacing);
  QVector<int> rowHeights = getSectionSizes(maxRowHeights, minRowHeights, mRowStretchFactors.toVector(), mRect.height()-totalRowSpacing);
  
  // go through cells and set rects accordingly:
  int yOffset = mRect.top();
  for (int row=0; row<rowCount(); ++row)
  {
    if (row > 0)
      yOffset += rowHeights.at(row-1)+mRowSpacing;
    int xOffset = mRect.left();
    for (int col=0; col<columnCount(); ++col)
    {
      if (col > 0)
        xOffset += colWidths.at(col-1)+mColumnSpacing;
      if (mElements.at(row).at(col))
        mElements.at(row).at(col)->setOuterRect(QRect(xOffset, yOffset, colWidths.at(col), rowHeights.at(row)));
    }
  }
}

int QCPLayoutGrid::elementCount() const
{
  return rowCount()*columnCount();
}

QCPLayoutElement *QCPLayoutGrid::elementAt(int index) const
{
  if (index >= 0 && index < elementCount())
    return mElements.at(index / columnCount()).at(index % columnCount());
  else
    return 0;
}

QCPLayoutElement *QCPLayoutGrid::takeAt(int index)
{
  if (QCPLayoutElement *el = elementAt(index))
  {
    releaseElement(el);
    mElements[index / columnCount()][index % columnCount()] = 0;
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
    qDebug() << Q_FUNC_INFO << "Element not in this layout, couldn't take";
  } else
    qDebug() << Q_FUNC_INFO << "Can't take null element";
  return false;
}

QList<QCPLayoutElement*> QCPLayoutGrid::elements() const
{
  QList<QCPLayoutElement*> result;
  int colC = columnCount();
  int rowC = rowCount();
  result.reserve(colC*rowC);
  for (int row=0; row<rowC; ++row)
  {
    for (int col=0; col<colC; ++col)
    {
      result.append(mElements.at(row).at(col));
    }
  }
  return result;
}

void QCPLayoutGrid::simplify()
{
  // remove rows with only empty cells:
  for (int row=rowCount()-1; row>=0; --row)
  {
    bool hasElements = false;
    for (int col=0; col<columnCount(); ++col)
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
      if (mElements.isEmpty()) // removed last element, also remove stretch factor (wouldn't happen below because also columnCount changed to 0 now)
        mColumnStretchFactors.clear();
    }
  }
  
  // remove columns with only empty cells:
  for (int col=columnCount()-1; col>=0; --col)
  {
    bool hasElements = false;
    for (int row=0; row<rowCount(); ++row)
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
      for (int row=0; row<rowCount(); ++row)
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
  result.rwidth() += (columnCount()-1) * mColumnSpacing + mMargins.left() + mMargins.right();
  result.rheight() += (rowCount()-1) * mRowSpacing + mMargins.top() + mMargins.bottom();
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
  result.rwidth() += (columnCount()-1) * mColumnSpacing + mMargins.left() + mMargins.right();
  result.rheight() += (rowCount()-1) * mRowSpacing + mMargins.top() + mMargins.bottom();
  return result;
}

void QCPLayoutGrid::getMinimumRowColSizes(QVector<int> *minColWidths, QVector<int> *minRowHeights) const
{
  *minColWidths = QVector<int>(columnCount(), 0);
  *minRowHeights = QVector<int>(rowCount(), 0);
  for (int row=0; row<rowCount(); ++row)
  {
    for (int col=0; col<columnCount(); ++col)
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
  *maxColWidths = QVector<int>(columnCount(), QWIDGETSIZE_MAX);
  *maxRowHeights = QVector<int>(rowCount(), QWIDGETSIZE_MAX);
  for (int row=0; row<rowCount(); ++row)
  {
    for (int col=0; col<columnCount(); ++col)
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


////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// QCPLayoutInset
////////////////////////////////////////////////////////////////////////////////////////////////////


QCPLayoutInset::QCPLayoutInset()
{
}

QCPLayoutInset::InsetPlacement QCPLayoutInset::insetPlacement(int index) const
{
  if (elementAt(index))
    return mInsetPlacement.at(index);
  else
  {
    qDebug() << Q_FUNC_INFO << "Invalid element index:" << index;
    return ipFree;
  }
}

Qt::Alignment QCPLayoutInset::insetAlignment(int index) const
{
  if (elementAt(index))
    return mInsetAlignment.at(index);
  else
  {
    qDebug() << Q_FUNC_INFO << "Invalid element index:" << index;
    return 0;
  }
}

QRectF QCPLayoutInset::insetRect(int index) const
{
  if (elementAt(index))
    return mInsetRect.at(index);
  else
  {
    qDebug() << Q_FUNC_INFO << "Invalid element index:" << index;
    return QRectF();
  }
}

void QCPLayoutInset::setInsetPlacement(int index, QCPLayoutInset::InsetPlacement placement)
{
  if (elementAt(index))
    mInsetPlacement[index] = placement;
  else
    qDebug() << Q_FUNC_INFO << "Invalid element index:" << index;
}

void QCPLayoutInset::setInsetAlignment(int index, Qt::Alignment alignment)
{
  if (elementAt(index))
    mInsetAlignment[index] = alignment;
  else
    qDebug() << Q_FUNC_INFO << "Invalid element index:" << index;
}

void QCPLayoutInset::setInsetRect(int index, const QRectF &rect)
{
  if (elementAt(index))
    mInsetRect[index] = rect;
  else
    qDebug() << Q_FUNC_INFO << "Invalid element index:" << index;
}

void QCPLayoutInset::updateLayout()
{
  for (int i=0; i<mElements.size(); ++i)
  {
    QRect insetRect;
    QSize minSize = mElements.at(i)->minimumSize().isValid() ? mElements.at(i)->minimumSize() : mElements.at(i)->minimumSizeHint();
    QSize maxSize = mElements.at(i)->maximumSize().isValid() ? mElements.at(i)->maximumSize() : mElements.at(i)->maximumSizeHint();
    if (mInsetPlacement.at(i) == ipFree)
    {
      insetRect = QRect(rect().x()+rect().width()*mInsetRect.at(i).x(),
                        rect().y()+rect().height()*mInsetRect.at(i).y(),
                        rect().width()*mInsetRect.at(i).width(),
                        rect().height()*mInsetRect.at(i).height());
      if (insetRect.size().width() < minSize.width())
        insetRect.setWidth(minSize.width());
      if (insetRect.size().height() < minSize.height())
        insetRect.setHeight(minSize.height());
      if (insetRect.size().width() > maxSize.width())
        insetRect.setWidth(maxSize.width());
      if (insetRect.size().height() > maxSize.height())
        insetRect.setHeight(maxSize.height());
    } else if (mInsetPlacement.at(i) == ipBorderAligned)
    {
      insetRect.setSize(minSize);
      Qt::Alignment al = mInsetAlignment.at(i);
      if (al.testFlag(Qt::AlignLeft)) insetRect.moveLeft(rect().x());
      else if (al.testFlag(Qt::AlignRight)) insetRect.moveRight(rect().x()+rect().width());
      else if (al.testFlag(Qt::AlignHCenter)) insetRect.moveLeft(rect().x()+rect().width()*0.5-minSize.width()*0.5);
      if (al.testFlag(Qt::AlignTop)) insetRect.moveTop(rect().y());
      else if (al.testFlag(Qt::AlignBottom)) insetRect.moveBottom(rect().y()+rect().height());
      else if (al.testFlag(Qt::AlignVCenter)) insetRect.moveTop(rect().y()+rect().height()*0.5-minSize.height()*0.5);
    }
    mElements.at(i)->setOuterRect(insetRect);
  }
}

int QCPLayoutInset::elementCount() const
{
  return mElements.size();
}

QCPLayoutElement *QCPLayoutInset::elementAt(int index) const
{
  if (index >= 0 && index < mElements.size())
    return mElements.at(index);
  else
    return 0;
}

QCPLayoutElement *QCPLayoutInset::takeAt(int index)
{
  if (QCPLayoutElement *el = elementAt(index))
  {
    releaseElement(el);
    mElements.removeAt(index);
    mInsetPlacement.removeAt(index);
    mInsetAlignment.removeAt(index);
    mInsetRect.removeAt(index);
    return el;
  } else
  {
    qDebug() << Q_FUNC_INFO << "Attempt to take invalid index:" << index;
    return 0;
  }
}

bool QCPLayoutInset::take(QCPLayoutElement *element)
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
    qDebug() << Q_FUNC_INFO << "Element not in this layout, couldn't take";
  } else
    qDebug() << Q_FUNC_INFO << "Can't take null element";
  return false;
}

void QCPLayoutInset::addElement(QCPLayoutElement *element, Qt::Alignment alignment)
{
  if (element)
  {
    if (element->layout()) // remove from old layout first
      element->layout()->take(element);
    mElements.append(element);
    mInsetPlacement.append(ipBorderAligned);
    mInsetAlignment.append(alignment);
    mInsetRect.append(QRectF(0.6, 0.6, 0.4, 0.4));
    adoptElement(element);
  } else
    qDebug() << Q_FUNC_INFO << "Can't add null element";
}

void QCPLayoutInset::addElement(QCPLayoutElement *element, const QRectF &rect)
{
  if (element)
  {
    if (element->layout()) // remove from old layout first
      element->layout()->take(element);
    mElements.append(element);
    mInsetPlacement.append(ipFree);
    mInsetAlignment.append(Qt::AlignRight|Qt::AlignTop);
    mInsetRect.append(rect);
    adoptElement(element);
  } else
    qDebug() << Q_FUNC_INFO << "Can't add null element";
}
