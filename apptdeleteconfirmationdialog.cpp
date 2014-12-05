#include "apptdeleteconfirmationdialog.h"
#include "ui_apptdeleteconfirmationdialog.h"

ApptDeleteConfirmationDialog::ApptDeleteConfirmationDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ApptDeleteConfirmationDialog)
{
    ui->setupUi(this);
}

ApptDeleteConfirmationDialog::~ApptDeleteConfirmationDialog()
{
    delete ui;
}
