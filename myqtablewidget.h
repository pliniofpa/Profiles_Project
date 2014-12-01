#ifndef MYQTABLEWIDGET_H
#define MYQTABLEWIDGET_H

#include <QTableWidget>
class QDropEvent;
class MyCell;
class RichTextDelegate;
class MyQTableWidget : public QTableWidget
{
    //Q_OBJECT
private:
    MyCell *originMyCellItem;
    void dropEvent(QDropEvent* event);
    void dragEnterEvent(QDragEnterEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    RichTextDelegate *delegate;
public:
    MyQTableWidget(QWidget *parent);
signals:
    void DragandDropFinished(){

    }
};

#endif // MYQTABLEWIDGET_H
