
%ModuleCode
#include "qcustomplot.h"
%End

class QCPBarsGroup : QObject
{
%TypeHeaderCode
#include "qcustomplot.h"
%End

public:
  enum SpacingType { stAbsolute       ///< Bar spacing is in absolute pixels
                     ,stAxisRectRatio ///< Bar spacing is given by a fraction of the axis rect size
                     ,stPlotCoords    ///< Bar spacing is in key coordinates and thus scales with the key axis range
                 };
  QCPBarsGroup(QCustomPlot *parentPlot);
  ~QCPBarsGroup();
  
  // getters:
  SpacingType spacingType() const;
  double spacing() const;
  
  // setters:
  void setSpacingType(SpacingType spacingType);
  void setSpacing(double spacing);
  
  // non-virtual methods:
  QList<QCPBars*> bars() const;
  QCPBars* bars(int index) const;
  int size() const;
  bool isEmpty() const;
  void clear();
  bool contains(QCPBars *bars) const;
  void append(QCPBars *bars);
  void insert(int i, QCPBars *bars);
  void remove(QCPBars *bars);
  
protected:
  // non-property members:
//  QCustomPlot *mParentPlot;
//  SpacingType mSpacingType;
//  double mSpacing;
//  QList<QCPBars*> mBars;
  
  // non-virtual methods:
  void registerBars(QCPBars *bars);
  void unregisterBars(QCPBars *bars);
  
  // virtual methods:
  double keyPixelOffset(const QCPBars *bars, double keyCoord);
  double getPixelSpacing(const QCPBars *bars, double keyCoord);
  
//private:
//  Q_DISABLE_COPY(QCPBarsGroup)
//  QCPBarsGroup(const QCPBarsGroup &);
  
//  friend class QCPBars;
};


class QCPBarData
{
%TypeHeaderCode
#include "qcustomplot.h"
%End

public:
  QCPBarData();
  QCPBarData(double key, double value);
//  double key, value;
  double key;
  double value;
};
//Q_DECLARE_TYPEINFO(QCPBarData, Q_MOVABLE_TYPE);

typedef QMap<double, QCPBarData> QCPBarDataMap;
typedef QMapIterator<double, QCPBarData> QCPBarDataMapIterator;
typedef QMutableMapIterator<double, QCPBarData> QCPBarDataMutableMapIterator;


class QCPBars : QCPAbstractPlottable
{
%TypeHeaderCode
#include "qcustomplot.h"
%End

public:
  enum WidthType { wtAbsolute       ///< Bar width is in absolute pixels
                   ,wtAxisRectRatio ///< Bar width is given by a fraction of the axis rect size
                   ,wtPlotCoords    ///< Bar width is in key coordinates and thus scales with the key axis range
                 };
//   Q_ENUMS(WidthType)
  
  explicit QCPBars(QCPAxis *keyAxis, QCPAxis *valueAxis);
  virtual ~QCPBars();
  
  // getters:
  double width() const;
  WidthType widthType() const;
  QCPBarsGroup *barsGroup() const;
  double baseValue() const;
  QCPBars *barBelow() const;
  QCPBars *barAbove() const;
// TODO: unsupported function return type - provide %MethodCode and a C++ signature
//  QCPBarDataMap *data() const;
  
  // setters:
  void setWidth(double width);
  void setWidthType(WidthType widthType);
  void setBarsGroup(QCPBarsGroup *barsGroup);
  void setBaseValue(double baseValue);
// TODO: unsupported function argument type - provide %MethodCode and a C++ signature
//  void setData(QCPBarDataMap *data, bool copy=false);
  void setData(const QVector<double> &key, const QVector<double> &value);
  
  // non-property methods:
  void moveBelow(QCPBars *bars);
  void moveAbove(QCPBars *bars);
// TODO: unsupported function argument type - provide %MethodCode and a C++ signature
//  void addData(const QCPBarDataMap &dataMap);
  void addData(const QCPBarData &data);
  void addData(double key, double value);
  void addData(const QVector<double> &keys, const QVector<double> &values);
  void removeDataBefore(double key);
  void removeDataAfter(double key);
  void removeData(double fromKey, double toKey);
  void removeData(double key);
  
  // reimplemented virtual methods:
  virtual void clearData();
  virtual double selectTest(const QPointF &pos, bool onlySelectable, QVariant *details=0) const;
  
protected:
  // property members:
//  QCPBarDataMap *mData;
//  double mWidth;
//  WidthType mWidthType;
//  QCPBarsGroup *mBarsGroup;
//  double mBaseValue;
//  QPointer<QCPBars> mBarBelow, mBarAbove;
  
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
// TODO: QCPBarDataMap::const_iterator is undefined
//  void getVisibleDataBounds(QCPBarDataMap::const_iterator &lower, QCPBarDataMap::const_iterator &upperEnd) const;
  QPolygonF getBarPolygon(double key, double value) const;
  void getPixelWidth(double key, double &lower, double &upper) const;
  double getStackedBaseValue(double key, bool positive) const;
  static void connectBars(QCPBars* lower, QCPBars* upper);
  
//  friend class QCustomPlot;
//  friend class QCPLegend;
//  friend class QCPBarsGroup;
};

