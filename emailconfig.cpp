#include "emailconfig.h"
#include "ui_emailconfig.h"
#include <QFileDialog>
emailConfig::emailConfig(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::emailConfig)
{
    ui->setupUi(this);
    //Removes 'What's It?' buttom in this dialog window.
    this->setWindowFlags(Qt::Dialog | Qt::WindowTitleHint | Qt::WindowSystemMenuHint | Qt::WindowCloseButtonHint);
#ifdef Q_OS_ANDROID
    this->showMaximized();//Show Full Screen for Android Devices
#endif
}

emailConfig::~emailConfig()
{
    delete ui;
}

int emailConfig::getPort(){
    return this->ui->port->text().toInt();
}
void emailConfig::setPort(int port){
    this->ui->port->setText(QString::number(port));
}
QString emailConfig::getServerName(){
    return this->ui->serverName->text();
}
void emailConfig::setServerName(QString server){
    this->ui->serverName->setText(server);
}
QString emailConfig::getSubject(){
    return this->ui->subject->text();
}
void emailConfig::setSubject(QString subject){
    this->ui->subject->setText(subject);
}
QString emailConfig::getUserName(){
    return this->ui->userName->text();
}
void emailConfig::setUserName(QString username){
    this->ui->userName->setText(username);
}
QString emailConfig::getPassword(){
    return this->ui->paswd->text();
}
void emailConfig::setPassword(QString password){
    this->ui->paswd->setText(password);
}
