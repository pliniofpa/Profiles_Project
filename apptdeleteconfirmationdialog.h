#ifndef APPTDELETECONFIRMATIONDIALOG_H
#define APPTDELETECONFIRMATIONDIALOG_H

#include <QDialog>

namespace Ui {
class ApptDeleteConfirmationDialog;
}

class ApptDeleteConfirmationDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ApptDeleteConfirmationDialog(QWidget *parent = 0);
    ~ApptDeleteConfirmationDialog();

public:
    Ui::ApptDeleteConfirmationDialog *ui;
};

#endif // APPTDELETECONFIRMATIONDIALOG_H
