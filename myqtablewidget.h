#ifndef MYQTABLEWIDGET_H
#define MYQTABLEWIDGET_H

#include <QTableWidget>
class QDropEvent;
class MyCell;
class RichTextDelegate;
class MainWindow;
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
    int getApptDurationInMinutes(QString begin_time_string, QString end_time_string);
    MainWindow *mainwindow;
public:
    MyQTableWidget(QWidget *parent);
    void setMainWindowPointer(MainWindow *pointer){
        this->mainwindow = pointer;
    }
};

#endif // MYQTABLEWIDGET_H
