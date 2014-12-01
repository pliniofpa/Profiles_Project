#include "richtextdelegate.h"
#include <QPainter>
#include <QTextDocument>
#include <QAbstractTextDocumentLayout>
#include <QTableWidget>
#include <QDebug>
RichTextDelegate::RichTextDelegate(QObject *parent) :
    QStyledItemDelegate(parent)
{

}
void RichTextDelegate::paint( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const
{
    painter->save();
    QTextDocument doc;
    QVariant value = index.data(Qt::DisplayRole);
    QBrush brush = qvariant_cast<QBrush>(index.data(Qt::BackgroundRole));
    QString colorString = brush.color().name();
    qDebug()<<colorString;
    if (value.isValid() && !value.isNull()) {
        QString text;
        text.append(QString("<div  style=\" background-color: %1\"").arg(colorString));
        text.append(value.toString());
        text.append("</div>");
        doc.setHtml(text);
        QAbstractTextDocumentLayout::PaintContext context;
        doc.setPageSize(option.rect.size());
        //painter->setClipRect( option.rect );
        painter->translate(option.rect.x(), option.rect.y());
        doc.documentLayout()->draw(painter, context);
        painter->restore();
    }
}
