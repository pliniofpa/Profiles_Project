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




private:
    Ui::EditAppointmentDialog *ui;
    GlobalConfig global_config;
    QDataWidgetMapper *appointment_mapper;
    QSqlTableModel *appt_assoc_names_model;
    //Helper variables for Filtering
    QString filter;
    QSqlTableModel *stylist_model,*service_model,*customer_model;
};

#endif // EDITAPPOINTMENTDIALOG_H
