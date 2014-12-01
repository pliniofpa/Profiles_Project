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
void MyCell::setStylistID(int stylistID){
     this->stylistID = stylistID;
}

int MyCell::getStylistID(){
    return this->stylistID;
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
void MyCell::setStylistID(int id){
    this->stylist_id = id;
}
int MyCell::getStylistID(){
    return this->stylist_id;
}
*/
