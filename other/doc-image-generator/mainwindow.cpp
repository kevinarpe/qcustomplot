#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow),
  defaultBrush(QColor(80, 215, 10, 70))
{
  ui->setupUi(this);
  setGeometry(300, 300, 500, 500);
  
  dir.setPath(qApp->applicationDirPath());
  dir.mkdir("images");
  if (!dir.cd("images"))
  {
    QMessageBox::critical(this, "Error", tr("Couldn't create and access image directory:\n%1").arg(dir.filePath("images")));
  } else
  {
    // invoke all methods of MainWindow that start with "gen":
    for (int i=this->metaObject()->methodOffset(); i<this->metaObject()->methodCount(); ++i)
    {
      if (QString::fromLatin1(this->metaObject()->method(i).signature()).startsWith("gen"))
      {
        if (!this->metaObject()->method(i).invoke(this))
          qDebug() << "Failed to invoke doc-image-generator method" << i;
      }
    }
  }
  QTimer::singleShot(0, qApp, SLOT(quit()));
}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::genScatterStyles()
{
  resetPlot();
  // Different scatter styles in separate images:
  customPlot->xAxis->setRange(-1, 1);
  customPlot->yAxis->setRange(-1, 1);
  customPlot->addGraph();
  customPlot->graph()->addData(0, 0);
  customPlot->graph()->setPen(QPen(Qt::black));
  customPlot->setBackground(Qt::transparent);
  QMap<QCPScatterStyle::ScatterShape, QString> scatterShapes;
  scatterShapes.insert(QCPScatterStyle::ssDot, "ssDot");
  scatterShapes.insert(QCPScatterStyle::ssCross, "ssCross");
  scatterShapes.insert(QCPScatterStyle::ssPlus, "ssPlus");
  scatterShapes.insert(QCPScatterStyle::ssCircle, "ssCircle");
  scatterShapes.insert(QCPScatterStyle::ssDisc, "ssDisc");
  scatterShapes.insert(QCPScatterStyle::ssSquare, "ssSquare");
  scatterShapes.insert(QCPScatterStyle::ssDiamond, "ssDiamond");
  scatterShapes.insert(QCPScatterStyle::ssStar, "ssStar");
  scatterShapes.insert(QCPScatterStyle::ssTriangle, "ssTriangle");
  scatterShapes.insert(QCPScatterStyle::ssTriangleInverted, "ssTriangleInverted");
  scatterShapes.insert(QCPScatterStyle::ssCrossSquare, "ssCrossSquare");
  scatterShapes.insert(QCPScatterStyle::ssPlusSquare, "ssPlusSquare");
  scatterShapes.insert(QCPScatterStyle::ssCrossCircle, "ssCrossCircle");
  scatterShapes.insert(QCPScatterStyle::ssPlusCircle, "ssPlusCircle");
  scatterShapes.insert(QCPScatterStyle::ssPeace, "ssPeace");
  QMapIterator<QCPScatterStyle::ScatterShape, QString> scatterIt(scatterShapes);
  while (scatterIt.hasNext())
  {
    scatterIt.next();
    customPlot->graph()->setScatterStyle(QCPScatterStyle(scatterIt.key(), 8.5));
    QPixmap pm = customPlot->toPixmap(16, 16);
    pm.save(dir.filePath(scatterIt.value()+".png"));
  }
}

void MainWindow::genItemPixmap()
{
  resetPlot();
  QCPItemPixmap *pixmapItem = new QCPItemPixmap(customPlot);
  customPlot->addItem(pixmapItem);
  pixmapItem->setPixmap(QPixmap("./gnu.png"));
  pixmapItem->setScaled(true, Qt::IgnoreAspectRatio);
  pixmapItem->topLeft->setCoords(-0.2, 1);
  pixmapItem->bottomRight->setCoords(0.3, 0);
  labelItemAnchors(pixmapItem);
  QCPItemPixmap *pixmapItem2 = new QCPItemPixmap(customPlot);
  customPlot->addItem(pixmapItem2);
  pixmapItem2->setPixmap(QPixmap("./gnu.png"));
  pixmapItem2->setScaled(true, Qt::IgnoreAspectRatio);
  pixmapItem2->topLeft->setCoords(1.2, 0);
  pixmapItem2->bottomRight->setCoords(0.7, 1);
  labelItemAnchors(pixmapItem2);
  customPlot->savePng(dir.filePath("QCPItemPixmap.png"), 460, 160);
}

void MainWindow::genItemRect()
{
  resetPlot();
  QCPItemRect *rect = new QCPItemRect(customPlot);
  customPlot->addItem(rect);
  rect->setBrush(defaultBrush);
  rect->topLeft->setCoords(0, 1);
  rect->bottomRight->setCoords(1, 0);
  labelItemAnchors(rect);
  customPlot->savePng(dir.filePath("QCPItemRect.png"), 230, 160);
}

void MainWindow::genItemEllipse()
{
  resetPlot();
  QCPItemEllipse *ellipse = new QCPItemEllipse(customPlot);
  customPlot->addItem(ellipse);
  ellipse->setBrush(defaultBrush);
  ellipse->topLeft->setCoords(-0.15, 1.1);
  ellipse->bottomRight->setCoords(1.15, 0);
  labelItemAnchors(ellipse);
  customPlot->savePng(dir.filePath("QCPItemEllipse.png"), 300, 200);
}

void MainWindow::genItemLine()
{
  resetPlot();
  QCPItemLine *line = new QCPItemLine(customPlot);
  customPlot->addItem(line);
  line->start->setCoords(-0.1, 0.8);
  line->end->setCoords(1.1, 0.2);
  line->setHead(QCPLineEnding::esSpikeArrow);
  labelItemAnchors(line);
  customPlot->savePng(dir.filePath("QCPItemLine.png"), 230, 160);
}

void MainWindow::genItemStraightLIne()
{
  resetPlot();
  QCPItemStraightLine *straightLine = new QCPItemStraightLine(customPlot);
  customPlot->addItem(straightLine);
  straightLine->point1->setCoords(0, 0.4);
  straightLine->point2->setCoords(1, 0.6);
  labelItemAnchors(straightLine);
  customPlot->savePng(dir.filePath("QCPItemStraightLine.png"), 230, 160);
}

void MainWindow::genItemCurve()
{
  resetPlot();
  QCPItemCurve *curve = new QCPItemCurve(customPlot);
  customPlot->addItem(curve);
  curve->start->setCoords(0, 1);
  curve->startDir->setCoords(0.5, 1);
  curve->endDir->setCoords(0.7, 0.2);
  curve->end->setCoords(1, 0);
  curve->setHead(QCPLineEnding::esSpikeArrow);
  curve->setTail(QCPLineEnding::esLineArrow);
  labelItemAnchors(curve);
  customPlot->savePng(dir.filePath("QCPItemCurve.png"), 230, 160);
}

void MainWindow::genItemBracket()
{
  resetPlot();
  QCPItemBracket *bracket = new QCPItemBracket(customPlot);
  customPlot->addItem(bracket);
  bracket->left->setCoords(-0.2, 0.35);
  bracket->right->setCoords(1.2, 0.65);
  bracket->setLength(12);
  labelItemAnchors(bracket, 8, true, false);
  customPlot->savePng(dir.filePath("QCPItemBracket.png"), 230, 160);
  customPlot->clearItems();
  
  // generate image of multiple bracket lengths/styles:
  for (int i=0; i<4; ++i)
  {
    QCPItemBracket *bracket = new QCPItemBracket(customPlot);
    customPlot->addItem(bracket);
    bracket->setStyle(QCPItemBracket::bsCalligraphic);
    bracket->left->setCoords(-0.35+i*0.18, 0.95);
    bracket->right->setCoords(-0.15+i*0.18, 0.05);
    bracket->setLength(10+i*5);
    labelItemAnchors(bracket, 0, true, false);
    QCPItemText *label = new QCPItemText(customPlot);
    customPlot->addItem(label);
    label->setText(QString::number(bracket->length()));
    label->position->setParentAnchor(bracket->right);
    label->position->setCoords(-5, 20);
    label->setFont(QFont(font().family(), 9));
  }
  for (int i=0; i<4; ++i)
  {
    QCPItemBracket *bracket = new QCPItemBracket(customPlot);
    customPlot->addItem(bracket);
    bracket->setStyle(QCPItemBracket::bsSquare);
    bracket->left->setCoords(0.55+i*0.18, 0.95);
    bracket->right->setCoords(0.75+i*0.18, 0.05);
    bracket->setLength(10+i*5);
    labelItemAnchors(bracket, 0, true, false);
    QCPItemText *label = new QCPItemText(customPlot);
    customPlot->addItem(label);
    label->setText(QString::number(bracket->length()));
    label->position->setParentAnchor(bracket->right);
    label->position->setCoords(-5, 20);
    label->setFont(QFont(font().family(), 9));
  }
  QCPItemText *topLabel1 = new QCPItemText(customPlot);
  customPlot->addItem(topLabel1);
  topLabel1->setText("bsCalligraphic");
  topLabel1->position->setCoords(-0.05, 1.1);
  topLabel1->setFont(QFont(font().family(), 10));
  QCPItemText *topLabel2 = new QCPItemText(customPlot);
  customPlot->addItem(topLabel2);
  topLabel2->setText("bsSquare");
  topLabel2->position->setCoords(0.85, 1.1);
  topLabel2->setFont(QFont(font().family(), 10));
  customPlot->savePng(dir.filePath("QCPItemBracket-length.png"), 450, 200);
}

void MainWindow::genItemText()
{
  resetPlot();
  QCPItemText *text = new QCPItemText(customPlot);
  customPlot->addItem(text);
  text->position->setCoords(0.5, 0.5);
  text->setText("QCustomPlot\nWidget");
  text->setFont(QFont(font().family(), 24));
  text->setRotation(12);
  text->setBrush(defaultBrush);
  labelItemAnchors(text);
  customPlot->savePng(dir.filePath("QCPItemText.png"), 300, 170);
}

void MainWindow::genItemTracer()
{
  resetPlot();
  customPlot->addGraph();
  QVector<double> x(50), y(50);
  for (int i=0; i<50; ++i)
  {
    x[i] = -0.4+1.8*i/49.0;
    y[i] = qSin(x[i]*M_PI*0.9)*0.4+0.4;
  }
  customPlot->graph()->setData(x, y);
  customPlot->graph()->setPen(QPen(Qt::red));
  QCPItemTracer *tracer = new QCPItemTracer(customPlot);
  customPlot->addItem(tracer);
  tracer->setStyle(QCPItemTracer::tsCrosshair);
  tracer->setGraph(customPlot->graph());
  tracer->setGraphKey(0.8);
  labelItemAnchors(tracer, 8, true, false);
  customPlot->savePng(dir.filePath("QCPItemTracer.png"), 230, 160);
}

void MainWindow::genLineEnding()
{
  resetPlot();
  QMetaEnum endingStyleEnum = QCPLineEnding::staticMetaObject.enumerator(QCPLineEnding::staticMetaObject.indexOfEnumerator("EndingStyle"));
  double offset = -0.2;
  double step = 1.4/((double)endingStyleEnum.keyCount()-1);
  for (int i=0; i<endingStyleEnum.keyCount(); ++i)
  {
    QCPLineEnding ending(static_cast<QCPLineEnding::EndingStyle>(endingStyleEnum.value(i)));
    QString endingName(endingStyleEnum.key(i)); 
    
    if (ending.style() == QCPLineEnding::esSkewedBar)
      ending.setInverted(true);
    
    QCPItemLine *line = new QCPItemLine(customPlot);
    line->setPen(QPen(Qt::black, 0, Qt::SolidLine, Qt::FlatCap));
    customPlot->addItem(line);
    line->start->setCoords(offset+i*step-0.1, -0.2);
    line->end->setCoords(offset+i*step, 0.5);
    line->setHead(ending);
    QCPItemText *text = new QCPItemText(customPlot);
    customPlot->addItem(text);
    text->position->setParentAnchor(line->end);
    text->position->setCoords(8, -15-(i%2)*15);
    text->setFont(QFont(font().family(), 8));
    text->setText(endingName);
  }

  customPlot->savePng(dir.filePath("QCPLineEnding.png"), 500, 100);
}

void MainWindow::genMarginGroup()
{
  resetPlot();
  
  customPlot->plotLayout()->clear();
  customPlot->plotLayout()->addElement(0, 0, new QCPAxisRect(customPlot));
  customPlot->plotLayout()->addElement(0, 1, new QCPAxisRect(customPlot));
  customPlot->plotLayout()->addElement(1, 0, new QCPAxisRect(customPlot));
  customPlot->plotLayout()->addElement(1, 1, new QCPAxisRect(customPlot));
  
  foreach (QCPAxisRect *r, customPlot->axisRects())
    r->axis(QCPAxis::atBottom)->setTickLabels(false);
  
  QCPMarginGroup *marginGroup = new QCPMarginGroup(customPlot);
  customPlot->axisRect(0)->setMarginGroup(QCP::msLeft, marginGroup);
  customPlot->axisRect(2)->setMarginGroup(QCP::msLeft, marginGroup);
  
  customPlot->axisRect(0)->axis(QCPAxis::atLeft)->setRange(0, 1300);
  customPlot->axisRect(1)->axis(QCPAxis::atLeft)->setRange(0, 1300);
  customPlot->axisRect(0)->axis(QCPAxis::atLeft)->setLabel("y");
  customPlot->axisRect(1)->axis(QCPAxis::atLeft)->setLabel("y");
  
  customPlot->plotLayout()->setAutoMargins(QCP::msLeft|QCP::msRight|QCP::msBottom);
  customPlot->plotLayout()->setMargins(QMargins(0, 25, 0, 0));
  
  QFont textFont;
  textFont.setBold(true);
  QCPItemText *leftCaption = new QCPItemText(customPlot);
  customPlot->addItem(leftCaption);
  leftCaption->position->setType(QCPItemPosition::ptViewportRatio);
  leftCaption->setClipToAxisRect(false);
  leftCaption->position->setCoords(0.25, 0);
  leftCaption->setPositionAlignment(Qt::AlignTop|Qt::AlignHCenter);
  leftCaption->setText("left sides in margin group");
  leftCaption->setFont(textFont);
  QCPItemText *rightCaption = new QCPItemText(customPlot);
  customPlot->addItem(rightCaption);
  rightCaption->position->setType(QCPItemPosition::ptViewportRatio);
  rightCaption->position->setCoords(0.75, 0);
  rightCaption->setClipToAxisRect(false);
  rightCaption->setPositionAlignment(Qt::AlignTop|Qt::AlignHCenter);
  rightCaption->setText("no margin group");
  rightCaption->setFont(textFont);
  
  QCPItemLine *splitter = new QCPItemLine(customPlot);
  splitter->start->setType(QCPItemPosition::ptViewportRatio);
  splitter->start->setCoords(0.5, 0);
  splitter->end->setType(QCPItemPosition::ptViewportRatio);
  splitter->end->setCoords(0.5, 1);
  splitter->setClipToAxisRect(false);
  splitter->setPen(QPen(Qt::gray, 0, Qt::DashLine));
  
  customPlot->savePng(dir.filePath("QCPMarginGroup.png"), 400, 400);
}

void MainWindow::genAxisRectSpacingOverview()
{
  resetPlot();
 
  customPlot->xAxis->setRange(-0.4, 1.4);
  customPlot->yAxis->setRange(100, 900);
  customPlot->xAxis->setVisible(true);
  customPlot->yAxis->setVisible(true);
  customPlot->axisRect()->setupFullAxesBox();
  customPlot->xAxis->setTickLabels(false);
  customPlot->axisRect()->setAutoMargins(QCP::msNone);
  customPlot->axisRect()->setMargins(QMargins(200, 50, 20, 165));
  customPlot->axisRect()->setBackground(QColor(245, 245, 245));
  
  customPlot->yAxis->setLabel("Axis Label");
  customPlot->yAxis->setOffset(30);
  customPlot->yAxis->setTickLabelPadding(30);
  customPlot->yAxis->setLabelPadding(30);
  customPlot->yAxis->setTickLengthOut(5);
  customPlot->yAxis->setSubTickLengthOut(2);
  
  addBracket(QPointF(200-95-27-17, 30), QPointF(1, 30), "Padding (if auto margins enabled)", QPointF(-25, -5), false, Qt::AlignLeft|Qt::AlignBottom);
  addBracket(QPointF(1, 370), QPointF(200, 370), "Margin", QPointF(0, 5), false, Qt::AlignHCenter|Qt::AlignTop);
  addBracket(QPointF(200-30, 240), QPointF(200, 240), "Axis offset", QPointF(-1, 5), true, Qt::AlignRight|Qt::AlignVCenter);
  addBracket(QPointF(200-35, 250), QPointF(200-30, 250), "Tick length out", QPointF(-1, 5), true, Qt::AlignRight|Qt::AlignVCenter);
  addBracket(QPointF(200-65, 240), QPointF(200-35, 240), "Tick label padding", QPointF(-1, 5), true, Qt::AlignRight|Qt::AlignVCenter);
  addBracket(QPointF(200-95-25, 240), QPointF(200-65-25, 240), "Label padding", QPointF(-1, 5), true, Qt::AlignRight|Qt::AlignVCenter);
  
  QCPItemLine *leftBorder = new QCPItemLine(customPlot);
  customPlot->addItem(leftBorder);
  leftBorder->setClipToAxisRect(false);
  leftBorder->start->setType(QCPItemPosition::ptViewportRatio);
  leftBorder->end->setType(QCPItemPosition::ptViewportRatio);
  leftBorder->start->setCoords(0, 0);
  leftBorder->end->setCoords(0, 1);
  leftBorder->setPen(QPen(Qt::gray, 0, Qt::DashLine));
  
  QCPItemText *axisRectLabel = new QCPItemText(customPlot);
  customPlot->addItem(axisRectLabel);
  axisRectLabel->position->setType(QCPItemPosition::ptAxisRectRatio);
  axisRectLabel->position->setCoords(0.5, 0.5);
  axisRectLabel->setFont(QFont(QFont().family(), 16));
  axisRectLabel->setText("QCPAxisRect");
  axisRectLabel->setColor(QColor(0, 0, 0, 60));
  
  customPlot->savePng(dir.filePath("AxisRectSpacingOverview.png"), 400, 400);
}

void MainWindow::genAxisNamesOverview()
{
  resetPlot();
 
  customPlot->xAxis->setRange(1, 2);
  customPlot->yAxis->setRange(-50, 150);
  customPlot->xAxis->setVisible(true);
  customPlot->yAxis->setVisible(true);
  customPlot->axisRect()->setupFullAxesBox();
  customPlot->xAxis->setTickLabels(false);
  customPlot->axisRect()->setAutoMargins(QCP::msNone);
  customPlot->axisRect()->setMargins(QMargins(250, 50, 20, 65));
  customPlot->yAxis->setLabel("Axis Label");
  
  addArrow(QPointF(216, 70), QPointF(150, 32), "Tick label", Qt::AlignRight|Qt::AlignVCenter);
  addArrow(QPointF(187, 110), QPointF(130, 76), "Axis label", Qt::AlignRight|Qt::AlignVCenter);
  addArrow(QPointF(260, 77), QPointF(300, 77), "Tick", Qt::AlignLeft|Qt::AlignVCenter);
  addArrow(QPointF(255, 95), QPointF(300, 95), "Sub tick", Qt::AlignLeft|Qt::AlignVCenter);
  addArrow(QPointF(297, 193), QPointF(297, 250), "Zero line", Qt::AlignHCenter|Qt::AlignTop);
  addArrow(QPointF(354, 165), QPointF(354, 266), "Grid line", Qt::AlignHCenter|Qt::AlignTop);
  addBracket(QPointF(263, 132), QPointF(263, 105), "Tick step", QPointF(8, 0), false, Qt::AlignLeft|Qt::AlignVCenter, QCPItemBracket::bsCurly);
  
  customPlot->savePng(dir.filePath("AxisNamesOverview.png"), 450, 300);
}

void MainWindow::labelItemAnchors(QCPAbstractItem *item, double fontSize, bool circle, bool labelBelow)
{
  QList<QCPItemAnchor*> anchors = item->anchors();
  for (int i=0; i<anchors.size(); ++i)
  {
    if (circle)
    {
      QCPItemEllipse *circ = new QCPItemEllipse(item->parentPlot());
      item->parentPlot()->addItem(circ);
      circ->setClipToAxisRect(false);
      circ->topLeft->setParentAnchor(anchors.at(i));
      circ->bottomRight->setParentAnchor(anchors.at(i));
      circ->topLeft->setCoords(-4, -4);
      circ->bottomRight->setCoords(4, 4);
      QPen p(Qt::blue, 0, Qt::CustomDashLine);
#if (QT_VERSION >= QT_VERSION_CHECK(4, 8, 0)) // Qt 4.8 changed the meaning of this completely...
      p.setDashPattern(QVector<qreal>()<<2<<1);
#else
      p.setDashPattern(QVector<qreal>()<<1<<3);
#endif
      circ->setPen(p);
      if (dynamic_cast<QCPItemPosition*>(anchors.at(i)))
      {
        QCPItemEllipse *circ2 = new QCPItemEllipse(item->parentPlot());
        item->parentPlot()->addItem(circ2);
        circ2->setClipToAxisRect(false);
        circ2->topLeft->setParentAnchor(anchors.at(i));
        circ2->bottomRight->setParentAnchor(anchors.at(i));
        circ2->topLeft->setCoords(-2.5, -2.5);
        circ2->bottomRight->setCoords(2.5, 2.5);
        circ2->setPen(Qt::NoPen);
        circ2->setBrush(Qt::blue);
      }
    }
    if (fontSize > 0)
    {
      QCPItemText *label = new QCPItemText(item->parentPlot());
      item->parentPlot()->addItem(label);
      label->setClipToAxisRect(false);
      label->setFont(QFont(font().family(), fontSize));
      label->setColor(Qt::blue);
      label->setText(QString("%2 (%1)").arg(i).arg(anchors.at(i)->name()));
      label->position->setParentAnchor(anchors.at(i));
      if (circle)
        label->position->setCoords(0, fontSize*2*(labelBelow?1:-1));
      else
        label->position->setCoords(0, 0);
      label->setTextAlignment(Qt::AlignCenter);
    }
  }
}

void MainWindow::addBracket(QPointF left, QPointF right, QString text, QPointF textOffset, bool textSideways, Qt::Alignment textAlign, QCPItemBracket::BracketStyle style)
{
  QCPItemBracket *bracket = new QCPItemBracket(customPlot);
  customPlot->addItem(bracket);
  bracket->setClipToAxisRect(false);
  bracket->left->setType(QCPItemPosition::ptAbsolute);
  bracket->right->setType(QCPItemPosition::ptAbsolute);
  bracket->left->setCoords(right);
  bracket->right->setCoords(left);
  bracket->setStyle(style);
  bracket->setLength(3);
  bracket->setPen(QPen(Qt::blue));
  
  QCPItemText *textItem = new QCPItemText(customPlot);
  customPlot->addItem(textItem);
  textItem->setClipToAxisRect(false);
  textItem->setText(text);
  textItem->setPositionAlignment(textAlign);
  if (textSideways)
    textItem->setRotation(-90);
  textItem->position->setParentAnchor(bracket->center);
  textItem->position->setCoords(textOffset);
  textItem->setColor(Qt::blue);
}

void MainWindow::addArrow(QPointF target, QPointF textPosition, QString text, Qt::Alignment textAlign)
{
  QCPItemText *textItem = new QCPItemText(customPlot);
  customPlot->addItem(textItem);
  textItem->setClipToAxisRect(false);
  textItem->setText(text);
  textItem->setPositionAlignment(textAlign);
  textItem->position->setType(QCPItemPosition::ptAbsolute);
  textItem->position->setCoords(textPosition);
  textItem->setColor(Qt::blue);
  QRectF textRect(textItem->topLeft->pixelPoint(), textItem->bottomRight->pixelPoint());
  
  QCPItemLine *arrowItem = new QCPItemLine(customPlot);
  customPlot->addItem(arrowItem);
  arrowItem->setClipToAxisRect(false);
  arrowItem->setHead(QCPLineEnding::esSpikeArrow);
  arrowItem->setPen(QPen(Qt::blue));
  arrowItem->end->setType(QCPItemPosition::ptAbsolute);
  arrowItem->end->setCoords(target);
  
  if (target.x() < textRect.left())
  {
    if (target.y() < textRect.top())
      arrowItem->start->setParentAnchor(textItem->topLeft);
    else if (target.y() > textRect.bottom())
      arrowItem->start->setParentAnchor(textItem->bottomLeft);
    else
      arrowItem->start->setParentAnchor(textItem->left);
  } else if (target.x() > textRect.right())
  {
    if (target.y() < textRect.top())
      arrowItem->start->setParentAnchor(textItem->topRight);
    else if (target.y() > textRect.bottom())
      arrowItem->start->setParentAnchor(textItem->bottomRight);
    else
      arrowItem->start->setParentAnchor(textItem->right);
  } else
  {
    if (target.y() < textRect.top())
      arrowItem->start->setParentAnchor(textItem->top);
    else if (target.y() > textRect.bottom())
      arrowItem->start->setParentAnchor(textItem->bottom);
  }
}

void MainWindow::addGridLayoutOutline(QCPLayoutGrid *layout)
{
  QList<QCPLayoutElement*> elements;
  elements << layout;
  elements << layout->elements(true);
  for (int i=0; i<elements.size(); ++i)
  {
    if (!elements.at(i))
      continue;
    
    qDebug() << elements.at(i) << elements.at(i)->outerRect();
    
    QCPItemRect *outerRect = new QCPItemRect(customPlot);
    customPlot->addItem(outerRect);
    outerRect->setClipToAxisRect(false);
    outerRect->setBrush(QColor(0, 0, 0, 25));
    outerRect->setPen(QPen(QColor(180, 180, 180)));
    outerRect->topLeft->setType(QCPItemPosition::ptAbsolute);
    outerRect->bottomRight->setType(QCPItemPosition::ptAbsolute);
    outerRect->topLeft->setPixelPoint(elements.at(i)->outerRect().topLeft());
    outerRect->bottomRight->setPixelPoint(elements.at(i)->outerRect().bottomRight());
    
    QCPItemRect *innerRect = new QCPItemRect(customPlot);
    customPlot->addItem(innerRect);
    innerRect->setClipToAxisRect(false);
    innerRect->setBrush(QColor(230, 100, 100, 25));
    innerRect->setPen(QPen(QColor(180, 180, 180)));
    innerRect->topLeft->setType(QCPItemPosition::ptAbsolute);
    innerRect->bottomRight->setType(QCPItemPosition::ptAbsolute);
    innerRect->topLeft->setCoords(elements.at(i)->rect().topLeft());
    innerRect->bottomRight->setCoords(elements.at(i)->rect().bottomRight());
    
  }
}

void MainWindow::resetPlot()
{
  customPlot = new QCustomPlot(this);
  setCentralWidget(customPlot);
  customPlot->xAxis->setRange(-0.4, 1.4);
  customPlot->yAxis->setRange(-0.2, 1.2);
  customPlot->xAxis->setVisible(false);
  customPlot->yAxis->setVisible(false);
  customPlot->axisRect()->setAutoMargins(QCP::msNone);
  customPlot->axisRect()->setMargins(QMargins(0, 0, 0, 0));
}
