#ifndef NEWSERVICEDIALOG_H
#define NEWSERVICEDIALOG_H

#include <QDialog>
#include "mainwindow.h"

namespace Ui {
class NewServiceDialog;
}

class NewServiceDialog : public QDialog
{
    friend class MainWindow;
    Q_OBJECT

public:
    explicit NewServiceDialog(QWidget *parent = 0);
    ~NewServiceDialog();

private:
    Ui::NewServiceDialog *ui;
};

#endif // NEWSERVICEDIALOG_H
