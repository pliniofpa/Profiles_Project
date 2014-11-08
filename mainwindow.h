#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QStringList>
#include "globalconfig.h"
class QTime;
class QSqlTableModel;
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
private slots:
    void showCreateCustomerDialog();
    void showCreateStylistDialog();
    void showCreateServiceDialog();
    void showCreateAppointmentDialog();
    void showCreateUserDialog();
    void showEditCustomerDialog();
    void showEditStylistDialog();

private:
    GlobalConfig global_config;
    Ui::MainWindow *ui;
    QTime *first_time,*last_time;
    int appt_inverval;
    void create_daily_appt();
    void create_employee_appt();    
    void make_connections();
};

#endif // MAINWINDOW_H
