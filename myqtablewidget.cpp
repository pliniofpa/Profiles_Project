#include "myqtablewidget.h"
#include <QDebug>
#include <QDropEvent>
#include <QMimeData>
#include <QSqlTableModel>
#include <QSqlRecord>
#include <QSqlError>
#include "richtextdelegate.h"
#include "mycell.h"
#include "apptconflitingdialog.h"
#include "ui_apptconflitingdialog.h"
#include <QMouseEvent>
MyQTableWidget::MyQTableWidget(QWidget *parent):
    QTableWidget(parent)
{
    delegate = new RichTextDelegate;
    //this->setItemDelegate(delegate);
}
void MyQTableWidget::dropEvent(QDropEvent* event)
{
    QSqlTableModel appointment_table;
    appointment_table.setTable("appointment");
    appointment_table.setFilter(QString("id='%1'").arg(originMyCellItem->getApptID()));
    appointment_table.setEditStrategy(QSqlTableModel::OnManualSubmit);
    bool selected = appointment_table.select();
    QSqlRecord curRecord = appointment_table.record(0);
    //
    MyCell *draggedColumnHeaderItem = (MyCell *)this->horizontalHeaderItem(this->columnAt(event->pos().x()));
    int stylistIDonDraggedColumn = draggedColumnHeaderItem->getStylistID();
    //
    if(itemAt(event->pos())){
        MyCell *destMyCellItem = (MyCell *)itemAt(event->pos());
        ApptConflitingDialog conflictDialog(this);
        conflictDialog.ui->warning_message_label->setText("Do you want to replace the following appointment:");
        conflictDialog.ui->customer_origin_value_label->setText("");
        conflictDialog.ui->service_origin_value_label->setText("");
        conflictDialog.ui->stylist_origin_value_label->setText("");
        conflictDialog.ui->begin_time_origin_label->setText("");
        conflictDialog.ui->end_time_origin_value_label->setText("");
        conflictDialog.ui->customer_dest_value_label->setText("");
        conflictDialog.ui->service_dest_value_label->setText("");
        conflictDialog.ui->stylist_dest_value_label->setText("");
        conflictDialog.ui->begin_time_dest_value_label->setText("");
        conflictDialog.ui->end_time_dest_value_label->setText("");
        if(conflictDialog.exec()){
            qDebug()<<"Substitui";
        }else{
            event->ignore();
            return;
        }
    }else{        
        if(selected && appointment_table.rowCount()){            
            if(!curRecord.isEmpty()){
                curRecord.setValue("stylist_id",stylistIDonDraggedColumn);
                appointment_table.setRecord(0,curRecord);
                if(!appointment_table.submitAll()){
                    qDebug()<<"Error: "<<appointment_table.lastError();
                }
            }
        }
    }
    QTableWidget::dropEvent(event);
    emit DragandDropFinished();
}
void MyQTableWidget::dragEnterEvent(QDragEnterEvent *event)
{
    //qDebug()<<"MyQTableWidget::dragEnterEvent";
    originMyCellItem = (MyCell*)this->itemAt(event->pos());
    qDebug()<<"here";
    if(originMyCellItem){
        qDebug()<<"there";
        QTableWidget::dragEnterEvent(event);
    }else{
        event->ignore();
    }
}
void MyQTableWidget::mousePressEvent(QMouseEvent *event){
    //qDebug()<<"void MyQTableWidget::mousePressEvent(QMouseEvent *event)";
    QTableWidget::mousePressEvent(event);
}
void MyQTableWidget::mouseMoveEvent(QMouseEvent *event){
    //qDebug()<<"void MyQTableWidget::mouseMoveEvent(QMouseEvent *event)";
    QTableWidget::mouseMoveEvent(event);
}

