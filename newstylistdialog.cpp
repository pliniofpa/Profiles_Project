#include "newstylistdialog.h"
#include "ui_newstylistdialog.h"
#include <QDate>
NewStylistDialog::NewStylistDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewStylistDialog)
{
    ui->setupUi(this);
    //Sets Birthday date to current Date
    this->ui->birthday_dateEdit->setDate(QDate::currentDate());
    //Removes "What's it?" button
    this->setWindowFlags(Qt::Dialog | Qt::WindowTitleHint | Qt::WindowSystemMenuHint | Qt::WindowCloseButtonHint);
}

NewStylistDialog::~NewStylistDialog()
{
    delete ui;
}
