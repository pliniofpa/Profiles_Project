#include "apptconflitingdialog.h"
#include "ui_apptconflitingdialog.h"

ApptConflitingDialog::ApptConflitingDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ApptConflitingDialog)
{
    ui->setupUi(this);
}

ApptConflitingDialog::~ApptConflitingDialog()
{
    delete ui;
}
