#ifndef EDITUSERDIALOG_H
#define EDITUSERDIALOG_H
#include <QDialog>
#include <QItemSelection>
#include <QDate>
#include <QModelIndex>
#include "globalconfig.h"
class QSqlTableModel;
class QDataWidgetMapper;
namespace Ui{
    class EditUserDialog;
}

class EditUserDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit EditUserDialog(QWidget *parent = 0);
    ~EditUserDialog();
    private slots:
    void beginEditing();
    void endEditing();
    void selectionChanged(const QItemSelection &selected, const QItemSelection &deselected);
    void searchTextChanged(QString text);
    void on_save_pushButton_3_clicked();
    void combobox_text_changed(QString text);
    void searchDateChanged(QDate date);
    
private:
    Ui::EditUserDialog *ui;
    GlobalConfig global_config;
    QDataWidgetMapper *user_mapper;
    QSqlTableModel *user_model;
    //Helper variables for Filtering
    QString filter;
};

#endif // EDITUSERDIALOG_H
