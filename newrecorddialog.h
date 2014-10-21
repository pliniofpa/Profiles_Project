#ifndef NEWRECORDDIALOG_H
#define NEWRECORDDIALOG_H

#include <QDialog>

namespace Ui {
class NewRecordDialog;
}

class NewRecordDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NewRecordDialog(QWidget *parent = 0);
    ~NewRecordDialog();

private:
    Ui::NewRecordDialog *ui;
};

#endif // NEWRECORDDIALOG_H
