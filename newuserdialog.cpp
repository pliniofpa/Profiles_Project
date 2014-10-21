#include "newuserdialog.h"
#include "ui_newuserdialog.h"

NewUserDialog::NewUserDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewUserDialog)
{
    ui->setupUi(this);
    //Removes "What's it?" button
    this->setWindowFlags(Qt::Dialog | Qt::WindowTitleHint | Qt::WindowSystemMenuHint | Qt::WindowCloseButtonHint);
}

NewUserDialog::~NewUserDialog()
{
    delete ui;
}
