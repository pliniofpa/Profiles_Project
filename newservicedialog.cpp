#include "newservicedialog.h"
#include "ui_newservicedialog.h"

NewServiceDialog::NewServiceDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewServiceDialog)
{
    ui->setupUi(this);
    //Removes "What's it?" button
    this->setWindowFlags(Qt::Dialog | Qt::WindowTitleHint | Qt::WindowSystemMenuHint | Qt::WindowCloseButtonHint);
}

NewServiceDialog::~NewServiceDialog()
{
    delete ui;
}
