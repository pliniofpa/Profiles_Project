#include "editcustumerdialog.h"
#include "ui_editcustumerdialog.h"
#include <QSqlTableModel>
#include <QLineEdit>
#include <QList>
#include <QItemSelectionModel>
#include <QDebug>
#include <QDataWidgetMapper>
#include <QSqlRelationalTableModel>
#include <QSqlRelationalDelegate>
#include <QSqlError>
EditCustumerDialog::EditCustumerDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditCustumerDialog)
{
    ui->setupUi(this);
    //Removes "What's it?" button
    this->setWindowFlags(Qt::Dialog | Qt::WindowTitleHint | Qt::WindowSystemMenuHint | Qt::WindowCloseButtonHint);
    //Make Connections
    QObject::connect(this->ui->edit_pushButton,SIGNAL(clicked()),this,SLOT(beginEditing()));
    QObject::connect(this->ui->save_pushButton_3,SIGNAL(clicked()),this,SLOT(endEditing()));
    QObject::connect(this->ui->cancel_pushButton_2,SIGNAL(clicked()),this,SLOT(endEditing()));
    //Sets Items for State Combobox
    this->ui->state_comboBox->addItems(global_config.usa_states);
    //Disables Edit, Cancel and Save Buttons
    this->ui->edit_pushButton->setEnabled(false);
    this->ui->cancel_pushButton_2->setEnabled(false);
    this->ui->save_pushButton_3->setEnabled(false);
    //Disables all LineEdits
    this->ui->personal_info_groupBox->setEnabled(false);
    this->ui->address_info_groupBox_2->setEnabled(false);
    //Defines Model Headers
    this->customer_model = new QSqlTableModel();
    customer_model->setTable("customer");
    //Define Headers
    customer_model->setHeaderData(customer_model->fieldIndex("id"), Qt::Horizontal, tr("#"));
    customer_model->setHeaderData(customer_model->fieldIndex("name"), Qt::Horizontal, tr("Name"));
    customer_model->setHeaderData(customer_model->fieldIndex("email"), Qt::Horizontal, tr("Email"));
    customer_model->setHeaderData(customer_model->fieldIndex("phone1"), Qt::Horizontal, tr("Phone 1"));
    customer_model->setHeaderData(customer_model->fieldIndex("phone2"), Qt::Horizontal, tr("Phone 2"));
    customer_model->setHeaderData(customer_model->fieldIndex("birthday"), Qt::Horizontal, tr("Birthday Date"));
    customer_model->setHeaderData(customer_model->fieldIndex("zip_code"), Qt::Horizontal, tr("Zip-Code"));
    customer_model->setHeaderData(customer_model->fieldIndex("address"), Qt::Horizontal, tr("Address"));
    customer_model->setHeaderData(customer_model->fieldIndex("state"), Qt::Horizontal, tr("State"));
    customer_model->setHeaderData(customer_model->fieldIndex("city"), Qt::Horizontal, tr("City"));
    //customer_model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    customer_model->select();
    //Sets Model on the Table View
    this->ui->customer_tableView->setModel(this->customer_model);
    //Disables Edit on Table
    this->ui->customer_tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    //Hides some columns on Table View
    this->ui->customer_tableView->hideColumn(customer_model->fieldIndex("points"));
    //Resizes the column according to its content
    this->ui->customer_tableView->resizeColumnsToContents();
    //Defines params for selection
    this->ui->customer_tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    this->ui->customer_tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    //Hides Vertical Header
    this->ui->customer_tableView->verticalHeader()->setVisible(false);
    //Fills the search ComboBox
    QStringList list = QString("#,Name,Email,Phone,Birthday,Address,Zip-Code,State,City").split(",");
    this->ui->comboBox->addItems(list);
    //Connect Selection Changed Signal
    connect(this->ui->customer_tableView->selectionModel(),SIGNAL(selectionChanged(QItemSelection,QItemSelection)),this,SLOT(selectionChanged(QItemSelection,QItemSelection)));
    connect(this->ui->search_lineEdit_2,SIGNAL(textEdited(QString)),this,SLOT(searchTextChanged(QString)));
    connect(this->ui->comboBox,SIGNAL(currentTextChanged(QString)),this,SLOT(combobox_text_changed(QString)));
    //Creates Data Mapper
    customer_mapper = new QDataWidgetMapper;
    customer_mapper->setModel(this->customer_model);
    customer_mapper->addMapping(this->ui->id_lineEdit, this->customer_model->fieldIndex("id"));
    customer_mapper->addMapping(this->ui->name_lineEdit_2, this->customer_model->fieldIndex("name"));
    customer_mapper->addMapping(this->ui->email_lineEdit_4, this->customer_model->fieldIndex("email"));
    customer_mapper->addMapping(this->ui->phone1_lineEdit_2, this->customer_model->fieldIndex("phone1"));
    customer_mapper->addMapping(this->ui->phone2_lineEdit_3, this->customer_model->fieldIndex("phone2"));
    customer_mapper->addMapping(this->ui->birthday_dateEdit, this->customer_model->fieldIndex("birthday"));
    customer_mapper->addMapping(this->ui->address_lineEdit_5, this->customer_model->fieldIndex("address"));
    customer_mapper->addMapping(this->ui->zip_code_lineEdit_6, this->customer_model->fieldIndex("zip_code"));
    customer_mapper->addMapping(this->ui->state_comboBox, this->customer_model->fieldIndex("state"));
    customer_mapper->addMapping(this->ui->city_lineEdit_7, this->customer_model->fieldIndex("city"));
    customer_mapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);
    //customer_mapper->toFirst();
    //Creates connection for selection of Row on Table
    connect(this->ui->customer_tableView->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),
            customer_mapper, SLOT(setCurrentModelIndex(QModelIndex)));
    //Update ComboBox Text
    this->combobox_text_changed(this->ui->comboBox->currentText());
    connect(this->ui->search_dateEdit,SIGNAL(dateChanged(QDate)),this,SLOT(searchDateChanged(QDate)));
}

void EditCustumerDialog::combobox_text_changed(QString text){
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

void EditCustumerDialog::selectionChanged(const QItemSelection & selected, const QItemSelection & deselected){
    if(selected.indexes().length()){
        this->ui->edit_pushButton->setEnabled(true);
        //int id = selected.indexes().at(0).data(0).toInt();
        //qDebug()<<id;
    }else{
        this->ui->edit_pushButton->setEnabled(false);
    }
}

void EditCustumerDialog::searchTextChanged(QString text){
    if(!text.isEmpty()){
        this->customer_model->setFilter(QString(filter).arg(text));
    }else{
        this->customer_model->setFilter("");
    }

}
void EditCustumerDialog::searchDateChanged(QDate date){
    QString tmp_filter = QString(filter).arg(date.toString(global_config.date_format));
    qDebug() << tmp_filter;
    this->customer_model->setFilter(tmp_filter);
    int row = this->ui->customer_tableView->selectionModel()->currentIndex().row();
    qDebug() << this->customer_model->data(this->ui->customer_tableView->selectionModel()->currentIndex()).toString();
}

EditCustumerDialog::~EditCustumerDialog()
{
    delete ui;
}
void EditCustumerDialog::beginEditing(){
    //Disables Edit Button
    this->ui->edit_pushButton->setEnabled(false);
    //Enables Save Button
    this->ui->save_pushButton_3->setEnabled(true);
    //Enables Cancel Button
    this->ui->cancel_pushButton_2->setEnabled(true);
    //Enables all LineEdits
    this->ui->personal_info_groupBox->setEnabled(true);
    this->ui->address_info_groupBox_2->setEnabled(true);
    /*
    QList<QLineEdit *> lineEdits = this->findChildren<QLineEdit *>();
    for(int i=0;i<lineEdits.length();++i){
        lineEdits.at(i)->setEnabled(true);
    }
    */
    this->ui->customers_groupBox_3->setEnabled(false);
}
void EditCustumerDialog::endEditing(){
    //Enables Edit Button
    this->ui->edit_pushButton->setEnabled(true);
    //Disables Save Button
    this->ui->save_pushButton_3->setEnabled(false);
    //Disables Cancel Button
    this->ui->cancel_pushButton_2->setEnabled(false);
    //Disables all LineEdits
    this->ui->personal_info_groupBox->setEnabled(false);
    this->ui->address_info_groupBox_2->setEnabled(false);
    //Enable table GroupBox and Search Form
    this->ui->customers_groupBox_3->setEnabled(true);
    this->ui->search_lineEdit_2->setEnabled(true);
    //Mapper
    customer_mapper->setCurrentIndex(customer_mapper->currentIndex());
}

void EditCustumerDialog::on_save_pushButton_3_clicked()
{
    if(customer_mapper->submit()){
        qDebug()<<"Record successfull edited";
        this->ui->customer_tableView->resizeColumnsToContents();
        this->ui->customer_tableView->resizeRowsToContents();
    }else{
        qDebug()<<"An error occorred while saving table "<<customer_model->lastError().text();
    }
}
