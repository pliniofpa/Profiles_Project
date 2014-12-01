#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "version.h"
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
#include "editappointmentdialog.h"
#include "editservicedialog.h"
#include "edituserdialog.h"
#include <QRegExp>
#include <QRegExpValidator>
#include <QSqlTableModel>
#include <QSqlRecord>
#include <QSettings>
#include "mycell.h"
#include "myqtablewidget.h"
#include <QLabel>
#include <QPrintDialog>
#include <QPrinter>
#include <QPainter>
#include "apptdeleteconfirmationdialog.h"
#include "ui_apptdeleteconfirmationdialog.h"
#include <QSqlError>
struct GlobalConfig;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->make_connections();
    //Set the date int the daily appointments date to current date
    this->ui->daily_appt_date_dateEdit->setDate(QDate::currentDate());
    //Set the interval of data from current date to 6 months from current date
    this->ui->daily_appt_date_dateEdit->setDateRange(QDate::currentDate().addYears(-5),QDate::currentDate().addMonths(6));
    loadUerPreferences();
    this->create_employee_appt();
    this->create_daily_appt();
    this->ui->daily_appt_tableWidget->setMainWindowPointer(this);
    this->ui->mainToolBar->setEnabled(false);
}
void MainWindow::genPdfUser(){
    QPrinter printer;
    printer.setPaperSize(QPrinter::A4);
    QPrintDialog printer_dialog(&printer);
    if (printer_dialog.exec() == QDialog::Accepted) {
        QPainter painter(&printer);
        painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform);

        this->ui->daily_appt_tableWidget->render(&painter);
    }

    QString filename="D:/users.pdf";
    //Paramètres d'impression
    //QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFileName(filename);
    //printer.setPaperSize(QPrinter::A4);
    printer.setOutputFormat(QPrinter::PdfFormat);

    QPainter painter(&printer);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform);
    this->ui->daily_appt_tableWidget->render( &painter );
    painter.end();

}
void MainWindow::dailyApptselectionChanged(const QItemSelection & selected, const QItemSelection & deselected){
    if(selected.indexes().length()){
        this->ui->mainToolBar->setEnabled(true);
        //for(int i=0;i<selected.indexes().length();i++){
        //qDebug()<<selected.indexes().at(0).data().toString();
        //qDebug()<<selected.indexes().at(0).row()<<selected.indexes().at(0).column();
        if(selected.indexes().at(0).data().isValid()){
            this->ui->actionDelete_Record->setEnabled(true);
            this->ui->actionEdit_Record->setEnabled(true);
        }else{
            this->ui->actionDelete_Record->setEnabled(false);
            this->ui->actionEdit_Record->setEnabled(false);
        }
        //}

    }else{
        this->ui->mainToolBar->setEnabled(false);
    }
}
void MainWindow::deleteSelectedAppt(){
    MyQTableWidget *curTable = this->ui->daily_appt_tableWidget;
    MyCell *selectedItem = (MyCell*)curTable->selectedItems().at(0);
    if(selectedItem){
        ApptDeleteConfirmationDialog deleteDialog(this);
        QString deletingApptStyle = "<span style=\"color: red\">";
        QString spanTagClose = "</span>";
        deleteDialog.ui->warning_message_label->setText("Do you want to delete the following appointment:");
        deleteDialog.ui->customer_origin_value_label->setText(deletingApptStyle+selectedItem->getApptCustomerName()+spanTagClose);
        deleteDialog.ui->service_origin_value_label->setText(deletingApptStyle+selectedItem->getApptServiceName()+spanTagClose);
        deleteDialog.ui->stylist_origin_value_label->setText(deletingApptStyle+selectedItem->getApptStylistName()+spanTagClose);
        deleteDialog.ui->begin_time_origin_label->setText(deletingApptStyle+selectedItem->getApptBeginTime()+spanTagClose);
        deleteDialog.ui->end_time_origin_value_label->setText(deletingApptStyle+selectedItem->getApptEndTime()+spanTagClose);
        deleteDialog.ui->details_origin_value_label->setText(deletingApptStyle+selectedItem->getApptDetails()+spanTagClose);
        if(deleteDialog.exec()){
            QSqlTableModel appointment_table;
            appointment_table.setTable("appointment");
            appointment_table.setFilter(QString("id='%1'").arg(selectedItem->getApptID()));
            appointment_table.setEditStrategy(QSqlTableModel::OnManualSubmit);
            bool selected = appointment_table.select();
            if(selected && appointment_table.rowCount()){
                bool rowRemoved = appointment_table.removeRow(0);
                if(rowRemoved && appointment_table.submitAll()){
                    qDebug()<<"Success deleting Record!";
                }else{
                    qDebug()<<"Error: "<<appointment_table.lastError();
                }
            }
            this->create_daily_appt();
        }
    }
}
void MainWindow::editSelectedAppt(){
    MyQTableWidget *curTable = this->ui->daily_appt_tableWidget;
    MyCell *selectedItem = (MyCell*)curTable->selectedItems().at(0);
    qDebug()<<selectedItem->row();
    qDebug()<<selectedItem->column();
    if(selectedItem){
        int apptID = selectedItem->getApptID();
        EditAppointmentDialog editDialog(this);
        editDialog.editApptbyID(apptID);
        editDialog.exec();
        this->create_daily_appt();
    }
}
void MainWindow::addAppt(){
    MyQTableWidget *curTable = this->ui->daily_appt_tableWidget;
    int selectedRow = curTable->selectionModel()->selectedIndexes().at(0).row();
    int selectedColumn = curTable->selectionModel()->selectedIndexes().at(0).column();
    qDebug()<<selectedRow;
    qDebug()<<selectedColumn;
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::create_daily_appt(){
    MyQTableWidget *curTable = this->ui->daily_appt_tableWidget;
    //Clean table
    curTable->clear();
    curTable->setColumnCount(0);
    curTable->setRowCount(0);
    curTable->verticalHeader()->setVisible(true);
    QTime curTime(global_config.first_time->hour(),global_config.first_time->minute(),global_config.first_time->second());
    do{
        //Adds one line in the table
        curTable->setRowCount(curTable->rowCount()+1);
        //Creates a new item for vertical header
        curTable->setVerticalHeaderItem(curTable->rowCount()-1, new QTableWidgetItem(curTime.toString(global_config.time_format)));
        curTable->verticalHeader()->resizeSection(curTable->rowCount()-1,60);
        curTime = curTime.addSecs(global_config.appointments_interval*60);
    }while(curTime <= (*global_config.last_time));
    //Fill table with Daily stylist appointments
    QSqlTableModel stylist_model;
    stylist_model.setTable("stylist");
    stylist_model.select();
    MyCell *curItem;
    QSqlTableModel appts_model;
    appts_model.setTable("appt_assoc_names");
    QString curDate = this->ui->daily_appt_date_dateEdit->date().toString(global_config.date_format);
    //qDebug()<<curDate;
    appts_model.setFilter(QString("date='%1'").arg(curDate));
    appts_model.select();
    //qDebug()<<stylist_model.rowCount();
    //Add Horizontal Header
    qreal sectionSize=0;
    MyCell *curHeaderItem;
    QString curHeaderString;
    int curStylistID;
    for(int i=0;i<stylist_model.rowCount();i++){
        //Adds a new column
        curTable->setColumnCount(curTable->columnCount()+1);
        //Adds a new Stylist name to the horizontal header list
        curHeaderString = stylist_model.record(i).value("name").toString();
        curStylistID = stylist_model.record(i).value("id").toInt();
        curHeaderItem = new MyCell(curHeaderString);
        //Set appointment Stylist ID
        curHeaderItem->setApptStylistID(curStylistID);
        curTable->setHorizontalHeaderItem(i,curHeaderItem);
        //Resize Columns
        curTable->resizeColumnsToContents();
        sectionSize = sectionSize<curTable->horizontalHeader()->sectionSize(i)?curTable->horizontalHeader()->sectionSize(i):sectionSize;
    }
    //Calculates the space for filling the screen with the table
    for(int i=0;i<stylist_model.rowCount();i++){
        QColor curColor(stylist_model.record(i).value("color").toString());
        int curStylistID = stylist_model.record(i).value("id").toInt();
        //curTable->horizontalHeaderItem(i)->setForeground(QBrush(curColor));
        //Equalize the size to all Horizontal Section
        curTable->horizontalHeader()->resizeSection(i,sectionSize>150?sectionSize:150);
        //Set Sections not resizable
        curTable->horizontalHeader()->setSectionResizeMode(i,QHeaderView::Fixed);
        QString curApptTimeBeginString,curApptStylistName, curApptTimeEndString, curApptCustomerName,curApptServiceName,curApptDetails;
        for(int p=0;p<appts_model.rowCount();p++){
            curApptTimeBeginString = appts_model.record(p).value("time_begin").toString();
            int curApptStylistID = appts_model.record(p).value("stylist_id").toInt();
            int curApptID = appts_model.record(p).value("id").toInt();
            curApptTimeEndString = appts_model.record(p).value("time_end").toString();
            for(int q=0;q<curTable->rowCount();q++){
                QString curVerticalHeaderText = curTable->verticalHeaderItem(q)->text();
                if(curApptTimeBeginString==curVerticalHeaderText && curStylistID==curApptStylistID){
                    curApptDetails = appts_model.record(p).value("details").toString();
                    curApptCustomerName = appts_model.record(p).value("customer_name").toString();
                    curApptServiceName = appts_model.record(p).value("service_name").toString();
                    curApptStylistName = appts_model.record(p).value("stylist_name").toString();
                    //QString curText = "<b>Customer:</b> "+curApptCustomerName+"<br><b>Service:</b> "+curApptServiceName+"<br><b>Details:</b> "+curApptDetails;
                    QString curText2 = "Customer: "+curApptCustomerName+" Service: "+curApptServiceName+" Details: "+curApptDetails;
                    curItem = new MyCell(curText2);
                    //Set Appointment ID on curItem
                    curItem->setApptID(curApptID);
                    //Set appointment Stylist ID on CurItem
                    curItem->setApptStylistID(curApptStylistID);
                    //Set appointment Stylist Name on CurItem
                    curItem->setApptStylistName(curApptStylistName);
                    //Set appointment Customer Name
                    curItem->setApptCustomerName(curApptCustomerName);
                    //Set appointment Service Name
                    curItem->setApptServiceName(curApptServiceName);
                    //Set appointment Details
                    curItem->setApptDetails(curApptDetails);
                    //Set appointment Begin Time
                    curItem->setApptBeginTime(curApptTimeBeginString);
                    //Set appointment End Time
                    curItem->setApptEndTime(curApptTimeEndString);
                    //QLabel *curLabel = new QLabel(curText);
                    //curLabel->setWordWrap(true);
                    //curLabel->setAcceptDrops(true);
                    //curLabel->setAutoFillBackground(true);
                    //curTable->setCellWidget(q,i,curLabel);
                    curItem->setBackgroundColor(curColor);
                    curTable->setItem(q,i,curItem);
                    //Set Span
                    QString time_format = global_config.time_format;
                    int numberOfAppts = this->getApptDurationInMinutes(QTime::fromString(curApptTimeBeginString,time_format),QTime::fromString(curApptTimeEndString,time_format))/global_config.appointments_interval;
                    curTable->setSpan(q,i,numberOfAppts,1);
                }
            }
        }
    }
    //Resize Rows
    //curTable->resizeRowsToContents();
    //Set Edit Strategy
    curTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    //Set Sections of Horizontal Headers as fixed
    curTable->horizontalHeader()->setSectionsMovable(true);
    //
    //Enables Drag and Drop
    curTable->setDragDropMode(QAbstractItemView::InternalMove);
    curTable->setDragDropOverwriteMode(false);
    curTable->setDropIndicatorShown(true);
    curTable->setDragEnabled(true);
    //curTable->setAcceptDrops(true);

    //
    //qDebug()<<curTable->rowSpan(1,1);
    //curTable->item(0,0)->setFlags(Qt::NoItemFlags);
    //qDebug()<<curTable->item(2,4)->text();
    //qDebug()<<curTable->item(3,4)->text();

}
void MainWindow::create_employee_appt(){
    //Sets Model for Combobox
    this->stylist_model = new QSqlTableModel();
    stylist_model->setTable("stylist");
    stylist_model->select();
    this->ui->stylist_comboBox->setModel(stylist_model);
    this->ui->stylist_comboBox->setModelColumn(stylist_model->fieldIndex("name"));

    //Creates Table
    QTableWidget *curTable = this->ui->employee_day_tableWidget;
    curTable->verticalHeader()->setVisible(false);
    curTable->horizontalHeader()->setVisible(false);
    QTime curTime(global_config.first_time->hour(),global_config.first_time->minute(),global_config.first_time->second());
    //QStringList vertical_headers_labels;
    int per_column_dates=20;
    curTable->setRowCount(per_column_dates);
    for(int i=0;curTime <= (*global_config.last_time);++i){
        curTable->setColumnCount(2+((int)(i/per_column_dates))*2);
        curTable->setItem(i%per_column_dates,(int)(i/per_column_dates)*2,new QTableWidgetItem(curTime.toString(global_config.time_format)));
        curTime = curTime.addSecs(global_config.appointments_interval*60);
    }
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
        QString stylesheet_string = dialog.ui->color_view_label_12->styleSheet();
        QString color_string = stylesheet_string.mid(stylesheet_string.indexOf(":")+2,stylesheet_string.length()-stylesheet_string.indexOf(":")-3);
        stylist_model.setValue("color",color_string);
        int stylist_id = stylist_model.submitAll();
        if(stylist_id>0){
            qDebug()<<QString("Stylist Saved. ID: %1").arg(stylist_id);
            //Update the Daily Appoitments Table
            this->create_daily_appt();
        }
        return;
    }
}
void MainWindow::showCreateServiceDialog(){
    NewServiceDialog dialog(this);
    if(dialog.exec()){
        MyDataModel service_model("service");
        service_model.setValue("name",dialog.ui->name_lineEdit_2->text());
        service_model.setValue("description",dialog.ui->description_textEdit->toPlainText());
        service_model.setValue("price",dialog.ui->price_lineEdit_3->text().toFloat());
        service_model.submitAll();
        int service_id = service_model.submitAll();
        if(service_id>0){
            qDebug()<<QString("Service Saved. ID: %1").arg(service_id);
        }
        return;
    }
}
void MainWindow::showCreateAppointmentDialog(){
    NewAppointmentDialog dialog(this);
    //Buttons Connections
    QObject::connect(dialog.ui->new_customer_pushButton_3,SIGNAL(clicked()),this,SLOT(showCreateCustomerDialog()));
    QObject::connect(dialog.ui->new_service_pushButton,SIGNAL(clicked()),this,SLOT(showCreateServiceDialog()));
    QObject::connect(dialog.ui->new_stylist_pushButton_2,SIGNAL(clicked()),this,SLOT(showCreateStylistDialog()));
    //Sets Model for Customer Combobox
    QSqlTableModel customer_model;
    customer_model.setTable("customer");
    customer_model.select();
    dialog.ui->customer_comboBox->setModel(&customer_model);
    dialog.ui->customer_comboBox->setModelColumn(customer_model.fieldIndex("name"));
    //Sets Model for Service Combobox
    QSqlTableModel service_model;
    service_model.setTable("service");
    service_model.select();
    dialog.ui->service_comboBox->setModel(&service_model);
    dialog.ui->service_comboBox->setModelColumn(service_model.fieldIndex("name"));
    //Sets Model for Stylist Combobox
    QSqlTableModel stylist_model;
    stylist_model.setTable("stylist");
    stylist_model.select();
    dialog.ui->stylist_comboBox->setModel(&stylist_model);
    dialog.ui->stylist_comboBox->setModelColumn(stylist_model.fieldIndex("name"));
    //Configure Dates and Times
    dialog.ui->dateEdit->setDate(QDate::currentDate());

    //Shows Modal Dialog
    if(dialog.exec()){
        MyDataModel appointment_model("appointment");
        //Recovery the IDs from Comboboxes Labels
        QString selectedCustomer,selectedService,selectedStylist;
        selectedCustomer = dialog.ui->customer_comboBox->currentText();
        selectedService = dialog.ui->service_comboBox->currentText();
        selectedStylist = dialog.ui->stylist_comboBox->currentText();
        customer_model.setFilter("");
        appointment_model.setValue("stylist_id",dialog.ui->stylist_comboBox->currentText());
        appointment_model.setValue("customer_id",dialog.ui->customer_comboBox->currentText());
        appointment_model.setValue("service_id",dialog.ui->service_comboBox->currentText());
        appointment_model.setValue("date",dialog.ui->dateEdit->date().toString(global_config.date_format));
        appointment_model.setValue("time_begin",dialog.ui->timebegin_timeEdit->time().toString(global_config.time_format));
        appointment_model.setValue("time_end",dialog.ui->timeend_timeEdit->time().toString(global_config.time_format));
        appointment_model.setValue("details",dialog.ui->details_plainTextEdit->toPlainText());
        int appointment_id = appointment_model.submitAll();
        if(appointment_id>0){
            qDebug()<<QString("Appointment Saved. ID: %1").arg(appointment_id);
            //Update the Daily Appoitments Table
            this->create_daily_appt();
        }
        return;
    }
}
void MainWindow::showCreateUserDialog(){
    NewUserDialog dialog(this);
    if(dialog.exec()){
        MyDataModel user_model("user");
        user_model.setValue("login",dialog.ui->login_lineEdit->text());
        user_model.setValue("password",dialog.ui->password_lineEdit->text());
        user_model.setValue("email",dialog.ui->email_lineEdit->text());
        int user_id = user_model.submitAll();
        if(user_id>0){
            qDebug()<<QString("User Saved. ID: %1").arg(user_id);
        }
        return;
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
    QObject::connect(this->ui->action_edit_Users,SIGNAL(triggered()),this,SLOT(showEditUserDialog()));
    QObject::connect(this->ui->action_edit_Services,SIGNAL(triggered()),this,SLOT(showEditServiceDialog()));
    QObject::connect(this->ui->action_edit_Appointments,SIGNAL(triggered()),this,SLOT(showEditAppointmentDialog()));
    QObject::connect(this->ui->daily_appt_date_dateEdit,SIGNAL(dateChanged(QDate)),this,SLOT(create_daily_appt()));
    QObject::connect(this->ui->actionAdd_Break,SIGNAL(triggered()),this,SLOT(genPdfUser()));
    QObject::connect(this->ui->daily_appt_tableWidget->selectionModel(),SIGNAL(selectionChanged(QItemSelection,QItemSelection)),this,SLOT(dailyApptselectionChanged(QItemSelection,QItemSelection)));
    QObject::connect(this->ui->actionDelete_Record,SIGNAL(triggered()),this,SLOT(deleteSelectedAppt()));
    QObject::connect(this->ui->actionEdit_Record,SIGNAL(triggered()),this,SLOT(editSelectedAppt()));
    QObject::connect(this->ui->actionAdd,SIGNAL(triggered()),this,SLOT(addAppt()));

}
void MainWindow::showEditCustomerDialog(){
    EditCustumerDialog dialog;
    dialog.exec();
    //Update the Daily Appoitments Table
    this->create_daily_appt();
}
void MainWindow::showEditStylistDialog(){
    EditStylistDialog dialog;
    dialog.exec();
    //Update the Daily Appoitments Table
    this->create_daily_appt();
}
void MainWindow::showEditServiceDialog(){
    EditServiceDialog dialog;
    dialog.exec();
    //Update the Daily Appoitments Table
    this->create_daily_appt();
}
void MainWindow::showEditUserDialog(){
    EditUserDialog dialog;
    dialog.exec();
}
void MainWindow::showEditAppointmentDialog(){
    EditAppointmentDialog dialog;
    dialog.exec();
    //Update the Daily Appoitments Table
    this->create_daily_appt();
}



void MainWindow::saveUserPreferences(){
    QSettings toSave(QSettings::IniFormat,QSettings::UserScope,VER_COMPANYNAME_STR,VER_FILEDESCRIPTION_STR);
    toSave.beginGroup("MainWindow");
    toSave.setValue("geometry", saveGeometry());
    toSave.endGroup();
}
void MainWindow::loadUerPreferences(){
    QSettings toLoad(QSettings::IniFormat,QSettings::UserScope,VER_COMPANYNAME_STR,VER_FILEDESCRIPTION_STR);
    toLoad.beginGroup("MainWindow");
    this->restoreGeometry(toLoad.value("geometry").toByteArray());
    toLoad.endGroup();
}
void MainWindow::closeEvent(QCloseEvent *event)
{
    saveUserPreferences();
    event->accept();

}
int MainWindow::getApptDurationInMinutes(QTime begin_time, QTime end_time){
    int durationInMinutes = (end_time.hour()*60+end_time.minute())-(begin_time.hour()*60+begin_time.minute());
    return durationInMinutes;
}
