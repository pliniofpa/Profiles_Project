#ifndef COMPANYCONFIGDIALOG_H
#define COMPANYCONFIGDIALOG_H

#include <QDialog>

namespace Ui {
class CompanyConfigDialog;
}

class CompanyConfigDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CompanyConfigDialog(QWidget *parent = 0);
    ~CompanyConfigDialog();

    Ui::CompanyConfigDialog *ui;
    QString logoFileName;
private slots:
    void on_select_logo_pushButton_clicked();
};

#endif // COMPANYCONFIGDIALOG_H
