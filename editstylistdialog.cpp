#include "editstylistdialog.h"
#include "ui_editstylistdialog.h"
#include <QSqlTableModel>
#include <QLineEdit>
#include <QList>
#include <QItemSelectionModel>
#include <QDebug>
#include <QDataWidgetMapper>
#include <QSqlRelationalTableModel>
#include <QSqlRelationalDelegate>
#include <QSqlError>
#include <QColorDialog>
#include <QSqlRecord>
EditStylistDialog::EditStylistDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditStylistDialog)
{
    ui->setupUi(this);
    //Removes "What's it?" button
    this->setWindowFlags(Qt::Dialog | Qt::WindowTitleHint | Qt::WindowSystemMenuHint | Qt::WindowCloseButtonHint);
    this->ui->color_value_lineEdit->setVisible(false);
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
    this->stylist_model = new QSqlTableModel();
    stylist_model->setTable("stylist");
    //Define Headers
    stylist_model->setHeaderData(stylist_model->fieldIndex("id"), Qt::Horizontal, tr("#"));
    stylist_model->setHeaderData(stylist_model->fieldIndex("name"), Qt::Horizontal, tr("Name"));
    stylist_model->setHeaderData(stylist_model->fieldIndex("nickname"), Qt::Horizontal, tr("Nickname"));
    stylist_model->setHeaderData(stylist_model->fieldIndex("email"), Qt::Horizontal, tr("Email"));
    stylist_model->setHeaderData(stylist_model->fieldIndex("phone1"), Qt::Horizontal, tr("Phone 1"));
    stylist_model->setHeaderData(stylist_model->fieldIndex("phone2"), Qt::Horizontal, tr("Phone 2"));
    stylist_model->setHeaderData(stylist_model->fieldIndex("birthday"), Qt::Horizontal, tr("Birthday Date"));
    stylist_model->setHeaderData(stylist_model->fieldIndex("zip_code"), Qt::Horizontal, tr("Zip-Code"));
    stylist_model->setHeaderData(stylist_model->fieldIndex("address"), Qt::Horizontal, tr("Address"));
    stylist_model->setHeaderData(stylist_model->fieldIndex("state"), Qt::Horizontal, tr("State"));
    stylist_model->setHeaderData(stylist_model->fieldIndex("city"), Qt::Horizontal, tr("City"));

    //stylist_model->setHeaderData(stylist_model->fieldIndex("color"), Qt::Horizontal, tr("Color"));
    stylist_model->select();
    //this->ui->stylist_tableView->setItemDelegate(new QSqlRelationalDelegate(this->ui->stylist_tableView));
    //Sets Model on the Table View
    this->ui->stylist_tableView->setModel(stylist_model);
    //Hides color column
    this->ui->stylist_tableView->hideColumn(stylist_model->fieldIndex("color"));
    //Set Color in the color in the color column of stylist_tableView
    //QTableView *curTable = this->ui->stylist_tableView;
    for(;;){
    break;
    }
    //Disables Edit on Table
    this->ui->stylist_tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    //Hides some columns on Table View
    this->ui->stylist_tableView->hideColumn(stylist_model->fieldIndex("points"));
    //Resizes the column according to its content
    this->ui->stylist_tableView->resizeColumnsToContents();
    //Defines params for selection
    this->ui->stylist_tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    this->ui->stylist_tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    //Hides Vertical Header
    this->ui->stylist_tableView->verticalHeader()->setVisible(false);
    //Fills the search ComboBox
    QStringList list = QString("#,Name,Email,Phone,Birthday,Address,Zip-Code,State,City").split(",");
    this->ui->comboBox->addItems(list);
    //Connect Selection Changed Signal
    connect(this->ui->stylist_tableView->selectionModel(),SIGNAL(selectionChanged(QItemSelection,QItemSelection)),this,SLOT(selectionChanged(QItemSelection,QItemSelection)));
    connect(this->ui->search_lineEdit_2,SIGNAL(textEdited(QString)),this,SLOT(searchTextChanged(QString)));
    connect(this->ui->comboBox,SIGNAL(currentTextChanged(QString)),this,SLOT(combobox_text_changed(QString)));
    //Creates Data Mapper
    stylist_mapper = new QDataWidgetMapper;
    stylist_mapper->setModel(stylist_model);
    stylist_mapper->addMapping(this->ui->id_lineEdit, this->stylist_model->fieldIndex("id"));
    stylist_mapper->addMapping(this->ui->name_lineEdit_2, this->stylist_model->fieldIndex("name"));
    stylist_mapper->addMapping(this->ui->nickname_lineEdit_3, this->stylist_model->fieldIndex("nickname"));
    stylist_mapper->addMapping(this->ui->email_lineEdit_4, this->stylist_model->fieldIndex("email"));
    stylist_mapper->addMapping(this->ui->phone1_lineEdit_2, this->stylist_model->fieldIndex("phone1"));
    stylist_mapper->addMapping(this->ui->phone2_lineEdit_3, this->stylist_model->fieldIndex("phone2"));
    stylist_mapper->addMapping(this->ui->birthday_dateEdit, this->stylist_model->fieldIndex("birthday"));
    stylist_mapper->addMapping(this->ui->address_lineEdit_5, this->stylist_model->fieldIndex("address"));
    stylist_mapper->addMapping(this->ui->zip_code_lineEdit_6, this->stylist_model->fieldIndex("zip_code"));
    stylist_mapper->addMapping(this->ui->state_comboBox, this->stylist_model->fieldIndex("state"));
    stylist_mapper->addMapping(this->ui->city_lineEdit_7, this->stylist_model->fieldIndex("city"));
    stylist_mapper->addMapping(this->ui->color_value_lineEdit, this->stylist_model->fieldIndex("color"));
    stylist_mapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);
    //mapper->toFirst();
    connect(this->ui->stylist_tableView->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),
            stylist_mapper, SLOT(setCurrentModelIndex(QModelIndex)));
    //Update ComboBox Text
    this->combobox_text_changed(this->ui->comboBox->currentText());
    connect(this->ui->search_dateEdit,SIGNAL(dateChanged(QDate)),this,SLOT(searchDateChanged(QDate)));
}

void EditStylistDialog::combobox_text_changed(QString text){
    QString field;
    QString other="";
    this->ui->search_lineEdit_2->setVisible(true);
    this->ui->search_dateEdit->setVisible(false);
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

void EditStylistDialog::selectionChanged(const QItemSelection & selected, const QItemSelection & deselected){
    Q_UNUSED(deselected)
    if(selected.indexes().length()){
        this->ui->edit_pushButton->setEnabled(true);
        int column_count = this->stylist_model->columnCount();
        QString color_string = selected.indexes().at(column_count-1).data(0).toString();
        qDebug()<<selected;
        QString stylesheet_string = QString("background-color: %1;").arg(color_string);
        this->ui->color_view_label_13->setStyleSheet(stylesheet_string);
        this->ui->color_value_lineEdit->setText(color_string);
    }else{
        this->ui->edit_pushButton->setEnabled(false);
        this->ui->color_view_label_13->setStyleSheet("");
    }
}

void EditStylistDialog::searchTextChanged(QString text){
    if(!text.isEmpty()){
        this->stylist_model->setFilter(QString(filter).arg(text));
        //qDebug()<<this->stylist_model->filter();
    }else{
        this->stylist_model->setFilter("");
    }

}
void EditStylistDialog::searchDateChanged(QDate date){
    QString tmp_filter = QString(filter).arg(date.toString(global_config.date_format));
    qDebug() << tmp_filter;
    this->stylist_model->setFilter(tmp_filter);
    //int row = this->ui->stylist_tableView->selectionModel()->currentIndex().row();
    qDebug() << this->stylist_model->data(this->ui->stylist_tableView->selectionModel()->currentIndex()).toString();
}

EditStylistDialog::~EditStylistDialog()
{
    delete ui;
}

void EditStylistDialog::beginEditing(){
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
    this->ui->stylists_groupBox_3->setEnabled(false);
}
void EditStylistDialog::endEditing(){
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
    this->ui->stylists_groupBox_3->setEnabled(true);
    this->ui->search_lineEdit_2->setEnabled(true);
    //Mapper
    stylist_mapper->setCurrentIndex(stylist_mapper->currentIndex());
}

void EditStylistDialog::on_save_pushButton_3_clicked()
{   
    if(stylist_mapper->submit()){
        /*
        int cur_index = this->stylist_mapper->currentIndex();
        QSqlRecord record = this->stylist_model->record(cur_index);
        record.setValue("color",this->ui->color_view_label_13);
        */
        qDebug()<<"Record successfull edited";
        this->ui->stylist_tableView->resizeColumnsToContents();
        this->ui->stylist_tableView->resizeRowsToContents();
    }else{
        qDebug()<<"An error occorred while saving table "<<stylist_model->lastError().text();
    }
}

void EditStylistDialog::on_choose_color_Button_2_clicked()
{
    QColorDialog color_dialog(this);
    //color_dialog.setOption(QColorDialog::ShowAlphaChannel);
    if(color_dialog.exec()){
        QString color_string = color_dialog.selectedColor().name(QColor::HexRgb);
       QString stylesheet_string = QString("background-color: %1;").arg(color_string);
       this->ui->color_view_label_13->setStyleSheet(stylesheet_string);
       //Set Value to Color Value Field
       this->ui->color_value_lineEdit->setText(color_string);

    }
}
