#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTime>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QStringList>
#include <QDebug>
#include "mydatamodel.h"
#include "newcustomerdialog.h"
#include "ui_newcustomerdialog.h"
#include "newstylistdialog.h"
#include "ui_newstylistdialog.h"
#include "newservicedialog.h"
#include "ui_newservicedialog.h"
#include "newappointmentdialog.h"
#include "ui_newappointmentdialog.h"
#include "newuserdialog.h"
#include "ui_newuserdialog.h"

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
    NewCustomerDialog dialog(this);
    if(dialog.exec()){
        MyDataModel model("address");
        model.setValue("address",dialog.ui->address_lineEdit_5->text());
        model.setValue("zip_code",dialog.ui->zip_code_lineEdit_6->text());
        model.setValue("state",dialog.ui->state_comboBox->currentText());
        model.setValue("city",dialog.ui->city_lineEdit_7->text());
        model.submitAll();
        return;
    }
    qDebug()<<"Sem erro";
    return;
}
void MainWindow::showCreateStylistDialog(){
    NewStylistDialog dialog(this);
    if(dialog.exec()){
        MyDataModel model("stylist");
        model.setValue("name",dialog.ui->name_lineEdit->text());
        model.setValue("nickname",dialog.ui->nickname_lineEdit_2->text());
        model.setValue("phone1",dialog.ui->phone1_lineEdit_4->text());
        model.setValue("phone2",dialog.ui->phone2_lineEdit_5->text());
        //MISSING EMAIL
        model.submitAll();
        return;
    }
    qDebug()<<"Sem erro";
    return;
}
void MainWindow::showCreateServiceDialog(){
    NewServiceDialog dialog(this);
    if(dialog.exec()){
        MyDataModel model("services");
        model.setValue("description",dialog.ui->description_textEdit->toPlainText());
        model.setValue("price",dialog.ui->price_lineEdit_3->text());
        model.submitAll();
        //MISSING NAME
        return;
    }
    qDebug()<<"Sem erro";
    return;
}
void MainWindow::showCreateAppointmentDialog(){
    NewAppointmentDialog dialog(this);
    //Buttons Connections
    QObject::connect(dialog.ui->new_customer_pushButton_3,SIGNAL(clicked()),this,SLOT(showCreateCustomerDialog()));
    QObject::connect(dialog.ui->new_service_pushButton,SIGNAL(clicked()),this,SLOT(showCreateServiceDialog()));
    QObject::connect(dialog.ui->new_stylist_pushButton_2,SIGNAL(clicked()),this,SLOT(showCreateStylistDialog()));
    if(dialog.exec()){
        MyDataModel model("schedule");
        model.setValue("stylist_id",dialog.ui->stylist_comboBox->currentText());
        model.setValue("customer_id",dialog.ui->customer_comboBox->currentText());
        model.setValue("service_id",dialog.ui->service_comboBox->currentText());
        //MISSING DATETIMES
        //model.setValue("datetime_beginn",dialog.ui->date_dateEdit->);
        return;
    }
    qDebug()<<"Sem erro";
    return;
}
void MainWindow::showCreateUserDialog(){
    NewUserDialog dialog(this);
    if(dialog.exec()){
        MyDataModel model("user");
        model.setValue("login",dialog.ui->login_lineEdit->text());
        model.setValue("password",dialog.ui->password_lineEdit->text());
        model.setValue("email",dialog.ui->email_lineEdit->text());

        return;
    }
    qDebug()<<"Sem erro";
    return;
}
void MainWindow::make_connections(){
    //Menu Connections
    QObject::connect(this->ui->action_create_Customer,SIGNAL(triggered()),this,SLOT(showCreateCustomerDialog()));
    QObject::connect(this->ui->action_create_Stylist,SIGNAL(triggered()),this,SLOT(showCreateStylistDialog()));
    QObject::connect(this->ui->action_create_Service,SIGNAL(triggered()),this,SLOT(showCreateServiceDialog()));
    QObject::connect(this->ui->action_create_Appointment,SIGNAL(triggered()),this,SLOT(showCreateAppointmentDialog()));
    QObject::connect(this->ui->action_create_User,SIGNAL(triggered()),this,SLOT(showCreateUserDialog()));    
}
