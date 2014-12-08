#ifndef APPTBYSTYLISTREPORTDIALOG_H
#define APPTBYSTYLISTREPORTDIALOG_H

#include <QDialog>

namespace Ui {
class ApptbyStylistReportDialog;
}

class ApptbyStylistReportDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ApptbyStylistReportDialog(QWidget *parent = 0);
    ~ApptbyStylistReportDialog();

    Ui::ApptbyStylistReportDialog *ui;
};

#endif // APPTBYSTYLISTREPORTDIALOG_H
