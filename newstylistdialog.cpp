#include "newstylistdialog.h"
#include "ui_newstylistdialog.h"
#include <QDate>
#include <QColorDialog>
#include <QDebug>
NewStylistDialog::NewStylistDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewStylistDialog)
{
    ui->setupUi(this);
    //Sets Birthday date to current Date
    this->ui->birthday_dateEdit->setDate(QDate::currentDate());
    //Removes "What's it?" button
    this->setWindowFlags(Qt::Dialog | Qt::WindowTitleHint | Qt::WindowSystemMenuHint | Qt::WindowCloseButtonHint);
    //Sets Email Validator Connection
    QObject::connect(this->ui->email_lineEdit_3,SIGNAL(textEdited(QString)),this,SLOT(validateEmail(QString)));
    //Sets Name Validator Connection
    QObject::connect(this->ui->name_lineEdit,SIGNAL(textEdited(QString)),this,SLOT(validateName(QString)));
    //Sets Nick Name Validator
    QObject::connect(this->ui->nickname_lineEdit_2,SIGNAL(editingFinished()),this,SLOT(validateNickName()));
    //Sets Phone Validator
    QObject::connect(this->ui->phone1_lineEdit_4,SIGNAL(textEdited(QString)),this,SLOT(validatePhone(QString)));
    QObject::connect(this->ui->phone2_lineEdit_5,SIGNAL(textEdited(QString)),this,SLOT(validatePhone2(QString)));
    //Sets City Validator
    QObject::connect(this->ui->city_lineEdit_7,SIGNAL(textChanged(QString)),this,SLOT(validateCity(QString)));
    //Sets Address Validator
    QObject::connect(this->ui->address_lineEdit_5,SIGNAL(textEdited(QString)),this,SLOT(validateAddress(QString)));

    //Disables Ok flags for default
    nameOK=false;
    emailOK=false;
    cityOK=false;
    addressOK = true;
    phoneOK=false;
    phone2OK=true;
    nickNameOk=false;
    nickNameEdited=false;
    validationFinished();
}

NewStylistDialog::~NewStylistDialog()
{
    delete ui;
}

void NewStylistDialog::on_choose_color_Button_clicked()
{
    QColorDialog color_dialog(this);
    //color_dialog.setOption(QColorDialog::ShowAlphaChannel);
    if(color_dialog.exec()){
        QString stylesheet_string = QString("background-color: %1;").arg(color_dialog.selectedColor().name(QColor::HexRgb));
        this->ui->color_view_label_12->setStyleSheet(stylesheet_string);
    }
}

void NewStylistDialog::validateEmail(QString text){
    QRegExp mailREX("\\b[A-Z0-9._%+-]+@[A-Z0-9.-]+\\.[A-Z]{2,4}\\b");
    mailREX.setCaseSensitivity(Qt::CaseInsensitive);
    mailREX.setPatternSyntax(QRegExp::RegExp);
    QRegExpValidator *validator = new QRegExpValidator(mailREX,0);
    int pos=0;
    if(validator->validate(text,pos)==QValidator::Invalid){
        this->ui->email_lineEdit_3->setStyleSheet("color: red");
        emailOK = false;
    }
    if(validator->validate(text,pos)==QValidator::Intermediate){
        this->ui->email_lineEdit_3->setStyleSheet("color: orange");
        emailOK = false;
    }
    if(validator->validate(text,pos)==QValidator::Acceptable){
        this->ui->email_lineEdit_3->setStyleSheet("color: green");
        emailOK = true;
    }
    validationFinished();
}
void NewStylistDialog::validateName(QString text){
    if(text.length()<5 || text.length()>100){
        this->ui->name_lineEdit->setStyleSheet("color: red");
        nameOK=false;
    }else{
        this->ui->name_lineEdit->setStyleSheet("color: green");
        nameOK=true;
    }
    validationFinished();
    QString firstName = text.split(" ").at(0);
    if(!nickNameEdited){
        this->ui->nickname_lineEdit_2->setText(firstName);
    }
}

void NewStylistDialog::validateNickName(){
    nickNameEdited = true;
    QString text = this->ui->nickname_lineEdit_2->text();
    if(text.length()<5 || text.length()>100){
        this->ui->nickname_lineEdit_2->setStyleSheet("color: red");
        nickNameOk=false;
    }else{
        this->ui->nickname_lineEdit_2->setStyleSheet("color: green");
        nickNameOk=true;
    }
    validationFinished();
}
void NewStylistDialog::validatePhone(QString text){
    //qDebug()<<text;
    if(text.length()<12){
        this->ui->phone1_lineEdit_4->setStyleSheet("color: red");
        phoneOK=false;
    }else{
        this->ui->phone1_lineEdit_4->setStyleSheet("color: green");
        phoneOK=true;
    }
    validationFinished();
}

void NewStylistDialog::validatePhone2(QString text){
    //qDebug()<<text;
    if(text.length()==12 || text.length()==2){
        this->ui->phone2_lineEdit_5->setStyleSheet("color: green");
        phone2OK=false;
    }else{
        this->ui->phone2_lineEdit_5->setStyleSheet("color: color");
        phone2OK=true;
    }
    validationFinished();
}

void NewStylistDialog::validateCity(QString text){
    if(text.length()<5 || text.length()>50){
        this->ui->city_lineEdit_7->setStyleSheet("color: red");
        cityOK=false;
    }else{
        this->ui->city_lineEdit_7->setStyleSheet("color: green");
        cityOK=true;
    }
    validationFinished();
}

void NewStylistDialog::validateAddress(QString text){
    if(text.length()<15 || text.length()>150){
        this->ui->address_lineEdit_5->setStyleSheet("color: red");
        addressOK=false;
    }else{
        this->ui->address_lineEdit_5->setStyleSheet("color: green");
        addressOK=true;
    }
    validationFinished();
}

void NewStylistDialog::validationFinished(){

    if(emailOK && nameOK && nickNameOk && phoneOK && cityOK && phone2OK && addressOK){
        this->ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
    }
    else{
        this->ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
    }
}
