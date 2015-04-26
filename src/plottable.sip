
%ModuleCode
#include "qcustomplot.h"
%End

class QCPAbstractPlottable : QCPLayerable /Abstract/
{
%TypeHeaderCode
#include "qcustomplot.h"
%End

public:
  QCPAbstractPlottable(QCPAxis *keyAxis, QCPAxis *valueAxis);
  
  // getters:
  QString name() const;
  bool antialiasedFill() const;
  bool antialiasedScatters() const;
  bool antialiasedErrorBars() const;
  QPen pen() const;
  QPen selectedPen() const;
  QBrush brush() const;
  QBrush selectedBrush() const;
  QCPAxis *keyAxis() const;
  QCPAxis *valueAxis() const;
  bool selectable() const;
  bool selected() const;
  
  // setters:
  void setName(const QString &name);
  void setAntialiasedFill(bool enabled);
  void setAntialiasedScatters(bool enabled);
  void setAntialiasedErrorBars(bool enabled);
  void setPen(const QPen &pen);
  void setSelectedPen(const QPen &pen);
  void setBrush(const QBrush &brush);
  void setSelectedBrush(const QBrush &brush);
  void setKeyAxis(QCPAxis *axis);
  void setValueAxis(QCPAxis *axis);
  Q_SLOT void setSelectable(bool selectable);
  Q_SLOT void setSelected(bool selected);

  // introduced virtual methods:
  virtual void clearData() = 0;
  virtual double selectTest(const QPointF &pos, bool onlySelectable, QVariant *details=0) const = 0;
  virtual bool addToLegend();
  virtual bool removeFromLegend() const;
  
  // non-property methods:
  void rescaleAxes(bool onlyEnlarge=false) const;
  void rescaleKeyAxis(bool onlyEnlarge=false) const;
  void rescaleValueAxis(bool onlyEnlarge=false) const;
  
signals:
  void selectionChanged(bool selected);
  void selectableChanged(bool selectable);
  
protected:
  enum SignDomain { sdNegative  ///< The negative sign domain, i.e. numbers smaller than zero
                    ,sdBoth     ///< Both sign domains, including zero, i.e. all (rational) numbers
                    ,sdPositive ///< The positive sign domain, i.e. numbers greater than zero
                  };
  
  // property members:
//  QString mName;
//  bool mAntialiasedFill, mAntialiasedScatters, mAntialiasedErrorBars;
//  QPen mPen, mSelectedPen;
//  QBrush mBrush, mSelectedBrush;
//  QPointer<QCPAxis> mKeyAxis, mValueAxis;
//  bool mSelectable, mSelected;
  
  // reimplemented virtual methods:
  virtual QRect clipRect() const;
  virtual void draw(QCPPainter *painter) = 0;
  virtual QCP::Interaction selectionCategory() const;
  void applyDefaultAntialiasingHint(QCPPainter *painter) const;
  // events:
  virtual void selectEvent(QMouseEvent *event, bool additive, const QVariant &details, bool *selectionStateChanged);
  virtual void deselectEvent(bool *selectionStateChanged);
  
  // introduced virtual methods:
  virtual void drawLegendIcon(QCPPainter *painter, const QRectF &rect) const = 0;
// TODO: error: ‘QCPAbstractPlottable::SignDomain sdBoth’ is protected
//  virtual QCPRange getKeyRange(bool &foundRange, SignDomain inSignDomain=sdBoth) const = 0;
  virtual QCPRange getKeyRange(bool &foundRange, SignDomain inSignDomain) const = 0;
// TODO: error: ‘QCPAbstractPlottable::SignDomain sdBoth’ is protected
//  virtual QCPRange getValueRange(bool &foundRange, SignDomain inSignDomain=sdBoth) const = 0;
  virtual QCPRange getValueRange(bool &foundRange, SignDomain inSignDomain) const = 0;
  
  // non-virtual methods:
// TODO: has overloaded functions with the same Python signature
//  void coordsToPixels(double key, double value, double &x, double &y) const;
  const QPointF coordsToPixels(double key, double value) const;
  void pixelsToCoords(double x, double y, double &key, double &value) const;
  void pixelsToCoords(const QPointF &pixelPos, double &key, double &value) const;
  QPen mainPen() const;
  QBrush mainBrush() const;
  void applyFillAntialiasingHint(QCPPainter *painter) const;
  void applyScattersAntialiasingHint(QCPPainter *painter) const;
  void applyErrorBarsAntialiasingHint(QCPPainter *painter) const;
  double distSqrToLine(const QPointF &start, const QPointF &end, const QPointF &point) const;

//private:
//  Q_DISABLE_COPY(QCPAbstractPlottable)
//  QCPAbstractPlottable(const QCPAbstractPlottable &);
  
//  friend class QCustomPlot;
//  friend class QCPAxis;
//  friend class QCPPlottableLegendItem;
};

