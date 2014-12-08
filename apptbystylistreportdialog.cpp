#include "apptbystylistreportdialog.h"
#include "ui_apptbystylistreportdialog.h"

ApptbyStylistReportDialog::ApptbyStylistReportDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ApptbyStylistReportDialog)
{
    ui->setupUi(this);
}

ApptbyStylistReportDialog::~ApptbyStylistReportDialog()
{
    delete ui;
}
