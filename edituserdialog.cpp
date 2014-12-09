#include "edituserdialog.h"
#include "ui_edituserdialog.h"
#include <QSqlTableModel>
#include <QLineEdit>
#include <QList>
#include <QItemSelectionModel>
#include <QDebug>
#include <QDataWidgetMapper>
#include <QSqlRelationalTableModel>
#include <QSqlRelationalDelegate>
#include <QSqlError>

EditUserDialog::EditUserDialog(QWidget *parent) :
QDialog(parent),
ui(new Ui::EditUserDialog)
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
    this->ui->personal_info_groupBox->setEnabled(false);
    //Defines Model Headers
    this->user_model = new QSqlTableModel();
    user_model->setTable("user");
    //Define Headers
    user_model->setHeaderData(user_model->fieldIndex("id"), Qt::Horizontal, tr("#"));
    user_model->setHeaderData(user_model->fieldIndex("login"), Qt::Horizontal, tr("Login"));
    user_model->setHeaderData(user_model->fieldIndex("email"), Qt::Horizontal, tr("Email"));
    user_model->setHeaderData(user_model->fieldIndex("password"), Qt::Horizontal, tr("Password"));
    //user_model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    user_model->select();
    //Sets Model on the Table View
    this->ui->user_tableView->setModel(this->user_model);
    //Disables Edit on Table
    this->ui->user_tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    //Hides some columns on Table View
    this->ui->user_tableView->hideColumn(user_model->fieldIndex("points"));
    //Resizes the column according to its content
    this->ui->user_tableView->resizeColumnsToContents();
    //Defines params for selection
    this->ui->user_tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    this->ui->user_tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    //Hides Vertical Header
    this->ui->user_tableView->verticalHeader()->setVisible(false);
    //Fills the search ComboBox
    QStringList list = QString("#,Login,Email").split(",");
    this->ui->comboBox->addItems(list);
    //Connect Selection Changed Signal
    connect(this->ui->user_tableView->selectionModel(),SIGNAL(selectionChanged(QItemSelection,QItemSelection)),this,SLOT(selectionChanged(QItemSelection,QItemSelection)));
    connect(this->ui->search_lineEdit_2,SIGNAL(textEdited(QString)),this,SLOT(searchTextChanged(QString)));
    connect(this->ui->comboBox,SIGNAL(currentTextChanged(QString)),this,SLOT(combobox_text_changed(QString)));
    //Creates Data Mapper
    user_mapper = new QDataWidgetMapper;
    user_mapper->setModel(this->user_model);
    user_mapper->addMapping(this->ui->id_lineEdit, this->user_model->fieldIndex("id"));
    user_mapper->addMapping(this->ui->login_lineEdit_2, this->user_model->fieldIndex("login"));
    user_mapper->addMapping(this->ui->email_lineEdit_4, this->user_model->fieldIndex("email"));
    user_mapper->addMapping(this->ui->password_lineEdit_4, this->user_model->fieldIndex("password"));
    user_mapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);
    //user_mapper->toFirst();
    //Creates connection for selection of Row on Table
    connect(this->ui->user_tableView->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),
            user_mapper, SLOT(setCurrentModelIndex(QModelIndex)));
    //Update ComboBox Text
    this->combobox_text_changed(this->ui->comboBox->currentText());
    connect(this->ui->search_dateEdit,SIGNAL(dateChanged(QDate)),this,SLOT(searchDateChanged(QDate)));
}

void EditUserDialog::combobox_text_changed(QString text){
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

void EditUserDialog::selectionChanged(const QItemSelection & selected, const QItemSelection & deselected){
    Q_UNUSED(deselected)
    if(selected.indexes().length()){
        this->ui->edit_pushButton->setEnabled(true);
        //int id = selected.indexes().at(0).data(0).toInt();
        //qDebug()<<id;
    }else{
        this->ui->edit_pushButton->setEnabled(false);
    }
}

void EditUserDialog::searchTextChanged(QString text){
    if(!text.isEmpty()){
        this->user_model->setFilter(QString(filter).arg(text));
    }else{
        this->user_model->setFilter("");
    }
    
}
void EditUserDialog::searchDateChanged(QDate date){
    QString tmp_filter = QString(filter).arg(date.toString(global_config.date_format));
    qDebug() << tmp_filter;
    this->user_model->setFilter(tmp_filter);
    //int row = this->ui->user_tableView->selectionModel()->currentIndex().row();
    qDebug() << this->user_model->data(this->ui->user_tableView->selectionModel()->currentIndex()).toString();
}
void EditUserDialog::beginEditing(){
    //Disables Edit Button
    this->ui->edit_pushButton->setEnabled(false);
    //Enables Save Button
    this->ui->save_pushButton_3->setEnabled(true);
    //Enables Cancel Button
    this->ui->cancel_pushButton_2->setEnabled(true);
    //Enables all LineEdits
    this->ui->personal_info_groupBox->setEnabled(true);
    /*
     QList<QLineEdit *> lineEdits = this->findChildren<QLineEdit *>();
     for(int i=0;i<lineEdits.length();++i){
     lineEdits.at(i)->setEnabled(true);
     }
     */
    this->ui->users_groupBox_3->setEnabled(false);
}
void EditUserDialog::endEditing(){
    //Enables Edit Button
    this->ui->edit_pushButton->setEnabled(true);
    //Disables Save Button
    this->ui->save_pushButton_3->setEnabled(false);
    //Disables Cancel Button
    this->ui->cancel_pushButton_2->setEnabled(false);
    //Disables all LineEdits
    this->ui->personal_info_groupBox->setEnabled(false);
    //Enable table GroupBox and Search Form
    this->ui->users_groupBox_3->setEnabled(true);
    this->ui->search_lineEdit_2->setEnabled(true);
    //Mapper
    user_mapper->setCurrentIndex(user_mapper->currentIndex());
}

void EditUserDialog::on_save_pushButton_3_clicked()
{
    if(user_mapper->submit()){
        qDebug()<<"Record successfull edited";
        this->ui->user_tableView->resizeColumnsToContents();
        this->ui->user_tableView->resizeRowsToContents();
    }else{
        qDebug()<<"An error occorred while saving table "<<user_model->lastError().text();
    }
}

EditUserDialog::~EditUserDialog()
{
    delete ui;
}
