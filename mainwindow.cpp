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
#include "editcustumerdialog.h"
#include "editstylistdialog.h"
#include <QRegExp>
#include <QRegExpValidator>
#include <QSqlTableModel>
#include <QColorDialog>
struct GlobalConfig;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->first_time = new QTime(7,0,0);
    this->last_time = new QTime(23,0,0);
    this->appt_inverval = 15;
    this->create_daily_appt();
    this->create_employee_appt();
    this->make_connections();
    //Set the date int the daily appointments date to current date
    this->ui->daily_appt_date_dateEdit->setDate(QDate::currentDate());
    //Set the interval of data from current date to 6 months from current date
    this->ui->daily_appt_date_dateEdit->setDateRange(QDate::currentDate(),QDate::currentDate().addMonths(6));
    QColorDialog teste(this);
    teste.exec();
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::create_daily_appt(){
    QTableWidget *curTable = this->ui->daily_appt_tableWidget;
    curTable->verticalHeader()->setVisible(true);
    QTime curTime(this->first_time->hour(),this->first_time->minute(),this->first_time->second());
    QStringList vertical_headers_labels;
    do{
        //Adds one line in the table
        curTable->setRowCount(curTable->rowCount()+1);
        //Creates a new item for vertical header
        vertical_headers_labels << (curTime.toString(global_config.time_format));
        //curTable->setVerticalHeaderItem((curTable->rowCount()-1),curItem);
        curTime = curTime.addSecs(this->appt_inverval*60);
    }while(curTime <= (*this->last_time));
    curTable->setVerticalHeaderLabels(vertical_headers_labels);
    //qDebug() << vertical_headers_labels;
}
void MainWindow::create_employee_appt(){
    //Sets Model for Combobox
    QSqlTableModel stylist_model;
    stylist_model.setTable("stylist");
    stylist_model.select();
    this->ui->stylist_comboBox->setModel(&stylist_model);
    this->ui->stylist_comboBox->setModelColumn(stylist_model.fieldIndex("name"));

    //Creates Table
    QTableWidget *curTable = this->ui->employee_day_tableWidget;
    curTable->verticalHeader()->setVisible(false);
    curTable->horizontalHeader()->setVisible(false);
    QTime curTime(this->first_time->hour(),this->first_time->minute(),this->first_time->second());
    //QStringList vertical_headers_labels;
    int per_column_dates=20;
    curTable->setRowCount(per_column_dates);
    for(int i=0;curTime <= (*this->last_time);++i){
        curTable->setColumnCount(2+((int)(i/per_column_dates))*2);
        curTable->setItem(i%per_column_dates,(int)(i/per_column_dates)*2,new QTableWidgetItem(curTime.toString(global_config.time_format)));
        curTime = curTime.addSecs(this->appt_inverval*60);
    }
    /*
    do{
        //Adds one line in the table
        curTable->setRowCount(curTable->rowCount()+1);
        //Creates a new item for vertical header
        vertical_headers_labels << (curTime.toString(GlobalConfig.time_format));
        //curTable->setVerticalHeaderItem((curTable->rowCount()-1),curItem);
        curTime = curTime.addSecs(this->appt_inverval*60);
    }while(curTime <= (*this->last_time));
    curTable->setVerticalHeaderLabels(vertical_headers_labels);
    */
}

void MainWindow::showCreateCustomerDialog(){
    NewCustomerDialog dialog(this);
    //Set USA states list to State Combobox and City
    dialog.ui->state_comboBox->addItems(global_config.usa_states);
    dialog.ui->city_lineEdit_7->setText("Fort Wayne");
    if(dialog.exec()){
        MyDataModel customer_model("customer");
        customer_model.setValue("name",dialog.ui->name_lineEdit->text());
        customer_model.setValue("birthday",dialog.ui->birthday_dateEdit->date());
        customer_model.setValue("email",dialog.ui->email_lineEdit_4->text());
        customer_model.setValue("phone1",dialog.ui->phone1_lineEdit_2->text());
        customer_model.setValue("phone2",dialog.ui->phone2_lineEdit_3->text());
        customer_model.setValue("address",dialog.ui->address_lineEdit_5->text());
        customer_model.setValue("zip_code",dialog.ui->zip_code_lineEdit_6->text());
        customer_model.setValue("state",dialog.ui->state_comboBox->currentText());
        customer_model.setValue("city",dialog.ui->city_lineEdit_7->text());
        int customer_id = customer_model.submitAll();
        if(customer_id>0){
            qDebug()<<QString("Customer Saved. ID: %1").arg(customer_id);
        }
    }
}
void MainWindow::showCreateStylistDialog(){
    NewStylistDialog dialog(this);
    //Set USA states list to State Combobox and City
    dialog.ui->state_comboBox->addItems(global_config.usa_states);
    dialog.ui->city_lineEdit_7->setText("Fort Wayne");
    if(dialog.exec()){
        MyDataModel stylist_model("stylist");
        stylist_model.setValue("name",dialog.ui->name_lineEdit->text());
        stylist_model.setValue("nickname",dialog.ui->nickname_lineEdit_2->text());
        stylist_model.setValue("phone1",dialog.ui->phone1_lineEdit_4->text());
        stylist_model.setValue("phone2",dialog.ui->phone2_lineEdit_5->text());
        stylist_model.setValue("email",dialog.ui->email_lineEdit_3->text());
        stylist_model.setValue("birthday",dialog.ui->birthday_dateEdit->date());
        stylist_model.setValue("address",dialog.ui->address_lineEdit_5->text());
        stylist_model.setValue("zip_code",dialog.ui->zip_code_lineEdit_6->text());
        stylist_model.setValue("state",dialog.ui->state_comboBox->currentText());
        stylist_model.setValue("city",dialog.ui->city_lineEdit_7->text());
        int stylist_id = stylist_model.submitAll();
        if(stylist_id>0){
            qDebug()<<QString("Stylist Saved. ID: %1").arg(stylist_id);
        }
        return;
    }
}
void MainWindow::showCreateServiceDialog(){
    NewServiceDialog dialog(this);
    if(dialog.exec()){
        MyDataModel model("services");
        model.setValue("description",dialog.ui->description_textEdit->toPlainText());
        model.setValue("price",dialog.ui->price_lineEdit_3->text().toFloat());
        model.submitAll();
        //MISSING NAME
        return;
    }
    return;
}
void MainWindow::showCreateAppointmentDialog(){
    NewAppointmentDialog dialog(this);
    //Buttons Connections
    QObject::connect(dialog.ui->new_customer_pushButton_3,SIGNAL(clicked()),this,SLOT(showCreateCustomerDialog()));
    QObject::connect(dialog.ui->new_service_pushButton,SIGNAL(clicked()),this,SLOT(showCreateServiceDialog()));
    QObject::connect(dialog.ui->new_stylist_pushButton_2,SIGNAL(clicked()),this,SLOT(showCreateStylistDialog()));
    //Sets Model for Combobox
    QSqlTableModel customer_model;
    customer_model.setTable("customer");
    customer_model.select();
    dialog.ui->customer_comboBox->setModel(&customer_model);
    dialog.ui->customer_comboBox->setModelColumn(customer_model.fieldIndex("name"));
    //Sets Model for Combobox
    QSqlTableModel service_model;
    service_model.setTable("service");
    service_model.select();
    dialog.ui->service_comboBox->setModel(&service_model);
    dialog.ui->service_comboBox->setModelColumn(service_model.fieldIndex("name"));
    //Sets Model for Combobox
    QSqlTableModel stylist_model;
    stylist_model.setTable("stylist");
    stylist_model.select();
    dialog.ui->stylist_comboBox->setModel(&stylist_model);
    dialog.ui->stylist_comboBox->setModelColumn(stylist_model.fieldIndex("name"));

    //Shows Modal Dialog
    if(dialog.exec()){
        MyDataModel model("schedule");
        model.setValue("stylist_id",dialog.ui->stylist_comboBox->currentText());
        model.setValue("customer_id",dialog.ui->customer_comboBox->currentText());
        model.setValue("service_id",dialog.ui->service_comboBox->currentText());
        //MISSING DATETIMES
        //model.setValue("datetime_beginn",dialog.ui->date_dateEdit->);
    }
}
void MainWindow::showCreateUserDialog(){
    NewUserDialog dialog(this);
    if(dialog.exec()){
        MyDataModel model("user");
        model.setValue("login",dialog.ui->login_lineEdit->text());
        model.setValue("password",dialog.ui->password_lineEdit->text());
        model.setValue("email",dialog.ui->email_lineEdit->text());
    }
}
void MainWindow::make_connections(){
    //Menu Connections
    QObject::connect(this->ui->action_create_Customer,SIGNAL(triggered()),this,SLOT(showCreateCustomerDialog()));
    QObject::connect(this->ui->action_create_Stylist,SIGNAL(triggered()),this,SLOT(showCreateStylistDialog()));
    QObject::connect(this->ui->action_create_Service,SIGNAL(triggered()),this,SLOT(showCreateServiceDialog()));
    QObject::connect(this->ui->action_create_Appointment,SIGNAL(triggered()),this,SLOT(showCreateAppointmentDialog()));
    QObject::connect(this->ui->action_create_User,SIGNAL(triggered()),this,SLOT(showCreateUserDialog()));
    QObject::connect(this->ui->action_edit_Customers,SIGNAL(triggered()),this,SLOT(showEditCustomerDialog()));
    QObject::connect(this->ui->action_edit_Stylists,SIGNAL(triggered()),this,SLOT(showEditStylistDialog()));
}
void MainWindow::showEditCustomerDialog(){
    EditCustumerDialog dialog;
    dialog.exec();
}
void MainWindow::showEditStylistDialog(){
    EditStylistDialog dialog;
    dialog.exec();
}
