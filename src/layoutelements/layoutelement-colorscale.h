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
**             Date: 09.12.13                                             **
**          Version: 1.1.1                                                **
****************************************************************************/

#ifndef QCP_LAYOUTELEMENT_COLORSCALE_H
#define QCP_LAYOUTELEMENT_COLORSCALE_H

#include "../global.h"
#include "../axis.h"
#include "../layout.h"

class QCPPainter;
class QCustomPlot;
class QCPColorMap;

class QCP_LIB_DECL QCPColorScale : public QCPLayoutElement
{
  Q_OBJECT
  /// \cond INCLUDE_QPROPERTIES
// TODO
  /// \endcond
public:
  explicit QCPColorScale(QCustomPlot *parentPlot);
  virtual ~QCPColorScale();
  
  // getters:

  
  // setters:
  
  
  // non-property methods:
  
  
  // reimplemented virtual methods:
  virtual void update();

protected:
  // property members:

  // non-property members:

  
  // reimplemented virtual methods:
  virtual void applyDefaultAntialiasingHint(QCPPainter *painter) const;
  virtual void draw(QCPPainter *painter);
  virtual int calculateAutoMargin(QCP::MarginSide side);
  // events:
  virtual void mousePressEvent(QMouseEvent *event);
  virtual void mouseMoveEvent(QMouseEvent *event);
  virtual void mouseReleaseEvent(QMouseEvent *event);
  virtual void wheelEvent(QWheelEvent *event);

  
private:
  Q_DISABLE_COPY(QCPColorScale)
};


#endif // QCP_LAYOUTELEMENT_COLORSCALE_H
