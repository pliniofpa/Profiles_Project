#ifndef NEWSTYLISTDIALOG_H
#define NEWSTYLISTDIALOG_H

#include <QDialog>
#include "mainwindow.h"
namespace Ui {
class NewStylistDialog;
}

class NewStylistDialog : public QDialog
{
    friend class MainWindow;
    Q_OBJECT

public:
    explicit NewStylistDialog(QWidget *parent = 0);
    ~NewStylistDialog();

private slots:
    void on_choose_color_Button_clicked();
    void validateEmail(QString text);
    void validateName(QString text);
    void validateNickName();
    void validatePhone(QString text);
    void validatePhone2(QString text);
    void validateCity(QString text);
    void validateAddress(QString text);
    void validationFinished();

private:
    Ui::NewStylistDialog *ui;
    bool nameOK,emailOK,cityOK,phoneOK,phone2OK,nickNameOk,addressOK;
    //Nick Name Flag
    bool nickNameEdited;

};

#endif // NEWSTYLISTDIALOG_H
