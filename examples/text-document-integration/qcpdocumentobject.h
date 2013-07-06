#ifndef QCPDOCUMENTOBJECT_H
#define QCPDOCUMENTOBJECT_H

#include <QObject>
#include <QTextObjectInterface>
#include <QPicture>
#include <QVariant>
#include <QPainter>
#include "../../qcustomplot.h"

class QCPDocumentObject : public QObject, public QTextObjectInterface
{
  Q_OBJECT
  Q_INTERFACES(QTextObjectInterface)
  
public:
  enum { PlotTextFormat = QTextFormat::UserObject + 3902 }; // if your application already uses the id (QTextFormat::UserObject + 3902), just change the id here
  enum { PicturePropertyId = 1 };
  
  explicit QCPDocumentObject(QObject *parent = 0);
  QSizeF intrinsicSize(QTextDocument *doc, int posInDocument, const QTextFormat &format);
  void drawObject(QPainter *painter, const QRectF &rect, QTextDocument *doc, int posInDocument, const QTextFormat &format);

  static QTextCharFormat generatePlotFormat(QCustomPlot *plot, int width=0, int height=0);

};
Q_DECLARE_METATYPE(QPicture)

#endif // QCPDOCUMENTOBJECT_H
