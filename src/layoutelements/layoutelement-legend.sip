
%ModuleCode
#include "qcustomplot.h"
%End

class QCPAbstractLegendItem : QCPLayoutElement /Abstract/
{
%TypeHeaderCode
#include "qcustomplot.h"
%End

public:
  explicit QCPAbstractLegendItem(QCPLegend *parent);
  
  // getters:
  QCPLegend *parentLegend() const;
  QFont font() const;
  QColor textColor() const;
  QFont selectedFont() const;
  QColor selectedTextColor() const;
  bool selectable() const;
  bool selected() const;
  
  // setters:
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
//  QCPLegend *mParentLegend;
//  QFont mFont;
//  QColor mTextColor;
//  QFont mSelectedFont;
//  QColor mSelectedTextColor;
//  bool mSelectable, mSelected;
  
  // reimplemented virtual methods:
  virtual QCP::Interaction selectionCategory() const;
  virtual void applyDefaultAntialiasingHint(QCPPainter *painter) const;
  virtual QRect clipRect() const;
  virtual void draw(QCPPainter *painter) = 0;
  // events:
  virtual void selectEvent(QMouseEvent *event, bool additive, const QVariant &details, bool *selectionStateChanged);
  virtual void deselectEvent(bool *selectionStateChanged);
  
//private:
//  Q_DISABLE_COPY(QCPAbstractLegendItem)
//  QCPAbstractLegendItem(const QCPAbstractLegendItem &);
  
//  friend class QCPLegend;
};


class QCPPlottableLegendItem : QCPAbstractLegendItem
{
%TypeHeaderCode
#include "qcustomplot.h"
%End

public:
  QCPPlottableLegendItem(QCPLegend *parent, QCPAbstractPlottable *plottable);
  
  // getters:
  QCPAbstractPlottable *plottable();
  
protected:
  // property members:
//  QCPAbstractPlottable *mPlottable;
  
  // reimplemented virtual methods:
  virtual void draw(QCPPainter *painter);
  virtual QSize minimumSizeHint() const;
  
  // non-virtual methods:
  QPen getIconBorderPen() const;
  QColor getTextColor() const;
  QFont getFont() const;
};


class QCPLegend : QCPLayoutGrid
{
%TypeHeaderCode
#include "qcustomplot.h"
%End

public:
  enum SelectablePart { spNone       = 0x000  ///< <tt>0x000</tt> None
                        ,spLegendBox  = 0x001 ///< <tt>0x001</tt> The legend box (frame)
                        ,spItems      = 0x002 ///< <tt>0x002</tt> Legend items individually (see \ref selectedItems)
                      };
//  Q_FLAGS(SelectablePart SelectableParts)
//  Q_DECLARE_FLAGS(SelectableParts, SelectablePart)
  typedef QFlags<QCPLegend::SelectablePart> SelectableParts;
  
  explicit QCPLegend();
  virtual ~QCPLegend();
  
  // getters:
  QPen borderPen() const;
  QBrush brush() const;
  QFont font() const;
  QColor textColor() const;
  QSize iconSize() const;
  int iconTextPadding() const;
  QPen iconBorderPen() const;
  SelectableParts selectableParts() const;
  SelectableParts selectedParts() const;
  QPen selectedBorderPen() const;
  QPen selectedIconBorderPen() const;
  QBrush selectedBrush() const;
  QFont selectedFont() const;
  QColor selectedTextColor() const;
  
  // setters:
  void setBorderPen(const QPen &pen);
  void setBrush(const QBrush &brush);
  void setFont(const QFont &font);
  void setTextColor(const QColor &color);
  void setIconSize(const QSize &size);
  void setIconSize(int width, int height);
  void setIconTextPadding(int padding);
  void setIconBorderPen(const QPen &pen);
  Q_SLOT void setSelectableParts(const SelectableParts &selectableParts);
  Q_SLOT void setSelectedParts(const SelectableParts &selectedParts);
  void setSelectedBorderPen(const QPen &pen);
  void setSelectedIconBorderPen(const QPen &pen);
  void setSelectedBrush(const QBrush &brush);
  void setSelectedFont(const QFont &font);
  void setSelectedTextColor(const QColor &color);
  
  // reimplemented virtual methods:
  virtual double selectTest(const QPointF &pos, bool onlySelectable, QVariant *details=0) const;
  
  // non-virtual methods:
  QCPAbstractLegendItem *item(int index) const;
  QCPPlottableLegendItem *itemWithPlottable(const QCPAbstractPlottable *plottable) const;
  int itemCount() const;
  bool hasItem(QCPAbstractLegendItem *item) const;
  bool hasItemWithPlottable(const QCPAbstractPlottable *plottable) const;
  bool addItem(QCPAbstractLegendItem *item);
  bool removeItem(int index);
  bool removeItem(QCPAbstractLegendItem *item);
  void clearItems();
  QList<QCPAbstractLegendItem*> selectedItems() const;
  
signals:
  void selectionChanged(QCPLegend::SelectableParts parts);
  void selectableChanged(QCPLegend::SelectableParts parts);
  
protected:
  // property members:
//  QPen mBorderPen, mIconBorderPen;
//  QBrush mBrush;
//  QFont mFont;
//  QColor mTextColor;
//  QSize mIconSize;
//  int mIconTextPadding;
//  SelectableParts mSelectedParts, mSelectableParts;
//  QPen mSelectedBorderPen, mSelectedIconBorderPen;
//  QBrush mSelectedBrush;
//  QFont mSelectedFont;
//  QColor mSelectedTextColor;
  
  // reimplemented virtual methods:
  virtual void parentPlotInitialized(QCustomPlot *parentPlot);
  virtual QCP::Interaction selectionCategory() const;
  virtual void applyDefaultAntialiasingHint(QCPPainter *painter) const;
  virtual void draw(QCPPainter *painter);
  // events:
  virtual void selectEvent(QMouseEvent *event, bool additive, const QVariant &details, bool *selectionStateChanged);
  virtual void deselectEvent(bool *selectionStateChanged);
  
  // non-virtual methods:
  QPen getBorderPen() const;
  QBrush getBrush() const;
  
//private:
//  Q_DISABLE_COPY(QCPLegend)
//  QCPLegend(const QCPLegend &);
  
//  friend class QCustomPlot;
//  friend class QCPAbstractLegendItem;
};
//Q_DECLARE_OPERATORS_FOR_FLAGS(QCPLegend::SelectableParts)
//Q_DECLARE_METATYPE(QCPLegend::SelectablePart)

