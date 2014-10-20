#include "newstylistdialog.h"
#include "ui_newstylistdialog.h"

NewStylistDialog::NewStylistDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewStylistDialog)
{
    ui->setupUi(this);
    //Removes "What's it button"
    this->setWindowFlags(Qt::Dialog | Qt::WindowTitleHint | Qt::WindowSystemMenuHint | Qt::WindowCloseButtonHint);
}

NewStylistDialog::~NewStylistDialog()
{
    delete ui;
}
