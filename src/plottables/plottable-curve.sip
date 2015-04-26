
%ModuleCode
#include "qcustomplot.h"
%End

class QCPCurveData
{
%TypeHeaderCode
#include "qcustomplot.h"
%End

public:
  QCPCurveData();
  QCPCurveData(double t, double key, double value);
//  double t, key, value;
  double t;
  double key;
  double value;
};
//Q_DECLARE_TYPEINFO(QCPCurveData, Q_MOVABLE_TYPE);


typedef QMap<double, QCPCurveData> QCPCurveDataMap;
typedef QMapIterator<double, QCPCurveData> QCPCurveDataMapIterator;
typedef QMutableMapIterator<double, QCPCurveData> QCPCurveDataMutableMapIterator;


class QCPCurve : QCPAbstractPlottable
{
%TypeHeaderCode
#include "qcustomplot.h"
%End

public:
  enum LineStyle { lsNone  ///< No line is drawn between data points (e.g. only scatters)
                   ,lsLine ///< Data points are connected with a straight line
                 };
  explicit QCPCurve(QCPAxis *keyAxis, QCPAxis *valueAxis);
  virtual ~QCPCurve();
  
  // getters:
// TODO: unsupported function return type - provide %MethodCode and a C++ signature
//  QCPCurveDataMap *data() const;
  QCPScatterStyle scatterStyle() const;
  LineStyle lineStyle() const;
  
  // setters:
// TODO: unsupported function argument type - provide %MethodCode and a C++ signature
//  void setData(QCPCurveDataMap *data, bool copy=false);
  void setData(const QVector<double> &t, const QVector<double> &key, const QVector<double> &value);
  void setData(const QVector<double> &key, const QVector<double> &value);
  void setScatterStyle(const QCPScatterStyle &style);
  void setLineStyle(LineStyle style);
  
  // non-property methods:
// TODO: unsupported function argument type - provide %MethodCode and a C++ signature
//  void addData(const QCPCurveDataMap &dataMap);
  void addData(const QCPCurveData &data);
  void addData(double t, double key, double value);
  void addData(double key, double value);
  void addData(const QVector<double> &ts, const QVector<double> &keys, const QVector<double> &values);
  void removeDataBefore(double t);
  void removeDataAfter(double t);
  void removeData(double fromt, double tot);
  void removeData(double t);
  
  // reimplemented virtual methods:
  virtual void clearData();
  virtual double selectTest(const QPointF &pos, bool onlySelectable, QVariant *details=0) const;
  
protected:
  // property members:
//  QCPCurveDataMap *mData;
//  QCPScatterStyle mScatterStyle;
//  LineStyle mLineStyle;
  
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
  virtual void drawScatterPlot(QCPPainter *painter, const QVector<QPointF> *pointData) const;
  
  // non-virtual methods:
  void getCurveData(QVector<QPointF> *lineData) const;
  int getRegion(double x, double y, double rectLeft, double rectTop, double rectRight, double rectBottom) const;
  QPointF getOptimizedPoint(int prevRegion, double prevKey, double prevValue, double key, double value, double rectLeft, double rectTop, double rectRight, double rectBottom) const;
  QVector<QPointF> getOptimizedCornerPoints(int prevRegion, int currentRegion, double prevKey, double prevValue, double key, double value, double rectLeft, double rectTop, double rectRight, double rectBottom) const;
  bool mayTraverse(int prevRegion, int currentRegion) const;
  bool getTraverse(double prevKey, double prevValue, double key, double value, double rectLeft, double rectTop, double rectRight, double rectBottom, QPointF &crossA, QPointF &crossB) const;
  void getTraverseCornerPoints(int prevRegion, int currentRegion, double rectLeft, double rectTop, double rectRight, double rectBottom, QVector<QPointF> &beforeTraverse, QVector<QPointF> &afterTraverse) const;
  double pointDistance(const QPointF &pixelPoint) const;
  
//  friend class QCustomPlot;
//  friend class QCPLegend;
};

