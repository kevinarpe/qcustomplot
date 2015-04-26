
%ModuleCode
#include "qcustomplot.h"
%End

class QCPColorMapData
{
%TypeHeaderCode
#include "qcustomplot.h"
%End

public:
  QCPColorMapData(int keySize, int valueSize, const QCPRange &keyRange, const QCPRange &valueRange);
  ~QCPColorMapData();
  QCPColorMapData(const QCPColorMapData &other);
//  QCPColorMapData &operator=(const QCPColorMapData &other);
  
  // getters:
  int keySize() const;
  int valueSize() const;
  QCPRange keyRange() const;
  QCPRange valueRange() const;
  QCPRange dataBounds() const;
  double data(double key, double value);
  double cell(int keyIndex, int valueIndex);
  
  // setters:
  void setSize(int keySize, int valueSize);
  void setKeySize(int keySize);
  void setValueSize(int valueSize);
  void setRange(const QCPRange &keyRange, const QCPRange &valueRange);
  void setKeyRange(const QCPRange &keyRange);
  void setValueRange(const QCPRange &valueRange);
  void setData(double key, double value, double z);
  void setCell(int keyIndex, int valueIndex, double z);
  
  // non-property methods:
  void recalculateDataBounds();
  void clear();
  void fill(double z);
  bool isEmpty() const;
  void coordToCell(double key, double value, int *keyIndex, int *valueIndex) const;
  void cellToCoord(int keyIndex, int valueIndex, double *key, double *value) const;
  
protected:
  // property members:
//  int mKeySize, mValueSize;
//  QCPRange mKeyRange, mValueRange;
//  bool mIsEmpty;
  // non-property members:
//  double *mData;
//  QCPRange mDataBounds;
//  bool mDataModified;
  
//  friend class QCPColorMap;
};


class QCPColorMap : QCPAbstractPlottable
{
%TypeHeaderCode
#include "qcustomplot.h"
%End

public:
  explicit QCPColorMap(QCPAxis *keyAxis, QCPAxis *valueAxis);
  virtual ~QCPColorMap();
  
  // getters:
  QCPColorMapData *data() const;
  QCPRange dataRange() const;
  QCPAxis::ScaleType dataScaleType() const;
  bool interpolate() const;
  bool tightBoundary() const;
  QCPColorGradient gradient() const;
  QCPColorScale *colorScale() const;
  
  // setters:
  void setData(QCPColorMapData *data, bool copy=false);
  Q_SLOT void setDataRange(const QCPRange &dataRange);
  Q_SLOT void setDataScaleType(QCPAxis::ScaleType scaleType);
  Q_SLOT void setGradient(const QCPColorGradient &gradient);
  void setInterpolate(bool enabled);
  void setTightBoundary(bool enabled);
  void setColorScale(QCPColorScale *colorScale);
  
  // non-property methods:
  void rescaleDataRange(bool recalculateDataBounds=false);
  Q_SLOT void updateLegendIcon(Qt::TransformationMode transformMode=Qt::SmoothTransformation, const QSize &thumbSize=QSize(32, 18));
  
  // reimplemented virtual methods:
  virtual void clearData();
  virtual double selectTest(const QPointF &pos, bool onlySelectable, QVariant *details=0) const;
  
signals:
  void dataRangeChanged(QCPRange newRange);
  void dataScaleTypeChanged(QCPAxis::ScaleType scaleType);
  void gradientChanged(QCPColorGradient newGradient);
  
protected:
  // property members:
//  QCPRange mDataRange;
//  QCPAxis::ScaleType mDataScaleType;
//  QCPColorMapData *mMapData;
//  QCPColorGradient mGradient;
//  bool mInterpolate;
//  bool mTightBoundary;
//  QPointer<QCPColorScale> mColorScale;
  // non-property members:
//  QImage mMapImage, mUndersampledMapImage;
//  QPixmap mLegendIcon;
//  bool mMapImageInvalidated;
  
  // introduced virtual methods:
  virtual void updateMapImage();
  
  // reimplemented virtual methods:
  virtual void draw(QCPPainter *painter);
  virtual void drawLegendIcon(QCPPainter *painter, const QRectF &rect) const;
// TODO: error: ‘QCPAbstractPlottable::SignDomain sdBoth’ is protected
//  virtual QCPRange getKeyRange(bool &foundRange, SignDomain inSignDomain=sdBoth) const;
  virtual QCPRange getKeyRange(bool &foundRange, SignDomain inSignDomain) const;
// TODO: error: ‘QCPAbstractPlottable::SignDomain sdBoth’ is protected
//  virtual QCPRange getValueRange(bool &foundRange, SignDomain inSignDomain=sdBoth) const;
  virtual QCPRange getValueRange(bool &foundRange, SignDomain inSignDomain) const;
  
//  friend class QCustomPlot;
//  friend class QCPLegend;
};

