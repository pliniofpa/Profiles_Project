#include "editcustumerdialog.h"
#include "ui_editcustumerdialog.h"
#include <QSqlTableModel>
EditCustumerDialog::EditCustumerDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditCustumerDialog)
{
    ui->setupUi(this);


    //Sets Model for Combobox
    this->customer_model = new QSqlTableModel();
    customer_model->setTable("customer");
    customer_model->select();
    customer_model->setHeaderData(customer_model->fieldIndex("name"), Qt::Horizontal, tr("Name"));
    customer_model->setHeaderData(customer_model->fieldIndex("email"), Qt::Horizontal, tr("Email"));
    customer_model->setHeaderData(customer_model->fieldIndex("phone1"), Qt::Horizontal, tr("Phone 1"));
    customer_model->setHeaderData(customer_model->fieldIndex("phone2"), Qt::Horizontal, tr("Phone 2"));
    customer_model->setHeaderData(customer_model->fieldIndex("birthday"), Qt::Horizontal, tr("Birthday Date"));
    this->ui->customer_tableView->setModel(this->customer_model);
    this->ui->customer_tableView->hideColumn(customer_model->fieldIndex("id"));
    this->ui->customer_tableView->hideColumn(customer_model->fieldIndex("address_id"));
    this->ui->customer_tableView->hideColumn(customer_model->fieldIndex("points"));
    this->ui->customer_tableView->resizeColumnsToContents();
}

EditCustumerDialog::~EditCustumerDialog()
{
    delete ui;
}
