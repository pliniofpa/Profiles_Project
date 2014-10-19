#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTime>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QStringList>
#include <QDebug>
#include "newcustomerdialog.h"
#include <QSqlDatabase>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->first_time = new QTime(7,0,0);
    this->last_time = new QTime(23,0,0);
    this->appt_inverval = 15;
    this->time_format = "hh:mm AP";
    this->create_daily_appt();
    this->make_connections();
    qDebug()<< QSqlDatabase::drivers();

}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::create_daily_appt(){
    QTableWidget *curTable = this->ui->daily_appt_tableWidget;
    curTable->verticalHeader()->setVisible(true);
    //curTable->setColumnCount(15);
    QTime curTime(this->first_time->hour(),this->first_time->minute(),this->first_time->second());
    QStringList vertical_headers_labels;
    do{
        //Adds one line in the table
        curTable->setRowCount(curTable->rowCount()+1);
        //Creates a new item for vertical header
        vertical_headers_labels << (curTime.toString(this->time_format));
        //curTable->setVerticalHeaderItem((curTable->rowCount()-1),curItem);
        curTime = curTime.addSecs(this->appt_inverval*60);
    }while(curTime <= (*this->last_time));
    curTable->setVerticalHeaderLabels(vertical_headers_labels);
    //qDebug() << vertical_headers_labels;
}
void MainWindow::showCreateCustomerDialog(){
    NewCustomerDialog dialog;
    if(dialog.exec()){
        qDebug()<<"Apertou OK";
        return;
    }
    qDebug()<<"Não apertou OK";
}
void MainWindow::make_connections(){
    QObject::connect(this->ui->action_create_Customers,SIGNAL(triggered()),this,SLOT(showCreateCustomerDialog()));
}
