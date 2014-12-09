#ifndef NEWCUSTOMERDIALOG_H
#define NEWCUSTOMERDIALOG_H

#include <QDialog>
#include "mainwindow.h"

namespace Ui {
class NewCustomerDialog;
}

class NewCustomerDialog : public QDialog
{
    friend class MainWindow;
    Q_OBJECT

public:
    explicit NewCustomerDialog(QWidget *parent = 0);
    ~NewCustomerDialog();

private:
    Ui::NewCustomerDialog *ui;
    bool nameOK,emailOK,cityOK,phoneOK,phone2OK,nickNameOk,addressOK;

private slots:
    void validateEmail(QString text);
    void validateName(QString text);
    void validatePhone(QString text);
    void validatePhone2(QString text);
    void validateCity(QString text);
    void validateAddress(QString text);
    void validationFinished();
};

#endif // NEWCUSTOMERDIALOG_H
