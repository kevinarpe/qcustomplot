#ifndef QCP_LAYOUT_H
#define QCP_LAYOUT_H

#include "global.h"

class QCPLayout;

class QCP_LIB_DECL QCPLayoutElement
{
public:
  QCPLayoutElement(QCPLayout *parentLayout=0);
  
  QCPLayout *layout() const { return mParentLayout; }
  QRect rect() const { return mRect; }
  QRect outerRect() const { return mOuterRect; }
  QMargins margins() const { return mMargins; }
  QCP::MarginSides autoMargins() const { return mAutoMargins; }
  QSize minimumSize() const { return mMinimumSize; }
  QSize maximumSize() const { return mMaximumSize; }
  
  void setOuterRect(const QRect &rect);
  void setMargins(const QMargins &margins);
  void setAutoMargins(QCP::MarginSides sides);
  void setMinimumSize(const QSize &size);
  void setMaximumSize(const QSize &size);
  
  virtual QSize minimumSizeHint() const;
  virtual QSize maximumSizeHint() const;
  
protected:
  QCPLayout *mParentLayout;
  QSize mMinimumSize, mMaximumSize;
  QRect mRect, mOuterRect;
  QMargins mMargins;
  QCP::MarginSides mAutoMargins;
  
  virtual void rectChangedEvent(const QRect &newRect);
  virtual QMargins calculateAutoMargins() const;
  
  friend class QCPLayout;
};


class QCP_LIB_DECL QCPLayout : public QCPLayoutElement
{
public:
  QCPLayout(QCPLayout *parentLayout=0);
  
  void update();
  
  virtual int elementCount() const = 0;
  virtual QCPLayoutElement* elementAt(int index) const = 0;
  virtual QCPLayoutElement* takeAt(int index) = 0;
  virtual bool take(QCPLayoutElement* element) = 0;
  virtual void simplify() = 0;
  
protected:
  bool mInvalidated;
  
  virtual void layoutElements();
  virtual void rectChangedEvent(const QRect &newRect);
  void adoptChild(QCPLayoutElement *el);
  void releaseChild(QCPLayoutElement *el);
};


class QCP_LIB_DECL QCPLayoutGrid : public QCPLayout
{
public:
  QCPLayoutGrid();
  
  int rows() const;
  int columns() const;
  QCPLayoutElement *element(int row, int column) const;
  bool addElement(QCPLayoutElement *element, int row, int column);
  bool hasElement(int row, int column);
  void setColumnStretchFactor(int column, double factor);
  void setColumnStretchFactors(const QList<double> &factors);
  void setRowStretchFactor(int row, double factor);
  void setRowStretchFactors(const QList<double> &factors);
  void expandTo(int rowCount, int columnCount);
  
  virtual void layoutElements();
  virtual int elementCount() const;
  virtual QCPLayoutElement* elementAt(int index) const;
  virtual QCPLayoutElement* takeAt(int index);
  virtual bool take(QCPLayoutElement* element);
  virtual void simplify();
  
  virtual QSize minimumSizeHint();
  virtual QSize maximumSizeHint();
  
protected:
  QList<QList<QCPLayoutElement*> > mElements;
  QList<double> mColumnStretchFactors;
  QList<double> mRowStretchFactors;
  
  void getMinimumRowColSizes(QVector<int> *minColWidths, QVector<int> *minRowHeights) const;
  void getMaximumRowColSizes(QVector<int> *maxColWidths, QVector<int> *maxRowHeights) const;
  
};

#endif // QCP_LAYOUT_H
