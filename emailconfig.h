#ifndef EMAILCONFIG_H
#define EMAILCONFIG_H

#include <QDialog>

namespace Ui {
class emailConfig;
}

class emailConfig : public QDialog
{
    Q_OBJECT

public:
    explicit emailConfig(QWidget *parent = 0);
    ~emailConfig();
    QString getServerName();
    void setServerName(QString server);
    QString getMessage();
    void setMessage(QString message);
    int getPort();
    void setPort(int port);
    QString getRcpt();
    void setRcpt(QString rcpt);
    QString getSubject();
    void setSubject(QString subject);
    QString getUserName();
    void setUserName(QString username);
    QString getPassword();
    void setPassword(QString password);
    QString getFileDirectory();
    void setFileDirectory(QString directory);

private:
    Ui::emailConfig *ui;
};

#endif // EMAILCONFIG_H
