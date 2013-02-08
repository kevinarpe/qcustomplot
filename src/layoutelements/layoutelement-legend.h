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

#ifndef QCP_LAYOUTELEMENT_LEGEND_H
#define QCP_LAYOUTELEMENT_LEGEND_H

#include "../global.h"
#include "../layer.h"
#include "../layout.h"

class QCPPainter;
class QCustomPlot;
class QCPAbstractPlottable;
class QCPLegend;

class QCP_LIB_DECL QCPAbstractLegendItem : public QCPLayoutElement
{
  Q_OBJECT
public:
  QCPAbstractLegendItem(QCPLegend *parent);
  
  // getters:
  bool antialiased() const { return mAntialiased; }
  QFont font() const { return mFont; }
  QColor textColor() const { return mTextColor; }
  QFont selectedFont() const { return mSelectedFont; }
  QColor selectedTextColor() const { return mSelectedTextColor; }
  bool selectable() const { return mSelectable; }
  bool selected() const { return mSelected; }
  
  // setters:
  void setAntialiased(bool enabled);
  void setFont(const QFont &font);
  void setTextColor(const QColor &color);
  void setSelectedFont(const QFont &font);
  void setSelectedTextColor(const QColor &color);
  void setSelectable(bool selectable);
  void setSelected(bool selected);
  
signals:
  void selectionChanged(bool selected);
  
protected:
  QCPLegend *mParentLegend;
  bool mAntialiased;
  QFont mFont;
  QColor mTextColor;
  QFont mSelectedFont;
  QColor mSelectedTextColor;
  bool mSelectable, mSelected;
  
  virtual void draw(QCPPainter *painter) const = 0;
  void applyAntialiasingHint(QCPPainter *painter) const;
  
private:
  Q_DISABLE_COPY(QCPAbstractLegendItem)
  
  friend class QCPLegend;
};


class QCP_LIB_DECL QCPPlottableLegendItem : public QCPAbstractLegendItem
{
  Q_OBJECT
public:
  QCPPlottableLegendItem(QCPLegend *parent, QCPAbstractPlottable *plottable);
  
  // getters:
  QCPAbstractPlottable *plottable() { return mPlottable; }
  
protected:
  QCPAbstractPlottable *mPlottable;
  
  QPen getIconBorderPen() const;
  QColor getTextColor() const;
  QFont getFont() const;

  virtual void draw(QCPPainter *painter) const;
  virtual QSize minimumSizeHint() const;
};


class QCP_LIB_DECL QCPLegend : public QCPLayoutGrid, public QCPLayerable
{
  Q_OBJECT
public:
  /*!
    Defines the selectable parts of a legend
  */
  enum SelectablePart { spNone       = 0      ///< None
                       ,spLegendBox  = 0x001  ///< The legend box (frame)
                       ,spItems      = 0x002  ///< Legend items individually (see \ref selectedItems)
                      };
  Q_ENUMS(SelectablePart)
  Q_DECLARE_FLAGS(SelectableParts, SelectablePart)
  
  explicit QCPLegend(QCustomPlot *parentPlot);
  virtual ~QCPLegend();
  
  // getters:
  QPen borderPen() const { return mBorderPen; }
  QBrush brush() const { return mBrush; }
  QFont font() const { return mFont; }
  QColor textColor() const { return mTextColor; }
  bool autoSize() const { return mAutoSize; }
  QSize size() const { return mSize; }
  QMargins padding() const { return mPadding; }
  int itemSpacing() const { return mItemSpacing; }
  QSize iconSize() const { return mIconSize; }
  int iconTextPadding() const { return mIconTextPadding; }
  QPen iconBorderPen() const { return mIconBorderPen; }
  SelectableParts selectable() const { return mSelectable; }
  SelectableParts selected() const { return mSelected; }
  QPen selectedBorderPen() const { return mSelectedBorderPen; }
  QPen selectedIconBorderPen() const { return mSelectedIconBorderPen; }
  QBrush selectedBrush() const { return mSelectedBrush; }
  QFont selectedFont() const { return mSelectedFont; }
  QColor selectedTextColor() const { return mSelectedTextColor; }
  
  // setters:
  void setBorderPen(const QPen &pen);
  void setBrush(const QBrush &brush);
  void setFont(const QFont &font);
  void setTextColor(const QColor &color);
  void setAutoSize(bool on);
  void setSize(const QSize &size);
  void setSize(int width, int height);
  void setPadding(QMargins padding);
  void setItemSpacing(int spacing);
  void setIconSize(const QSize &size);
  void setIconSize(int width, int height);
  void setIconTextPadding(int padding);
  void setIconBorderPen(const QPen &pen);
  void setSelectable(const SelectableParts &selectable);
  void setSelected(const SelectableParts &selected);
  void setSelectedBorderPen(const QPen &pen);
  void setSelectedIconBorderPen(const QPen &pen);
  void setSelectedBrush(const QBrush &brush);
  void setSelectedFont(const QFont &font);
  void setSelectedTextColor(const QColor &color);

  // non-property methods:
  QCPAbstractLegendItem *item(int index) const;
  QCPPlottableLegendItem *itemWithPlottable(const QCPAbstractPlottable *plottable) const;
  int itemCount() const;
  bool hasItem(QCPAbstractLegendItem *item) const;
  bool hasItemWithPlottable(const QCPAbstractPlottable *plottable) const;
  bool addItem(QCPAbstractLegendItem *item);
  bool removeItem(int index);
  bool removeItem(QCPAbstractLegendItem *item);
  void clearItems();
  QList<QCPAbstractLegendItem*> selectedItems() const;
  
  bool selectTestLegend(const QPointF &pos) const;
  QCPAbstractLegendItem *selectTestItem(const QPoint pos) const;
  
signals:
  void selectionChanged(QCPLegend::SelectableParts selection);
  
protected:
  // simple properties with getters and setters:
  QPen mBorderPen, mIconBorderPen;
  QBrush mBrush;
  QFont mFont;
  QColor mTextColor;
  QSize mSize, mIconSize;
  bool mAutoSize;
  QMargins mPadding;
  int mItemSpacing, mIconTextPadding;
  SelectableParts mSelected, mSelectable;
  QPen mSelectedBorderPen, mSelectedIconBorderPen;
  QBrush mSelectedBrush;
  QFont mSelectedFont;
  QColor mSelectedTextColor;
  
  // internal or not explicitly exposed properties:
  QList<QCPAbstractLegendItem*> mItems;
  QMap<QCPAbstractLegendItem*, QRect> mItemBoundingBoxes;
  
  virtual void updateSelectionState();
  virtual bool handleLegendSelection(QMouseEvent *event, bool additiveSelection, bool &modified);
  // introduced methods:
  virtual void applyDefaultAntialiasingHint(QCPPainter *painter) const;
  virtual void draw(QCPPainter *painter);
  
  // drawing helpers:
  QPen getBorderPen() const;
  QBrush getBrush() const;
  
private:
  Q_DISABLE_COPY(QCPLegend)
  
  friend class QCustomPlot;
  friend class QCPAbstractLegendItem;
};
Q_DECLARE_OPERATORS_FOR_FLAGS(QCPLegend::SelectableParts)

#endif // QCP_LAYOUTELEMENT_LEGEND_H
