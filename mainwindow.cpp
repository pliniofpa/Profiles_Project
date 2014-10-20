#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTime>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QStringList>
#include <QDebug>
#include "newcustomerdialog.h"
#include <QSqlTableModel>
#include <QSqlRecord>
#include <QSqlError>
#include "ui_newcustomerdialog.h"

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
        QSqlTableModel customer_table_model;
        customer_table_model.setEditStrategy(QSqlTableModel::OnManualSubmit);
        customer_table_model.setTable("address");
        QSqlRecord new_record;
        //Adds data from form to Record
        new_record.setValue("address",dialog.ui->address_lineEdit_5->text());
        new_record.setValue("zip_code",dialog.ui->zip_code_lineEdit_6->text());
        new_record.setValue("city",dialog.ui->city_lineEdit_7->text());
        new_record.setValue("state",dialog.ui->state_comboBox->currentText());
        //Test
        new_record.setValue("address","asdfasdf");
        new_record.setValue("zip_code","asdfdf");
        new_record.setValue("city","asdfsdaf");
        new_record.setValue("state","IN");
        //Adds record to model
        customer_table_model.insertRecord(1,new_record);
        //Aplies the change to table
        if(!customer_table_model.submitAll()){
            qDebug()<<customer_table_model.lastError();
        }
        return;
    }
}
void MainWindow::make_connections(){
    QObject::connect(this->ui->action_create_Customers,SIGNAL(triggered()),this,SLOT(showCreateCustomerDialog()));
}
