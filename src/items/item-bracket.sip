
%ModuleCode
#include "qcustomplot.h"
%End

class QCPItemBracket : QCPAbstractItem
{
%TypeHeaderCode
#include "qcustomplot.h"
%End

public:
  enum BracketStyle { bsSquare  ///< A brace with angled edges
                      ,bsRound  ///< A brace with round edges
                      ,bsCurly  ///< A curly brace
                      ,bsCalligraphic ///< A curly brace with varying stroke width giving a calligraphic impression
  };

  QCPItemBracket(QCustomPlot *parentPlot);
  virtual ~QCPItemBracket();
  
  // getters:
  QPen pen() const;
  QPen selectedPen() const;
  double length() const;
  BracketStyle style() const;
  
  // setters;
  void setPen(const QPen &pen);
  void setSelectedPen(const QPen &pen);
  void setLength(double length);
  void setStyle(BracketStyle style);
  
  // reimplemented virtual methods:
  virtual double selectTest(const QPointF &pos, bool onlySelectable, QVariant *details=0) const;
  
//  QCPItemPosition * const left;
  QCPItemPosition *left
  {
    %SetCode
    // This is a *const, no setter
    %End
  };
//  QCPItemPosition * const right;
  QCPItemPosition *right
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
  // property members:
//  enum AnchorIndex {aiCenter};
//  QPen mPen, mSelectedPen;
//  double mLength;
//  BracketStyle mStyle;
  
  // reimplemented virtual methods:
  virtual void draw(QCPPainter *painter);
  virtual QPointF anchorPixelPoint(int anchorId) const;
  
  // non-virtual methods:
  QPen mainPen() const;
};

