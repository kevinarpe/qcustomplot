
%ModuleCode
#include "qcustomplot.h"
%End

class QCPItemText : QCPAbstractItem
{
%TypeHeaderCode
#include "qcustomplot.h"
%End

public:
  QCPItemText(QCustomPlot *parentPlot);
  virtual ~QCPItemText();
  
  // getters:
  QColor color() const;
  QColor selectedColor() const;
  QPen pen() const;
  QPen selectedPen() const;
  QBrush brush() const;
  QBrush selectedBrush() const;
  QFont font() const;
  QFont selectedFont() const;
  QString text() const;
  Qt::Alignment positionAlignment() const;
  Qt::Alignment textAlignment() const;
  double rotation() const;
  QMargins padding() const;
  
  // setters;
  void setColor(const QColor &color);
  void setSelectedColor(const QColor &color);
  void setPen(const QPen &pen);
  void setSelectedPen(const QPen &pen);
  void setBrush(const QBrush &brush);
  void setSelectedBrush(const QBrush &brush);
  void setFont(const QFont &font);
  void setSelectedFont(const QFont &font);
  void setText(const QString &text);
  void setPositionAlignment(Qt::Alignment alignment);
  void setTextAlignment(Qt::Alignment alignment);
  void setRotation(double degrees);
  void setPadding(const QMargins &padding);
  
  // reimplemented virtual methods:
  virtual double selectTest(const QPointF &pos, bool onlySelectable, QVariant *details=0) const;
  
//  QCPItemPosition * const position;
  QCPItemPosition *position
  {
    %SetCode
    // This is a *const, no setter
    %End
  };
//  QCPItemAnchor * const topLeft;
  QCPItemAnchor *topLeft
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
//  QCPItemAnchor * const topRight;
  QCPItemAnchor *topRight
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
//  QCPItemAnchor * const bottomRight;
  QCPItemAnchor *bottomRight
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
//  QCPItemAnchor * const bottomLeft;
  QCPItemAnchor *bottomLeft
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
  
protected:
  enum AnchorIndex {aiTopLeft, aiTop, aiTopRight, aiRight, aiBottomRight, aiBottom, aiBottomLeft, aiLeft};
  
  // property members:
//  QColor mColor, mSelectedColor;
//  QPen mPen, mSelectedPen;
//  QBrush mBrush, mSelectedBrush;
//  QFont mFont, mSelectedFont;
//  QString mText;
//  Qt::Alignment mPositionAlignment;
//  Qt::Alignment mTextAlignment;
//  double mRotation;
//  QMargins mPadding;
  
  // reimplemented virtual methods:
  virtual void draw(QCPPainter *painter);
  virtual QPointF anchorPixelPoint(int anchorId) const;
  
  // non-virtual methods:
  QPointF getTextDrawPoint(const QPointF &pos, const QRectF &rect, Qt::Alignment positionAlignment) const;
  QFont mainFont() const;
  QColor mainColor() const;
  QPen mainPen() const;
  QBrush mainBrush() const;
};

