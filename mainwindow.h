#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QStringList>
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

private:
    Ui::MainWindow *ui;
    QStringList usa_states;
    QTime *first_time,*last_time;
    int appt_inverval;
    void create_daily_appt();
    void create_employee_appt();
    QString time_format;
    void make_connections();
};

#endif // MAINWINDOW_H
