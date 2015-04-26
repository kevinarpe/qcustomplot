
%ModuleCode
#include "qcustomplot.h"
%End

class QCPData
{
%TypeHeaderCode
#include "qcustomplot.h"
%End

public:
  QCPData();
  QCPData(double key, double value);
//  double key, value;
  double key;
  double value;
//  double keyErrorPlus, keyErrorMinus;
  double keyErrorPlus;
  double keyErrorMinus;
//  double valueErrorPlus, valueErrorMinus;
  double valueErrorPlus;
  double valueErrorMinus;
};
//Q_DECLARE_TYPEINFO(QCPData, Q_MOVABLE_TYPE);

typedef QMap<double, QCPData> QCPDataMap;
typedef QMapIterator<double, QCPData> QCPDataMapIterator;
typedef QMutableMapIterator<double, QCPData> QCPDataMutableMapIterator;


class QCPGraph : QCPAbstractPlottable
{
%TypeHeaderCode
#include "qcustomplot.h"
%End

public:
  enum LineStyle { lsNone        ///< data points are not connected with any lines (e.g. data only represented
                                 ///< with symbols according to the scatter style, see \ref setScatterStyle)
                   ,lsLine       ///< data points are connected by a straight line
                   ,lsStepLeft   ///< line is drawn as steps where the step height is the value of the left data point
                   ,lsStepRight  ///< line is drawn as steps where the step height is the value of the right data point
                   ,lsStepCenter ///< line is drawn as steps where the step is in between two data points
                   ,lsImpulse    ///< each data point is represented by a line parallel to the value axis, which reaches from the data point to the zero-value-line
                 };
//  Q_ENUMS(LineStyle)
  enum ErrorType { etNone   ///< No error bars are shown
                   ,etKey   ///< Error bars for the key dimension of the data point are shown
                   ,etValue ///< Error bars for the value dimension of the data point are shown
                   ,etBoth  ///< Error bars for both key and value dimensions of the data point are shown
                 };
//  Q_ENUMS(ErrorType)
  
  explicit QCPGraph(QCPAxis *keyAxis, QCPAxis *valueAxis);
  virtual ~QCPGraph();
  
  // getters:
// TODO: unsupported function return type - provide %MethodCode and a C++ signature
//  QCPDataMap *data() const;
  LineStyle lineStyle() const;
  QCPScatterStyle scatterStyle() const;
  ErrorType errorType() const;
  QPen errorPen() const;
  double errorBarSize() const;
  bool errorBarSkipSymbol() const;
  QCPGraph *channelFillGraph() const;
  bool adaptiveSampling() const;
  
  // setters:
// TODO: unsupported function argument type - provide %MethodCode and a C++ signature
//  void setData(QCPDataMap *data, bool copy=false);
  void setData(const QVector<double> &key, const QVector<double> &value);
  void setDataKeyError(const QVector<double> &key, const QVector<double> &value, const QVector<double> &keyError);
  void setDataKeyError(const QVector<double> &key, const QVector<double> &value, const QVector<double> &keyErrorMinus, const QVector<double> &keyErrorPlus);
  void setDataValueError(const QVector<double> &key, const QVector<double> &value, const QVector<double> &valueError);
  void setDataValueError(const QVector<double> &key, const QVector<double> &value, const QVector<double> &valueErrorMinus, const QVector<double> &valueErrorPlus);
  void setDataBothError(const QVector<double> &key, const QVector<double> &value, const QVector<double> &keyError, const QVector<double> &valueError);
  void setDataBothError(const QVector<double> &key, const QVector<double> &value, const QVector<double> &keyErrorMinus, const QVector<double> &keyErrorPlus, const QVector<double> &valueErrorMinus, const QVector<double> &valueErrorPlus);
  void setLineStyle(LineStyle ls);
  void setScatterStyle(const QCPScatterStyle &style);
  void setErrorType(ErrorType errorType);
  void setErrorPen(const QPen &pen);
  void setErrorBarSize(double size);
  void setErrorBarSkipSymbol(bool enabled);
  void setChannelFillGraph(QCPGraph *targetGraph);
  void setAdaptiveSampling(bool enabled);
  
  // non-property methods:
// TODO: unsupported function argument type - provide %MethodCode and a C++ signature
//  void addData(const QCPDataMap &dataMap);
  void addData(const QCPData &data);
  void addData(double key, double value);
  void addData(const QVector<double> &keys, const QVector<double> &values);
  void removeDataBefore(double key);
  void removeDataAfter(double key);
  void removeData(double fromKey, double toKey);
  void removeData(double key);
  
  // reimplemented virtual methods:
  virtual void clearData();
  virtual double selectTest(const QPointF &pos, bool onlySelectable, QVariant *details=0) const;
//  using QCPAbstractPlottable::rescaleAxes;
//  using QCPAbstractPlottable::rescaleKeyAxis;
//  using QCPAbstractPlottable::rescaleValueAxis;
  void rescaleAxes(bool onlyEnlarge, bool includeErrorBars) const; // overloads base class interface
  void rescaleKeyAxis(bool onlyEnlarge, bool includeErrorBars) const; // overloads base class interface
  void rescaleValueAxis(bool onlyEnlarge, bool includeErrorBars) const; // overloads base class interface
  
protected:
  // property members:
//  QCPDataMap *mData;
//  QPen mErrorPen;
//  LineStyle mLineStyle;
//  QCPScatterStyle mScatterStyle;
//  ErrorType mErrorType;
//  double mErrorBarSize;
//  bool mErrorBarSkipSymbol;
//  QPointer<QCPGraph> mChannelFillGraph;
//  bool mAdaptiveSampling;
  
  // reimplemented virtual methods:
  virtual void draw(QCPPainter *painter);
  virtual void drawLegendIcon(QCPPainter *painter, const QRectF &rect) const;
// TODO: error: ‘QCPAbstractPlottable::SignDomain sdBoth’ is protected
//  virtual QCPRange getKeyRange(bool &foundRange, SignDomain inSignDomain=sdBoth) const;
  virtual QCPRange getKeyRange(bool &foundRange, SignDomain inSignDomain) const;
// TODO: error: ‘QCPAbstractPlottable::SignDomain sdBoth’ is protected
//  virtual QCPRange getValueRange(bool &foundRange, SignDomain inSignDomain=sdBoth) const;
  virtual QCPRange getValueRange(bool &foundRange, SignDomain inSignDomain) const;
  virtual QCPRange getKeyRange(bool &foundRange, SignDomain inSignDomain, bool includeErrors) const; // overloads base class interface
  virtual QCPRange getValueRange(bool &foundRange, SignDomain inSignDomain, bool includeErrors) const; // overloads base class interface
  
  // introduced virtual methods:
  virtual void drawFill(QCPPainter *painter, QVector<QPointF> *lineData) const;
  virtual void drawScatterPlot(QCPPainter *painter, QVector<QCPData> *scatterData) const;
  virtual void drawLinePlot(QCPPainter *painter, QVector<QPointF> *lineData) const;
  virtual void drawImpulsePlot(QCPPainter *painter, QVector<QPointF> *lineData) const;
  
  // non-virtual methods:
  void getPreparedData(QVector<QCPData> *lineData, QVector<QCPData> *scatterData) const;
  void getPlotData(QVector<QPointF> *lineData, QVector<QCPData> *scatterData) const;
  void getScatterPlotData(QVector<QCPData> *scatterData) const;
  void getLinePlotData(QVector<QPointF> *linePixelData, QVector<QCPData> *scatterData) const;
  void getStepLeftPlotData(QVector<QPointF> *linePixelData, QVector<QCPData> *scatterData) const;
  void getStepRightPlotData(QVector<QPointF> *linePixelData, QVector<QCPData> *scatterData) const;
  void getStepCenterPlotData(QVector<QPointF> *linePixelData, QVector<QCPData> *scatterData) const;
  void getImpulsePlotData(QVector<QPointF> *linePixelData, QVector<QCPData> *scatterData) const;
  void drawError(QCPPainter *painter, double x, double y, const QCPData &data) const;
// TODO: QCPDataMap::const_iterator is undefined
//  void getVisibleDataBounds(QCPDataMap::const_iterator &lower, QCPDataMap::const_iterator &upper) const;
// TODO: QCPDataMap::const_iterator is undefined
//  int countDataInBounds(const QCPDataMap::const_iterator &lower, const QCPDataMap::const_iterator &upper, int maxCount) const;
  void addFillBasePoints(QVector<QPointF> *lineData) const;
  void removeFillBasePoints(QVector<QPointF> *lineData) const;
  QPointF lowerFillBasePoint(double lowerKey) const;
  QPointF upperFillBasePoint(double upperKey) const;
  const QPolygonF getChannelFillPolygon(const QVector<QPointF> *lineData) const;
  int findIndexBelowX(const QVector<QPointF> *data, double x) const;
  int findIndexAboveX(const QVector<QPointF> *data, double x) const;
  int findIndexBelowY(const QVector<QPointF> *data, double y) const;
  int findIndexAboveY(const QVector<QPointF> *data, double y) const;
  double pointDistance(const QPointF &pixelPoint) const;
  
//  friend class QCustomPlot;
//  friend class QCPLegend;
};

