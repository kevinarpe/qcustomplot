
%ModuleCode
#include "qcustomplot.h"
%End

class QCPFinancialData
{
%TypeHeaderCode
#include "qcustomplot.h"
%End

public:
  QCPFinancialData();
  QCPFinancialData(double key, double open, double high, double low, double close);
//  double key, open, high, low, close;
  double key;
  double open;
  double high;
  double low;
  double close;
};
//Q_DECLARE_TYPEINFO(QCPFinancialData, Q_MOVABLE_TYPE);

typedef QMap<double, QCPFinancialData> QCPFinancialDataMap;
typedef QMapIterator<double, QCPFinancialData> QCPFinancialDataMapIterator;
typedef QMutableMapIterator<double, QCPFinancialData> QCPFinancialDataMutableMapIterator;


class QCPFinancial : QCPAbstractPlottable
{
%TypeHeaderCode
#include "qcustomplot.h"
%End

public:
  enum ChartStyle { csOhlc         ///< Open-High-Low-Close bar representation
                   ,csCandlestick  ///< Candlestick representation
                  };
//  Q_ENUMS(ChartStyle)
  
  explicit QCPFinancial(QCPAxis *keyAxis, QCPAxis *valueAxis);
  virtual ~QCPFinancial();
  
  // getters:
// TODO: unsupported function return type - provide %MethodCode and a C++ signature
//  QCPFinancialDataMap *data() const;
  ChartStyle chartStyle() const;
  double width() const;
  bool twoColored() const;
  QBrush brushPositive() const;
  QBrush brushNegative() const;
  QPen penPositive() const;
  QPen penNegative() const;
  
  
  // setters:
// TODO: unsupported function argument type - provide %MethodCode and a C++ signature
//  void setData(QCPFinancialDataMap *data, bool copy=false);
  void setData(const QVector<double> &key, const QVector<double> &open, const QVector<double> &high, const QVector<double> &low, const QVector<double> &close);
  void setChartStyle(ChartStyle style);
  void setWidth(double width);
  void setTwoColored(bool twoColored);
  void setBrushPositive(const QBrush &brush);
  void setBrushNegative(const QBrush &brush);
  void setPenPositive(const QPen &pen);
  void setPenNegative(const QPen &pen);
  
  // non-property methods:
// TODO: unsupported function argument type - provide %MethodCode and a C++ signature
//  void addData(const QCPFinancialDataMap &dataMap);
  void addData(const QCPFinancialData &data);
  void addData(double key, double open, double high, double low, double close);
  void addData(const QVector<double> &key, const QVector<double> &open, const QVector<double> &high, const QVector<double> &low, const QVector<double> &close);
  void removeDataBefore(double key);
  void removeDataAfter(double key);
  void removeData(double fromKey, double toKey);
  void removeData(double key);
  
  // reimplemented virtual methods:
  virtual void clearData();
  virtual double selectTest(const QPointF &pos, bool onlySelectable, QVariant *details=0) const;
  
  // static methods:
// TODO: unsupported function return type - provide %MethodCode and a C++ signature
//  static QCPFinancialDataMap timeSeriesToOhlc(const QVector<double> &time, const QVector<double> &value, double timeBinSize, double timeBinOffset = 0);
  
protected:
  // property members:
//  QCPFinancialDataMap *mData;
//  ChartStyle mChartStyle;
//  double mWidth;
//  bool mTwoColored;
//  QBrush mBrushPositive, mBrushNegative;
//  QPen mPenPositive, mPenNegative;
  
  // reimplemented virtual methods:
  virtual void draw(QCPPainter *painter);
  virtual void drawLegendIcon(QCPPainter *painter, const QRectF &rect) const;
// TODO: error: ‘QCPAbstractPlottable::SignDomain sdBoth’ is protected
//  virtual QCPRange getKeyRange(bool &foundRange, SignDomain inSignDomain=sdBoth) const;
  virtual QCPRange getKeyRange(bool &foundRange, SignDomain inSignDomain) const;
// TODO: error: ‘QCPAbstractPlottable::SignDomain sdBoth’ is protected
//  virtual QCPRange getValueRange(bool &foundRange, SignDomain inSignDomain=sdBoth) const;
  virtual QCPRange getValueRange(bool &foundRange, SignDomain inSignDomain) const;
  
  // non-virtual methods:
// TODO: QCPFinancialDataMap::const_iterator is undefined
//  void drawOhlcPlot(QCPPainter *painter, const QCPFinancialDataMap::const_iterator &begin, const QCPFinancialDataMap::const_iterator &end);
// TODO: QCPFinancialDataMap::const_iterator is undefined
//  void drawCandlestickPlot(QCPPainter *painter, const QCPFinancialDataMap::const_iterator &begin, const QCPFinancialDataMap::const_iterator &end);
// TODO: QCPFinancialDataMap::const_iterator is undefined
//  double ohlcSelectTest(const QPointF &pos, const QCPFinancialDataMap::const_iterator &begin, const QCPFinancialDataMap::const_iterator &end) const;
// TODO: QCPFinancialDataMap::const_iterator is undefined
//  double candlestickSelectTest(const QPointF &pos, const QCPFinancialDataMap::const_iterator &begin, const QCPFinancialDataMap::const_iterator &end) const;
// TODO: QCPFinancialDataMap::const_iterator is undefined
//  void getVisibleDataBounds(QCPFinancialDataMap::const_iterator &lower, QCPFinancialDataMap::const_iterator &upper) const;
  
//  friend class QCustomPlot;
//  friend class QCPLegend;
};

