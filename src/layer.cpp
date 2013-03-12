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

#include "layer.h"

#include "painter.h"
#include "core.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// QCPLayer
////////////////////////////////////////////////////////////////////////////////////////////////////

/*! \class QCPLayer
  \brief A layer that may contain objects, to control the rendering order
  
  The Layering system of QCustomPlot is the mechanism to control the rendering order of the
  elements inside the plot, e.g. that the grid is drawn behind plottables etc.
  
  It is based on the two classes QCPLayer and QCPLayerable. A QCustomPlot contains an ordered list
  of one or more instances of QCPLayer (see QCustomPlot::addLayer, QCustomPlot::layer,
  QCustomPlot::moveLayer, etc.). The layers are drawn in the order they are in the list.

  A QCPLayer itself contains an ordered list of QCPLayerable instances. QCPLayerable is an abstract
  base class from which almost all visible objects derive, like axes, grids, graphs, items, etc.
  
  By default, QCustomPlot has five layers: "background", "grid", "main", "axes" and "legend" (in
  that order). By default, the QCPGrid instances are on the "grid" layer, so the grid will be drawn
  beneath the objects on the other two layers. The top two layers are "axes" and "legend" which
  contain all axes and legends respectively, so they will be drawn on top. In the middle, there is
  the "main" layer. It is initially empty and set as the current layer (see
  QCustomPlot::setCurrentLayer). This means, all new plottables, items etc. are created on this
  layer by default, and are thus drawn above the grid but below the axes. Axis rect backgrounds
  shall be drawn behind everything else, thus QCPAxisRect instances place themselves on the
  "background" layer by default.
  
  Controlling the ordering of objects is easy: Create a new layer in the position you want it to
  be, e.g. above "main", with QCustomPlot::addLayer. Then set the current layer with
  QCustomPlot::setCurrentLayer to that new layer and finally create the objects normally. They will
  be placed on the new layer automatically, due to the current layer setting. Alternatively you
  could have also ignored the current layer setting and just moved the objects with
  QCPLayerable::setLayer to the desired layer after creating them.
  
  It is also possible to move whole layers. For example, If you want the grid to be shown in front
  of all plottables/items on the "main" layer, just move it above "main" with
  QCustomPlot::moveLayer. This way the ordering might now be "main", "grid", "axes", so while the
  grid will still be beneath the axes, it will now be drawn above plottables/items on "main", as
  intended.
  
  The rendering order within one layer is simply by order of creation. The item created last (or
  added last to the layer), is drawn on top of all other objects on that layer.  
  
  When a layer is deleted, the objects on it are not deleted with it, but fall on the layer below
  the deleted layer, see QCustomPlot::removeLayer.
*/

/* start documentation of inline functions */

/*! \fn QList<QCPLayerable*> QCPLayer::children() const
  
  Returns a list of all layerables on this layer. The order corresponds to the rendering order,
  i.e. layerables with higher indices are drawn above layerables with lower indices.
*/

/*! \fn int QCPLayer::index() const
  
  Returns the index this layer has in the QCustomPlot. The index is the integer number by which this layer can be
  accessed via QCustomPlot::layer.
  
  Layers with greater indices will be drawn above layers with smaller indices.
*/

/* end documentation of inline functions */

/*!
  Creates a new QCPLayer instance.
  
  Normally you shouldn't directly create layers like this, use QCustomPlot::addLayer instead.
  
  \warning It is not checked that \a layerName is actually an unique layer name in \a parentPlot.
  This check is only performed by QCustomPlot::addLayer.
*/
QCPLayer::QCPLayer(QCustomPlot *parentPlot, const QString &layerName) :
  QObject(parentPlot),
  mParentPlot(parentPlot),
  mName(layerName),
  mIndex(-1) // will be set to a proper value by the QCustomPlot layer creation function
{
  // Note: no need to make sure layerName is unique, because layer
  // management is done with QCustomPlot functions.
}

QCPLayer::~QCPLayer()
{
  // If child layerables are still on this layer, detach them, so they don't try to reach back to this
  // then invalid layer once they get deleted/moved themselves. This only happens when layers are deleted
  // directly, like in the QCustomPlot destructor. (The regular layer removal procedure for the user is to
  // call QCustomPlot::removeLayer, which moves all layerables off this layer before deleting it.)
  
  while (!mChildren.isEmpty())
    mChildren.last()->setLayer(0); // removes itself from mChildren via removeChild()
  
  if (mParentPlot->currentLayer() == this)
    qDebug() << Q_FUNC_INFO << "The parent plot's mCurrentLayer will be a dangling pointer. Should have been set to a valid layer or 0 beforehand.";
}

/*! \internal
  
  Adds the \a layerable to the list of this layer. If \a prepend is set to true, the layerable will
  be prepended to the list, i.e. be drawn beneath the other layerables already in the list.
  
  This function does not change the \a mLayer member of \a layerable to this layer. (Use
  QCPLayerable::setLayer to change the layer of an object, not this function.)
  
  \see removeChild
*/
void QCPLayer::addChild(QCPLayerable *layerable, bool prepend)
{
  if (!mChildren.contains(layerable))
  {
    if (prepend)
      mChildren.prepend(layerable);
    else
      mChildren.append(layerable);
  } else
    qDebug() << Q_FUNC_INFO << "layerable is already child of this layer" << reinterpret_cast<quintptr>(layerable);
}

/*! \internal
  
  Removes the \a layerable from the list of this layer.
  
  This function does not change the \a mLayer member of \a layerable. (Use QCPLayerable::setLayer
  to change the layer of an object, not this function.)
  
  \see addChild
*/
void QCPLayer::removeChild(QCPLayerable *layerable)
{
  if (!mChildren.removeOne(layerable))
    qDebug() << Q_FUNC_INFO << "layerable is not child of this layer" << reinterpret_cast<quintptr>(layerable);
}


////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// QCPLayerable
////////////////////////////////////////////////////////////////////////////////////////////////////

/*! \class QCPLayerable
  \brief Base class for all objects that can be placed on layers
  
  This is the abstract base class most visible objects derive from, e.g. plottables, axes, grid
  etc.

  Every layerable is on a layer (QCPLayer) which allows controlling the rendering order by stacking
  the layers accordingly.
  
  For details about the layering mechanism, see the QCPLayer documentation.
*/

/* start documentation of pure virtual functions */

/*! \fn virtual void QCPLayerable::applyDefaultAntialiasingHint(QCPPainter *painter) const = 0
  \internal
  
  This function applies the default antialiasing setting to the specified \a painter, using the
  function \ref applyAntialiasingHint. This is the antialiasing state the painter is in, when \ref
  draw is called on the layerable. If the layerable has multiple entities whose antialiasing
  setting may be specified individually, this function should set the antialiasing state of the
  most prominent entity. In this case however, the \ref draw function usually calls the specialized
  versions of this function before drawing each entity, effectively overriding the setting of the
  default antialiasing hint.
  
  <b>First example:</b> QCPGraph has multiple entities that have an antialiasing setting: The graph
  line, fills, scatters and error bars. Those can be configured via QCPGraph::setAntialiased,
  QCPGraph::setAntialiasedFill, QCPGraph::setAntialiasedScatters etc. Consequently, there isn't
  only the QCPGraph::applyDefaultAntialiasingHint function (which corresponds to the graph line's
  antialiasing), but specialized ones like QCPGraph::applyFillAntialiasingHint and
  QCPGraph::applyScattersAntialiasingHint. So before drawing one of those entities, QCPGraph::draw
  calls the respective specialized applyAntialiasingHint function.
  
  <b>Second example:</b> QCPItemLine consists only of a line so there is only one antialiasing
  setting which can be controlled with QCPItemLine::setAntialiased. (This function is inherited by
  all layerables. The specialized functions, as seen on QCPGraph, must be added explicitly to the
  respective layerable subclass.) Consequently it only has the normal
  QCPItemLine::applyDefaultAntialiasingHint. The \ref QCPItemLine::draw function doesn't need to
  care about setting any antialiasing states, because the default antialiasing hint is already set
  on the painter when the \ref draw function is called, and that's the state it wants to draw the
  line with.
*/

/*! \fn virtual void QCPLayerable::draw(QCPPainter *painter) const = 0
  \internal
  
  This function draws the layerable to the specified \a painter.
  
  Before this function is called, the painter's antialiasing state is set via \ref
  applyDefaultAntialiasingHint, see the documentation there. Further, its clipping rectangle was
  set to \ref clipRect.
*/

/* end documentation of pure virtual functions */

/*!
  Creates a new QCPLayerable instance.
  
  Since QCPLayerable is an abstract base class, it can't be instantiated directly. Use one of the
  derived classes.
  
  If \a plot is provided, it automatically places itself on the layer named \a targetLayer. If \a
  targetLayer is an empty string, it places itself on the current layer of the plot (see \ref
  QCustomPlot::setCurrentLayer).
*/
QCPLayerable::QCPLayerable(QCustomPlot *plot, QString targetLayer) :
  mVisible(true),
  mParentPlot(plot),
  mLayer(0),
  mAntialiased(true)
{
  if (mParentPlot)
  {
    if (targetLayer.isEmpty())
      setLayer(mParentPlot->currentLayer());
    else if (!setLayer(targetLayer))
      qDebug() << Q_FUNC_INFO << "setting QCPlayerable initial layer to" << targetLayer << "failed.";
  }
}

QCPLayerable::~QCPLayerable()
{
  if (mLayer)
  {
    mLayer->removeChild(this);
    mLayer = 0;
  }
}

/*!
  Sets the visibility of this layerable object. If an object is not visible, it will not be drawn
  on the QCustomPlot surface, and user interaction with it (e.g. click/selection) is not possible.
*/
void QCPLayerable::setVisible(bool on)
{
  mVisible = on;
}

/*!
  Sets the \a layer of this layerable object. The object will be placed on top of the other objects
  already on \a layer.
  
  Returns true on success, i.e. if \a layer is a valid layer.
*/
bool QCPLayerable::setLayer(QCPLayer *layer)
{
  return moveToLayer(layer, false);
}

/*! \overload
  Sets the layer of this layerable object by name
  
  Returns true on success, i.e. if \a layerName is a valid layer name.
*/
bool QCPLayerable::setLayer(const QString &layerName)
{
  if (!mParentPlot)
  {
    qDebug() << Q_FUNC_INFO << "no parent QCustomPlot set";
    return false;
  }
  if (QCPLayer *layer = mParentPlot->layer(layerName))
  {
    return setLayer(layer);
  } else
  {
    qDebug() << Q_FUNC_INFO << "there is no layer with name" << layerName;
    return false;
  }
}

/*!
  Sets whether this object will be drawn antialiased or not.
  
  Note that antialiasing settings may be overridden by QCustomPlot::setAntialiasedElements and
  QCustomPlot::setNotAntialiasedElements.
*/
void QCPLayerable::setAntialiased(bool enabled)
{
  mAntialiased = enabled;
}

bool QCPLayerable::isAbove(QCPLayerable *other) const
{
  if (other->parentPlot() != mParentPlot)
  {
    qDebug() << Q_FUNC_INFO << "Other layerable is not in same QCustomPlot as this";
    return false;
  }
  
  if (layer() == other->layer())
  {
    const QList<QCPLayerable*> layerables = mLayer->children();
    for (int i=layerables.size()-1; i>=0; --i)
    {
      if (layerables.at(i) == this)
        return true;
      if (layerables.at(i) == other)
        return false;
    }
    qDebug() << Q_FUNC_INFO << "Neither this nor other layerable found in layer. Layering mechanism out of sync.";
    return false;
  } else
    return layer()->index() > other->layer()->index();
}

double QCPLayerable::selectTest(const QPointF &pos, bool onlySelectable, QVariant *details) const
{
  Q_UNUSED(pos)
  Q_UNUSED(onlySelectable)
  Q_UNUSED(details)
  return -1.0;
}

/*! \internal
  
  Moves this layerable object to \a layer. If \a prepend is true, this object will be prepended to
  the new layer's list, i.e. it will be drawn below the objects already on the layer. If it is
  false, the object will be appended.
  
  Returns true on success, i.e. if \a layer is a valid layer.
*/
bool QCPLayerable::moveToLayer(QCPLayer *layer, bool prepend)
{
  if (layer && !mParentPlot)
  {
    qDebug() << Q_FUNC_INFO << "no parent QCustomPlot set";
    return false;
  }
  if (layer && layer->parentPlot() != mParentPlot)
  {
    qDebug() << Q_FUNC_INFO << "layer" << layer->name() << "is not in same QCustomPlot as this layerable";
    return false;
  }
  if (layer == mLayer)
  {
    qDebug() << Q_FUNC_INFO << "layerable is already on layer" << reinterpret_cast<quintptr*>(layer);
    return true;
  }
  
  if (mLayer)
    mLayer->removeChild(this);
  mLayer = layer;
  if (mLayer)
    mLayer->addChild(this, prepend);
  return true;
}

/*! \internal

  Sets the QPainter::Antialiasing render hint on the provided \a painter, depending on the
  \a localAntialiased value as well as the overrides \ref QCustomPlot::setAntialiasedElements and
  \ref QCustomPlot::setNotAntialiasedElements. Which override enum this function takes into account is
  controlled via \a overrideElement.
*/
void QCPLayerable::applyAntialiasingHint(QCPPainter *painter, bool localAntialiased, QCP::AntialiasedElement overrideElement) const
{
  if (mParentPlot && mParentPlot->notAntialiasedElements().testFlag(overrideElement))
    painter->setAntialiasing(false);
  else if (mParentPlot && mParentPlot->antialiasedElements().testFlag(overrideElement))
    painter->setAntialiasing(true);
  else
    painter->setAntialiasing(localAntialiased);
}

QCP::Interaction QCPLayerable::selectionCategory() const
{
  return QCP::iSelectOther;
}

/*! \internal
  
  Returns the clipping rectangle of this layerable object. By default, this is the viewport of the parent QCustomPlot.
  Specific subclasses may reimplement this function to provide different clipping rects.
  
  The returned clipping rect is set on the painter before the draw function of the respective
  object is called.
*/
QRect QCPLayerable::clipRect() const
{
  if (mParentPlot)
    return mParentPlot->viewport();
  else
    return QRect();
}

void QCPLayerable::selectEvent(QMouseEvent *event, bool additive, const QVariant &details, bool *selectionStateChanged)
{
  Q_UNUSED(event)
  Q_UNUSED(additive)
  Q_UNUSED(details)
  Q_UNUSED(selectionStateChanged)
}

void QCPLayerable::deselectEvent(bool *selectionStateChanged)
{
  Q_UNUSED(selectionStateChanged)
}
