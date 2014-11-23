#include "editservicedialog.h"
#include "ui_editservicedialog.h"
#include <QSqlTableModel>
#include <QLineEdit>
#include <QSqlError>
#include <QList>
#include <QItemSelectionModel>
#include <QDebug>
#include <QDataWidgetMapper>
#include <QSqlRelationalTableModel>
#include <QSqlRelationalDelegate>
EditServiceDialog::EditServiceDialog(QWidget *parent) :
QDialog(parent),
ui(new Ui::EditServiceDialog)
{
    ui->setupUi(this);
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
    this->ui->service_info_groupBox_2->setEnabled(false);
    //Defines Model Headers
    this->service_model = new QSqlTableModel();
    service_model->setTable("service");
    //Define Headers
    service_model->setHeaderData(service_model->fieldIndex("id"), Qt::Horizontal, tr("#"));
    service_model->setHeaderData(service_model->fieldIndex("name"), Qt::Horizontal, tr("Name"));
    service_model->setHeaderData(service_model->fieldIndex("price"), Qt::Horizontal, tr("Price"));
    service_model->setHeaderData(service_model->fieldIndex("description"), Qt::Horizontal, tr("Description"));
    service_model->select();
    //this->ui->customer_tableView->setItemDelegate(new QSqlRelationalDelegate(this->ui->customer_tableView));
    //Sets Model on the Table View
    this->ui->service_tableView->setModel(this->service_model);
    //Disables Edit on Table
    this->ui->service_tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    //Hides some columns on Table View
    //--this->ui->customer_tableView->hideColumn(customer_model->fieldIndex("address_id"));
    //--this->ui->customer_tableView->hideColumn(customer_model->fieldIndex("points"));
    //Resizes the column according to its content
    this->ui->service_tableView->resizeColumnsToContents();
    //Defines params for selection
    this->ui->service_tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    this->ui->service_tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    //Hides Vertical Header
    this->ui->service_tableView->verticalHeader()->setVisible(false);
    //Fills the search ComboBox
    QStringList list = QString("#,Name,Price").split(",");
    this->ui->comboBox->addItems(list);
    //Connect Selection Changed Signal
    connect(this->ui->service_tableView->selectionModel(),SIGNAL(selectionChanged(QItemSelection,QItemSelection)),this,SLOT(selectionChanged(QItemSelection,QItemSelection)));
    connect(this->ui->search_lineEdit_2,SIGNAL(textEdited(QString)),this,SLOT(searchTextChanged(QString)));
    connect(this->ui->comboBox,SIGNAL(currentTextChanged(QString)),this,SLOT(combobox_text_changed(QString)));
    //Creates Data Mapper
    service_mapper = new QDataWidgetMapper;
    service_mapper->setModel(this->service_model);
    service_mapper->addMapping(this->ui->id_lineEdit, this->service_model->fieldIndex("id"));
    service_mapper->addMapping(this->ui->name_lineEdit_3, this->service_model->fieldIndex("name"));
    service_mapper->addMapping(this->ui->price_lineEdit_4, this->service_model->fieldIndex("price"));
    service_mapper->addMapping(this->ui->description_plainTextEdit, this->service_model->fieldIndex("description"));
    service_mapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);
    //service_mapper->toFirst();
    connect(this->ui->service_tableView->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),
            service_mapper, SLOT(setCurrentModelIndex(QModelIndex)));
    //Update ComboBox Text
    this->combobox_text_changed(this->ui->comboBox->currentText());
    connect(this->ui->search_dateEdit,SIGNAL(dateChanged(QDate)),this,SLOT(searchDateChanged(QDate)));
}

EditServiceDialog::~EditServiceDialog()
{
    delete ui;
}

void EditServiceDialog::combobox_text_changed(QString text){
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

void EditServiceDialog::selectionChanged(const QItemSelection & selected, const QItemSelection & deselected){
    if(selected.indexes().length()){
        this->ui->edit_pushButton->setEnabled(true);
        //int id = selected.indexes().at(0).data(0).toInt();
        //qDebug()<<id;
    }else{
        this->ui->edit_pushButton->setEnabled(false);
    }
}

void EditServiceDialog::searchTextChanged(QString text){
    if(!text.isEmpty()){
        this->service_model->setFilter(QString(filter).arg(text));
    }else{
        this->service_model->setFilter("");
    }
    
}
void EditServiceDialog::searchDateChanged(QDate date){
    QString tmp_filter = QString(filter).arg(date.toString(global_config.date_format));
    qDebug() << tmp_filter;
    this->service_model->setFilter(tmp_filter);
    int row = this->ui->service_tableView->selectionModel()->currentIndex().row();
    qDebug() << this->service_model->data(this->ui->service_tableView->selectionModel()->currentIndex()).toString();
}

void EditServiceDialog::beginEditing(){
    //Disables Edit Button
    this->ui->edit_pushButton->setEnabled(false);
    //Enables Save Button
    this->ui->save_pushButton_3->setEnabled(true);
    //Enables Cancel Button
    this->ui->cancel_pushButton_2->setEnabled(true);
    //Enables all LineEdits
    this->ui->service_info_groupBox_2->setEnabled(true);
    /*
     QList<QLineEdit *> lineEdits = this->findChildren<QLineEdit *>();
     for(int i=0;i<lineEdits.length();++i){
     lineEdits.at(i)->setEnabled(true);
     }
     */
    this->ui->services_groupBox_3->setEnabled(false);
}
void EditServiceDialog::endEditing(){
    //Enables Edit Button
    this->ui->edit_pushButton->setEnabled(true);
    //Disables Save Button
    this->ui->save_pushButton_3->setEnabled(false);
    //Disables Cancel Button
    this->ui->cancel_pushButton_2->setEnabled(false);
    //Disables all LineEdits
    this->ui->service_info_groupBox_2->setEnabled(false);
    //Enable table GroupBox and Search Form
    this->ui->services_groupBox_3->setEnabled(true);
    this->ui->search_lineEdit_2->setEnabled(true);
    //Mapper
    service_mapper->setCurrentIndex(service_mapper->currentIndex());
}

void EditServiceDialog::on_save_pushButton_3_clicked()
{
    if(service_mapper->submit()){
        qDebug()<<"Record successfull edited";
        this->ui->service_tableView->resizeColumnsToContents();
        this->ui->service_tableView->resizeRowsToContents();
    }else{
        qDebug()<<"An error occorred while saving table "<<service_model->lastError().text();
    }
}

