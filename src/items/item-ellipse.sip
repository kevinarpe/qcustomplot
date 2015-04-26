
%ModuleCode
#include "qcustomplot.h"
%End

class QCPItemEllipse : QCPAbstractItem
{
%TypeHeaderCode
#include "qcustomplot.h"
%End

public:
  QCPItemEllipse(QCustomPlot *parentPlot);
  virtual ~QCPItemEllipse();
  
  // getters:
  QPen pen() const;
  QPen selectedPen() const;
  QBrush brush() const;
  QBrush selectedBrush() const;
  
  // setters;
  void setPen(const QPen &pen);
  void setSelectedPen(const QPen &pen);
  void setBrush(const QBrush &brush);
  void setSelectedBrush(const QBrush &brush);
  
  // reimplemented virtual methods:
  virtual double selectTest(const QPointF &pos, bool onlySelectable, QVariant *details=0) const;
  
//  QCPItemPosition * const topLeft;
  QCPItemPosition *topLeft
  {
    %SetCode
    // This is a *const, no setter
    %End
  };
//  QCPItemPosition * const bottomRight;
  QCPItemPosition *bottomRight
  {
    %SetCode
    // This is a *const, no setter
    %End
  };
//  QCPItemAnchor * const topLeftRim;
  QCPItemAnchor *topLeftRim
  {
    %SetCode
    // This is a *const, no setter
    %End
  };
//  QCPItemAnchor * const top;
  QCPItemAnchor *top
  {
    %SetCode
    // This is a *const, no setter
    %End
  };
//  QCPItemAnchor * const topRightRim;
  QCPItemAnchor *topRightRim
  {
    %SetCode
    // This is a *const, no setter
    %End
  };
//  QCPItemAnchor * const right;
  QCPItemAnchor *right
  {
    %SetCode
    // This is a *const, no setter
    %End
  };
//  QCPItemAnchor * const bottomRightRim;
  QCPItemAnchor *bottomRightRim
  {
    %SetCode
    // This is a *const, no setter
    %End
  };
//  QCPItemAnchor * const bottom;
  QCPItemAnchor *bottom
  {
    %SetCode
    // This is a *const, no setter
    %End
  };
//  QCPItemAnchor * const bottomLeftRim;
  QCPItemAnchor *bottomLeftRim
  {
    %SetCode
    // This is a *const, no setter
    %End
  };
//  QCPItemAnchor * const left;
  QCPItemAnchor *left
  {
    %SetCode
    // This is a *const, no setter
    %End
  };
//  QCPItemAnchor * const center;
  QCPItemAnchor *center
  {
    %SetCode
    // This is a *const, no setter
    %End
  };
  
protected:
  enum AnchorIndex {aiTopLeftRim, aiTop, aiTopRightRim, aiRight, aiBottomRightRim, aiBottom, aiBottomLeftRim, aiLeft, aiCenter};
  
  // property members:
//  QPen mPen, mSelectedPen;
//  QBrush mBrush, mSelectedBrush;
  
  // reimplemented virtual methods:
  virtual void draw(QCPPainter *painter);
  virtual QPointF anchorPixelPoint(int anchorId) const;
  
  // non-virtual methods:
  QPen mainPen() const;
  QBrush mainBrush() const;
};

