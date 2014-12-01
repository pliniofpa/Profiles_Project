#ifndef RICHTEXTDELEGATE_H
#define RICHTEXTDELEGATE_H

#include <QStyledItemDelegate>
class QTableWidget;
class RichTextDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit RichTextDelegate(QObject *parent = 0);
    void paint( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const;
signals:

public slots:

};

#endif // RICHTEXTDELEGATE_H
