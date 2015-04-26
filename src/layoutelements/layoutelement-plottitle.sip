
%ModuleCode
#include "qcustomplot.h"
%End

class QCPPlotTitle : QCPLayoutElement
{
%TypeHeaderCode
#include "qcustomplot.h"
%End

public:
  explicit QCPPlotTitle(QCustomPlot *parentPlot);
  explicit QCPPlotTitle(QCustomPlot *parentPlot, const QString &text);
  
  // getters:
  QString text() const;
  QFont font() const;
  QColor textColor() const;
  QFont selectedFont() const;
  QColor selectedTextColor() const;
  bool selectable() const;
  bool selected() const;
  
  // setters:
  void setText(const QString &text);
  void setFont(const QFont &font);
  void setTextColor(const QColor &color);
  void setSelectedFont(const QFont &font);
  void setSelectedTextColor(const QColor &color);
  Q_SLOT void setSelectable(bool selectable);
  Q_SLOT void setSelected(bool selected);
  
  // reimplemented virtual methods:
  virtual double selectTest(const QPointF &pos, bool onlySelectable, QVariant *details=0) const;
  
signals:
  void selectionChanged(bool selected);
  void selectableChanged(bool selectable);
  
protected:
  // property members:
//  QString mText;
//  QFont mFont;
//  QColor mTextColor;
//  QFont mSelectedFont;
//  QColor mSelectedTextColor;
//  QRect mTextBoundingRect;
//  bool mSelectable, mSelected;
  
  // reimplemented virtual methods:
  virtual void applyDefaultAntialiasingHint(QCPPainter *painter) const;
  virtual void draw(QCPPainter *painter);
  virtual QSize minimumSizeHint() const;
  virtual QSize maximumSizeHint() const;
  // events:
  virtual void selectEvent(QMouseEvent *event, bool additive, const QVariant &details, bool *selectionStateChanged);
  virtual void deselectEvent(bool *selectionStateChanged);
  
  // non-virtual methods:
  QFont mainFont() const;
  QColor mainTextColor() const;
  
//private:
//  Q_DISABLE_COPY(QCPPlotTitle)
//  QCPPlotTitle(const QCPPlotTitle &);
};

