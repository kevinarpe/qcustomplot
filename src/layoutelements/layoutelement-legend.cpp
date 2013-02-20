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

#include "layoutelement-legend.h"

#include "../painter.h"
#include "../core.h"
#include "../plottable.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// QCPAbstractLegendItem
////////////////////////////////////////////////////////////////////////////////////////////////////

/*! \class QCPAbstractLegendItem
  \brief The abstract base class for all items in a QCPLegend.
  
  It defines a very basic interface to items in a QCPLegend. For representing plottables in the
  legend, the subclass QCPPlottableLegendItem is more suitable.
  
  Only derive directly from this class when you need absolute freedom (i.e. a legend item that's
  not associated with a plottable).

  You must implement the following pure virtual functions:
  \li \ref draw
  \li \ref size
  
  You inherit the following members you may use:
  <table>
    <tr>
      <td>QCPLegend *\b mParentLegend</td>
      <td>A pointer to the parent QCPLegend.</td>
    </tr><tr>
      <td>QFont \b mFont</td>
      <td>The generic font of the item. You should use this font for all or at least the most prominent text of the item.</td>
    </tr>
  </table>
*/

/* start documentation of pure virtual functions */

/*! \fn void QCPAbstractLegendItem::draw(QCPPainter *painter, const QRect &rect) const = 0;
  
  Draws this legend item with \a painter inside the specified \a rect. The \a rect typically has
  the size which was returned from a preceding \ref size call.
*/

/*! \fn QSize QCPAbstractLegendItem::size(const QSize &targetSize) const = 0;

  Returns the size this item occupies in the legend. The legend will adapt its layout with the help
  of this function. If this legend item can have a variable width (e.g. auto-wrapping text), this
  function tries to find a size with a width close to the width of \a targetSize. The height of \a
  targetSize only may have meaning in specific sublasses. Typically, it's ignored.
*/

/* end documentation of pure virtual functions */
/* start of documentation of signals */

/*! \fn void QCPAbstractLegendItem::selectionChanged(bool selected)
  
  This signal is emitted when the selection state of this legend item has changed, either by user interaction
  or by a direct call to \ref setSelected.
*/

/* end of documentation of signals */

/*!
  Constructs a QCPAbstractLegendItem and associates it with the QCPLegend \a parent. This does not
  cause the item to be added to \a parent, so \ref QCPLegend::addItem must be called separately.
*/
QCPAbstractLegendItem::QCPAbstractLegendItem(QCPLegend *parent) : 
  mParentLegend(parent),
  mAntialiased(false),
  mFont(parent->font()),
  mTextColor(parent->textColor()),
  mSelectedFont(parent->selectedFont()),
  mSelectedTextColor(parent->selectedTextColor()),
  mSelectable(true),
  mSelected(false)
{
  setMargins(QMargins(10, 2, 10, 2));
}

/*!
  Sets whether this legend item is drawn antialiased or not.
  
  Note that this setting may be overridden by \ref QCustomPlot::setAntialiasedElements and \ref
  QCustomPlot::setNotAntialiasedElements.
*/
void QCPAbstractLegendItem::setAntialiased(bool enabled)
{
  mAntialiased = enabled;
}

/*!
  Sets the default font of this specific legend item to \a font.
  
  \see setTextColor, QCPLegend::setFont
*/
void QCPAbstractLegendItem::setFont(const QFont &font)
{
  mFont = font;
}

/*!
  Sets the default text color of this specific legend item to \a color.
  
  \see setFont, QCPLegend::setTextColor
*/
void QCPAbstractLegendItem::setTextColor(const QColor &color)
{
  mTextColor = color;
}

/*!
  When this legend item is selected, \a font is used to draw generic text, instead of the normal
  font set with \ref setFont.
  
  \see setFont, QCPLegend::setSelectedFont
*/
void QCPAbstractLegendItem::setSelectedFont(const QFont &font)
{
  mSelectedFont = font;
}

/*!
  When this legend item is selected, \a color is used to draw generic text, instead of the normal
  color set with \ref setTextColor.
  
  \see setTextColor, QCPLegend::setSelectedTextColor
*/
void QCPAbstractLegendItem::setSelectedTextColor(const QColor &color)
{
  mSelectedTextColor = color;
}

/*!
  Sets whether this specific legend item is selectable.
  
  \see setSelected, QCustomPlot::setInteractions
*/
void QCPAbstractLegendItem::setSelectable(bool selectable)
{
  mSelectable = selectable;
}

/*!
  Sets whether this specific legend item is selected.
  
  It is possible to set the selection state of this item by calling this function directly, even if
  setSelectable is set to false.
  
  \see setSelectable, QCustomPlot::setInteractions
*/
void QCPAbstractLegendItem::setSelected(bool selected)
{
  if (mSelected != selected)
  {
    mSelected = selected;
    emit selectionChanged(mSelected);
  }
}

/*! \internal

  Sets the QPainter::Antialiasing render hint on the provided \a painter, depending on the \ref
  setAntialiased state of this legend item as well as the overrides \ref
  QCustomPlot::setAntialiasedElements and \ref QCustomPlot::setNotAntialiasedElements.
*/
void QCPAbstractLegendItem::applyAntialiasingHint(QCPPainter *painter) const
{
  if (mParentLegend->mParentPlot->notAntialiasedElements().testFlag(QCP::aeLegendItems))
    painter->setAntialiasing(false);
  else if (mParentLegend->mParentPlot->antialiasedElements().testFlag(QCP::aeLegendItems))
    painter->setAntialiasing(true);
  else
    painter->setAntialiasing(mAntialiased);
}


////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// QCPPlottableLegendItem
////////////////////////////////////////////////////////////////////////////////////////////////////

/*! \class QCPPlottableLegendItem
  \brief A legend item representing a plottable with an icon and the plottable name.
  
  This is the standard legend item for plottables. It displays an icon of the plottable next to the
  plottable name. The icon is drawn by the respective plottable itself (\ref
  QCPAbstractPlottable::drawLegendIcon), and tries to give an intuitive symbol for the plottable.
  For example, the QCPGraph draws a centered horizontal line with a single scatter point in the
  middle and filling (if enabled) below.
  
  Legend items of this type are always associated with one plottable (retrievable via the
  plottable() function and settable with the constructor). You may change the font of the plottable
  name with \ref setFont. If \ref setTextWrap is set to true, the plottable name will wrap at the
  right legend boundary (see \ref QCPLegend::setMinimumSize). Icon padding and border pen is taken
  from the parent QCPLegend, see \ref QCPLegend::setIconBorderPen and \ref
  QCPLegend::setIconTextPadding.

  The function \ref QCPAbstractPlottable::addToLegend/\ref QCPAbstractPlottable::removeFromLegend
  creates/removes legend items of this type in the default implementation. However, these functions
  may be reimplemented such that a different kind of legend item (e.g a direct subclass of
  QCPAbstractLegendItem) is used for that plottable.
*/

/*!
  Creates a new legend item associated with \a plottable.
  
  Once it's created, it can be added to the legend via \ref QCPLegend::addItem.
  
  A more convenient way of adding/removing a plottable to/from the legend is via the functions \ref
  QCPAbstractPlottable::addToLegend and \ref QCPAbstractPlottable::removeFromLegend.
*/
QCPPlottableLegendItem::QCPPlottableLegendItem(QCPLegend *parent, QCPAbstractPlottable *plottable) :
  QCPAbstractLegendItem(parent),
  mPlottable(plottable)
{
}

/*! \internal
  
  Returns the pen that shall be used to draw the icon border, taking into account the selection
  state of this item.
*/
QPen QCPPlottableLegendItem::getIconBorderPen() const
{
  return mSelected ? mParentLegend->selectedIconBorderPen() : mParentLegend->iconBorderPen();
}

/*! \internal
  
  Returns the text color that shall be used to draw text, taking into account the selection state
  of this item.
*/
QColor QCPPlottableLegendItem::getTextColor() const
{
  return mSelected ? mSelectedTextColor : mTextColor;
}

/*! \internal
  
  Returns the font that shall be used to draw text, taking into account the selection state of this
  item.
*/
QFont QCPPlottableLegendItem::getFont() const
{
  return mSelected ? mSelectedFont : mFont;
}

/*! \internal
  
  Draws the item with \a painter. The size and position of the drawn legend item is defined by the
  parent layout (typically a \ref QCPLegend) and the \ref minimumSizeHint and \ref maximumSizeHint
  of this legend item.
*/
void QCPPlottableLegendItem::draw(QCPPainter *painter) const
{
  if (!mPlottable) return;
  painter->setFont(getFont());
  painter->setPen(QPen(getTextColor()));
  QSizeF iconSize = mParentLegend->iconSize();
  QRectF textRect = painter->fontMetrics().boundingRect(0, 0, 0, iconSize.height(), Qt::TextDontClip, mPlottable->name());
  QRectF iconRect(mRect.topLeft(), iconSize);
  int textHeight = qMax(textRect.height(), iconSize.height());  // if text has smaller height than icon, center text vertically in icon height, else align tops
  painter->drawText(mRect.x()+iconSize.width()+mParentLegend->iconTextPadding(), mRect.y(), textRect.width(), textHeight, Qt::TextDontClip, mPlottable->name());
  // draw icon:
  painter->save();
  painter->setClipRect(iconRect, Qt::IntersectClip);
  mPlottable->drawLegendIcon(painter, iconRect);
  painter->restore();
  // draw icon border:
  if (getIconBorderPen().style() != Qt::NoPen)
  {
    painter->setPen(getIconBorderPen());
    painter->setBrush(Qt::NoBrush);
    painter->drawRect(iconRect);
  }
}

/*! \internal
  
  Calculates and returns the size of this item. This includes the icon, the text and the padding in
  between.
*/
QSize QCPPlottableLegendItem::minimumSizeHint() const
{
  if (!mPlottable) return QSize();
  QSize result(0, 0);
  QRect textRect;
  QFontMetrics fontMetrics(getFont());
  QSize iconSize = mParentLegend->iconSize();
  textRect = fontMetrics.boundingRect(0, 0, 0, iconSize.height(), Qt::TextDontClip, mPlottable->name());
  result.setWidth(iconSize.width() + mParentLegend->iconTextPadding() + textRect.width() + mMargins.left() + mMargins.right());
  result.setHeight(qMax(textRect.height(), iconSize.height()) + mMargins.top() + mMargins.bottom());
  return result;
}


////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// QCPLegend
////////////////////////////////////////////////////////////////////////////////////////////////////

/*! \class QCPLegend
  \brief Manages a legend inside a QCustomPlot.

  Doesn't need to be instantiated externally, rather access QCustomPlot::legend
*/

/* start of documentation of signals */

/*! \fn void QCPLegend::selectionChanged(QCPLegend::SelectableParts selection);

  This signal is emitted when the selection state of this legend has changed.
  
  \see setSelected, setSelectable
*/

/* end of documentation of signals */

/*!
  Constructs a new QCPLegend instance with \a parentPlot as the containing plot and default
  values. Under normal usage, QCPLegend needn't be instantiated outside of QCustomPlot.
  Access QCustomPlot::legend to modify the legend (set to invisible by default, see \ref
  setVisible).
*/
QCPLegend::QCPLegend(QCustomPlot *parentPlot) :
  QCPLayerable(parentPlot)
{
  setRowSpacing(0);
  setColumnSpacing(10);
  
  setAntialiased(false);
  setSize(100, 28);
  setIconSize(32, 18);
  setAutoSize(true);
  
  setIconTextPadding(7);
  
  setSelectable(spLegendBox | spItems);
  setSelected(spNone);
  
  setBorderPen(QPen(Qt::black));
  setSelectedBorderPen(QPen(Qt::blue, 2));
  setIconBorderPen(Qt::NoPen);
  setSelectedIconBorderPen(QPen(Qt::blue, 2));
  setBrush(Qt::white);
  setSelectedBrush(Qt::white);
  setFont(parentPlot->font());
  setSelectedFont(parentPlot->font());
  setTextColor(Qt::black);
  setSelectedTextColor(Qt::blue);
}

QCPLegend::~QCPLegend()
{
  clearItems();
}

QCPLegend::SelectableParts QCPLegend::selected() const
{
  // check whether any legend elements selected, if yes, add spItems to return value
  bool hasSelectedItems = false;
  for (int i=0; i<itemCount(); ++i)
  {
    if (item(i) && item(i)->selected())
    {
      hasSelectedItems = true;
      break;
    }
  }
  if (hasSelectedItems)
    return mSelected | spItems;
  else
    return mSelected & ~spItems;
}

/*!
  Sets the pen, the border of the entire legend is drawn with.
*/
void QCPLegend::setBorderPen(const QPen &pen)
{
  mBorderPen = pen;
}

/*!
  Sets the brush of the legend background.
*/
void QCPLegend::setBrush(const QBrush &brush)
{
  mBrush = brush;
}

/*!
  Sets the default font of legend text. Legend items that draw text (e.g. the name of a graph) will
  use this font by default. However, a different font can be specified on a per-item-basis by
  accessing the specific legend item.
  
  This function will also set \a font on all already existing legend items.
  
  \see QCPAbstractLegendItem::setFont
*/
void QCPLegend::setFont(const QFont &font)
{
  mFont = font;
  for (int i=0; i<itemCount(); ++i)
    item(i)->setFont(mFont);
}

/*!
  Sets the default color of legend text. Legend items that draw text (e.g. the name of a graph)
  will use this color by default. However, a different colors can be specified on a per-item-basis
  by accessing the specific legend item.
  
  This function will also set \a color on all already existing legend items.
  
  \see QCPAbstractLegendItem::setTextColor
*/
void QCPLegend::setTextColor(const QColor &color)
{
  mTextColor = color;
  for (int i=0; i<itemCount(); ++i)
   item(i)->setTextColor(color);
}

/*!
  Sets whether the size of the legend should be calculated automatically to fit all the content
  (plus padding), or whether the size must be specified manually with \ref setSize.
  
  If the autoSize mechanism is enabled, the legend will have the smallest possible size to still
  display all its content. For items with text wrapping (QCPPlottableLegendItem::setTextWrap) this
  means, they would become very compressed, i.e. wrapped at every word. To prevent this, set a
  reasonable \ref setMinimumSize width.
*/
void QCPLegend::setAutoSize(bool on)
{
  mAutoSize = on;
}

/*!
  Sets the size of the legend. Setting the size manually with this function only has an effect, if
  \ref setAutoSize is set to false.
  
  If you want to control the minimum size (or the text-wrapping width) while still leaving the
  autoSize mechanism enabled, consider using \ref setMinimumSize.
  
  \see setAutoSize, setMinimumSize
*/
void QCPLegend::setSize(const QSize &size)
{
  mSize = size;
}

/*! \overload
*/
void QCPLegend::setSize(int width, int height)
{
  mSize = QSize(width, height);
}

/*!
  Sets the padding of the legend. Padding is the space by what the legend box is made larger than
  minimally needed for the content to fit. I.e. it's the space left blank on each side inside the
  legend.
*/
void QCPLegend::setPadding(QMargins padding)
{
  mPadding = padding;
}

/*!
  Sets the size of legend icons. Legend items that draw an icon (e.g. a visual
  representation of the graph) will use this size by default.
*/
void QCPLegend::setIconSize(const QSize &size)
{
  mIconSize = size;
}

/*! \overload
*/
void QCPLegend::setIconSize(int width, int height)
{
  mIconSize.setWidth(width);
  mIconSize.setHeight(height);
}

/*!
  Sets the horizontal space in pixels between the legend icon and the text next to it.
  Legend items that draw an icon (e.g. a visual representation of the graph) and text (e.g. the
  name of the graph) will use this space by default.
  
  \see setItemSpacing
*/
void QCPLegend::setIconTextPadding(int padding)
{
  mIconTextPadding = padding;
}

/*!
  Sets the pen used to draw a border around each legend icon. Legend items that draw an
  icon (e.g. a visual representation of the graph) will use this pen by default.
  
  If no border is wanted, set this to \a Qt::NoPen.
*/
void QCPLegend::setIconBorderPen(const QPen &pen)
{
  mIconBorderPen = pen;
}

/*!
  Sets whether the user can (de-)select the parts in \a selectable by clicking on the QCustomPlot surface.
  (When \ref QCustomPlot::setInteractions contains iSelectLegend.)
  
  However, even when \a selectable is set to a value not allowing the selection of a specific part,
  it is still possible to set the selection of this part manually, by calling \ref setSelected
  directly.
  
  \see SelectablePart, setSelected
*/
void QCPLegend::setSelectable(const SelectableParts &selectable)
{
  mSelectable = selectable;
}

/*!
  Sets the selected state of the respective legend parts described by \ref SelectablePart. When a part
  is selected, it uses a different pen/font and brush. If some legend items are selected and \a selected
  doesn't contain \ref spItems, those items become deselected.
  
  The entire selection mechanism is handled automatically when \ref QCustomPlot::setInteractions
  contains iSelectLegend. You only need to call this function when you wish to change the selection
  state manually.
  
  This function can change the selection state of a part even when \ref setSelectable was set to a
  value that actually excludes the part.
  
  emits the \ref selectionChanged signal when \a selected is different from the previous selection state.
  
  Note that it doesn't make sense to set the selected state \ref spItems here when it wasn't set
  before, because there's no way to specify which exact items to newly select. Do this by calling
  \ref QCPAbstractLegendItem::setSelected directly on the legend item you wish to select.
  
  \see SelectablePart, setSelectable, selectTest, setSelectedBorderPen, setSelectedIconBorderPen, setSelectedBrush,
  setSelectedFont
*/
void QCPLegend::setSelected(const SelectableParts &selected)
{
  SelectableParts newSelected = selected;
  mSelected = this->selected(); // update mSelected in case item selection changed

  if (mSelected != newSelected)
  {
    if (!mSelected.testFlag(spItems) && newSelected.testFlag(spItems)) // attemt to set spItems flag (can't do that)
    {
      qDebug() << Q_FUNC_INFO << "spItems flag can not be set, it can only be unset manually";
      newSelected &= ~spItems;
    }
    if (mSelected.testFlag(spItems) && !newSelected.testFlag(spItems)) // spItems flag was unset, so clear item selection
    {
      for (int i=0; i<itemCount(); ++i)
        item(i)->setSelected(false);
    }
    mSelected = newSelected;
    emit selectionChanged(mSelected);
  }
}

/*!
  When the legend box is selected, this pen is used to draw the border instead of the normal pen
  set via \ref setBorderPen.

  \see setSelected, setSelectable, setSelectedBrush
*/
void QCPLegend::setSelectedBorderPen(const QPen &pen)
{
  mSelectedBorderPen = pen;
}

/*!
  Sets the pen legend items will use to draw their icon borders, when they are selected.

  \see setSelected, setSelectable, setSelectedFont
*/
void QCPLegend::setSelectedIconBorderPen(const QPen &pen)
{
  mSelectedIconBorderPen = pen;
}

/*!
  When the legend box is selected, this brush is used to draw the legend background instead of the normal brush
  set via \ref setBrush.

  \see setSelected, setSelectable, setSelectedBorderPen
*/
void QCPLegend::setSelectedBrush(const QBrush &brush)
{
  mSelectedBrush = brush;
}

/*!
  Sets the default font that is used by legend items when they are selected.
  
  This function will also set \a font on all already existing legend items.

  \see setFont, QCPAbstractLegendItem::setSelectedFont
*/
void QCPLegend::setSelectedFont(const QFont &font)
{
  mSelectedFont = font;
  for (int i=0; i<itemCount(); ++i)
    item(i)->setSelectedFont(font);
}

/*!
  Sets the default text color that is used by legend items when they are selected.
  
  This function will also set \a color on all already existing legend items.

  \see setTextColor, QCPAbstractLegendItem::setSelectedTextColor
*/
void QCPLegend::setSelectedTextColor(const QColor &color)
{
  mSelectedTextColor = color;
  for (int i=0; i<itemCount(); ++i)
    item(i)->setSelectedTextColor(color);
}

/*!
  Returns the item with index \a i.
  
  \see itemCount
*/
QCPAbstractLegendItem *QCPLegend::item(int index) const
{
  return qobject_cast<QCPAbstractLegendItem*>(elementAt(index));
}

/*!
  Returns the QCPPlottableLegendItem which is associated with \a plottable (e.g. a \ref QCPGraph*).
  If such an item isn't in the legend, returns 0.
  
  \see hasItemWithPlottable
*/
QCPPlottableLegendItem *QCPLegend::itemWithPlottable(const QCPAbstractPlottable *plottable) const
{
  for (int i=0; i<itemCount(); ++i)
  {
    if (QCPPlottableLegendItem *pli = qobject_cast<QCPPlottableLegendItem*>(item(i)))
    {
      if (pli->plottable() == plottable)
        return pli;
    }
  }
  return 0;
}

/*!
  Returns the number of items currently in the legend.
  \see item
*/
int QCPLegend::itemCount() const
{
  return elementCount();
}

/*!
  Returns whether the legend contains \a itm.
*/
bool QCPLegend::hasItem(QCPAbstractLegendItem *item) const
{
  for (int i=0; i<itemCount(); ++i)
  {
    if (item == this->item(i))
        return true;
  }
  return false;
}

/*!
  Returns whether the legend contains a QCPPlottableLegendItem which is associated with \a plottable (e.g. a \ref QCPGraph*).
  If such an item isn't in the legend, returns false.
  
  \see itemWithPlottable
*/
bool QCPLegend::hasItemWithPlottable(const QCPAbstractPlottable *plottable) const
{
  return itemWithPlottable(plottable);
}

/*!
  Adds \a item to the legend, if it's not present already.
  
  Returns true on sucess, i.e. if the item wasn't in the list already and has been successfuly added.
  
  The legend takes ownership of the item.
*/
bool QCPLegend::addItem(QCPAbstractLegendItem *item)
{
  if (!hasItem(item))
  {
    return addElement(rowCount(), 0, item);
  } else
    return false;
}

/*!
  Removes the item with index \a index from the legend.

  Returns true, if successful.
  
  \see itemCount, clearItems
*/
bool QCPLegend::removeItem(int index)
{
  if (QCPAbstractLegendItem *ali = item(index))
  {
    bool success = remove(ali);
    simplify();
    return success;
  } else
    return false;
}

/*! \overload
  
  Removes \a item from the legend.

  Returns true, if successful.
  
  \see clearItems
*/
bool QCPLegend::removeItem(QCPAbstractLegendItem *item)
{
  bool success = remove(item);
  simplify();
  return success;
}

/*!
  Removes all items from the legend.
*/
void QCPLegend::clearItems()
{
  for (int i=itemCount()-1; i>=0; --i)
    removeItem(i);
}

/*!
  Returns the legend items that are currently selected. If no items are selected,
  the list is empty.
  
  \see QCPAbstractLegendItem::setSelected, setSelectable
*/
QList<QCPAbstractLegendItem *> QCPLegend::selectedItems() const
{
  QList<QCPAbstractLegendItem*> result;
  for (int i=0; i<itemCount(); ++i)
  {
    if (QCPAbstractLegendItem *ali = item(i))
    {
      if (ali->selected())
        result.append(ali);
    }
  }
  return result;
}

/*!
  When the point \a pos in pixels hits a legend item, the item is returned. If no item is hit, 0 is
  returned.
  
  \see selectTestLegend
*/
QCPAbstractLegendItem *QCPLegend::itemAtPos(const QPointF &pos) const
{
  for (int i=0; i<elementCount(); ++i)
  {
    if (QCPAbstractLegendItem *ali = item(i))
    {
      if (ali->selectTest(pos))
        return ali;
    }
  }
  return 0;
}

/*! \internal
  
  Handles the selection \a event and returns true when the selection event hit any parts of the
  legend. If the selection state of any parts of the legend was changed, the output parameter \a
  modified is set to true.
  
  When \a additiveSelecton is true, any new selections become selected in addition to the recent
  selections. The recent selections are not cleared. Further, clicking on one object multiple times
  in additive selection mode, toggles the selection of that object on and off.
  
  To indicate that an event deselects the legend (i.e. the parts that are deselectable by the user,
  see \ref setSelectable), pass 0 as \a event.
*/
bool QCPLegend::handleLegendSelection(QMouseEvent *event, bool additiveSelection, bool &modified)
{
  modified = false;
  bool selectionFound = false;
  
  if (event && selectTest(event->pos())) // clicked inside legend somewhere
  {
    QCPAbstractLegendItem *ali = itemAtPos(event->pos());
    if (selectable().testFlag(QCPLegend::spItems) && ali && ali->selectable()) // items shall be selectable and item ali was clicked 
    {
      selectionFound = true;
      // deselect legend box:
      if (!additiveSelection && selected().testFlag(QCPLegend::spLegendBox) && selectable().testFlag(QCPLegend::spLegendBox))
        setSelected(selected() & ~QCPLegend::spLegendBox);
      // first select clicked item:
      if (!ali->selected() || additiveSelection) // if additive selection, we toggle selection on and off per click
      {
        modified = true;
        ali->setSelected(!ali->selected());
      }
      // finally, deselect all other items (if we had deselected all first, the selectionChanged signal of QCPLegend might have been emitted twice):
      if (!additiveSelection)
      {
        for (int i=0; i<itemCount(); ++i)
        {
          if (item(i) != ali && item(i)->selected() && item(i)->selectable())
          {
            modified = true;
            item(i)->setSelected(false);
          }
        }
      }
    } else // no specific item clicked or items not selectable
    {
      // if items actually were selectable, this means none were clicked, deselect them:
      if (selectable().testFlag(QCPLegend::spItems) && selected().testFlag(QCPLegend::spItems) && !additiveSelection)
      {
        for (int i=0; i<itemCount(); ++i)
        {
          if (item(i)->selectable())
            item(i)->setSelected(false);
        }
        modified = true;
      }
      // if legend box is selectable, select it:
      if (selectable().testFlag(QCPLegend::spLegendBox))
      {
        if (!selected().testFlag(QCPLegend::spLegendBox) || additiveSelection)
        {
          selectionFound = true;
          setSelected(selected() ^ QCPLegend::spLegendBox); // xor because we always toggle
          modified = true;
        }
      }
    }
  } else if (selected() != QCPLegend::spNone && selectable() != QCPLegend::spNone && !additiveSelection) // legend not clicked, deselect it if selectable allows that (and all child items)
  {
    // only deselect parts that are allowed to be changed by user according to selectable()
    // deselect child items (and automatically removes spItems from selected state of legend, if last item gets deselected):
    if (selectable().testFlag(spItems)) 
    {
      for (int i=0; i<itemCount(); ++i)
      {
        if (item(i)->selected() && item(i)->selectable())
        {
          item(i)->setSelected(false);
          modified = true;
        }
      }
    }
    // only deselect parts that are allowed to be changed (are selectable). Don't forcibly remove
    // spItems, because some selected items might not be selectable, i.e. allowed to be deselected
    // by user interaction. If that's not the case, spItems will have been removed from selected()
    // state in previous loop by individual setSelected(false) calls on the items anyway.
    QCPLegend::SelectableParts newState = selected() & ~(selectable()&~spItems);
    if (newState != selected())
    {
      setSelected(newState);
      modified = true;
    }
  }
  
  return selectionFound;
}

/*! \internal

  A convenience function to easily set the QPainter::Antialiased hint on the provided \a painter
  before drawing main legend elements.

  This is the antialiasing state the painter passed to the \ref draw method is in by default.
  
  This function takes into account the local setting of the antialiasing flag as well as
  the overrides set e.g. with \ref QCustomPlot::setNotAntialiasedElements.
  
  \see setAntialiased
*/
void QCPLegend::applyDefaultAntialiasingHint(QCPPainter *painter) const
{
  applyAntialiasingHint(painter, mAntialiased, QCP::aeLegend);
}

/*! \internal
  
  Returns the pen used to paint the border of the legend, taking into account the selection state
  of the legend box.
*/
QPen QCPLegend::getBorderPen() const
{
  return mSelected.testFlag(spLegendBox) ? mSelectedBorderPen : mBorderPen;
}

/*! \internal
  
  Returns the brush used to paint the background of the legend, taking into account the selection
  state of the legend box.
*/
QBrush QCPLegend::getBrush() const
{
  return mSelected.testFlag(spLegendBox) ? mSelectedBrush : mBrush;
}

/*! \internal
  
  Draws the legend with the provided \a painter.
*/
void QCPLegend::draw(QCPPainter *painter)
{
  painter->setBrush(getBrush());
  painter->setPen(getBorderPen());
  // draw background rect:
  painter->drawRect(mRect);
  // draw legend items:
  painter->setPen(QPen());
  painter->setBrush(Qt::NoBrush);
  for (int i=0; i<itemCount(); ++i)
  {
    painter->save();
    painter->setClipRect(item(i)->outerRect());
    item(i)->applyAntialiasingHint(painter);
    item(i)->draw(painter);
    painter->restore();
  }
}
