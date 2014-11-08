#include "newcustomerdialog.h"
#include "ui_newcustomerdialog.h"
#include <QRegExp>
#include <QRegExpValidator>
#include <QDebug>
#include <QDate>
NewCustomerDialog::NewCustomerDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewCustomerDialog)
{
    ui->setupUi(this);
    //Sets Birthday date to current Date
    this->ui->birthday_dateEdit->setDate(QDate::currentDate());
    //Removes "What's it?" button
    this->setWindowFlags(Qt::Dialog | Qt::WindowTitleHint | Qt::WindowSystemMenuHint | Qt::WindowCloseButtonHint);
    //Sets Email Validator Connection
    QObject::connect(this->ui->email_lineEdit_4,SIGNAL(textEdited(QString)),this,SLOT(validateEmail(QString)));
}

NewCustomerDialog::~NewCustomerDialog()
{
    delete ui;
}
void NewCustomerDialog::validateEmail(QString text){
    QString strPatt = "\\b[A-Z0-9._%+-]+@[A-Z0-9.-]+\\.[A-Z]{2,4}\\b";
    QRegExp rx(strPatt);
    qDebug()<<rx.matchedLength();
    if(rx.exactMatch(text)){
        this->ui->email_lineEdit_4->setStyleSheet("color: green");
    }else{
        this->ui->email_lineEdit_4->setStyleSheet("color: red");
    }
}
