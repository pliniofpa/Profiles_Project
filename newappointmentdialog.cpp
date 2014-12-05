#include "newappointmentdialog.h"
#include "ui_newappointmentdialog.h"

NewAppointmentDialog::NewAppointmentDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewAppointmentDialog)
{
    ui->setupUi(this);
    //Removes "What's it?" button
    this->setWindowFlags(Qt::Dialog | Qt::WindowTitleHint | Qt::WindowSystemMenuHint | Qt::WindowCloseButtonHint);
}

NewAppointmentDialog::~NewAppointmentDialog()
{
    delete ui;
}
