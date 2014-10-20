#ifndef NEWSTYLISTDIALOG_H
#define NEWSTYLISTDIALOG_H

#include <QDialog>
#include "mainwindow.h"
namespace Ui {
class NewStylistDialog;
}

class NewStylistDialog : public QDialog
{
    friend class MainWindow;
    Q_OBJECT

public:
    explicit NewStylistDialog(QWidget *parent = 0);
    ~NewStylistDialog();

private:
    Ui::NewStylistDialog *ui;
};

#endif // NEWSTYLISTDIALOG_H
