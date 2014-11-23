#ifndef EDITAPPOINTMENTDIALOG_H
#define EDITAPPOINTMENTDIALOG_H
#include <QDialog>
#include <QItemSelection>
#include <QDate>
#include <QModelIndex>
#include "globalconfig.h"
class QSqlTableModel;
class QDataWidgetMapper;
#include <QDialog>
namespace Ui {
class EditAppointmentDialog;
}

class EditAppointmentDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EditAppointmentDialog(QWidget *parent = 0);
    ~EditAppointmentDialog();
private slots:
    void beginEditing();
    void endEditing();
    void selectionChanged(const QItemSelection &selected, const QItemSelection &deselected);
    void searchTextChanged(QString text);
    void on_save_pushButton_3_clicked();
    void combobox_text_changed(QString text);
    void searchDateChanged(QDate date);

    void setRange(QTime beginTime);
    void updateEndTime();
    void updateDuration();
private:
    Ui::EditAppointmentDialog *ui;
    GlobalConfig global_config;
    QDataWidgetMapper *appointment_mapper;
    QSqlTableModel *appt_assoc_names_model;
    //Helper variables for Filtering
    QString filter;
    QSqlTableModel *stylist_model,*service_model,*customer_model;
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

#endif // EDITAPPOINTMENTDIALOG_H
