#include "newstylistdialog.h"
#include "ui_newstylistdialog.h"
#include <QDate>
#include <QColorDialog>
NewStylistDialog::NewStylistDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewStylistDialog)
{
    ui->setupUi(this);
    //Sets Birthday date to current Date
    this->ui->birthday_dateEdit->setDate(QDate::currentDate());
    //Removes "What's it?" button
    this->setWindowFlags(Qt::Dialog | Qt::WindowTitleHint | Qt::WindowSystemMenuHint | Qt::WindowCloseButtonHint);
}

NewStylistDialog::~NewStylistDialog()
{
    delete ui;
}

void NewStylistDialog::on_choose_color_Button_clicked()
{
    QColorDialog color_dialog(this);
    //color_dialog.setOption(QColorDialog::ShowAlphaChannel);
    if(color_dialog.exec()){
       QString stylesheet_string = QString("background-color: %1;").arg(color_dialog.selectedColor().name(QColor::HexRgb));
       this->ui->color_view_label_12->setStyleSheet(stylesheet_string);
    }
}
