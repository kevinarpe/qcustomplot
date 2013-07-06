#include "qcpdocumentobject.h"

QCPDocumentObject::QCPDocumentObject(QObject *parent) :
  QObject(parent)
{
}

QSizeF QCPDocumentObject::intrinsicSize(QTextDocument *doc, int posInDocument, const QTextFormat &format)
{
  Q_UNUSED(doc)
  Q_UNUSED(posInDocument)
  QPicture pic = qvariant_cast<QPicture>(format.property(PicturePropertyId));
  if (pic.isNull())
  {
    qDebug() << Q_FUNC_INFO << "Plot object is empty";
    return QSizeF(10, 10);
  } else
    return QSizeF(pic.boundingRect().size());
}

void QCPDocumentObject::drawObject(QPainter *painter, const QRectF &rect, QTextDocument *doc, int posInDocument, const QTextFormat &format)
{
  Q_UNUSED(doc)
  Q_UNUSED(posInDocument)
  QPicture pic = qvariant_cast<QPicture>(format.property(PicturePropertyId));
  if (pic.isNull())
    return;
  
  QSize finalSize = pic.boundingRect().size();
  finalSize.scale(rect.size().toSize(), Qt::KeepAspectRatio);
  double scaleFactor = finalSize.width()/(double)pic.boundingRect().size().width();
  painter->save();
  painter->scale(scaleFactor, scaleFactor);
  painter->setClipRect(rect);
  painter->drawPicture(rect.topLeft(), pic);
  painter->restore();
}

QTextCharFormat QCPDocumentObject::generatePlotFormat(QCustomPlot *plot, int width, int height)
{
  QPicture picture;
  QCPPainter qcpPainter;
  qcpPainter.begin(&picture);
  plot->toPainter(&qcpPainter, width, height);
  qcpPainter.end();
  
  QTextCharFormat result;
  result.setObjectType(QCPDocumentObject::PlotTextFormat);
  result.setProperty(QCPDocumentObject::PicturePropertyId, QVariant::fromValue(picture));
  return result;
}
