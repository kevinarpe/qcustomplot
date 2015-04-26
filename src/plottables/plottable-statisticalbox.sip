
%ModuleCode
#include "qcustomplot.h"
%End

class QCPStatisticalBox : QCPAbstractPlottable
{
%TypeHeaderCode
#include "qcustomplot.h"
%End

public:
  explicit QCPStatisticalBox(QCPAxis *keyAxis, QCPAxis *valueAxis);
  
  // getters:
  double key() const;
  double minimum() const;
  double lowerQuartile() const;
  double median() const;
  double upperQuartile() const;
  double maximum() const;
  QVector<double> outliers() const;
  double width() const;
  double whiskerWidth() const;
  QPen whiskerPen() const;
  QPen whiskerBarPen() const;
  QPen medianPen() const;
  QCPScatterStyle outlierStyle() const;

  // setters:
  void setKey(double key);
  void setMinimum(double value);
  void setLowerQuartile(double value);
  void setMedian(double value);
  void setUpperQuartile(double value);
  void setMaximum(double value);
  void setOutliers(const QVector<double> &values);
  void setData(double key, double minimum, double lowerQuartile, double median, double upperQuartile, double maximum);
  void setWidth(double width);
  void setWhiskerWidth(double width);
  void setWhiskerPen(const QPen &pen);
  void setWhiskerBarPen(const QPen &pen);
  void setMedianPen(const QPen &pen);
  void setOutlierStyle(const QCPScatterStyle &style);
  
  // non-property methods:
  virtual void clearData();
  virtual double selectTest(const QPointF &pos, bool onlySelectable, QVariant *details=0) const;
  
protected:
  // property members:
//  QVector<double> mOutliers;
//  double mKey, mMinimum, mLowerQuartile, mMedian, mUpperQuartile, mMaximum;
//  double mWidth;
//  double mWhiskerWidth;
//  QPen mWhiskerPen, mWhiskerBarPen, mMedianPen;
//  QCPScatterStyle mOutlierStyle;
  
  // reimplemented virtual methods:
  virtual void draw(QCPPainter *painter);
  virtual void drawLegendIcon(QCPPainter *painter, const QRectF &rect) const;
// TODO: error: ‘QCPAbstractPlottable::SignDomain sdBoth’ is protected
//  virtual QCPRange getKeyRange(bool &foundRange, SignDomain inSignDomain=sdBoth) const;
  virtual QCPRange getKeyRange(bool &foundRange, SignDomain inSignDomain) const;
// TODO: error: ‘QCPAbstractPlottable::SignDomain sdBoth’ is protected
//  virtual QCPRange getValueRange(bool &foundRange, SignDomain inSignDomain=sdBoth) const;
  virtual QCPRange getValueRange(bool &foundRange, SignDomain inSignDomain) const;
  
  // introduced virtual methods:
  virtual void drawQuartileBox(QCPPainter *painter, QRectF *quartileBox=0) const;
  virtual void drawMedian(QCPPainter *painter) const;
  virtual void drawWhiskers(QCPPainter *painter) const;
  virtual void drawOutliers(QCPPainter *painter) const;
  
//  friend class QCustomPlot;
//  friend class QCPLegend;
};

