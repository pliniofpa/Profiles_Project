#include "myqtablewidget.h"
#include <QDebug>
#include <QDropEvent>
#include <QMimeData>
#include <QSqlTableModel>
#include <QSqlRecord>
#include <QSqlError>
#include "mycell.h"
MyQTableWidget::MyQTableWidget(QWidget *parent):
    QTableWidget(parent)
{
}
void MyQTableWidget::dropEvent(QDropEvent* event)
{
    if(itemAt(event->pos())){
        qDebug() << itemAt(event->pos());
        qDebug()<<"Já tem um!";
    }else{
        QSqlTableModel appointment_table;
        appointment_table.setTable("appointment");
        appointment_table.setFilter(QString("id='%1'").arg(this->curDragginApptID));
        appointment_table.setEditStrategy(QSqlTableModel::OnManualSubmit);
        bool selected = appointment_table.select();
        if(selected && appointment_table.rowCount()){
            QSqlRecord curRecord = appointment_table.record(0);
            if(!curRecord.isEmpty()){
                curRecord.setValue("stylist_id",0);
                appointment_table.setRecord(0,curRecord);
                if(!appointment_table.submitAll()){
                    qDebug()<<"Error: "<<appointment_table.lastError();
                }
            }
        }
    }
    //qDebug()<<event->mimeData()->formats();
    //qDebug()<<event->mimeData()->html();
    //qDebug()<<"Text: "<< event->mimeData()->text();
    //qDebug()<<event->mimeData()->data("application/x-qabstractitemmodeldatalist");
    QTableWidget::dropEvent(event);

}
void MyQTableWidget::dragEnterEvent(QDragEnterEvent *event)
{
    //qDebug()<<"MyQTableWidget::dragEnterEvent";
    MyCell *test = (MyCell*)this->itemAt(event->pos());
    qDebug()<<"here";
    if(test){
        qDebug()<<"there";
        this->curDragginApptID = test->getApptID();
        this->curDraggingApptStylistID = test->getApptStylistID();
    }
    QTableWidget::dragEnterEvent(event);
}
