#ifndef APPTCONFLITINGDIALOG_H
#define APPTCONFLITINGDIALOG_H

#include <QDialog>

namespace Ui {
class ApptConflitingDialog;
}

class ApptConflitingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ApptConflitingDialog(QWidget *parent = 0);
    ~ApptConflitingDialog();

private:
    Ui::ApptConflitingDialog *ui;
};

#endif // APPTCONFLITINGDIALOG_H
