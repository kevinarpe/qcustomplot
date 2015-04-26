
%ModuleCode
#include "qcustomplot.h"
%End

class QCPScatterStyle
{
%TypeHeaderCode
#include "qcustomplot.h"
%End

public:
//  Q_ENUMS(ScatterShape)
  enum ScatterShape { ssNone       ///< no scatter symbols are drawn (e.g. in QCPGraph, data only represented with lines)
                      ,ssDot       ///< \enumimage{ssDot.png} a single pixel (use \ref ssDisc or \ref ssCircle if you want a round shape with a certain radius)
                      ,ssCross     ///< \enumimage{ssCross.png} a cross
                      ,ssPlus      ///< \enumimage{ssPlus.png} a plus
                      ,ssCircle    ///< \enumimage{ssCircle.png} a circle
                      ,ssDisc      ///< \enumimage{ssDisc.png} a circle which is filled with the pen's color (not the brush as with ssCircle)
                      ,ssSquare    ///< \enumimage{ssSquare.png} a square
                      ,ssDiamond   ///< \enumimage{ssDiamond.png} a diamond
                      ,ssStar      ///< \enumimage{ssStar.png} a star with eight arms, i.e. a combination of cross and plus
                      ,ssTriangle  ///< \enumimage{ssTriangle.png} an equilateral triangle, standing on baseline
                      ,ssTriangleInverted ///< \enumimage{ssTriangleInverted.png} an equilateral triangle, standing on corner
                      ,ssCrossSquare      ///< \enumimage{ssCrossSquare.png} a square with a cross inside
                      ,ssPlusSquare       ///< \enumimage{ssPlusSquare.png} a square with a plus inside
                      ,ssCrossCircle      ///< \enumimage{ssCrossCircle.png} a circle with a cross inside
                      ,ssPlusCircle       ///< \enumimage{ssPlusCircle.png} a circle with a plus inside
                      ,ssPeace     ///< \enumimage{ssPeace.png} a circle, with one vertical and two downward diagonal lines
                      ,ssPixmap    ///< a custom pixmap specified by \ref setPixmap, centered on the data point coordinates
                      ,ssCustom    ///< custom painter operations are performed per scatter (As QPainterPath, see \ref setCustomPath)
                    };

  QCPScatterStyle();
  QCPScatterStyle(ScatterShape shape, double size=6);
  QCPScatterStyle(ScatterShape shape, const QColor &color, double size);
  QCPScatterStyle(ScatterShape shape, const QColor &color, const QColor &fill, double size);
  QCPScatterStyle(ScatterShape shape, const QPen &pen, const QBrush &brush, double size);
  QCPScatterStyle(const QPixmap &pixmap);
  QCPScatterStyle(const QPainterPath &customPath, const QPen &pen, const QBrush &brush=Qt::NoBrush, double size=6);
  
  // getters:
  double size() const;
  ScatterShape shape() const;
  QPen pen() const;
  QBrush brush() const;
  QPixmap pixmap() const;
  QPainterPath customPath() const;

  // setters:
  void setSize(double size);
  void setShape(ScatterShape shape);
  void setPen(const QPen &pen);
  void setBrush(const QBrush &brush);
  void setPixmap(const QPixmap &pixmap);
  void setCustomPath(const QPainterPath &customPath);

  // non-property methods:
  bool isNone() const;
  bool isPenDefined() const;
  void applyTo(QCPPainter *painter, const QPen &defaultPen) const;
  void drawShape(QCPPainter *painter, QPointF pos) const;
  void drawShape(QCPPainter *painter, double x, double y) const;

//protected:
  // property members:
//  double mSize;
//  ScatterShape mShape;
//  QPen mPen;
//  QBrush mBrush;
//  QPixmap mPixmap;
//  QPainterPath mCustomPath;
  
  // non-property members:
//  bool mPenDefined;
};

class QCPPainter : QPainter
{
%TypeHeaderCode
#include "qcustomplot.h"
%End

public:
  enum PainterMode { pmDefault       = 0x00   ///< <tt>0x00</tt> Default mode for painting on screen devices
                     ,pmVectorized   = 0x01   ///< <tt>0x01</tt> Mode for vectorized painting (e.g. PDF export). For example, this prevents some antialiasing fixes.
                     ,pmNoCaching    = 0x02   ///< <tt>0x02</tt> Mode for all sorts of exports (e.g. PNG, PDF,...). For example, this prevents using cached pixmap labels
                     ,pmNonCosmetic  = 0x04   ///< <tt>0x04</tt> Turns pen widths 0 to 1, i.e. disables cosmetic pens. (A cosmetic pen is always drawn with width 1 pixel in the vector image/pdf viewer, independent of zoom.)
                   };
//  Q_FLAGS(PainterMode PainterModes)
//  Q_DECLARE_FLAGS(PainterModes, PainterMode)
  typedef QFlags<QCPPainter::PainterMode> PainterModes;
  
  QCPPainter();
  QCPPainter(QPaintDevice *device);
  ~QCPPainter();
  
  // getters:
  bool antialiasing() const;
  PainterModes modes() const;

  // setters:
  void setAntialiasing(bool enabled);
  void setMode(PainterMode mode, bool enabled=true);
  void setModes(PainterModes modes);

  // methods hiding non-virtual base class functions (QPainter bug workarounds):
  bool begin(QPaintDevice *device);
  void setPen(const QPen &pen);
  void setPen(const QColor &color);
  void setPen(Qt::PenStyle penStyle);
  void drawLine(const QLineF &line);
  void drawLine(const QPointF &p1, const QPointF &p2);
  void save();
  void restore();
  
  // non-virtual methods:
  void makeNonCosmetic();
  
//protected:
  // property members:
//  PainterModes mModes;
//  bool mIsAntialiasing;
  
  // non-property members:
//  QStack<bool> mAntialiasingStack;
};

