/**
 * @file mycell.cpp
 * This Class defines a table cell item.
 * @brief Source file for MyCell class definition.
 *
 * This class implements a QTableWidgetItem with a specific tags, text alignment and color.
 *
 * @author Plinio Andrade &lt;PAndrade@fele.com&gt;
 * @version 1.0.0.0 (Qt: 5.3.1)
 */
#include "mycell.h"
#include <QMouseEvent>
MyCell::MyCell(const QString &text)
{
    //QTableWidgetItem(1001);
    this->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    this->setTextAlignment(Qt::AlignCenter);
    this->setText(text);
    this->setFlags(this->flags()|Qt::ItemIsDragEnabled|Qt::ItemIsDropEnabled);
}

void MyCell::setApptID(int id){
    this->apptID = id;
}

int MyCell::getApptID(){
    return this->apptID;
}
void MyCell::setApptStylistID(int stylistID){
    this->apptStylistID = stylistID;
}

int MyCell::getApptStylistID(){
    return this->apptStylistID;
}
void MyCell::setApptStylistName(QString stylistName){
    this->apptStylistName = new QString (stylistName);
}

void MyCell::setApptServiceName(QString serviceName){
    this->apptServiceName = new QString (serviceName);
}

void MyCell::setApptDetails(QString details){
    this->apptDetails = new QString (details);
}

void MyCell::setApptCustomerName(QString customerName){
    this->apptCustomerName = new QString (customerName);
}

void MyCell::setApptBeginTime (QString beginTime){
    this->apptBeginTime = new QString (beginTime);
}

void MyCell::setApptEndTime (QString endTime){
    this->apptEndTime = new QString (endTime);
}
QString MyCell::getApptStylistName(){

    return *this->apptStylistName;
}

QString MyCell::getApptServiceName(){
    return *this->apptServiceName;
}

QString MyCell::getApptDetails(){
    return *this->apptDetails;
}

QString MyCell::getApptCustomerName(){
    return *this->apptCustomerName;
}

QString MyCell::getApptBeginTime(){
    return *this->apptBeginTime;
}

QString MyCell::getApptEndTime(){
    return *this->apptEndTime;
}

/*
void MyCell::dropEvent(QDropEvent *event)
{
    QTableWidget::dropEvent(event);
}

void MyCell::dragMoveEvent(QDragMoveEvent *event)
{
    QTableWidget::dragMoveEvent(event);
}

void MyCell::dragEnterEvent(QDragEnterEvent *event)
{
    QTableWidget::dragEnterEvent(event);
}

bool MyCell::dropMimeData(int row, int column, const QMimeData *data, Qt::DropAction action)
{
    return QTableWidget::dropMimeData(row, column, data, action);
}
*/
