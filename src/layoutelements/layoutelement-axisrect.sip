
%ModuleCode
#include "qcustomplot.h"
%End

class QCPAxisRect : QCPLayoutElement
{
%TypeHeaderCode
#include "qcustomplot.h"
%End

public:
  explicit QCPAxisRect(QCustomPlot *parentPlot, bool setupDefaultAxes=true);
  virtual ~QCPAxisRect();
  
  // getters:
  QPixmap background() const;
  bool backgroundScaled() const;
  Qt::AspectRatioMode backgroundScaledMode() const;
  Qt::Orientations rangeDrag() const;
  Qt::Orientations rangeZoom() const;
  QCPAxis *rangeDragAxis(Qt::Orientation orientation);
  QCPAxis *rangeZoomAxis(Qt::Orientation orientation);
  double rangeZoomFactor(Qt::Orientation orientation);
  
  // setters:
  void setBackground(const QPixmap &pm);
  void setBackground(const QPixmap &pm, bool scaled, Qt::AspectRatioMode mode=Qt::KeepAspectRatioByExpanding);
  void setBackground(const QBrush &brush);
  void setBackgroundScaled(bool scaled);
  void setBackgroundScaledMode(Qt::AspectRatioMode mode);
  void setRangeDrag(Qt::Orientations orientations);
  void setRangeZoom(Qt::Orientations orientations);
  void setRangeDragAxes(QCPAxis *horizontal, QCPAxis *vertical);
  void setRangeZoomAxes(QCPAxis *horizontal, QCPAxis *vertical);
  void setRangeZoomFactor(double horizontalFactor, double verticalFactor);
  void setRangeZoomFactor(double factor);
  
  // non-property methods:
  int axisCount(QCPAxis::AxisType type) const;
  QCPAxis *axis(QCPAxis::AxisType type, int index=0) const;
  QList<QCPAxis*> axes(QCPAxis::AxisTypes types) const;
  QList<QCPAxis*> axes() const;
  QCPAxis *addAxis(QCPAxis::AxisType type, QCPAxis *axis=0);
  QList<QCPAxis*> addAxes(QCPAxis::AxisTypes types);
  bool removeAxis(QCPAxis *axis);
  QCPLayoutInset *insetLayout() const;
  
  void setupFullAxesBox(bool connectRanges=false);
  QList<QCPAbstractPlottable*> plottables() const;
  QList<QCPGraph*> graphs() const;
  QList<QCPAbstractItem*> items() const;
  
  // read-only interface imitating a QRect:
  int left() const;
  int right() const;
  int top() const;
  int bottom() const;
  int width() const;
  int height() const;
  QSize size() const;
  QPoint topLeft() const;
  QPoint topRight() const;
  QPoint bottomLeft() const;
  QPoint bottomRight() const;
  QPoint center() const;
  
  // reimplemented virtual methods:
  virtual void update(UpdatePhase phase);
  virtual QList<QCPLayoutElement*> elements(bool recursive) const;

protected:
  // property members:
//  QBrush mBackgroundBrush;
//  QPixmap mBackgroundPixmap;
//  QPixmap mScaledBackgroundPixmap;
//  bool mBackgroundScaled;
//  Qt::AspectRatioMode mBackgroundScaledMode;
//  QCPLayoutInset *mInsetLayout;
//  Qt::Orientations mRangeDrag, mRangeZoom;
//  QPointer<QCPAxis> mRangeDragHorzAxis, mRangeDragVertAxis, mRangeZoomHorzAxis, mRangeZoomVertAxis;
//  double mRangeZoomFactorHorz, mRangeZoomFactorVert;
  // non-property members:
//  QCPRange mDragStartHorzRange, mDragStartVertRange;
//  QCP::AntialiasedElements mAADragBackup, mNotAADragBackup;
//  QPoint mDragStart;
//  bool mDragging;
//  QHash<QCPAxis::AxisType, QList<QCPAxis*> > mAxes;
  
  // reimplemented virtual methods:
  virtual void applyDefaultAntialiasingHint(QCPPainter *painter) const;
  virtual void draw(QCPPainter *painter);
  virtual int calculateAutoMargin(QCP::MarginSide side);
  // events:
  virtual void mousePressEvent(QMouseEvent *event);
  virtual void mouseMoveEvent(QMouseEvent *event);
  virtual void mouseReleaseEvent(QMouseEvent *event);
  virtual void wheelEvent(QWheelEvent *event);
  
  // non-property methods:
  void drawBackground(QCPPainter *painter);
  void updateAxesOffset(QCPAxis::AxisType type);
  
//private:
//  Q_DISABLE_COPY(QCPAxisRect)
//  QCPAxisRect(const QCPAxisRect &);
  
//  friend class QCustomPlot;
};

