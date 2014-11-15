#ifndef EDITSTYLISTDIALOG_H
#define EDITSTYLISTDIALOG_H

#include <QDialog>
#include <QItemSelection>
#include <QDate>
#include "globalconfig.h"
class QSqlTableModel;
class QDataWidgetMapper;
namespace Ui {
class EditStylistDialog;
}

class EditStylistDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EditStylistDialog(QWidget *parent = 0);
    ~EditStylistDialog();
private slots:
    void beginEditing();
    void endEditing();
    void selectionChanged(const QItemSelection &selected, const QItemSelection &deselected);
    void searchTextChanged(QString text);
    void on_save_pushButton_3_clicked();
    void combobox_text_changed(QString text);
    void searchDateChanged(QDate date);

    void on_choose_color_Button_2_clicked();

private:
    GlobalConfig global_config;
    QDataWidgetMapper *stylist_mapper;
    Ui::EditStylistDialog *ui;
    QSqlTableModel *stylist_model;
    //Helper variables for Filtering
    QString filter;
};

#endif // EDITSTYLISTDIALOG_H
