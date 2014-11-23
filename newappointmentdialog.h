#ifndef NEWAPPOINTMENTDIALOG_H
#define NEWAPPOINTMENTDIALOG_H

#include <QDialog>
#include "mainwindow.h"
#include <QTime>
#include "globalconfig.h"
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
    GlobalConfig global_config;
private slots:
    void setRange(QTime beginTime);
    void updateEndTime();
    void updateDuration();
};

#endif // NEWAPPOINTMENTDIALOG_H
