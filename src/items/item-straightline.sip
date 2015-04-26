
%ModuleCode
#include "qcustomplot.h"
%End

class QCPItemStraightLine : QCPAbstractItem
{
%TypeHeaderCode
#include "qcustomplot.h"
%End

public:
  QCPItemStraightLine(QCustomPlot *parentPlot);
  virtual ~QCPItemStraightLine();
  
  // getters:
  QPen pen() const;
  QPen selectedPen() const;
  
  // setters;
  void setPen(const QPen &pen);
  void setSelectedPen(const QPen &pen);
  
  // reimplemented virtual methods:
  virtual double selectTest(const QPointF &pos, bool onlySelectable, QVariant *details=0) const;
  
//  QCPItemPosition * const point1;
  QCPItemPosition *point1
  {
    %SetCode
    // This is a *const, no setter
    %End
  };
//  QCPItemPosition * const point2;
  QCPItemPosition *point2
  {
    %SetCode
    // This is a *const, no setter
    %End
  };
  
protected:
  // property members:
//  QPen mPen, mSelectedPen;
  
  // reimplemented virtual methods:
  virtual void draw(QCPPainter *painter);
  
  // non-virtual methods:
  double distToStraightLine(const QVector2D &point1, const QVector2D &vec, const QVector2D &point) const;
  QLineF getRectClippedStraightLine(const QVector2D &point1, const QVector2D &vec, const QRect &rect) const;
  QPen mainPen() const;
};

