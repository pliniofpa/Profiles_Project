#include "companyconfigdialog.h"
#include "ui_companyconfigdialog.h"
#include <QFileDialog>
CompanyConfigDialog::CompanyConfigDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CompanyConfigDialog)
{
    ui->setupUi(this);
}

CompanyConfigDialog::~CompanyConfigDialog()
{
    delete ui;
}

void CompanyConfigDialog::on_select_logo_pushButton_clicked()
{
    this->logoFileName = QFileDialog::getOpenFileName(this,"Select Your Image File.",QString(),"Images (*.png *.xpm *.jpg)");
    this->ui->logo_label->setPixmap(QPixmap(this->logoFileName));
}
