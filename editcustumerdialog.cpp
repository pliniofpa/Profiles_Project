#include "editcustumerdialog.h"
#include "ui_editcustumerdialog.h"
#include <QSqlTableModel>
#include <QLineEdit>
#include <QList>
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
    //Disables Edit, Cancel and Save Buttons
    //this->ui->edit_pushButton->setEnabled(false);
    //this->ui->cancel_pushButton_2->setEnabled(false);
    //this->ui->save_pushButton_3->setEnabled(false);
    //Defines Model Headers
    this->customer_model = new QSqlTableModel();
    customer_model->setTable("customer");
    customer_model->select();
    customer_model->setHeaderData(customer_model->fieldIndex("id"), Qt::Horizontal, tr("#"));
    customer_model->setHeaderData(customer_model->fieldIndex("name"), Qt::Horizontal, tr("Name"));
    customer_model->setHeaderData(customer_model->fieldIndex("email"), Qt::Horizontal, tr("Email"));
    customer_model->setHeaderData(customer_model->fieldIndex("phone1"), Qt::Horizontal, tr("Phone 1"));
    customer_model->setHeaderData(customer_model->fieldIndex("phone2"), Qt::Horizontal, tr("Phone 2"));
    customer_model->setHeaderData(customer_model->fieldIndex("birthday"), Qt::Horizontal, tr("Birthday Date"));
    //Set Model on the Table View
    this->ui->customer_tableView->setModel(this->customer_model);
    //Hides some columns on Table View
    this->ui->customer_tableView->hideColumn(customer_model->fieldIndex("address_id"));
    this->ui->customer_tableView->hideColumn(customer_model->fieldIndex("points"));
    //Resizes the column according to its content
    this->ui->customer_tableView->resizeColumnsToContents();
    //Defines params for selection
    this->ui->customer_tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    this->ui->customer_tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    //Hides Vertical Header
    this->ui->customer_tableView->verticalHeader()->setVisible(false);
}

EditCustumerDialog::~EditCustumerDialog()
{
    delete ui;
}
void EditCustumerDialog::beginEditing(){
    this->ui->edit_pushButton->setEnabled(false);
    this->ui->save_pushButton_3->setEnabled(true);
    this->ui->cancel_pushButton_2->setEnabled(true);
    QList<QLineEdit *> lineEdits = this->findChildren<QLineEdit *>();
    for(int i=0;i<lineEdits.length();++i){
        lineEdits.at(i)->setEnabled(true);
    }
    this->ui->customers_groupBox_3->setEnabled(false);
}
void EditCustumerDialog::endEditing(){
    this->ui->edit_pushButton->setEnabled(true);
    this->ui->save_pushButton_3->setEnabled(false);
    this->ui->cancel_pushButton_2->setEnabled(false);
    QList<QLineEdit *> lineEdits = this->findChildren<QLineEdit *>();
    for(int i=0;i<lineEdits.length();++i){
        lineEdits.at(i)->setEnabled(false);
    }
    this->ui->customers_groupBox_3->setEnabled(true);
    this->ui->search_lineEdit_2->setEnabled(true);
}
