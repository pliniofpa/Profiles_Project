#include "aboutdialog.h"
#include "ui_aboutdialog.h"
/**
 * @file aboutdialog.cpp
 * This Class defines application About window.
 * @brief Source file for AboutDialog class definition.
 *
 * It's a simple QDialog derived class that defines application About window.
 *
 * @author Plinio Andrade &lt;PAndrade@fele.com&gt;
 * @version 1.0.0.0 (Qt: 5.3.1)
 */

AboutDialog::AboutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutDialog)
{
    ui->setupUi(this);
    //Removes 'What's It?' buttom in this dialog window.
    this->setWindowFlags(Qt::Dialog | Qt::WindowTitleHint | Qt::WindowSystemMenuHint | Qt::WindowCloseButtonHint);

}

AboutDialog::~AboutDialog()
{
    delete ui;
}
void AboutDialog::setVersionText(QString text){
    this->ui->version->setText(text);
}
