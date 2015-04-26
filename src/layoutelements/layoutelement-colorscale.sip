
%ModuleCode
#include "qcustomplot.h"
%End

class QCPColorScaleAxisRectPrivate : QCPAxisRect
{
%TypeHeaderCode
#include "qcustomplot.h"
%End

public:
  explicit QCPColorScaleAxisRectPrivate(QCPColorScale *parentColorScale);
protected:
//  QCPColorScale *mParentColorScale;
//  QImage mGradientImage;
//  bool mGradientImageInvalidated;
  // re-using some methods of QCPAxisRect to make them available to friend class QCPColorScale
//  using QCPAxisRect::calculateAutoMargin;
//  using QCPAxisRect::mousePressEvent;
//  using QCPAxisRect::mouseMoveEvent;
//  using QCPAxisRect::mouseReleaseEvent;
//  using QCPAxisRect::wheelEvent;
//  using QCPAxisRect::update;
  virtual void draw(QCPPainter *painter);
  void updateGradientImage();
  Q_SLOT void axisSelectionChanged(QCPAxis::SelectableParts selectedParts);
  Q_SLOT void axisSelectableChanged(QCPAxis::SelectableParts selectableParts);
//  friend class QCPColorScale;
};


class QCPColorScale : QCPLayoutElement
{
%TypeHeaderCode
#include "qcustomplot.h"
%End

public:
  explicit QCPColorScale(QCustomPlot *parentPlot);
  virtual ~QCPColorScale();
  
  // getters:
  QCPAxis *axis() const;
  QCPAxis::AxisType type() const;
  QCPRange dataRange() const;
  QCPAxis::ScaleType dataScaleType() const;
  QCPColorGradient gradient() const;
  QString label() const;
  int barWidth () const;
  bool rangeDrag() const;
  bool rangeZoom() const;
  
  // setters:
  void setType(QCPAxis::AxisType type);
  Q_SLOT void setDataRange(const QCPRange &dataRange);
  Q_SLOT void setDataScaleType(QCPAxis::ScaleType scaleType);
  Q_SLOT void setGradient(const QCPColorGradient &gradient);
  void setLabel(const QString &str);
  void setBarWidth(int width);
  void setRangeDrag(bool enabled);
  void setRangeZoom(bool enabled);
  
  // non-property methods:
  QList<QCPColorMap*> colorMaps() const;
  void rescaleDataRange(bool onlyVisibleMaps);
  
  // reimplemented virtual methods:
  virtual void update(UpdatePhase phase);
  
signals:
  void dataRangeChanged(QCPRange newRange);
  void dataScaleTypeChanged(QCPAxis::ScaleType scaleType);
  void gradientChanged(QCPColorGradient newGradient);

protected:
  // property members:
//  QCPAxis::AxisType mType;
//  QCPRange mDataRange;
//  QCPAxis::ScaleType mDataScaleType;
//  QCPColorGradient mGradient;
//  int mBarWidth;
  
  // non-property members:
//  QPointer<QCPColorScaleAxisRectPrivate> mAxisRect;
//  QPointer<QCPAxis> mColorAxis;
  
  // reimplemented virtual methods:
  virtual void applyDefaultAntialiasingHint(QCPPainter *painter) const;
  // events:
  virtual void mousePressEvent(QMouseEvent *event);
  virtual void mouseMoveEvent(QMouseEvent *event);
  virtual void mouseReleaseEvent(QMouseEvent *event);
  virtual void wheelEvent(QWheelEvent *event);
  
//private:
//  Q_DISABLE_COPY(QCPColorScale)
//  QCPColorScale(const QCPColorScale &);
  
//  friend class QCPColorScaleAxisRectPrivate;
};

