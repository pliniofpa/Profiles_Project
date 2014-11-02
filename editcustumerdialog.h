#ifndef EDITCUSTUMERDIALOG_H
#define EDITCUSTUMERDIALOG_H

#include <QDialog>
#include <QItemSelection>
#include <QDate>
#include "globalconfig.h"
class QSqlTableModel;
class QDataWidgetMapper;
namespace Ui {
class EditCustumerDialog;
}

class EditCustumerDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EditCustumerDialog(QWidget *parent = 0);
    ~EditCustumerDialog();
private slots:
    void beginEditing();
    void endEditing();
    void selectionChanged(const QItemSelection &selected, const QItemSelection &deselected);
    void searchTextChanged(QString text);
    void on_save_pushButton_3_clicked();
    void combobox_text_changed(QString text);
    void searchDateChanged(QDate date);

private:
    GlobalConfig global_config;
    QDataWidgetMapper *mapper;
    Ui::EditCustumerDialog *ui;
    QSqlTableModel *customer_model;
    //Helper variables for Filtering
    QString filter;
};

#endif // EDITCUSTUMERDIALOG_H
