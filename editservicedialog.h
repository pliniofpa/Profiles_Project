#ifndef EDITSERVICEDIALOG_H
#define EDITSERVICEDIALOG_H

#include <QDialog>
#include <QItemSelection>
#include <QDate>
#include "globalconfig.h"
class QSqlTableModel;
class QDataWidgetMapper;
namespace Ui {
    class EditServiceDialog;
}

class EditServiceDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit EditServiceDialog(QWidget *parent = 0);
    ~EditServiceDialog();
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
    QDataWidgetMapper *service_mapper;
    Ui::EditServiceDialog *ui;
    QSqlTableModel *service_model;
    //Helper variables for Filtering
    QString filter;
};

#endif // EDITSERVICEDIALOG_H
