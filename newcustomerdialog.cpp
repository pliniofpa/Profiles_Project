#include "newcustomerdialog.h"
#include "ui_newcustomerdialog.h"

NewCustomerDialog::NewCustomerDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewCustomerDialog)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::Dialog | Qt::WindowTitleHint | Qt::WindowSystemMenuHint | Qt::WindowCloseButtonHint);
}

NewCustomerDialog::~NewCustomerDialog()
{
    delete ui;
}
