
%ModuleCode
#include "qcustomplot.h"
%End

namespace QCP
{
%TypeHeaderCode
#include "qcustomplot.h"
%End

enum MarginSide { msLeft     = 0x01 ///< <tt>0x01</tt> left margin
                  ,msRight   = 0x02 ///< <tt>0x02</tt> right margin
                  ,msTop     = 0x04 ///< <tt>0x04</tt> top margin
                  ,msBottom  = 0x08 ///< <tt>0x08</tt> bottom margin
                  ,msAll     = 0xFF ///< <tt>0xFF</tt> all margins
                  ,msNone    = 0x00 ///< <tt>0x00</tt> no margin
                };
//Q_DECLARE_FLAGS(MarginSides, MarginSide)
typedef QFlags<QCP::MarginSide> MarginSides;
// Ref: qtbase/src/corelib/global/qflags.h
// #define Q_DECLARE_FLAGS(Flags, Enum)\
// typedef QFlags<Enum> Flags;

enum AntialiasedElement { aeAxes           = 0x0001 ///< <tt>0x0001</tt> Axis base line and tick marks
                          ,aeGrid          = 0x0002 ///< <tt>0x0002</tt> Grid lines
                          ,aeSubGrid       = 0x0004 ///< <tt>0x0004</tt> Sub grid lines
                          ,aeLegend        = 0x0008 ///< <tt>0x0008</tt> Legend box
                          ,aeLegendItems   = 0x0010 ///< <tt>0x0010</tt> Legend items
                          ,aePlottables    = 0x0020 ///< <tt>0x0020</tt> Main lines of plottables (excluding error bars, see element \ref aeErrorBars)
                          ,aeItems         = 0x0040 ///< <tt>0x0040</tt> Main lines of items
                          ,aeScatters      = 0x0080 ///< <tt>0x0080</tt> Scatter symbols of plottables (excluding scatter symbols of type ssPixmap)
                          ,aeErrorBars     = 0x0100 ///< <tt>0x0100</tt> Error bars
                          ,aeFills         = 0x0200 ///< <tt>0x0200</tt> Borders of fills (e.g. under or between graphs)
                          ,aeZeroLine      = 0x0400 ///< <tt>0x0400</tt> Zero-lines, see \ref QCPGrid::setZeroLinePen
                          ,aeAll           = 0xFFFF ///< <tt>0xFFFF</tt> All elements
                          ,aeNone          = 0x0000 ///< <tt>0x0000</tt> No elements
                        };
//Q_DECLARE_FLAGS(AntialiasedElements, AntialiasedElement)
typedef QFlags<QCP::AntialiasedElement> AntialiasedElements;

enum PlottingHint { phNone            = 0x000 ///< <tt>0x000</tt> No hints are set
                    ,phFastPolylines  = 0x001 ///< <tt>0x001</tt> Graph/Curve lines are drawn with a faster method. This reduces the quality
                                              ///<                especially of the line segment joins. (Only relevant for solid line pens.)
                    ,phForceRepaint   = 0x002 ///< <tt>0x002</tt> causes an immediate repaint() instead of a soft update() when QCustomPlot::replot() is called with parameter \ref QCustomPlot::rpHint.
                                              ///<                This is set by default to prevent the plot from freezing on fast consecutive replots (e.g. user drags ranges with mouse).
                    ,phCacheLabels    = 0x004 ///< <tt>0x004</tt> axis (tick) labels will be cached as pixmaps, increasing replot performance.
                  };
//Q_DECLARE_FLAGS(PlottingHints, PlottingHint)
typedef QFlags<QCP::PlottingHint> PlottingHints;

enum Interaction { iRangeDrag         = 0x001 ///< <tt>0x001</tt> Axis ranges are draggable (see \ref QCPAxisRect::setRangeDrag, \ref QCPAxisRect::setRangeDragAxes)
                   ,iRangeZoom        = 0x002 ///< <tt>0x002</tt> Axis ranges are zoomable with the mouse wheel (see \ref QCPAxisRect::setRangeZoom, \ref QCPAxisRect::setRangeZoomAxes)
                   ,iMultiSelect      = 0x004 ///< <tt>0x004</tt> The user can select multiple objects by holding the modifier set by \ref QCustomPlot::setMultiSelectModifier while clicking
                   ,iSelectPlottables = 0x008 ///< <tt>0x008</tt> Plottables are selectable (e.g. graphs, curves, bars,... see QCPAbstractPlottable)
                   ,iSelectAxes       = 0x010 ///< <tt>0x010</tt> Axes are selectable (or parts of them, see QCPAxis::setSelectableParts)
                   ,iSelectLegend     = 0x020 ///< <tt>0x020</tt> Legends are selectable (or their child items, see QCPLegend::setSelectableParts)
                   ,iSelectItems      = 0x040 ///< <tt>0x040</tt> Items are selectable (Rectangles, Arrows, Textitems, etc. see \ref QCPAbstractItem)
                   ,iSelectOther      = 0x080 ///< <tt>0x080</tt> All other objects are selectable (e.g. your own derived layerables, the plot title,...)
                 };
//Q_DECLARE_FLAGS(Interactions, Interaction)
typedef QFlags<QCP::Interaction> Interactions;

bool isInvalidData(double value);
bool isInvalidData(double value1, double value2);
void setMarginValue(QMargins &margins, QCP::MarginSide side, int value);
int getMarginValue(const QMargins &margins, QCP::MarginSide side);

//Q_DECLARE_OPERATORS_FOR_FLAGS(QCP::AntialiasedElements)
//QFlags<QCP::AntialiasedElements::enum_type> operator|(QCP::AntialiasedElements::enum_type f1, QCP::AntialiasedElements::enum_type f2);
//Q_DECLARE_OPERATORS_FOR_FLAGS(QCP::PlottingHints)
//QFlags<QCP::PlottingHints::enum_type> operator|(QCP::PlottingHints::enum_type f1, QCP::PlottingHints::enum_type f2);
//Q_DECLARE_OPERATORS_FOR_FLAGS(QCP::MarginSides)
//QFlags<QCP::MarginSides::enum_type> operator|(QCP::MarginSides::enum_type f1, QCP::MarginSides::enum_type f2);
//Q_DECLARE_OPERATORS_FOR_FLAGS(QCP::Interactions)
//QFlags<QCP::Interactions::enum_type> operator|(QCP::Interactions::enum_type f1, QCP::Interactions::enum_type f2);
// Ref: qtbase/src/corelib/global/qflags.h
// #define Q_DECLARE_OPERATORS_FOR_FLAGS(Flags) \
// Q_DECL_CONSTEXPR inline QFlags<Flags::enum_type> operator|(Flags::enum_type f1, Flags::enum_type f2) Q_DECL_NOTHROW \
// { return QFlags<Flags::enum_type>(f1) | f2; } \
// Q_DECL_CONSTEXPR inline QFlags<Flags::enum_type> operator|(Flags::enum_type f1, QFlags<Flags::enum_type> f2) Q_DECL_NOTHROW \
// { return f2 | f1; } Q_DECLARE_INCOMPATIBLE_FLAGS(Flags)

};

