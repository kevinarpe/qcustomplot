
%ModuleCode
#include "qcustomplot.h"
%End

class QCPMarginGroup : QObject
{
%TypeHeaderCode
#include "qcustomplot.h"
%End

public:
  QCPMarginGroup(QCustomPlot *parentPlot);
  ~QCPMarginGroup();
  
  // non-virtual methods:
  QList<QCPLayoutElement*> elements(QCP::MarginSide side) const;
  bool isEmpty() const;
  void clear();
  
protected:
  // non-property members:
//  QCustomPlot *mParentPlot;
//  QHash<QCP::MarginSide, QList<QCPLayoutElement*> > mChildren;
  
  // non-virtual methods:
  int commonMargin(QCP::MarginSide side) const;
  void addChild(QCP::MarginSide side, QCPLayoutElement *element);
  void removeChild(QCP::MarginSide side, QCPLayoutElement *element);
  
//private:
//  Q_DISABLE_COPY(QCPMarginGroup)
//  QCPMarginGroup(const QCPMarginGroup &);
  
//  friend class QCPLayoutElement;
};

class QCPLayoutElement : QCPLayerable
{
%TypeHeaderCode
#include "qcustomplot.h"
%End

public:
  enum UpdatePhase { upPreparation ///< Phase used for any type of preparation that needs to be done before margin calculation and layout
                     ,upMargins    ///< Phase in which the margins are calculated and set
                     ,upLayout     ///< Final phase in which the layout system places the rects of the elements
                   };
//  Q_ENUMS(UpdatePhase)

  explicit QCPLayoutElement(QCustomPlot *parentPlot=0);
  virtual ~QCPLayoutElement();
  
  // getters:
  QCPLayout *layout() const;
  QRect rect() const;
  QRect outerRect() const;
  QMargins margins() const;
  QMargins minimumMargins() const;
  QCP::MarginSides autoMargins() const;
  QSize minimumSize() const;
  QSize maximumSize() const;
  QCPMarginGroup *marginGroup(QCP::MarginSide side) const;
// TODO: unsupported function return type - provide %MethodCode and a C++ signature
//  QHash<QCP::MarginSide, QCPMarginGroup*> marginGroups() const;
  
  // setters:
  void setOuterRect(const QRect &rect);
  void setMargins(const QMargins &margins);
  void setMinimumMargins(const QMargins &margins);
  void setAutoMargins(QCP::MarginSides sides);
  void setMinimumSize(const QSize &size);
  void setMinimumSize(int width, int height);
  void setMaximumSize(const QSize &size);
  void setMaximumSize(int width, int height);
  void setMarginGroup(QCP::MarginSides sides, QCPMarginGroup *group);
  
  // introduced virtual methods:
  virtual void update(UpdatePhase phase);
  virtual QSize minimumSizeHint() const;
  virtual QSize maximumSizeHint() const;
  virtual QList<QCPLayoutElement*> elements(bool recursive) const;
  
  // reimplemented virtual methods:
  virtual double selectTest(const QPointF &pos, bool onlySelectable, QVariant *details=0) const;
  
protected:
  // property members:
//  QCPLayout *mParentLayout;
//  QSize mMinimumSize, mMaximumSize;
//  QRect mRect, mOuterRect;
//  QMargins mMargins, mMinimumMargins;
//  QCP::MarginSides mAutoMargins;
//  QHash<QCP::MarginSide, QCPMarginGroup*> mMarginGroups;
  
  // introduced virtual methods:
  virtual int calculateAutoMargin(QCP::MarginSide side);
  // events:
  virtual void mousePressEvent(QMouseEvent *event);
  virtual void mouseMoveEvent(QMouseEvent *event);
  virtual void mouseReleaseEvent(QMouseEvent *event);
  virtual void mouseDoubleClickEvent(QMouseEvent *event);
  virtual void wheelEvent(QWheelEvent *event);
  
  // reimplemented virtual methods:
  virtual void applyDefaultAntialiasingHint(QCPPainter *painter) const;
  virtual void draw(QCPPainter *painter);
  virtual void parentPlotInitialized(QCustomPlot *parentPlot);

//private:
//  Q_DISABLE_COPY(QCPLayoutElement)
//  QCPLayoutElement(const QCPLayoutElement &);
  
//  friend class QCustomPlot;
//  friend class QCPLayout;
//  friend class QCPMarginGroup;
};

class QCPLayout : QCPLayoutElement
{
%TypeHeaderCode
#include "qcustomplot.h"
%End

public:
  explicit QCPLayout();
  
  // reimplemented virtual methods:
  virtual void update(UpdatePhase phase);
  virtual QList<QCPLayoutElement*> elements(bool recursive) const;
  
  // introduced virtual methods:
  virtual int elementCount() const = 0;
  virtual QCPLayoutElement* elementAt(int index) const = 0;
  virtual QCPLayoutElement* takeAt(int index) = 0;
  virtual bool take(QCPLayoutElement* element) = 0;
  virtual void simplify();
  
  // non-virtual methods:
  bool removeAt(int index);
  bool remove(QCPLayoutElement* element);
  void clear();
  
protected:
  // introduced virtual methods:
  virtual void updateLayout();
  
  // non-virtual methods:
  void sizeConstraintsChanged() const;
  void adoptElement(QCPLayoutElement *el);
  void releaseElement(QCPLayoutElement *el);
  QVector<int> getSectionSizes(QVector<int> maxSizes, QVector<int> minSizes, QVector<double> stretchFactors, int totalSize) const;
  
//private:
//  Q_DISABLE_COPY(QCPLayout)
//  QCPLayout(const QCPLayout &);
//  friend class QCPLayoutElement;
};

class QCPLayoutGrid : QCPLayout
{
%TypeHeaderCode
#include "qcustomplot.h"
%End

public:
  explicit QCPLayoutGrid();
  virtual ~QCPLayoutGrid();
  
  // getters:
  int rowCount() const;
  int columnCount() const;
  QList<double> columnStretchFactors() const;
  QList<double> rowStretchFactors() const;
  int columnSpacing() const;
  int rowSpacing() const;
  
  // setters:
  void setColumnStretchFactor(int column, double factor);
  void setColumnStretchFactors(const QList<double> &factors);
  void setRowStretchFactor(int row, double factor);
  void setRowStretchFactors(const QList<double> &factors);
  void setColumnSpacing(int pixels);
  void setRowSpacing(int pixels);
  
  // reimplemented virtual methods:
  virtual void updateLayout();
  virtual int elementCount() const;
  virtual QCPLayoutElement* elementAt(int index) const;
  virtual QCPLayoutElement* takeAt(int index);
  virtual bool take(QCPLayoutElement* element);
  virtual QList<QCPLayoutElement*> elements(bool recursive) const;
  virtual void simplify();
  virtual QSize minimumSizeHint() const;
  virtual QSize maximumSizeHint() const;
  
  // non-virtual methods:
  QCPLayoutElement *element(int row, int column) const;
  bool addElement(int row, int column, QCPLayoutElement *element);
  bool hasElement(int row, int column);
  void expandTo(int newRowCount, int newColumnCount);
  void insertRow(int newIndex);
  void insertColumn(int newIndex);
  
protected:
  // property members:
//  QList<QList<QCPLayoutElement*> > mElements;
//  QList<double> mColumnStretchFactors;
//  QList<double> mRowStretchFactors;
//  int mColumnSpacing, mRowSpacing;
  
  // non-virtual methods:
  void getMinimumRowColSizes(QVector<int> *minColWidths, QVector<int> *minRowHeights) const;
  void getMaximumRowColSizes(QVector<int> *maxColWidths, QVector<int> *maxRowHeights) const;
  
//private:
//  Q_DISABLE_COPY(QCPLayoutGrid)
//  QCPLayoutGrid(const QCPLayoutGrid &);
};

class QCPLayoutInset : QCPLayout
{
%TypeHeaderCode
#include "qcustomplot.h"
%End

public:
  enum InsetPlacement { ipFree            ///< The element may be positioned/sized arbitrarily, see \ref setInsetRect
                        ,ipBorderAligned  ///< The element is aligned to one of the layout sides, see \ref setInsetAlignment
                      };
  
  explicit QCPLayoutInset();
  virtual ~QCPLayoutInset();
  
  // getters:
  InsetPlacement insetPlacement(int index) const;
  Qt::Alignment insetAlignment(int index) const;
  QRectF insetRect(int index) const;
  
  // setters:
  void setInsetPlacement(int index, InsetPlacement placement);
  void setInsetAlignment(int index, Qt::Alignment alignment);
  void setInsetRect(int index, const QRectF &rect);
  
  // reimplemented virtual methods:
  virtual void updateLayout();
  virtual int elementCount() const;
  virtual QCPLayoutElement* elementAt(int index) const;
  virtual QCPLayoutElement* takeAt(int index);
  virtual bool take(QCPLayoutElement* element);
  virtual void simplify();
  virtual double selectTest(const QPointF &pos, bool onlySelectable, QVariant *details=0) const;
  
  // non-virtual methods:
  void addElement(QCPLayoutElement *element, Qt::Alignment alignment);
  void addElement(QCPLayoutElement *element, const QRectF &rect);
  
protected:
  // property members:
//  QList<QCPLayoutElement*> mElements;
//  QList<InsetPlacement> mInsetPlacement;
//  QList<Qt::Alignment> mInsetAlignment;
//  QList<QRectF> mInsetRect;
  
//private:
//  Q_DISABLE_COPY(QCPLayoutInset)
//  QCPLayoutInset(const QCPLayoutInset &);
};

