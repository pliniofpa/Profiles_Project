#include "editappointmentdialog.h"
#include "ui_editappointmentdialog.h"
#include <QSqlTableModel>
#include <QLineEdit>
#include <QList>
#include <QItemSelectionModel>
#include <QDebug>
#include <QDataWidgetMapper>
#include <QSqlRelationalTableModel>
#include <QSqlRelationalDelegate>
#include <QSqlError>
#include <QSqlRecord>
#include <QSettings>
#include "version.h"
EditAppointmentDialog::EditAppointmentDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditAppointmentDialog)
{
    ui->setupUi(this);
    //Sets Model for Service Combobox
    this->service_model = new QSqlTableModel();
    service_model->setTable("service");
    service_model->select();
    this->ui->service_comboBox->setModel(service_model);
    this->ui->service_comboBox->setModelColumn(service_model->fieldIndex("name"));
    //Sets Model for Stylist Combobox
    this->stylist_model = new QSqlTableModel();
    stylist_model->setTable("stylist");
    stylist_model->select();
    this->ui->stylist_comboBox->setModel(stylist_model);
    this->ui->stylist_comboBox->setModelColumn(stylist_model->fieldIndex("name"));
    //Sets Model for Customer Combobox
    this->customer_model = new QSqlTableModel();
    customer_model->setTable("customer");
    customer_model->select();
    this->ui->customer_comboBox->setModel(customer_model);
    this->ui->customer_comboBox->setModelColumn(customer_model->fieldIndex("name"));
    //connect(this->ui->buttonBox_2,SIGNAL(accepted()),this,SLOT(accept()));
    //Removes "What's it?" button
    this->setWindowFlags(Qt::Dialog | Qt::WindowTitleHint | Qt::WindowSystemMenuHint | Qt::WindowCloseButtonHint);
    //Make Connections
    QObject::connect(this->ui->edit_pushButton,SIGNAL(clicked()),this,SLOT(beginEditing()));
    QObject::connect(this->ui->save_pushButton_3,SIGNAL(clicked()),this,SLOT(endEditing()));
    QObject::connect(this->ui->cancel_pushButton_2,SIGNAL(clicked()),this,SLOT(endEditing()));
    //Disables Edit, Cancel and Save Buttons
    this->ui->edit_pushButton->setEnabled(false);
    this->ui->cancel_pushButton_2->setEnabled(false);
    this->ui->save_pushButton_3->setEnabled(false);
    //Disables all LineEdits
    this->ui->appointment_info_groupBox->setEnabled(false);
    //Defines Model Headers
    this->appt_assoc_names_model = new QSqlTableModel();
    appt_assoc_names_model->setTable("appt_assoc_names");
    //Define Headers
    //appt_assoc_names_model->setHeaderData(appt_assoc_names_model->fieldIndex("id"), Qt::Horizontal, tr("#"));
    appt_assoc_names_model->setHeaderData(appt_assoc_names_model->fieldIndex("time_begin"), Qt::Horizontal, tr("Start Time"));
    appt_assoc_names_model->setHeaderData(appt_assoc_names_model->fieldIndex("details"), Qt::Horizontal, tr("Details"));
    appt_assoc_names_model->setHeaderData(appt_assoc_names_model->fieldIndex("date"), Qt::Horizontal, tr("Date"));
    appt_assoc_names_model->setHeaderData(appt_assoc_names_model->fieldIndex("time_end"), Qt::Horizontal, tr("End Time"));
    appt_assoc_names_model->setHeaderData(appt_assoc_names_model->fieldIndex("customer_name"), Qt::Horizontal, tr("Customer"));
    appt_assoc_names_model->setHeaderData(appt_assoc_names_model->fieldIndex("stylist_name"), Qt::Horizontal, tr("Stylist"));
    appt_assoc_names_model->setHeaderData(appt_assoc_names_model->fieldIndex("service_name"), Qt::Horizontal, tr("Service"));
    appt_assoc_names_model->setHeaderData(appt_assoc_names_model->fieldIndex("id"), Qt::Horizontal, tr("#"));
    //appt_assoc_names_model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    appt_assoc_names_model->select();
    //Sets Model on the Table View
    this->ui->appointment_tableView->setModel(this->appt_assoc_names_model);
    //Disables Edit on Table
    this->ui->appointment_tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    //Hides some columns on Table View
    //this->ui->appointment_tableView->hideColumn(appt_assoc_names_model->fieldIndex("id"));
    this->ui->appointment_tableView->hideColumn(appt_assoc_names_model->fieldIndex("customer_id"));
    this->ui->appointment_tableView->hideColumn(appt_assoc_names_model->fieldIndex("stylist_id"));
    this->ui->appointment_tableView->hideColumn(appt_assoc_names_model->fieldIndex("service_id"));
    //Resizes the column according to its content
    this->ui->appointment_tableView->resizeColumnsToContents();
    //Defines params for selection
    this->ui->appointment_tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    this->ui->appointment_tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    //Hides Vertical Header
    this->ui->appointment_tableView->verticalHeader()->setVisible(false);
    //Fills the search ComboBox
    QStringList list = QString("Stylist,Service,Customer,Start Time,End Time").split(",");
    this->ui->comboBox->addItems(list);
    //Connect Selection Changed Signal
    connect(this->ui->appointment_tableView->selectionModel(),SIGNAL(selectionChanged(QItemSelection,QItemSelection)),this,SLOT(selectionChanged(QItemSelection,QItemSelection)));
    connect(this->ui->search_lineEdit_2,SIGNAL(textEdited(QString)),this,SLOT(searchTextChanged(QString)));
    connect(this->ui->comboBox,SIGNAL(currentTextChanged(QString)),this,SLOT(combobox_text_changed(QString)));
    //Update ComboBox Text
    this->combobox_text_changed(this->ui->comboBox->currentText());
    connect(this->ui->search_dateEdit,SIGNAL(dateChanged(QDate)),this,SLOT(searchDateChanged(QDate)));
    //Configure Date and Time Ranges
    QObject::connect(this->ui->duration_spinBox,SIGNAL(editingFinished()),this,SLOT(updateEndTime()));
    QObject::connect(this->ui->timeend_timeEdit,SIGNAL(editingFinished()),this,SLOT(updateDuration()));
    QObject::connect(this->ui->timebegin_timeEdit,SIGNAL(timeChanged(QTime)),this,SLOT(setRange(QTime)));
    this->ui->timebegin_timeEdit->setTimeRange(*this->global_config.first_time,*this->global_config.last_time);
    //Load Preferences
    this->loadUerPreferences();
}

EditAppointmentDialog::~EditAppointmentDialog()
{
    delete ui;
}

void EditAppointmentDialog::combobox_text_changed(QString text){
    QString field;
    QString other="";
    this->ui->search_lineEdit_2->setVisible(true);
    this->ui->search_dateEdit->setVisible(false);
    this->searchTextChanged(QString());
    if (text=="#") {
        field = "id";
        this->ui->search_lineEdit_2->setVisible(true);
        this->ui->search_dateEdit->setVisible(false);
        this->ui->search_lineEdit_2->setInputMask("D0000");
    }else
        if(text=="Name")
        {
            field = "name";
            this->ui->search_lineEdit_2->setInputMask("");
        }else
            if(text=="Phone")
            {
                field = "phone1";
                other = " OR phone2 LIKE '%%2%'";
                this->ui->search_lineEdit_2->setInputMask("0000000000");
            }else
                if(text=="Email")
                {
                    field = "email";
                    this->ui->search_lineEdit_2->setInputMask("");
                }else
                    if(text=="Birthday")
                    {
                        field = "birthday";
                        this->ui->search_lineEdit_2->setVisible(false);
                        this->ui->search_dateEdit->setDate(QDate::currentDate());
                        this->ui->search_dateEdit->setVisible(true);
                    }
    this->filter = QString("%1 LIKE '%%2%'"+other).arg(field);
}

void EditAppointmentDialog::selectionChanged(const QItemSelection & selected, const QItemSelection & deselected){
    if(selected.indexes().length()){
        this->ui->edit_pushButton->setEnabled(true);
        QString date_string = selected.indexes().at(this->appt_assoc_names_model->fieldIndex("date")).data(0).toString();
        QString time_begin_string = selected.indexes().at(this->appt_assoc_names_model->fieldIndex("time_begin")).data(0).toString();
        QString time_end_string = selected.indexes().at(this->appt_assoc_names_model->fieldIndex("time_end")).data(0).toString();
        QString customer_name = selected.indexes().at(this->appt_assoc_names_model->fieldIndex("customer_name")).data(0).toString();
        QString service_name = selected.indexes().at(this->appt_assoc_names_model->fieldIndex("service_name")).data(0).toString();
        QString stylist_name = selected.indexes().at(this->appt_assoc_names_model->fieldIndex("stylist_name")).data(0).toString();
        QString details = selected.indexes().at(this->appt_assoc_names_model->fieldIndex("details")).data(0).toString();
        this->ui->dateEdit->setDate(QDate::fromString(date_string,global_config.date_format));
        this->ui->timebegin_timeEdit->setTime(QTime::fromString(time_begin_string,global_config.time_format));
        this->ui->timeend_timeEdit->setTime(QTime::fromString(time_end_string,global_config.time_format));
        this->ui->customer_comboBox->setCurrentText(customer_name);
        this->ui->service_comboBox->setCurrentText(service_name);
        this->ui->stylist_comboBox->setCurrentText(stylist_name);
        this->ui->details_plainTextEdit->setPlainText(details);
        //Update Interval
        QTime end_time = this->ui->timeend_timeEdit->time();
        QTime begin_time = this->ui->timebegin_timeEdit->time();
        this->ui->duration_spinBox->setValue((end_time.hour()*60+end_time.minute())-(begin_time.hour()*60+begin_time.minute()));
    }else{
        this->ui->edit_pushButton->setEnabled(false);
    }
}

void EditAppointmentDialog::searchTextChanged(QString text){
    if(!text.isEmpty()){
        this->appt_assoc_names_model->setFilter(QString(filter).arg(text));
    }else{
        this->appt_assoc_names_model->setFilter("");
    }

}
void EditAppointmentDialog::searchDateChanged(QDate date){
    QString tmp_filter = QString(filter).arg(date.toString(global_config.date_format));
    qDebug() << tmp_filter;
    this->appt_assoc_names_model->setFilter(tmp_filter);
    int row = this->ui->appointment_tableView->selectionModel()->currentIndex().row();
    qDebug() << this->appt_assoc_names_model->data(this->ui->appointment_tableView->selectionModel()->currentIndex()).toString();
}

void EditAppointmentDialog::beginEditing(){
    //Disables Edit Button
    this->ui->edit_pushButton->setEnabled(false);
    //Enables Save Button
    this->ui->save_pushButton_3->setEnabled(true);
    //Enables Cancel Button
    this->ui->cancel_pushButton_2->setEnabled(true);
    //Enables all LineEdits
    this->ui->appointment_info_groupBox->setEnabled(true);



    /*
    QList<QLineEdit *> lineEdits = this->findChildren<QLineEdit *>();
    for(int i=0;i<lineEdits.length();++i){
        lineEdits.at(i)->setEnabled(true);
    }
    */
    this->ui->appointments_groupBox_3->setEnabled(false);
}
void EditAppointmentDialog::endEditing(){
    //Enables Edit Button
    this->ui->edit_pushButton->setEnabled(true);
    //Disables Save Button
    this->ui->save_pushButton_3->setEnabled(false);
    //Disables Cancel Button
    this->ui->cancel_pushButton_2->setEnabled(false);
    //Disables all LineEdits
    this->ui->appointment_info_groupBox->setEnabled(false);
    //Enable table GroupBox and Search Form
    this->ui->appointments_groupBox_3->setEnabled(true);
    this->ui->search_lineEdit_2->setEnabled(true);
    /*
    //Mapper
    appointment_mapper->setCurrentIndex(appointment_mapper->currentIndex());
    */
}

void EditAppointmentDialog::on_save_pushButton_3_clicked()
{
    //Recovery the Record from Appointment Table
    int curRow = this->ui->appointment_tableView->currentIndex().row();
    QSqlTableModel appointment_model;
    appointment_model.setTable("appointment");
    appointment_model.setEditStrategy(QSqlTableModel::OnManualSubmit);
    appointment_model.select();
    QSqlRecord curRecord = appointment_model.record(curRow);
    //Update Date, Times and Details
    curRecord.setValue("date",this->ui->dateEdit->date().toString(global_config.date_format));
    curRecord.setValue("time_begin",this->ui->timebegin_timeEdit->time().toString(global_config.time_format));
    curRecord.setValue("time_end",this->ui->timeend_timeEdit->time().toString(global_config.time_format));
    curRecord.setValue("details",this->ui->details_plainTextEdit->toPlainText());
    //Update Customer Table
    this->customer_model->setFilter(QString("name='%1'").arg(this->ui->customer_comboBox->currentText()));
    QSqlRecord curCustomerRecord = customer_model->record(0);
    curRecord.setValue("customer_id",curCustomerRecord.value("id"));
    this->customer_model->setFilter("");
    //Update Service Table
    this->service_model->setFilter(QString("name='%1'").arg(this->ui->service_comboBox->currentText()));
    QSqlRecord curServiceRecord = service_model->record(0);
    curRecord.setValue("service_id",curServiceRecord.value("id"));
    this->service_model->setFilter("");
    //Update Stylist Table
    this->stylist_model->setFilter(QString("name='%1'").arg(this->ui->stylist_comboBox->currentText()));
    QSqlRecord curStylistRecord = stylist_model->record(0);
    curRecord.setValue("stylist_id",curStylistRecord.value("id"));
    this->stylist_model->setFilter("");
    appointment_model.setRecord(curRow,curRecord);
    if(appointment_model.submitAll()){
        //Recovery the data from view
        appt_assoc_names_model->select();
        //Resize Table View Columns and Rows
        this->ui->appointment_tableView->resizeColumnsToContents();
        this->ui->appointment_tableView->resizeRowsToContents();
        qDebug()<<"Record successfull edited";
    }else{
        qDebug()<<"An error occorred while saving table "<<appt_assoc_names_model->lastError().text();
    }
}
void EditAppointmentDialog::setRange(QTime beginTime){
    this->ui->timeend_timeEdit->setTimeRange(beginTime.addSecs(global_config.appointments_interval*60),*this->global_config.last_time);
}
void EditAppointmentDialog::updateDuration(){
     QTime curTime = this->ui->timeend_timeEdit->time();
    int minutes = curTime.minute();
    int rest = minutes%global_config.appointments_interval;
    if(rest){
        float result= (float)rest/(float)global_config.appointments_interval;
        curTime = curTime.addSecs(-minutes*60);
        if(result>=0.5){
            this->ui->timeend_timeEdit->setTime(curTime.addSecs(60*(minutes+(global_config.appointments_interval-rest))));
        }else{
            this->ui->timeend_timeEdit->setTime(curTime.addSecs((minutes-rest)*60));
        }
    }
    //Update End Time
    QTime end_time = this->ui->timeend_timeEdit->time();
    QTime begin_time = this->ui->timebegin_timeEdit->time();
    this->ui->duration_spinBox->setValue((end_time.hour()*60+end_time.minute())-(begin_time.hour()*60+begin_time.minute()));
}
void EditAppointmentDialog::updateEndTime(){
    int duration = this->ui->duration_spinBox->value();
    int rest = duration%global_config.appointments_interval;
    if(rest){
        float result= (float)rest/(float)global_config.appointments_interval;
        if(result>=0.5){
            this->ui->duration_spinBox->setValue(duration+(global_config.appointments_interval-rest));
        }else{
            this->ui->duration_spinBox->setValue(duration-rest);
        }
    }
    //Update End Time
    this->ui->timeend_timeEdit->setTime(this->ui->timebegin_timeEdit->time().addSecs(this->ui->duration_spinBox->value()*60));
}
void EditAppointmentDialog::saveUserPreferences(){
    QSettings toSave(QSettings::IniFormat,QSettings::UserScope,VER_COMPANYNAME_STR,VER_FILEDESCRIPTION_STR);
    toSave.beginGroup("Appointments");
    toSave.setValue("geometry", saveGeometry());
    toSave.endGroup();
}
void EditAppointmentDialog::loadUerPreferences(){
    QSettings toLoad(QSettings::IniFormat,QSettings::UserScope,VER_COMPANYNAME_STR,VER_FILEDESCRIPTION_STR);
    toLoad.beginGroup("Appointments");
    this->restoreGeometry(toLoad.value("geometry").toByteArray());
    toLoad.endGroup();
}
void EditAppointmentDialog::closeEvent(QCloseEvent *event)
{
    saveUserPreferences();
    event->accept();

}
