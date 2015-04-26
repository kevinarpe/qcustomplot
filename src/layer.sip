
%ModuleCode
#include "qcustomplot.h"
%End

class QCPLayer : QObject
{
%TypeHeaderCode
#include "qcustomplot.h"
%End

public:
  QCPLayer(QCustomPlot* parentPlot, const QString &layerName);
  ~QCPLayer();
  
  // getters:
  QCustomPlot *parentPlot() const;
  QString name() const;
  int index() const;
  QList<QCPLayerable*> children() const;
  bool visible() const;
  
  // setters:
  void setVisible(bool visible);
  
protected:
  // property members:
//  QCustomPlot *mParentPlot;
//  QString mName;
//  int mIndex;
//  QList<QCPLayerable*> mChildren;
//  bool mVisible;
  
  // non-virtual methods:
  void addChild(QCPLayerable *layerable, bool prepend);
  void removeChild(QCPLayerable *layerable);
  
//private:
//  Q_DISABLE_COPY(QCPLayer)
//  QCPLayer(const QCPLayer &);
  
//  friend class QCustomPlot;
//  friend class QCPLayerable;
};

class QCPLayerable : QObject
{
%TypeHeaderCode
#include "qcustomplot.h"
%End

public:
  QCPLayerable(QCustomPlot *plot, QString targetLayer=QString(), QCPLayerable *parentLayerable=0);
  ~QCPLayerable();
  
  // getters:
  bool visible() const;
  QCustomPlot *parentPlot() const;
  QCPLayerable *parentLayerable() const;
  QCPLayer *layer() const;
  bool antialiased() const;
  
  // setters:
  void setVisible(bool on);
  Q_SLOT bool setLayer(QCPLayer *layer);
  bool setLayer(const QString &layerName);
  void setAntialiased(bool enabled);
  
  // introduced virtual methods:
  virtual double selectTest(const QPointF &pos, bool onlySelectable, QVariant *details=0) const;
  
  // non-property methods:
  bool realVisibility() const;
  
signals:
  void layerChanged(QCPLayer *newLayer);
  
protected:
  // property members:
//  bool mVisible;
//  QCustomPlot *mParentPlot;
//  QPointer<QCPLayerable> mParentLayerable;
//  QCPLayer *mLayer;
//  bool mAntialiased;
  
  // introduced virtual methods:
  virtual void parentPlotInitialized(QCustomPlot *parentPlot);
  virtual QCP::Interaction selectionCategory() const;
  virtual QRect clipRect() const;
  virtual void applyDefaultAntialiasingHint(QCPPainter *painter) const = 0;
  virtual void draw(QCPPainter *painter) = 0;
  // events:
  virtual void selectEvent(QMouseEvent *event, bool additive, const QVariant &details, bool *selectionStateChanged);
  virtual void deselectEvent(bool *selectionStateChanged);
  
  // non-property methods:
  void initializeParentPlot(QCustomPlot *parentPlot);
  void setParentLayerable(QCPLayerable* parentLayerable);
  bool moveToLayer(QCPLayer *layer, bool prepend);
  void applyAntialiasingHint(QCPPainter *painter, bool localAntialiased, QCP::AntialiasedElement overrideElement) const;
  
//private:
//  Q_DISABLE_COPY(QCPLayerable)
//  QCPLayerable(const QCPLayerable &);
  
//  friend class QCustomPlot;
//  friend class QCPAxisRect;
};

