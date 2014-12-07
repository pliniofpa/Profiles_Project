#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QStringList>
#include "globalconfig.h"
#include <QItemSelection>
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

public slots:
    void showCreateCustomerDialog();
    void showCreateStylistDialog();
    void showCreateServiceDialog();
    void showCreateUserDialog();
    void showEditCustomerDialog();
    void showEditStylistDialog();
    void showEditServiceDialog();
    void showEditUserDialog();
    void showEditAppointmentDialog();
    void create_daily_appt();    
    void create_employee_appt();
    //Exporter la liste des utilisateurs
    void genPdfUser();
    void tableSelectionChanged(const QItemSelection & selected, const QItemSelection & deselected);
    void deleteSelectedAppt();
    void editSelectedAppt();
    void addAppt();
    void tabChanged(int current);
    void SendEmail();
    void showEmailConfigDialog();
    void mailSent(QString status);
    void showCompanyConfigDialog();
    void updateCompanyInformation();


private:
    GlobalConfig global_config;
    Ui::MainWindow *ui;    
    QSqlTableModel *stylist_model;
    void make_connections();
    int getApptDurationInMinutes(QTime begin_time, QTime end_time);
    //About Email
    QString serverName,username,password,subject,message,rcpt,fileDir,fileName;
    int port;
    //About Company
    QString companyName,companyPhone,companySlogan,companyAddress,companyState,companyCity,companyEmail,companyLogoFile;

    //Save/Load User Preferences members
    /**
     * @brief Saves user preferences.
     *
     * Saves user preferences such as window position, window size, user modified fields. This function is called always\n
     * that this windows is closed using the Ok button. For more information about how it works see the QSettings documentation.
     */
    void saveUserPreferences();
    /**
     * @brief Loads user preferences.
     *
     * Loads user preferences such as window position, window size, user modified fields. This function is called always\n
     * that this windows is closed using the Ok button. For more information about how it works see the QSettings documentation.
     */
    void loadUerPreferences();
    /**
     * @brief Window close event.
     *
     * This Event is called always that this windows is closed. This function is used to perform saving of user preferences.
     * @param event QCloseEvent object.
     */
    void closeEvent(QCloseEvent *event);

};

#endif // MAINWINDOW_H
