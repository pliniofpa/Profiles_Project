#ifndef EDITCUSTUMERDIALOG_H
#define EDITCUSTUMERDIALOG_H

#include <QDialog>
class QSqlTableModel;

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

private:
    Ui::EditCustumerDialog *ui;
    QSqlTableModel *customer_model;
};

#endif // EDITCUSTUMERDIALOG_H
