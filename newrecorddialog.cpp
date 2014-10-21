#include "newrecorddialog.h"
#include "ui_newrecorddialog.h"

NewRecordDialog::NewRecordDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewRecordDialog)
{
    ui->setupUi(this);
}

NewRecordDialog::~NewRecordDialog()
{
    delete ui;
}
