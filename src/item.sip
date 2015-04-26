
%ModuleCode
#include "qcustomplot.h"
%End

class QCPItemAnchor
{
%TypeHeaderCode
#include "qcustomplot.h"
%End

public:
  QCPItemAnchor(QCustomPlot *parentPlot, QCPAbstractItem *parentItem, const QString name, int anchorId=-1);
  virtual ~QCPItemAnchor();
  
  // getters:
  QString name() const;
  virtual QPointF pixelPoint() const;
  
protected:
  // property members:
//  QString mName;
  
  // non-property members:
// TODO(kca): Maybe create getters/setters in the original C++ code?
//  QCustomPlot *mParentPlot;
//  QCPAbstractItem *mParentItem;
//  int mAnchorId;
//  QSet<QCPItemPosition*> mChildrenX, mChildrenY;
  
  // introduced virtual methods:
  virtual QCPItemPosition *toQCPItemPosition();
  
  // non-virtual methods:
  void addChildX(QCPItemPosition* pos); // called from pos when this anchor is set as parent
  void removeChildX(QCPItemPosition *pos); // called from pos when its parent anchor is reset or pos deleted
  void addChildY(QCPItemPosition* pos); // called from pos when this anchor is set as parent
  void removeChildY(QCPItemPosition *pos); // called from pos when its parent anchor is reset or pos deleted
  
private:
//  Q_DISABLE_COPY(QCPItemAnchor)
  QCPItemAnchor(const QCPItemAnchor &);
  
//  friend class QCPItemPosition;
};



class QCPItemPosition : QCPItemAnchor
{
%TypeHeaderCode
#include "qcustomplot.h"
%End

public:
  enum PositionType { ptAbsolute        ///< Static positioning in pixels, starting from the top left corner of the viewport/widget.
                      ,ptViewportRatio  ///< Static positioning given by a fraction of the viewport size. For example, if you call setCoords(0, 0), the position will be at the top
                                        ///< left corner of the viewport/widget. setCoords(1, 1) will be at the bottom right corner, setCoords(0.5, 0) will be horizontally centered and
                                        ///< vertically at the top of the viewport/widget, etc.
                      ,ptAxisRectRatio  ///< Static positioning given by a fraction of the axis rect size (see \ref setAxisRect). For example, if you call setCoords(0, 0), the position will be at the top
                                        ///< left corner of the axis rect. setCoords(1, 1) will be at the bottom right corner, setCoords(0.5, 0) will be horizontally centered and
                                        ///< vertically at the top of the axis rect, etc. You can also go beyond the axis rect by providing negative coordinates or coordinates larger than 1.
                      ,ptPlotCoords     ///< Dynamic positioning at a plot coordinate defined by two axes (see \ref setAxes).
                    };
  
  QCPItemPosition(QCustomPlot *parentPlot, QCPAbstractItem *parentItem, const QString name);
  virtual ~QCPItemPosition();
  
  // getters:
  PositionType type() const;
  PositionType typeX() const;
  PositionType typeY() const;
  QCPItemAnchor *parentAnchor() const;
  QCPItemAnchor *parentAnchorX() const;
  QCPItemAnchor *parentAnchorY() const;
  double key() const;
  double value() const;
  QPointF coords() const;
  QCPAxis *keyAxis() const;
  QCPAxis *valueAxis() const;
  QCPAxisRect *axisRect() const;
  virtual QPointF pixelPoint() const;
  
  // setters:
  void setType(PositionType type);
  void setTypeX(PositionType type);
  void setTypeY(PositionType type);
  bool setParentAnchor(QCPItemAnchor *parentAnchor, bool keepPixelPosition=false);
  bool setParentAnchorX(QCPItemAnchor *parentAnchor, bool keepPixelPosition=false);
  bool setParentAnchorY(QCPItemAnchor *parentAnchor, bool keepPixelPosition=false);
  void setCoords(double key, double value);
  void setCoords(const QPointF &coords);
  void setAxes(QCPAxis* keyAxis, QCPAxis* valueAxis);
  void setAxisRect(QCPAxisRect *axisRect);
  void setPixelPoint(const QPointF &pixelPoint);
  
protected:
  // property members:
//  PositionType mPositionTypeX, mPositionTypeY;
//  QPointer<QCPAxis> mKeyAxis, mValueAxis;
//  QPointer<QCPAxisRect> mAxisRect;
//  double mKey, mValue;
//  QCPItemAnchor *mParentAnchorX, *mParentAnchorY;
  
  // reimplemented virtual methods:
  virtual QCPItemPosition *toQCPItemPosition();
  
private:
//  Q_DISABLE_COPY(QCPItemPosition)
  QCPItemPosition(const QCPItemPosition &);
  
};


class QCPAbstractItem : QCPLayerable /Abstract/
{
%TypeHeaderCode
#include "qcustomplot.h"
%End

public:
  QCPAbstractItem(QCustomPlot *parentPlot);
  virtual ~QCPAbstractItem();
  
  // getters:
  bool clipToAxisRect() const;
  QCPAxisRect *clipAxisRect() const;
  bool selectable() const;
  bool selected() const;
  
  // setters:
  void setClipToAxisRect(bool clip);
  void setClipAxisRect(QCPAxisRect *rect);
  Q_SLOT void setSelectable(bool selectable);
  Q_SLOT void setSelected(bool selected);
  
  // reimplemented virtual methods:
  virtual double selectTest(const QPointF &pos, bool onlySelectable, QVariant *details=0) const = 0;
  
  // non-virtual methods:
  QList<QCPItemPosition*> positions() const;
  QList<QCPItemAnchor*> anchors() const;
  QCPItemPosition *position(const QString &name) const;
  QCPItemAnchor *anchor(const QString &name) const;
  bool hasAnchor(const QString &name) const;
  
signals:
  void selectionChanged(bool selected);
  void selectableChanged(bool selectable);
  
protected:
  // property members:
//  bool mClipToAxisRect;
//  QPointer<QCPAxisRect> mClipAxisRect;
//  QList<QCPItemPosition*> mPositions;
//  QList<QCPItemAnchor*> mAnchors;
//  bool mSelectable, mSelected;
  
  // reimplemented virtual methods:
  virtual QCP::Interaction selectionCategory() const;
  virtual QRect clipRect() const;
  virtual void applyDefaultAntialiasingHint(QCPPainter *painter) const;
  virtual void draw(QCPPainter *painter) = 0;
  // events:
  virtual void selectEvent(QMouseEvent *event, bool additive, const QVariant &details, bool *selectionStateChanged);
  virtual void deselectEvent(bool *selectionStateChanged);
  
  // introduced virtual methods:
  virtual QPointF anchorPixelPoint(int anchorId) const;
  
  // non-virtual methods:
  double distSqrToLine(const QPointF &start, const QPointF &end, const QPointF &point) const;
  double rectSelectTest(const QRectF &rect, const QPointF &pos, bool filledRect) const;
  QCPItemPosition *createPosition(const QString &name);
  QCPItemAnchor *createAnchor(const QString &name, int anchorId);
  
//private:
//  Q_DISABLE_COPY(QCPAbstractItem)
//  QCPAbstractItem(const QCPAbstractItem &);
  
//  friend class QCustomPlot;
//  friend class QCPItemAnchor;
};

