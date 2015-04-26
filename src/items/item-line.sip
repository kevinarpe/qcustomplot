
%ModuleCode
#include "qcustomplot.h"
%End

class QCPItemLine : QCPAbstractItem
{
%TypeHeaderCode
#include "qcustomplot.h"
%End

public:
  QCPItemLine(QCustomPlot *parentPlot);
  virtual ~QCPItemLine();
  
  // getters:
  QPen pen() const;
  QPen selectedPen() const;
  QCPLineEnding head() const;
  QCPLineEnding tail() const;
  
  // setters;
  void setPen(const QPen &pen);
  void setSelectedPen(const QPen &pen);
  void setHead(const QCPLineEnding &head);
  void setTail(const QCPLineEnding &tail);
  
  // reimplemented virtual methods:
  virtual double selectTest(const QPointF &pos, bool onlySelectable, QVariant *details=0) const;
  
//  QCPItemPosition * const start;
  QCPItemPosition *start
  {
    %SetCode
    // This is a *const, no setter
    %End
  };
//  QCPItemPosition * const end;
  QCPItemPosition *end
  {
    %SetCode
    // This is a *const, no setter
    %End
  };
  
protected:
  // property members:
//  QPen mPen, mSelectedPen;
//  QCPLineEnding mHead, mTail;
  
  // reimplemented virtual methods:
  virtual void draw(QCPPainter *painter);
  
  // non-virtual methods:
  QLineF getRectClippedLine(const QVector2D &start, const QVector2D &end, const QRect &rect) const;
  QPen mainPen() const;
};

