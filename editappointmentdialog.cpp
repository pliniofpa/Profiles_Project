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
    this->appointment_model = new QSqlTableModel();
    appointment_model->setTable("schedule");
    //Define Headers
    //appointment_model->setHeaderData(appointment_model->fieldIndex("id"), Qt::Horizontal, tr("#"));
    appointment_model->setHeaderData(appointment_model->fieldIndex("stylist_id"), Qt::Horizontal, tr("Stylist"));
    appointment_model->setHeaderData(appointment_model->fieldIndex("service_id"), Qt::Horizontal, tr("Service"));
    appointment_model->setHeaderData(appointment_model->fieldIndex("customer_id"), Qt::Horizontal, tr("Customer"));
    appointment_model->setHeaderData(appointment_model->fieldIndex("datetime_begin"), Qt::Horizontal, tr("Start Time"));
    appointment_model->setHeaderData(appointment_model->fieldIndex("datetime_end"), Qt::Horizontal, tr("End Time"));
    //appointment_model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    appointment_model->select();
    //Sets Model on the Table View
    this->ui->appointment_tableView->setModel(this->appointment_model);
    //Disables Edit on Table
    this->ui->appointment_tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    //Hides some columns on Table View
    this->ui->appointment_tableView->hideColumn(appointment_model->fieldIndex("points"));
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
    //Creates Data Mapper
    appointment_mapper = new QDataWidgetMapper;
    appointment_mapper->setModel(this->appointment_model);
    appointment_mapper->addMapping(this->ui->details_plainTextEdit, this->appointment_model->fieldIndex("details"));
    appointment_mapper->addMapping(this->ui->stylist_comboBox, this->appointment_model->fieldIndex("stylist_id"));
    appointment_mapper->addMapping(this->ui->service_comboBox, this->appointment_model->fieldIndex("service_id"));
    appointment_mapper->addMapping(this->ui->customer_comboBox, this->appointment_model->fieldIndex("customer_id"));
    //appointment_mapper->addMapping(this->ui->timebegin_timeEdit, this->appointment_model->fieldIndex("time_begin"));
    //appointment_mapper->addMapping(this->ui->timeend_timeEdit, this->appointment_model->fieldIndex("time_end"));

    appointment_mapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);
    //appointment_mapper->toFirst();
    //Creates connection for selection of Row on Table
    connect(this->ui->appointment_tableView->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),
            appointment_mapper, SLOT(setCurrentModelIndex(QModelIndex)));
    //Update ComboBox Text
    this->combobox_text_changed(this->ui->comboBox->currentText());
    connect(this->ui->search_dateEdit,SIGNAL(dateChanged(QDate)),this,SLOT(searchDateChanged(QDate)));
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
        QString date_string = selected.indexes().at(this->appointment_model->fieldIndex("date")).data(0).toString();
        QString time_begin_string = selected.indexes().at(this->appointment_model->fieldIndex("time_begin")).data(0).toString();
        QString time_end_string = selected.indexes().at(this->appointment_model->fieldIndex("time_end")).data(0).toString();
        qDebug()<<date_string<<time_begin_string<<time_end_string;
        this->ui->dateEdit->setDate(QDate::fromString(date_string,global_config.date_format));
        this->ui->timebegin_timeEdit->setTime(QTime::fromString(time_begin_string,global_config.time_format));
        this->ui->timeend_timeEdit->setTime(QTime::fromString(time_end_string,global_config.time_format));
    }else{
        this->ui->edit_pushButton->setEnabled(false);
    }
}

void EditAppointmentDialog::searchTextChanged(QString text){
    if(!text.isEmpty()){
        this->appointment_model->setFilter(QString(filter).arg(text));
    }else{
        this->appointment_model->setFilter("");
    }

}
void EditAppointmentDialog::searchDateChanged(QDate date){
    QString tmp_filter = QString(filter).arg(date.toString(global_config.date_format));
    qDebug() << tmp_filter;
    this->appointment_model->setFilter(tmp_filter);
    int row = this->ui->appointment_tableView->selectionModel()->currentIndex().row();
    qDebug() << this->appointment_model->data(this->ui->appointment_tableView->selectionModel()->currentIndex()).toString();
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
    //Mapper
    appointment_mapper->setCurrentIndex(appointment_mapper->currentIndex());
}

void EditAppointmentDialog::on_save_pushButton_3_clicked()
{
    if(appointment_mapper->submit()){
        qDebug()<<"Record successfull edited";
        this->ui->appointment_tableView->resizeColumnsToContents();
        this->ui->appointment_tableView->resizeRowsToContents();
        //Update Date and Times
        int curRow = this->ui->appointment_tableView->currentIndex().row();
        QSqlRecord curRecord = this->appointment_model->record(curRow);
        curRecord.setValue("date",this->ui->dateEdit->date().toString(global_config.date_format));
        curRecord.setValue("time_begin",this->ui->timebegin_timeEdit->time().toString(global_config.time_format));
        curRecord.setValue("time_end",this->ui->timeend_timeEdit->time().toString(global_config.time_format));
        this->appointment_model->setEditStrategy(QSqlTableModel::OnManualSubmit);
        this->appointment_model->setRecord(curRow,curRecord);
        this->appointment_model->submitAll();
    }else{
        qDebug()<<"An error occorred while saving table "<<appointment_model->lastError().text();
    }
}
