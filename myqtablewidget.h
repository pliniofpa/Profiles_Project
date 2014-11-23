#ifndef MYQTABLEWIDGET_H
#define MYQTABLEWIDGET_H

#include <QTableWidget>
class QDropEvent;
class MyQTableWidget : public QTableWidget
{
private:
    int curDragginApptID;
    int curDraggingApptStylistID;
    void dropEvent(QDropEvent* event);
    void dragEnterEvent(QDragEnterEvent *event);
public:
    MyQTableWidget(QWidget *parent);
};

#endif // MYQTABLEWIDGET_H
