
%ModuleCode
#include "qcustomplot.h"
%End

class QCPItemPixmap : QCPAbstractItem
{
%TypeHeaderCode
#include "qcustomplot.h"
%End

public:
  QCPItemPixmap(QCustomPlot *parentPlot);
  virtual ~QCPItemPixmap();
  
  // getters:
  QPixmap pixmap() const;
  bool scaled() const;
  Qt::AspectRatioMode aspectRatioMode() const;
  Qt::TransformationMode transformationMode() const;
  QPen pen() const;
  QPen selectedPen() const;
  
  // setters;
  void setPixmap(const QPixmap &pixmap);
  void setScaled(bool scaled, Qt::AspectRatioMode aspectRatioMode=Qt::KeepAspectRatio, Qt::TransformationMode transformationMode=Qt::SmoothTransformation);
  void setPen(const QPen &pen);
  void setSelectedPen(const QPen &pen);
  
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
  enum AnchorIndex {aiTop, aiTopRight, aiRight, aiBottom, aiBottomLeft, aiLeft};
  
  // property members:
//  QPixmap mPixmap;
//  QPixmap mScaledPixmap;
//  bool mScaled;
//  Qt::AspectRatioMode mAspectRatioMode;
//  Qt::TransformationMode mTransformationMode;
//  QPen mPen, mSelectedPen;
  
  // reimplemented virtual methods:
  virtual void draw(QCPPainter *painter);
  virtual QPointF anchorPixelPoint(int anchorId) const;
  
  // non-virtual methods:
  void updateScaledPixmap(QRect finalRect=QRect(), bool flipHorz=false, bool flipVert=false);
  QRect getFinalRect(bool *flippedHorz=0, bool *flippedVert=0) const;
  QPen mainPen() const;
};

