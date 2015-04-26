
%ModuleCode
#include "qcustomplot.h"
%End

class QCPItemTracer : QCPAbstractItem
{
%TypeHeaderCode
#include "qcustomplot.h"
%End

public:
  enum TracerStyle { tsNone        ///< The tracer is not visible
                     ,tsPlus       ///< A plus shaped crosshair with limited size
                     ,tsCrosshair  ///< A plus shaped crosshair which spans the complete axis rect
                     ,tsCircle     ///< A circle
                     ,tsSquare     ///< A square
                   };
//  Q_ENUMS(TracerStyle)

  QCPItemTracer(QCustomPlot *parentPlot);
  virtual ~QCPItemTracer();

  // getters:
  QPen pen() const;
  QPen selectedPen() const;
  QBrush brush() const;
  QBrush selectedBrush() const;
  double size() const;
  TracerStyle style() const;
  QCPGraph *graph() const;
  double graphKey() const;
  bool interpolating() const;

  // setters;
  void setPen(const QPen &pen);
  void setSelectedPen(const QPen &pen);
  void setBrush(const QBrush &brush);
  void setSelectedBrush(const QBrush &brush);
  void setSize(double size);
  void setStyle(TracerStyle style);
  void setGraph(QCPGraph *graph);
  void setGraphKey(double key);
  void setInterpolating(bool enabled);

  // reimplemented virtual methods:
  virtual double selectTest(const QPointF &pos, bool onlySelectable, QVariant *details=0) const;
  
  // non-virtual methods:
  void updatePosition();

//  QCPItemPosition * const position;
  QCPItemPosition *position
  {
    %SetCode
    // This is a *const, no setter
    %End
  };

protected:
  // property members:
//  QPen mPen, mSelectedPen;
//  QBrush mBrush, mSelectedBrush;
//  double mSize;
//  TracerStyle mStyle;
//  QCPGraph *mGraph;
//  double mGraphKey;
//  bool mInterpolating;

  // reimplemented virtual methods:
  virtual void draw(QCPPainter *painter);

  // non-virtual methods:
  QPen mainPen() const;
  QBrush mainBrush() const;
};

