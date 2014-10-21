#ifndef NEWUSERDIALOG_H
#define NEWUSERDIALOG_H

#include <QDialog>
#include "mainwindow.h"

namespace Ui {
class NewUserDialog;
}

class NewUserDialog : public QDialog
{
    friend class MainWindow;
    Q_OBJECT

public:
    explicit NewUserDialog(QWidget *parent = 0);
    ~NewUserDialog();

private:
    Ui::NewUserDialog *ui;
};

#endif // NEWUSERDIALOG_H
