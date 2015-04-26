
%ModuleCode
#include "qcustomplot.h"
%End

class QCPGrid : QCPLayerable
{
%TypeHeaderCode
#include "qcustomplot.h"
%End

public:
  QCPGrid(QCPAxis *parentAxis);
  
  // getters:
  bool subGridVisible() const;
  bool antialiasedSubGrid() const;
  bool antialiasedZeroLine() const;
  QPen pen() const;
  QPen subGridPen() const;
  QPen zeroLinePen() const;
  
  // setters:
  void setSubGridVisible(bool visible);
  void setAntialiasedSubGrid(bool enabled);
  void setAntialiasedZeroLine(bool enabled);
  void setPen(const QPen &pen);
  void setSubGridPen(const QPen &pen);
  void setZeroLinePen(const QPen &pen);
  
protected:
  // property members:
//  bool mSubGridVisible;
//  bool mAntialiasedSubGrid, mAntialiasedZeroLine;
//  QPen mPen, mSubGridPen, mZeroLinePen;
  // non-property members:
//  QCPAxis *mParentAxis;
  
  // reimplemented virtual methods:
  virtual void applyDefaultAntialiasingHint(QCPPainter *painter) const;
  virtual void draw(QCPPainter *painter);
  
  // non-virtual methods:
  void drawGridLines(QCPPainter *painter) const;
  void drawSubGridLines(QCPPainter *painter) const;
  
//  friend class QCPAxis;
};


class QCPAxis : QCPLayerable
{
%TypeHeaderCode
#include "qcustomplot.h"
%End

public:
  enum AxisType { atLeft    = 0x01  ///< <tt>0x01</tt> Axis is vertical and on the left side of the axis rect
                  ,atRight  = 0x02  ///< <tt>0x02</tt> Axis is vertical and on the right side of the axis rect
                  ,atTop    = 0x04  ///< <tt>0x04</tt> Axis is horizontal and on the top side of the axis rect
                  ,atBottom = 0x08  ///< <tt>0x08</tt> Axis is horizontal and on the bottom side of the axis rect
                };
//  Q_FLAGS(AxisType AxisTypes)
//  Q_DECLARE_FLAGS(AxisTypes, AxisType)
  typedef QFlags<QCPAxis::AxisType> AxisTypes;
  enum LabelType { ltNumber    ///< Tick coordinate is regarded as normal number and will be displayed as such. (see \ref setNumberFormat)
                   ,ltDateTime ///< Tick coordinate is regarded as a date/time (seconds since 1970-01-01T00:00:00 UTC) and will be displayed and formatted as such. (for details, see \ref setDateTimeFormat)
                 };
//  Q_ENUMS(LabelType)
  enum LabelSide { lsInside    ///< Tick labels will be displayed inside the axis rect and clipped to the inner axis rect
                   ,lsOutside  ///< Tick labels will be displayed outside the axis rect
                 };
//  Q_ENUMS(LabelSide)
  enum ScaleType { stLinear       ///< Linear scaling
                   ,stLogarithmic ///< Logarithmic scaling with correspondingly transformed plots and (major) tick marks at every base power (see \ref setScaleLogBase).
                 };
//  Q_ENUMS(ScaleType)
  enum SelectablePart { spNone        = 0      ///< None of the selectable parts
                        ,spAxis       = 0x001  ///< The axis backbone and tick marks
                        ,spTickLabels = 0x002  ///< Tick labels (numbers) of this axis (as a whole, not individually)
                        ,spAxisLabel  = 0x004  ///< The axis label
                      };
//  Q_FLAGS(SelectablePart SelectableParts)
//  Q_DECLARE_FLAGS(SelectableParts, SelectablePart)
  typedef QFlags<QCPAxis::SelectablePart> SelectableParts;
  
  explicit QCPAxis(QCPAxisRect *parent, AxisType type);
  virtual ~QCPAxis();
  
  // getters:
  AxisType axisType() const;
  QCPAxisRect *axisRect() const;
  ScaleType scaleType() const;
  double scaleLogBase() const;
  const QCPRange range() const;
  bool rangeReversed() const;
  bool autoTicks() const;
  int autoTickCount() const;
  bool autoTickLabels() const;
  bool autoTickStep() const;
  bool autoSubTicks() const;
  bool ticks() const;
  bool tickLabels() const;
  int tickLabelPadding() const;
  LabelType tickLabelType() const;
  QFont tickLabelFont() const;
  QColor tickLabelColor() const;
  double tickLabelRotation() const;
  LabelSide tickLabelSide() const;
  QString dateTimeFormat() const;
  Qt::TimeSpec dateTimeSpec() const;
  QString numberFormat() const;
  int numberPrecision() const;
  double tickStep() const;
  QVector<double> tickVector() const;
  QVector<QString> tickVectorLabels() const;
  int tickLengthIn() const;
  int tickLengthOut() const;
  int subTickCount() const;
  int subTickLengthIn() const;
  int subTickLengthOut() const;
  QPen basePen() const;
  QPen tickPen() const;
  QPen subTickPen() const;
  QFont labelFont() const;
  QColor labelColor() const;
  QString label() const;
  int labelPadding() const;
  int padding() const;
  int offset() const;
  SelectableParts selectedParts() const;
  SelectableParts selectableParts() const;
  QFont selectedTickLabelFont() const;
  QFont selectedLabelFont() const;
  QColor selectedTickLabelColor() const;
  QColor selectedLabelColor() const;
  QPen selectedBasePen() const;
  QPen selectedTickPen() const;
  QPen selectedSubTickPen() const;
  QCPLineEnding lowerEnding() const;
  QCPLineEnding upperEnding() const;
  QCPGrid *grid() const;
  
  // setters:
  Q_SLOT void setScaleType(QCPAxis::ScaleType type);
  void setScaleLogBase(double base);
  Q_SLOT void setRange(const QCPRange &range);
  void setRange(double lower, double upper);
  void setRange(double position, double size, Qt::AlignmentFlag alignment);
  void setRangeLower(double lower);
  void setRangeUpper(double upper);
  void setRangeReversed(bool reversed);
  void setAutoTicks(bool on);
  void setAutoTickCount(int approximateCount);
  void setAutoTickLabels(bool on);
  void setAutoTickStep(bool on);
  void setAutoSubTicks(bool on);
  void setTicks(bool show);
  void setTickLabels(bool show);
  void setTickLabelPadding(int padding);
  void setTickLabelType(LabelType type);
  void setTickLabelFont(const QFont &font);
  void setTickLabelColor(const QColor &color);
  void setTickLabelRotation(double degrees);
  void setTickLabelSide(LabelSide side);
  void setDateTimeFormat(const QString &format);
  void setDateTimeSpec(const Qt::TimeSpec &timeSpec);
  void setNumberFormat(const QString &formatCode);
  void setNumberPrecision(int precision);
  void setTickStep(double step);
  void setTickVector(const QVector<double> &vec);
  void setTickVectorLabels(const QVector<QString> &vec);
  void setTickLength(int inside, int outside=0);
  void setTickLengthIn(int inside);
  void setTickLengthOut(int outside);
  void setSubTickCount(int count);
  void setSubTickLength(int inside, int outside=0);
  void setSubTickLengthIn(int inside);
  void setSubTickLengthOut(int outside);
  void setBasePen(const QPen &pen);
  void setTickPen(const QPen &pen);
  void setSubTickPen(const QPen &pen);
  void setLabelFont(const QFont &font);
  void setLabelColor(const QColor &color);
  void setLabel(const QString &str);
  void setLabelPadding(int padding);
  void setPadding(int padding);
  void setOffset(int offset);
  void setSelectedTickLabelFont(const QFont &font);
  void setSelectedLabelFont(const QFont &font);
  void setSelectedTickLabelColor(const QColor &color);
  void setSelectedLabelColor(const QColor &color);
  void setSelectedBasePen(const QPen &pen);
  void setSelectedTickPen(const QPen &pen);
  void setSelectedSubTickPen(const QPen &pen);
  Q_SLOT void setSelectableParts(const QCPAxis::SelectableParts &selectableParts);
  Q_SLOT void setSelectedParts(const QCPAxis::SelectableParts &selectedParts);
  void setLowerEnding(const QCPLineEnding &ending);
  void setUpperEnding(const QCPLineEnding &ending);
  
  // reimplemented virtual methods:
  virtual double selectTest(const QPointF &pos, bool onlySelectable, QVariant *details=0) const;
  
  // non-property methods:
  Qt::Orientation orientation() const;
  void moveRange(double diff);
  void scaleRange(double factor, double center);
  void setScaleRatio(const QCPAxis *otherAxis, double ratio=1.0);
  void rescale(bool onlyVisiblePlottables=false);
  double pixelToCoord(double value) const;
  double coordToPixel(double value) const;
  SelectablePart getPartAt(const QPointF &pos) const;
  QList<QCPAbstractPlottable*> plottables() const;
  QList<QCPGraph*> graphs() const;
  QList<QCPAbstractItem*> items() const;
  
  static AxisType marginSideToAxisType(QCP::MarginSide side);
  static Qt::Orientation orientation(AxisType type);
  static AxisType opposite(AxisType type);
  
signals:
  void ticksRequest();
  void rangeChanged(const QCPRange &newRange);
  void rangeChanged(const QCPRange &newRange, const QCPRange &oldRange);
  void scaleTypeChanged(QCPAxis::ScaleType scaleType);
  void selectionChanged(const QCPAxis::SelectableParts &parts);
  void selectableChanged(const QCPAxis::SelectableParts &parts);

protected:
  // property members:
  // axis base:
//  AxisType mAxisType;
//  QCPAxisRect *mAxisRect;
  //int mOffset; // in QCPAxisPainter
//  int mPadding;
//  Qt::Orientation mOrientation;
//  SelectableParts mSelectableParts, mSelectedParts;
//  QPen mBasePen, mSelectedBasePen;
  //QCPLineEnding mLowerEnding, mUpperEnding; // in QCPAxisPainter
  // axis label:
  //int mLabelPadding; // in QCPAxisPainter
//  QString mLabel;
//  QFont mLabelFont, mSelectedLabelFont;
//  QColor mLabelColor, mSelectedLabelColor;
  // tick labels:
  //int mTickLabelPadding; // in QCPAxisPainter
//  bool mTickLabels, mAutoTickLabels;
  //double mTickLabelRotation; // in QCPAxisPainter
//  LabelType mTickLabelType;
//  QFont mTickLabelFont, mSelectedTickLabelFont;
//  QColor mTickLabelColor, mSelectedTickLabelColor;
//  QString mDateTimeFormat;
//  Qt::TimeSpec mDateTimeSpec;
//  int mNumberPrecision;
//  QLatin1Char mNumberFormatChar;
//  bool mNumberBeautifulPowers;
  //bool mNumberMultiplyCross; // QCPAxisPainter
  // ticks and subticks:
//  bool mTicks;
//  double mTickStep;
//  int mSubTickCount, mAutoTickCount;
//  bool mAutoTicks, mAutoTickStep, mAutoSubTicks;
  //int mTickLengthIn, mTickLengthOut, mSubTickLengthIn, mSubTickLengthOut; // QCPAxisPainter
//  QPen mTickPen, mSelectedTickPen;
//  QPen mSubTickPen, mSelectedSubTickPen;
  // scale and range:
//  QCPRange mRange;
//  bool mRangeReversed;
//  ScaleType mScaleType;
//  double mScaleLogBase, mScaleLogBaseLogInv;
  
  // non-property members:
//  QCPGrid *mGrid;
//  QCPAxisPainterPrivate *mAxisPainter;
//  int mLowestVisibleTick, mHighestVisibleTick;
//  QVector<double> mTickVector;
//  QVector<QString> mTickVectorLabels;
//  QVector<double> mSubTickVector;
//  bool mCachedMarginValid;
//  int mCachedMargin;
  
  // introduced virtual methods:
  virtual void setupTickVectors();
  virtual void generateAutoTicks();
  virtual int calculateAutoSubTickCount(double tickStep) const;
  virtual int calculateMargin();
  
  // reimplemented virtual methods:
  virtual void applyDefaultAntialiasingHint(QCPPainter *painter) const;
  virtual void draw(QCPPainter *painter);
  virtual QCP::Interaction selectionCategory() const;
  // events:
  virtual void selectEvent(QMouseEvent *event, bool additive, const QVariant &details, bool *selectionStateChanged);
  virtual void deselectEvent(bool *selectionStateChanged);
  
  // non-virtual methods:
  void visibleTickBounds(int &lowIndex, int &highIndex) const;
  double baseLog(double value) const;
  double basePow(double value) const;
  QPen getBasePen() const;
  QPen getTickPen() const;
  QPen getSubTickPen() const;
  QFont getTickLabelFont() const;
  QFont getLabelFont() const;
  QColor getTickLabelColor() const;
  QColor getLabelColor() const;
  
//private:
//  Q_DISABLE_COPY(QCPAxis)
//  QCPAxis(const QCPAxis &);
  
//  friend class QCustomPlot;
//  friend class QCPGrid;
//  friend class QCPAxisRect;
};
//Q_DECLARE_OPERATORS_FOR_FLAGS(QCPAxis::SelectableParts)
//Q_DECLARE_OPERATORS_FOR_FLAGS(QCPAxis::AxisTypes)
//Q_DECLARE_METATYPE(QCPAxis::SelectablePart)

class QCPAxisPainterPrivate
{
%TypeHeaderCode
#include "qcustomplot.h"
%End

public:
  explicit QCPAxisPainterPrivate(QCustomPlot *parentPlot);
  virtual ~QCPAxisPainterPrivate();
  
  virtual void draw(QCPPainter *painter);
  virtual int size() const;
  void clearCache();
  
  QRect axisSelectionBox() const;
  QRect tickLabelsSelectionBox() const;
  QRect labelSelectionBox() const;
  
  // public property members:
  QCPAxis::AxisType type;
  QPen basePen;
//  QCPLineEnding lowerEnding, upperEnding; // directly accessed by QCPAxis setters/getters
  QCPLineEnding lowerEnding; // directly accessed by QCPAxis setters/getters
  QCPLineEnding upperEnding; // directly accessed by QCPAxis setters/getters
  int labelPadding; // directly accessed by QCPAxis setters/getters
  QFont labelFont;
  QColor labelColor;
  QString label;
  int tickLabelPadding; // directly accessed by QCPAxis setters/getters
  double tickLabelRotation; // directly accessed by QCPAxis setters/getters
  QCPAxis::LabelSide tickLabelSide; // directly accessed by QCPAxis setters/getters
  bool substituteExponent;
  bool numberMultiplyCross; // directly accessed by QCPAxis setters/getters
//  int tickLengthIn, tickLengthOut, subTickLengthIn, subTickLengthOut; // directly accessed by QCPAxis setters/getters
  int tickLengthIn; // directly accessed by QCPAxis setters/getters
  int tickLengthOut; // directly accessed by QCPAxis setters/getters
  int subTickLengthIn; // directly accessed by QCPAxis setters/getters
  int subTickLengthOut; // directly accessed by QCPAxis setters/getters
//  QPen tickPen, subTickPen;
  QPen tickPen;
  QPen subTickPen;
  QFont tickLabelFont;
  QColor tickLabelColor;
//  QRect axisRect, viewportRect;
  QRect axisRect;
  QRect viewportRect;
  double offset; // directly accessed by QCPAxis setters/getters
  bool abbreviateDecimalPowers;
  bool reversedEndings;
  
  QVector<double> subTickPositions;
  QVector<double> tickPositions;
  QVector<QString> tickLabels;
  
protected:
// TODO: Weird compiler error
//  struct CachedLabel
//  {
//    QPointF offset;
//    QPixmap pixmap;
//  };
// TODO: Weird compiler error
//  struct TickLabelData
//  {
////    QString basePart, expPart;
//    QString basePart;
//    QString expPart;
////    QRect baseBounds, expBounds, totalBounds, rotatedTotalBounds;
//    QRect baseBounds;
//    QRect expBounds;
//    QRect totalBounds;
//    QRect rotatedTotalBounds;
////    QFont baseFont, expFont;
//    QFont baseFont;
//    QFont expFont;
//  };
//  QCustomPlot *mParentPlot;
//  QByteArray mLabelParameterHash; // to determine whether mLabelCache needs to be cleared due to changed parameters
//  QCache<QString, CachedLabel> mLabelCache;
//  QRect mAxisSelectionBox, mTickLabelsSelectionBox, mLabelSelectionBox;
  
  virtual QByteArray generateLabelParameterHash() const;
  
  virtual void placeTickLabel(QCPPainter *painter, double position, int distanceToAxis, const QString &text, QSize *tickLabelsSize);
// TODO: Weird compiler error
//  virtual void drawTickLabel(QCPPainter *painter, double x, double y, const TickLabelData &labelData) const;
// TODO: Weird compiler error
//  virtual TickLabelData getTickLabelData(const QFont &font, const QString &text) const;
// TODO: Weird compiler error
//  virtual QPointF getTickLabelDrawOffset(const TickLabelData &labelData) const;
  virtual void getMaxTickLabelSize(const QFont &font, const QString &text, QSize *tickLabelsSize) const;

private:
  QCPAxisPainterPrivate(const QCPAxisPainterPrivate &);
};

