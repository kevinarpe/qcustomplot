#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QMetaEnum>
#include <QTimer>
#include <QDir>
#include "../../src/qcp.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT
  
public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();
  
public slots:
  // image generators:
  void genScatterStyles();
  void genItemPixmap();
  void genItemRect();
  void genItemEllipse();
  void genItemLine();
  void genItemStraightLIne();
  void genItemCurve();
  void genItemBracket();
  void genItemText();
  void genItemTracer();
  void genLineEnding();
  void genMarginGroup();
  void genAxisRectSpacingOverview();
  void genAxisNamesOverview();
  
private:
  // helpers:
  void labelItemAnchors(QCPAbstractItem *item, double fontSize=8, bool circle=true, bool labelBelow=true);
  void addBracket(QPointF left, QPointF right, QString text, QPointF textOffset, bool textSideways, Qt::Alignment textAlign, QCPItemBracket::BracketStyle style=QCPItemBracket::bsRound);
  void addArrow(QPointF target, QPointF textPosition, QString text, Qt::Alignment textAlign=Qt::AlignCenter);
  void resetPlot();
  
  Ui::MainWindow *ui;
  QCustomPlot *customPlot;
  QDir dir; // target directory for images
  QBrush defaultBrush;
};

#endif // MAINWINDOW_H
