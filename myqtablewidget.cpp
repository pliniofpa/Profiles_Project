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
#include <QTime>
#include <mainwindow.h>
MyQTableWidget::MyQTableWidget(QWidget *parent):
    QTableWidget(parent)
{
    //delegate = new RichTextDelegate;
    //this->setItemDelegate(delegate);
    this->mainwindow = 0;
    //Enables Drag and Drop
    this->setDragDropMode(QAbstractItemView::InternalMove);
    this->setDragDropOverwriteMode(false);
    this->setDropIndicatorShown(true);
    this->setDragEnabled(true);
    //Set Edit Strategy
    this->setEditTriggers(QAbstractItemView::NoEditTriggers);

}
void MyQTableWidget::dropEvent(QDropEvent* event)
{    
    int draggedColumn = this->columnAt(event->pos().x());
    int draggedRow = this->rowAt(event->pos().y());
    int destItemRow = draggedRow;
    int draggedRowSpan = this->rowSpan(draggedRow,draggedColumn);
    for(int i=1;i<draggedRowSpan;i++){
        if((draggedRow-i)>=0){
            if(this->item(draggedRow-i,draggedColumn)){
                destItemRow = draggedRow-i;
                break;
            }
        }
    }

    MyCell *draggedColumnHeaderItem = (MyCell *)this->horizontalHeaderItem(draggedColumn);
    MyCell *destRowHeaderItem = (MyCell *)this->verticalHeaderItem(destItemRow);
    int stylistIDonDraggedColumn = draggedColumnHeaderItem->getApptStylistID();
    QString beginTimeonDestColumn;
    if(destRowHeaderItem){
        beginTimeonDestColumn = destRowHeaderItem->text();
    }else{
        beginTimeonDestColumn = this->item(destItemRow,draggedColumn-1)->text();
    }
    //
    MyCell *destMyCellItem = (MyCell *)this->item(destItemRow,draggedColumn);
    int rowSpan = this->rowSpan(originMyCellItem->row(),originMyCellItem->column());
    for(int i=0;i<rowSpan;i++){
        destMyCellItem = (MyCell *)this->item(destItemRow+i,draggedColumn);
        if(destMyCellItem &&(destMyCellItem!=originMyCellItem)){
            ApptConflitingDialog conflictDialog(this);
            QString newApptStyle = "<span style=\"color: green\">";
            QString replacedApptStyle = "<span style=\"color: red\">";
            QString spanTagClose = "</span>";
            conflictDialog.ui->warning_message_label->setText("Do you want to replace the following appointment:");
            conflictDialog.ui->customer_origin_value_label->setText(newApptStyle+originMyCellItem->getApptCustomerName()+spanTagClose);
            conflictDialog.ui->service_origin_value_label->setText(newApptStyle+originMyCellItem->getApptServiceName()+spanTagClose);
            conflictDialog.ui->stylist_origin_value_label->setText(newApptStyle+originMyCellItem->getApptStylistName()+spanTagClose);
            conflictDialog.ui->begin_time_origin_label->setText(replacedApptStyle+originMyCellItem->getApptBeginTime()+spanTagClose);
            conflictDialog.ui->end_time_origin_value_label->setText(replacedApptStyle+originMyCellItem->getApptEndTime()+spanTagClose);
            conflictDialog.ui->details_origin_value_label->setText(newApptStyle+originMyCellItem->getApptDetails()+spanTagClose);
            conflictDialog.ui->customer_dest_value_label->setText(replacedApptStyle+destMyCellItem->getApptCustomerName()+spanTagClose);
            conflictDialog.ui->service_dest_value_label->setText(replacedApptStyle+destMyCellItem->getApptServiceName()+spanTagClose);
            conflictDialog.ui->stylist_dest_value_label->setText(replacedApptStyle+destMyCellItem->getApptStylistName()+spanTagClose);
            conflictDialog.ui->begin_time_dest_value_label->setText(newApptStyle+destMyCellItem->getApptBeginTime()+spanTagClose);
            conflictDialog.ui->end_time_dest_value_label->setText(newApptStyle+destMyCellItem->getApptEndTime()+spanTagClose);
            conflictDialog.ui->details_dest_value_label->setText(replacedApptStyle+destMyCellItem->getApptDetails()+spanTagClose);
            if(conflictDialog.exec()){
                qDebug()<<"Substitui";
                QSqlTableModel appointment_table;
                appointment_table.setTable("appointment");
                appointment_table.setFilter(QString("id='%1'").arg(destMyCellItem->getApptID()));
                appointment_table.setEditStrategy(QSqlTableModel::OnManualSubmit);
                bool selected = appointment_table.select();
                if(selected && appointment_table.rowCount()){
                    appointment_table.removeRow(0);
                    if(!appointment_table.submitAll()){
                        qDebug()<<"Error: "<<appointment_table.lastError();
                    }
                }

            }else{
                event->ignore();
                return;
            }
        }
    }
    QSqlTableModel appointment_table;
    appointment_table.setTable("appointment");
    appointment_table.setFilter(QString("id='%1'").arg(originMyCellItem->getApptID()));
    appointment_table.setEditStrategy(QSqlTableModel::OnManualSubmit);
    bool selected = appointment_table.select();
    QSqlRecord curRecord = appointment_table.record(0);

    if(selected && appointment_table.rowCount()){
        if(!curRecord.isEmpty()){
            curRecord.setValue("stylist_id",stylistIDonDraggedColumn);
            curRecord.setValue("time_begin",beginTimeonDestColumn);
            GlobalConfig global_config;
            int apptDuration = getApptDurationInMinutes(originMyCellItem->getApptBeginTime(),originMyCellItem->getApptEndTime());
            QString end_time = QTime::fromString(beginTimeonDestColumn,global_config.time_format).addSecs(apptDuration*60).toString(global_config.time_format);
            curRecord.setValue("time_end",end_time);
            appointment_table.setRecord(0,curRecord);
            if(!appointment_table.submitAll()){
                qDebug()<<"Error: "<<appointment_table.lastError();
            }
        }
    }

    QTableWidget::dropEvent(event);
    if(mainwindow){
        mainwindow->create_daily_appt();
        mainwindow->create_employee_appt();
    }
}
void MyQTableWidget::dragEnterEvent(QDragEnterEvent *event)
{
    //qDebug()<<"MyQTableWidget::dragEnterEvent";
    originMyCellItem = (MyCell*)this->itemAt(event->pos());
    this->setSpan(originMyCellItem->row(),originMyCellItem->column(),1,1);
    if(originMyCellItem){
        QList<MyCell *> *children = originMyCellItem->getChildenList();
        for(int i=0;i<children->length();++i){
            this->setSpan(children->at(i)->row(),children->at(i)->column(),1,1);
            this->setItem(children->at(i)->row(),children->at(i)->column(),0);
        }
        children->clear();
        this->selectionModel()->clear();
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

int MyQTableWidget::getApptDurationInMinutes(QString begin_time_string, QString end_time_string){
    GlobalConfig global_config;
    QTime begin_time = QTime::fromString(begin_time_string,global_config.time_format);
    QTime end_time = QTime::fromString(end_time_string,global_config.time_format);
    int durationInMinutes = (end_time.msecsSinceStartOfDay()/1000/60)-(begin_time.msecsSinceStartOfDay()/1000/60);
    return durationInMinutes;
}
