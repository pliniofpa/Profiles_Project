#ifndef NEWAPPOINTMENTDIALOG_H
#define NEWAPPOINTMENTDIALOG_H

#include <QDialog>
#include "mainwindow.h"

namespace Ui {
class NewAppointmentDialog;
}

class NewAppointmentDialog : public QDialog
{
    friend class MainWindow;
    Q_OBJECT

public:
    explicit NewAppointmentDialog(QWidget *parent = 0);
    ~NewAppointmentDialog();

private:
    Ui::NewAppointmentDialog *ui;
};

#endif // NEWAPPOINTMENTDIALOG_H
