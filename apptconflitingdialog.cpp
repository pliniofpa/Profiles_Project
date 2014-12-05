#include "apptconflitingdialog.h"
#include "ui_apptconflitingdialog.h"

ApptConflitingDialog::ApptConflitingDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ApptConflitingDialog)
{
    ui->setupUi(this);
    //Removes "What's it?" button
    this->setWindowFlags(Qt::Dialog | Qt::WindowTitleHint | Qt::WindowSystemMenuHint | Qt::WindowCloseButtonHint);

}

ApptConflitingDialog::~ApptConflitingDialog()
{
    delete ui;
}
