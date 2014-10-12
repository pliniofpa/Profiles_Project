#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    int b;
    int c;
    QString vc;
    int p;
}

MainWindow::~MainWindow()
{
    delete ui;
}
