#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "version.h"
#include <QTime>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QStringList>
#include <QDebug>
#include "mydatamodel.h"
#include "newcustomerdialog.h"
#include "ui_newcustomerdialog.h"
#include "newstylistdialog.h"
#include "ui_newstylistdialog.h"
#include "newservicedialog.h"
#include "ui_newservicedialog.h"
#include "newappointmentdialog.h"
#include "ui_newappointmentdialog.h"
#include "newuserdialog.h"
#include "ui_newuserdialog.h"
#include "editcustumerdialog.h"
#include "editstylistdialog.h"
#include "editappointmentdialog.h"
#include "editservicedialog.h"
#include "edituserdialog.h"
#include <QRegExp>
#include <QRegExpValidator>
#include <QSqlTableModel>
#include <QSqlRecord>
#include <QSettings>
#include "mycell.h"
#include "myqtablewidget.h"
#include <QLabel>
#include <QPrintDialog>
#include <QPrinter>
#include <QPainter>
#include "apptdeleteconfirmationdialog.h"
#include "ui_apptdeleteconfirmationdialog.h"
#include "emailconfig.h"
#include <QSqlError>
#include <KDReports/KDReports>
#include <QMessageBox>
#include "smtp/smtp.h"
#include "companyconfigdialog.h"
#include "ui_companyconfigdialog.h"
#include "aboutdialog.h"
#include "apptbystylistreportdialog.h"
#include "ui_apptbystylistreportdialog.h"
struct GlobalConfig;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->make_connections();
    //Set the date int the daily appointments date to current date
    this->ui->daily_appt_date_dateEdit->setDate(QDate::currentDate());
    this->ui->employee_day_date_dateEdit->setDate(QDate::currentDate());
    //Set the interval of data from current date to 6 months from current date
    this->ui->daily_appt_date_dateEdit->setDateRange(QDate::currentDate().addYears(-5),QDate::currentDate().addMonths(6));
    this->ui->employee_day_date_dateEdit->setDateRange(QDate::currentDate().addYears(-5),QDate::currentDate().addMonths(6));

    loadUerPreferences();
    this->create_employee_appt();
    this->create_daily_appt();
    this->ui->daily_appt_tableWidget->setMainWindowPointer(this);
    this->ui->employee_day_tableWidget->setMainWindowPointer(this);
    //this->ui->mainToolBar->setEnabled(false);
    this->ui->actionAdd->setEnabled(false);
    this->ui->actionAdd_Break->setEnabled(false);
    this->ui->actionDelete_Record->setEnabled(false);
    this->ui->actionEdit_Record->setEnabled(false);
    //Sets Model for Combobox
    this->stylist_model = new QSqlTableModel();
    stylist_model->setTable("stylist");
    stylist_model->select();
    this->ui->stylist_comboBox->setModel(stylist_model);
    this->ui->stylist_comboBox->setModelColumn(stylist_model->fieldIndex("name"));
    updateCompanyInformation();
}
void MainWindow::showCompanyConfigDialog(){
    CompanyConfigDialog dialog;
    //Set USA states list to State Combobox and City
    dialog.ui->state_comboBox->addItems(global_config.usa_states);
    dialog.ui->address_lineEdit_5->setText(this->companyAddress);
    dialog.ui->city_lineEdit_7->setText(this->companyCity);
    dialog.ui->email_lineEdit_7->setText(this->companyEmail);
    if(!this->companyLogoFile.isEmpty()){
        dialog.ui->logo_label->setPixmap(QPixmap(this->companyLogoFile));
    }
    dialog.ui->phone_lineEdit_6->setText(this->companyPhone);
    dialog.ui->slogan_lineEdit_7->setText(this->companySlogan);
    dialog.ui->name_lineEdit_6->setText(this->companyName);
    dialog.ui->state_comboBox->setCurrentText(this->companyState);
    if(dialog.exec()){
        this->companyAddress = dialog.ui->address_lineEdit_5->text();
        this->companyCity = dialog.ui->city_lineEdit_7->text();
        this->companyEmail = dialog.ui->email_lineEdit_7->text();
        this->companyLogoFile = dialog.logoFileName;
        this->companyName = dialog.ui->name_lineEdit_6->text();
        this->companyPhone = dialog.ui->phone_lineEdit_6->text();
        this->companySlogan = dialog.ui->slogan_lineEdit_7->text();
        this->companyState = dialog.ui->state_comboBox->currentText();
        updateCompanyInformation();
    }
}
void MainWindow::updateCompanyInformation(){
    this->ui->company_address_label_5->setText(this->companyAddress);
    this->ui->company_name_label_5->setText(this->companyName);
    this->ui->company_phone_label_7->setText(this->companyPhone);
    this->ui->company_slogan_label_7->setText(this->companySlogan);
    this->ui->company_email_label_4->setText(this->companyEmail);
    this->ui->company_state_city_label_4->setText(this->companyState+" - "+this->companyCity);
    if(!this->companyLogoFile.isEmpty()){
        this->ui->company_logo_label_4->setPixmap(QPixmap(this->companyLogoFile));

    }
}

void MainWindow::generateApptbyStylistReport(QString date, QString stylist_name, QString pdfFileName=QString()){
    QSqlTableModel *appt_assoc_names_model = new QSqlTableModel;
    appt_assoc_names_model->setTable("appt_by_stylist_report_view");
    QString curDate = date;
    QString curStylistName = stylist_name;
    //qDebug()<<curDate;
    appt_assoc_names_model->setFilter(QString("date='%1' AND stylist_name='%2'").arg(curDate).arg(curStylistName));
    appt_assoc_names_model->setSort(appt_assoc_names_model->fieldIndex("time_begin"),Qt::AscendingOrder);
    //Define Headers
    //appt_assoc_names_model->setHeaderData(appt_assoc_names_model->fieldIndex("id"), Qt::Horizontal, tr("#"));
    appt_assoc_names_model->setHeaderData(appt_assoc_names_model->fieldIndex("time_begin"), Qt::Horizontal, tr("Start Time"));
    appt_assoc_names_model->setHeaderData(appt_assoc_names_model->fieldIndex("details"), Qt::Horizontal, tr("Details"));
    appt_assoc_names_model->setHeaderData(appt_assoc_names_model->fieldIndex("date"), Qt::Horizontal, tr("Date"));
    appt_assoc_names_model->setHeaderData(appt_assoc_names_model->fieldIndex("time_end"), Qt::Horizontal, tr("End Time"));
    appt_assoc_names_model->setHeaderData(appt_assoc_names_model->fieldIndex("customer_name"), Qt::Horizontal, tr("Customer"));
    appt_assoc_names_model->setHeaderData(appt_assoc_names_model->fieldIndex("stylist_name"), Qt::Horizontal, tr("Stylist"));
    appt_assoc_names_model->setHeaderData(appt_assoc_names_model->fieldIndex("service_name"), Qt::Horizontal, tr("Service"));
    appt_assoc_names_model->setHeaderData(appt_assoc_names_model->fieldIndex("id"), Qt::Horizontal, tr("#"));
    appt_assoc_names_model->select();
    QSqlTableModel *stylist_model = new QSqlTableModel;
    stylist_model->setTable("stylist");
    stylist_model->setFilter(QString("name='%1'").arg(curStylistName));
    stylist_model->select();
    QSqlRecord record = stylist_model->record(0);
    QColor stylistColor(record.value("color").toString());
    //Set Email Address
    this->rcpt = record.value("email").toString();
    KDReports::Report report;
    report.setMargins(10,10,10,10);
    report.setHeaderBodySpacing(5);

    report.header().addElement(KDReports::ImageElement(this->ui->company_logo_label_4->pixmap()->scaled(80,80)),Qt::AlignRight);
    //Set Company Information
    KDReports::TextElement companyName(this->companyName);
    companyName.setBold(true);
    companyName.setPointSize(12);
    report.header().addElement(companyName);

    KDReports::TextElement companySlogan(this->companySlogan);
    companyName.setBold(true);
    companyName.setPointSize(8);
    report.header().addElement(companySlogan);

    KDReports::TextElement companyPhone(this->companyPhone);
    companyName.setBold(true);
    companyName.setPointSize(8);
    report.header().addElement(companyPhone);

    KDReports::TextElement companyAddress(this->companyAddress);
    companyName.setBold(true);
    companyName.setPointSize(8);
    report.header().addElement(companyAddress);

    KDReports::TextElement companyStateCity(this->ui->company_state_city_label_4->text());
    companyName.setBold(true);
    companyName.setPointSize(8);
    report.header().addElement(companyStateCity);
    // Add Title
    KDReports::TextElement title;
    title.setText(QString("%1 appointments for %2").arg(curStylistName).arg(curDate));
    title.setBold(true);
    title.setPointSize(14);
    report.addElement(title,Qt::AlignCenter);
    report.addVerticalSpacing(5);
    KDReports::AutoTableElement table(appt_assoc_names_model);
    table.setHeaderBackground(QBrush(stylistColor));
    table.setWidth(200-report.leftPageMargins()-report.rightPageMargins());
    report.addElement( table,Qt::AlignCenter );
    if(pdfFileName.isEmpty()){
        // show a print preview
        KDReports::PreviewDialog preview( &report );
        preview.showMaximized();
        preview.exec();
    }else{        
        report.exportToFile(pdfFileName);
        QString htmlFileName = pdfFileName.replace(".pdf",".html");
        report.exportToHtml(htmlFileName);
        QFile htmlFile(htmlFileName);
        htmlFile.open(QIODevice::ReadOnly);
        QString htmlDocument(htmlFile.readAll());
        this->message = htmlDocument;
    }    
}
void MainWindow::showAboutDialog(){
    AboutDialog dialog;
    dialog.exec();
}
void MainWindow::showApptStylistReportDialog(){
    ApptbyStylistReportDialog dialog;
    dialog.ui->stylist_comboBox->setModel(stylist_model);
    dialog.ui->stylist_comboBox->setModelColumn(stylist_model->fieldIndex("name"));
    dialog.ui->stylist_comboBox->setCurrentText(this->ui->stylist_comboBox->currentText());
    dialog.ui->employee_day_date_dateEdit->setDate(this->ui->employee_day_date_dateEdit->date());
    if(dialog.exec()){
        generateApptbyStylistReport(dialog.ui->employee_day_date_dateEdit->date().toString(global_config.date_format),dialog.ui->stylist_comboBox->currentText());
    }
}

void MainWindow::tableSelectionChanged(const QItemSelection & selected, const QItemSelection & deselected){
    if(selected.indexes().length()){
        //this->ui->mainToolBar->setEnabled(true);
        if(selected.indexes().at(0).data().isValid()){
            this->ui->actionDelete_Record->setEnabled(true);
            this->ui->actionEdit_Record->setEnabled(true);
            this->ui->actionAdd->setEnabled(false);
            this->ui->actionAdd_Break->setEnabled(false);
        }else{
            this->ui->actionDelete_Record->setEnabled(false);
            this->ui->actionEdit_Record->setEnabled(false);
            this->ui->actionAdd->setEnabled(true);
            this->ui->actionAdd_Break->setEnabled(true);
        }
        //}

    }else{
        //this->ui->mainToolBar->setEnabled(false);
    }
}
void MainWindow::mailSent(QString status)
{
    if(status == "Message sent")
        QMessageBox::warning( 0, this->windowTitle(), tr( "Message sent!\n\n" ) );
}
void MainWindow::showEmailConfigDialog(){
    emailConfig config;
    config.setServerName(this->serverName);
    config.setPort(this->port);
    config.setUserName(this->username);
    config.setPassword(this->password);
    config.setSubject(this->subject);
    if(config.exec()){
        this->serverName = config.getServerName();
        this->port = config.getPort();
        this->username = config.getUserName();
        this->password = config.getPassword();
        this->subject = config.getSubject();
    }
}
void MainWindow::deleteSelectedAppt(){
    MyQTableWidget *curTable;
    if(this->ui->tabWidget->currentIndex()==0){
        curTable = this->ui->daily_appt_tableWidget;
    }else{
        curTable = this->ui->employee_day_tableWidget;
    }
    MyCell *selectedItem = (MyCell*)curTable->selectedItems().at(0);
    if(selectedItem){
        ApptDeleteConfirmationDialog deleteDialog(this);
        QString deletingApptStyle = "<span style=\"color: red\">";
        QString spanTagClose = "</span>";
        deleteDialog.ui->warning_message_label->setText("Do you want to delete the following appointment:");
        deleteDialog.ui->customer_origin_value_label->setText(deletingApptStyle+selectedItem->getApptCustomerName()+spanTagClose);
        deleteDialog.ui->service_origin_value_label->setText(deletingApptStyle+selectedItem->getApptServiceName()+spanTagClose);
        deleteDialog.ui->stylist_origin_value_label->setText(deletingApptStyle+selectedItem->getApptStylistName()+spanTagClose);
        deleteDialog.ui->begin_time_origin_label->setText(deletingApptStyle+selectedItem->getApptBeginTime()+spanTagClose);
        deleteDialog.ui->end_time_origin_value_label->setText(deletingApptStyle+selectedItem->getApptEndTime()+spanTagClose);
        deleteDialog.ui->details_origin_value_label->setText(deletingApptStyle+selectedItem->getApptDetails()+spanTagClose);
        if(deleteDialog.exec()){
            QSqlTableModel appointment_table;
            appointment_table.setTable("appointment");
            appointment_table.setFilter(QString("id='%1'").arg(selectedItem->getApptID()));
            appointment_table.setEditStrategy(QSqlTableModel::OnManualSubmit);
            bool selected = appointment_table.select();
            if(selected && appointment_table.rowCount()){
                bool rowRemoved = appointment_table.removeRow(0);
                if(rowRemoved && appointment_table.submitAll()){
                    qDebug()<<"Success deleting Record!";
                }else{
                    qDebug()<<"Error: "<<appointment_table.lastError();
                }
            }
            this->create_daily_appt();
        }
    }
}
void MainWindow::editSelectedAppt(){
    MyQTableWidget *curTable;
    if(this->ui->tabWidget->currentIndex()==0){
        curTable = this->ui->daily_appt_tableWidget;

    }else{
        curTable = this->ui->employee_day_tableWidget;
    }
    MyCell *selectedItem = (MyCell*)curTable->selectedItems().at(0);
    //qDebug()<<selectedItem->row();
    //qDebug()<<selectedItem->column();
    if(selectedItem){
        int apptID = selectedItem->getApptID();
        EditAppointmentDialog editDialog(this);
        editDialog.editApptbyID(apptID);
        editDialog.exec();
        this->create_daily_appt();
        this->create_employee_appt();
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::create_daily_appt(){
    MyQTableWidget *curTable = this->ui->daily_appt_tableWidget;
    //Clean table
    curTable->clear();
    curTable->setColumnCount(0);
    curTable->setRowCount(0);
    curTable->verticalHeader()->setVisible(true);
    QTime curTime(global_config.first_time->hour(),global_config.first_time->minute(),global_config.first_time->second());
    do{
        //Adds one line in the table
        curTable->setRowCount(curTable->rowCount()+1);
        //Creates a new item for vertical header
        curTable->setVerticalHeaderItem(curTable->rowCount()-1, new QTableWidgetItem(curTime.toString(global_config.time_format)));
        curTable->verticalHeader()->resizeSection(curTable->rowCount()-1,global_config.verticalSectionSize);
        curTime = curTime.addSecs(global_config.appointments_interval*60);
    }while(curTime <= (*global_config.last_time));
    //Fill table with Daily stylist appointments
    QSqlTableModel stylist_model;
    stylist_model.setTable("stylist");
    stylist_model.select();
    MyCell *curItem;
    QSqlTableModel appts_model;
    appts_model.setTable("appt_assoc_names");
    QString curDate = this->ui->daily_appt_date_dateEdit->date().toString(global_config.date_format);
    //qDebug()<<curDate;
    appts_model.setFilter(QString("date='%1'").arg(curDate));
    appts_model.select();
    //qDebug()<<stylist_model.rowCount();
    //Add Horizontal Header
    qreal sectionSize=0;
    MyCell *curHeaderItem;
    QString curHeaderString;
    int curStylistID;
    for(int i=0;i<stylist_model.rowCount();i++){
        //Adds a new column
        curTable->setColumnCount(curTable->columnCount()+1);
        //Adds a new Stylist name to the horizontal header list
        curHeaderString = stylist_model.record(i).value("name").toString();
        curStylistID = stylist_model.record(i).value("id").toInt();
        curHeaderItem = new MyCell(curHeaderString);
        //Set appointment Stylist ID
        curHeaderItem->setApptStylistID(curStylistID);
        curTable->setHorizontalHeaderItem(i,curHeaderItem);
        //Resize Columns
        curTable->resizeColumnsToContents();
        sectionSize = sectionSize<curTable->horizontalHeader()->sectionSize(i)?curTable->horizontalHeader()->sectionSize(i):sectionSize;
    }
    //Calculates the space for filling the screen with the table
    for(int i=0;i<stylist_model.rowCount();i++){
        QColor curColor(stylist_model.record(i).value("color").toString());
        int curStylistID = stylist_model.record(i).value("id").toInt();
        //curTable->horizontalHeaderItem(i)->setForeground(QBrush(curColor));
        //Equalize the size to all Horizontal Section
        curTable->horizontalHeader()->resizeSection(i,sectionSize>global_config.default_column_size?sectionSize:global_config.default_column_size);
        //Set Sections not resizable
        curTable->horizontalHeader()->setSectionResizeMode(i,QHeaderView::Fixed);
        QString curApptTimeBeginString,curApptStylistName, curApptTimeEndString, curApptCustomerName,curApptServiceName,curApptDetails;
        for(int p=0;p<appts_model.rowCount();p++){
            curApptTimeBeginString = appts_model.record(p).value("time_begin").toString();
            int curApptStylistID = appts_model.record(p).value("stylist_id").toInt();
            int curApptID = appts_model.record(p).value("id").toInt();
            curApptTimeEndString = appts_model.record(p).value("time_end").toString();
            for(int q=0;q<curTable->rowCount();q++){
                QString curVerticalHeaderText = curTable->verticalHeaderItem(q)->text();
                if(curApptTimeBeginString==curVerticalHeaderText && curStylistID==curApptStylistID){
                    curApptDetails = appts_model.record(p).value("details").toString();
                    curApptCustomerName = appts_model.record(p).value("customer_name").toString();
                    curApptServiceName = appts_model.record(p).value("service_name").toString();
                    curApptStylistName = appts_model.record(p).value("stylist_name").toString();
                    //QString curText = "<b>Customer:</b> "+curApptCustomerName+"<br><b>Service:</b> "+curApptServiceName+"<br><b>Details:</b> "+curApptDetails;
                    QString curText2 = "Customer: "+curApptCustomerName+" Service: "+curApptServiceName+" Details: "+curApptDetails;
                    curItem = new MyCell(curText2);
                    //Set Appointment ID on curItem
                    curItem->setApptID(curApptID);
                    //Set appointment Stylist ID on CurItem
                    curItem->setApptStylistID(curApptStylistID);
                    //Set appointment Stylist Name on CurItem
                    curItem->setApptStylistName(curApptStylistName);
                    //Set appointment Customer Name
                    curItem->setApptCustomerName(curApptCustomerName);
                    //Set appointment Service Name
                    curItem->setApptServiceName(curApptServiceName);
                    //Set appointment Details
                    curItem->setApptDetails(curApptDetails);
                    //Set appointment Begin Time
                    curItem->setApptBeginTime(curApptTimeBeginString);
                    //Set appointment End Time
                    curItem->setApptEndTime(curApptTimeEndString);
                    //QLabel *curLabel = new QLabel(curText);
                    //curLabel->setWordWrap(true);
                    //curLabel->setAcceptDrops(true);
                    //curLabel->setAutoFillBackground(true);
                    //curTable->setCellWidget(q,i,curLabel);
                    curItem->setBackgroundColor(curColor);
                    curTable->setItem(q,i,curItem);
                    //Set Span
                    QString time_format = global_config.time_format;
                    int numberOfCells = this->getApptDurationInMinutes(QTime::fromString(curApptTimeBeginString,time_format),QTime::fromString(curApptTimeEndString,time_format))/global_config.appointments_interval;
                    curTable->setSpan(q,i,numberOfCells,1);
                }
            }
        }
    }
    //Resize Rows
    //curTable->resizeRowsToContents();
    //Set Sections of Horizontal Headers as fixed
    curTable->horizontalHeader()->setSectionsMovable(true);
}
void MainWindow::create_employee_appt(){    
    //Creates Table
    QTableWidget *curTable = this->ui->employee_day_tableWidget;
    //Clean table
    curTable->clear();
    curTable->setColumnCount(0);
    curTable->setRowCount(0);
    curTable->verticalHeader()->setVisible(false);
    curTable->horizontalHeader()->setVisible(false);
    QTime curTime(global_config.first_time->hour(),global_config.first_time->minute(),global_config.first_time->second());
    //QStringList vertical_headers_labels;
    curTable->setRowCount(global_config.per_column_appts);
    for(int i=0;curTime <= (*global_config.last_time);++i){
        curTable->setColumnCount(2+((int)(i/global_config.per_column_appts))*2);
        MyCell *curItem = new MyCell(curTime.toString(global_config.time_format));
        curItem->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        curItem->setFlags(Qt::ItemIsEnabled);
        QFont font = curItem->font();
        font.setBold(true);
        curItem->setFont(font);
        curTable->setItem(i%global_config.per_column_appts,(int)(i/global_config.per_column_appts)*2,curItem);
        curTime = curTime.addSecs(global_config.appointments_interval*60);
        curTable->verticalHeader()->resizeSection(i,global_config.verticalSectionSize);
    }
    QSqlTableModel stylist_model;
    stylist_model.setTable("stylist");
    stylist_model.setFilter(QString("name='%1'").arg(this->ui->stylist_comboBox->currentText()));
    stylist_model.select();
    QSqlRecord stylist_record = stylist_model.record(0);
    QColor curColor;
    int stylist_id=-1;
    if(!stylist_record.isEmpty()){
        curColor.setNamedColor(stylist_record.value("color").toString());
        stylist_id = stylist_record.value("id").toInt();
    }
    MyCell *curHeaderItem;
    for(int i=0;i<curTable->columnCount();++i){
        //Equalize the size to all Horizontal Section
        curTable->horizontalHeader()->resizeSection(i,global_config.default_column_size);
        //Set Sections not resizable
        curTable->horizontalHeader()->setSectionResizeMode(i,QHeaderView::Fixed);
        //Set Stylist ID on Header Items
        curHeaderItem = new MyCell(this->ui->stylist_comboBox->currentText());
        curHeaderItem->setApptStylistID(stylist_id);
        curTable->setHorizontalHeaderItem(i,curHeaderItem);
    }
    QSqlTableModel appts_model;
    appts_model.setTable("appt_assoc_names");
    QString curDate = this->ui->employee_day_date_dateEdit->date().toString(global_config.date_format);
    appts_model.setFilter(QString("date='%1' AND stylist_name='%2'").arg(curDate).arg(this->ui->stylist_comboBox->currentText()));
    appts_model.select();
    QString curApptTimeBeginString,curApptStylistName, curApptTimeEndString, curApptCustomerName,curApptServiceName,curApptDetails;
    int curApptID, curApptStylistID;
    QString time_format = global_config.time_format;
    for(int p=0;p<appts_model.rowCount();p++){
        curApptTimeBeginString = appts_model.record(p).value("time_begin").toString();
        curApptTimeEndString = appts_model.record(p).value("time_end").toString();
        int rowNumber = this->getApptDurationInMinutes(*global_config.first_time,QTime::fromString(curApptTimeBeginString,time_format))/global_config.appointments_interval;
        int columnNumber = 1;
        if(rowNumber>=global_config.per_column_appts){
            columnNumber = ((int)(rowNumber/global_config.per_column_appts))*2+1;
            rowNumber = rowNumber%global_config.per_column_appts;
        }
        curApptDetails = appts_model.record(p).value("details").toString();
        curApptCustomerName = appts_model.record(p).value("customer_name").toString();
        curApptServiceName = appts_model.record(p).value("service_name").toString();
        curApptStylistName = appts_model.record(p).value("stylist_name").toString();
        curApptID = appts_model.record(p).value("id").toInt();
        curApptStylistID = appts_model.record(p).value("stylist_id").toInt();
        QString curText = "Customer: "+curApptCustomerName+" Service: "+curApptServiceName+" Details: "+curApptDetails;
        MyCell *curItem = new MyCell(curText);
        //Set Appointment ID on curItem
        curItem->setApptID(curApptID);
        //Set appointment Stylist ID on CurItem
        curItem->setApptStylistID(curApptStylistID);
        //Set appointment Stylist Name on CurItem
        curItem->setApptStylistName(curApptStylistName);
        //Set appointment Customer Name
        curItem->setApptCustomerName(curApptCustomerName);
        //Set appointment Service Name
        curItem->setApptServiceName(curApptServiceName);
        //Set appointment Details
        curItem->setApptDetails(curApptDetails);
        //Set appointment Begin Time
        curItem->setApptBeginTime(curApptTimeBeginString);
        //Set appointment End Time
        curItem->setApptEndTime(curApptTimeEndString);
        //Set Cell Color
        curItem->setBackgroundColor(curColor);
        //Set Item Flags
        //curItem->setFlags(Qt::ItemIsEnabled);
        curTable->setItem(rowNumber,columnNumber,curItem);
        int numberOfCells = this->getApptDurationInMinutes(QTime::fromString(curApptTimeBeginString,time_format),QTime::fromString(curApptTimeEndString,time_format))/global_config.appointments_interval;
        if(rowNumber+(numberOfCells-1)>=global_config.per_column_appts){
            curTable->setSpan(rowNumber,columnNumber,(global_config.per_column_appts-rowNumber),1);
            int numberOfExtraCellsSpan = numberOfCells - (global_config.per_column_appts-rowNumber);
            int numberOfExtraColumnsSpan = numberOfExtraCellsSpan/global_config.per_column_appts;
            int numberOfRestingCellsSpan = numberOfExtraCellsSpan%global_config.per_column_appts;
            MyCell *newItem = new MyCell(QString("*** ").append(curItem->text()).append(" ***"));
            //Add this item as chield of main item
            curItem->addMyCellChild(newItem);
            newItem->setFlags(Qt::ItemIsEnabled);
            QColor newColor = curColor;
            newColor.setAlpha(128);
            newItem->setBackgroundColor(newColor);
            curTable->setItem(0,columnNumber+2,newItem);
            if(numberOfExtraColumnsSpan){
                curTable->setSpan(0,columnNumber+2,global_config.per_column_appts,1);
                int i;
                for(i=0;i<numberOfExtraColumnsSpan;i++){
                    MyCell *newItem = new MyCell(QString("*** ").append(curItem->text()).append(" ***"));
                    //Add this item as chield of main item
                    curItem->addMyCellChild(newItem);
                    newItem->setFlags(Qt::ItemIsEnabled);
                    QColor newColor = curColor;
                    newColor.setAlpha(128);
                    newItem->setBackgroundColor(newColor);
                    curTable->setItem(0,columnNumber+4+2*i,newItem);
                    if(numberOfExtraColumnsSpan-1>i){
                        curTable->setSpan(0,columnNumber+4+2*i,global_config.per_column_appts,1);
                    }else{
                        curTable->setSpan(0,columnNumber+4+2*i,numberOfRestingCellsSpan,1);
                    }
                }
            }else{
                curTable->setSpan(0,columnNumber+2,numberOfRestingCellsSpan,1);
            }
        }else{
            curTable->setSpan(rowNumber,columnNumber,numberOfCells,1);
        }
    }

}

void MainWindow::showCreateCustomerDialog(){
    NewCustomerDialog dialog(this);
    //Set USA states list to State Combobox and City
    dialog.ui->state_comboBox->addItems(global_config.usa_states);
    dialog.ui->city_lineEdit_7->setText("Fort Wayne");
    if(dialog.exec()){
        MyDataModel customer_model("customer");
        customer_model.setValue("name",dialog.ui->name_lineEdit->text());
        customer_model.setValue("birthday",dialog.ui->birthday_dateEdit->date());
        customer_model.setValue("email",dialog.ui->email_lineEdit_4->text());
        customer_model.setValue("phone1",dialog.ui->phone1_lineEdit_2->text());
        customer_model.setValue("phone2",dialog.ui->phone2_lineEdit_3->text());
        customer_model.setValue("address",dialog.ui->address_lineEdit_5->text());
        customer_model.setValue("zip_code",dialog.ui->zip_code_lineEdit_6->text());
        customer_model.setValue("state",dialog.ui->state_comboBox->currentText());
        customer_model.setValue("city",dialog.ui->city_lineEdit_7->text());
        int customer_id = customer_model.submitAll();
        if(customer_id>0){
            qDebug()<<QString("Customer Saved. ID: %1").arg(customer_id);
        }
    }
}
void MainWindow::showCreateStylistDialog(){
    NewStylistDialog dialog(this);
    //Set USA states list to State Combobox and City
    dialog.ui->state_comboBox->addItems(global_config.usa_states);
    dialog.ui->city_lineEdit_7->setText("Fort Wayne");
    if(dialog.exec()){
        MyDataModel stylist_model("stylist");
        stylist_model.setValue("name",dialog.ui->name_lineEdit->text());
        stylist_model.setValue("nickname",dialog.ui->nickname_lineEdit_2->text());
        stylist_model.setValue("phone1",dialog.ui->phone1_lineEdit_4->text());
        stylist_model.setValue("phone2",dialog.ui->phone2_lineEdit_5->text());
        stylist_model.setValue("email",dialog.ui->email_lineEdit_3->text());
        stylist_model.setValue("birthday",dialog.ui->birthday_dateEdit->date());
        stylist_model.setValue("address",dialog.ui->address_lineEdit_5->text());
        stylist_model.setValue("zip_code",dialog.ui->zip_code_lineEdit_6->text());
        stylist_model.setValue("state",dialog.ui->state_comboBox->currentText());
        stylist_model.setValue("city",dialog.ui->city_lineEdit_7->text());
        QString stylesheet_string = dialog.ui->color_view_label_12->styleSheet();
        QString color_string = stylesheet_string.mid(stylesheet_string.indexOf(":")+2,stylesheet_string.length()-stylesheet_string.indexOf(":")-3);
        stylist_model.setValue("color",color_string);
        int stylist_id = stylist_model.submitAll();
        if(stylist_id>0){
            qDebug()<<QString("Stylist Saved. ID: %1").arg(stylist_id);
            //Update the Daily Appoitments Table
            this->create_daily_appt();
        }
        return;
    }
}
void MainWindow::showCreateServiceDialog(){
    NewServiceDialog dialog(this);
    if(dialog.exec()){
        MyDataModel service_model("service");
        service_model.setValue("name",dialog.ui->name_lineEdit_2->text());
        service_model.setValue("description",dialog.ui->description_textEdit->toPlainText());
        service_model.setValue("price",dialog.ui->price_lineEdit_3->text().toFloat());
        service_model.submitAll();
        int service_id = service_model.submitAll();
        if(service_id>0){
            qDebug()<<QString("Service Saved. ID: %1").arg(service_id);
        }
        return;
    }
}
void MainWindow::addAppt(){
    NewAppointmentDialog dialog(this);
    //Buttons Connections
    QObject::connect(dialog.ui->new_customer_pushButton_3,SIGNAL(clicked()),this,SLOT(showCreateCustomerDialog()));
    QObject::connect(dialog.ui->new_service_pushButton,SIGNAL(clicked()),this,SLOT(showCreateServiceDialog()));
    QObject::connect(dialog.ui->new_stylist_pushButton_2,SIGNAL(clicked()),this,SLOT(showCreateStylistDialog()));
    //Sets Model for Customer Combobox
    QSqlTableModel customer_model;
    customer_model.setTable("customer");
    customer_model.select();
    dialog.ui->customer_comboBox->setModel(&customer_model);
    dialog.ui->customer_comboBox->setModelColumn(customer_model.fieldIndex("name"));
    //Sets Model for Service Combobox
    QSqlTableModel service_model;
    service_model.setTable("service");
    service_model.select();
    dialog.ui->service_comboBox->setModel(&service_model);
    dialog.ui->service_comboBox->setModelColumn(service_model.fieldIndex("name"));
    //Sets Model for Stylist Combobox
    QSqlTableModel stylist_model;
    stylist_model.setTable("stylist");
    stylist_model.select();
    dialog.ui->stylist_comboBox->setModel(&stylist_model);
    dialog.ui->stylist_comboBox->setModelColumn(stylist_model.fieldIndex("name"));
    //Configure Dates and Times
    dialog.ui->dateEdit->setDate(this->ui->daily_appt_date_dateEdit->date());
    //Set Params of new Appointment for selected column
    MyQTableWidget *curTable;
    int selectedRow,selectedColumn;
    QString beginTimeString;
    if(this->ui->tabWidget->currentIndex()==0){
        curTable = this->ui->daily_appt_tableWidget;
        selectedRow = curTable->selectionModel()->selectedIndexes().at(0).row();
        selectedColumn = curTable->selectionModel()->selectedIndexes().at(0).column();
        beginTimeString = curTable->verticalHeaderItem(selectedRow)->text();
    }else{
        curTable = this->ui->employee_day_tableWidget;
        selectedRow = curTable->selectionModel()->selectedIndexes().at(0).row();
        selectedColumn = curTable->selectionModel()->selectedIndexes().at(0).column();
        beginTimeString = curTable->item(selectedRow,selectedColumn-1)->text();
    }
    QString stylistName = curTable->horizontalHeaderItem(selectedColumn)->text();
    dialog.ui->stylist_comboBox->setCurrentText(stylistName);
    dialog.ui->timebegin_timeEdit->setTime(QTime::fromString(beginTimeString,global_config.time_format));

    //Shows Modal Dialog
    if(dialog.exec()){
        MyDataModel appointment_model("appointment");
        //Retrieves the IDs from Comboboxes Labels
        QString selectedCustomer,selectedService,selectedStylist;
        selectedCustomer = dialog.ui->customer_comboBox->currentText();
        selectedService = dialog.ui->service_comboBox->currentText();
        selectedStylist = dialog.ui->stylist_comboBox->currentText();
        //Retrieves Customer ID
        QSqlTableModel *idRetrieveryTable = new QSqlTableModel;
        idRetrieveryTable->setTable("customer");
        idRetrieveryTable->select();
        idRetrieveryTable->setFilter(QString("name='%1'").arg(selectedCustomer));
        QSqlRecord curCustomerRecord = idRetrieveryTable->record(0);
        int selectedCustomerID = curCustomerRecord.value("id").toInt();
        //Update Service Table
        idRetrieveryTable->setTable("service");
        idRetrieveryTable->select();
        idRetrieveryTable->setFilter(QString("name='%1'").arg(selectedService));
        QSqlRecord curServiceRecord = idRetrieveryTable->record(0);
        int selectedServiceID = curServiceRecord.value("id").toInt();
        //Update Stylist Table
        idRetrieveryTable->setTable("stylist");
        idRetrieveryTable->select();
        idRetrieveryTable->setFilter(QString("name='%1'").arg(selectedStylist));
        QSqlRecord curStylistRecord = idRetrieveryTable->record(0);
        int selectedStylistID = curStylistRecord.value("id").toInt();
        appointment_model.setValue("stylist_id",selectedStylistID);
        appointment_model.setValue("customer_id",selectedCustomerID);
        appointment_model.setValue("service_id",selectedServiceID);
        appointment_model.setValue("date",dialog.ui->dateEdit->date().toString(global_config.date_format));
        appointment_model.setValue("time_begin",dialog.ui->timebegin_timeEdit->time().toString(global_config.time_format));
        appointment_model.setValue("time_end",dialog.ui->timeend_timeEdit->time().toString(global_config.time_format));
        appointment_model.setValue("details",dialog.ui->details_plainTextEdit->toPlainText());
        int appointment_id = appointment_model.submitAll();
        if(appointment_id>0){
            qDebug()<<QString("Appointment Saved. ID: %1").arg(appointment_id);
            //Update the Daily Appoitments Table
            this->create_daily_appt();
            this->create_employee_appt();
        }
        return;
    }
}
void MainWindow::showCreateUserDialog(){
    NewUserDialog dialog(this);
    if(dialog.exec()){
        MyDataModel user_model("user");
        user_model.setValue("login",dialog.ui->login_lineEdit->text());
        user_model.setValue("password",dialog.ui->password_lineEdit->text());
        user_model.setValue("email",dialog.ui->email_lineEdit->text());
        int user_id = user_model.submitAll();
        if(user_id>0){
            qDebug()<<QString("User Saved. ID: %1").arg(user_id);
        }
        return;
    }
}
void MainWindow::make_connections(){
    //Menu Connections
    QObject::connect(this->ui->action_create_Customer,SIGNAL(triggered()),this,SLOT(showCreateCustomerDialog()));
    QObject::connect(this->ui->action_create_Stylist,SIGNAL(triggered()),this,SLOT(showCreateStylistDialog()));
    QObject::connect(this->ui->action_create_Service,SIGNAL(triggered()),this,SLOT(showCreateServiceDialog()));
    QObject::connect(this->ui->action_create_User,SIGNAL(triggered()),this,SLOT(showCreateUserDialog()));
    QObject::connect(this->ui->action_edit_Customers,SIGNAL(triggered()),this,SLOT(showEditCustomerDialog()));
    QObject::connect(this->ui->action_edit_Stylists,SIGNAL(triggered()),this,SLOT(showEditStylistDialog()));
    QObject::connect(this->ui->action_edit_Users,SIGNAL(triggered()),this,SLOT(showEditUserDialog()));
    QObject::connect(this->ui->action_edit_Services,SIGNAL(triggered()),this,SLOT(showEditServiceDialog()));
    QObject::connect(this->ui->action_edit_Appointments,SIGNAL(triggered()),this,SLOT(showEditAppointmentDialog()));
    QObject::connect(this->ui->daily_appt_date_dateEdit,SIGNAL(dateChanged(QDate)),this,SLOT(create_daily_appt()));
    QObject::connect(this->ui->employee_day_date_dateEdit,SIGNAL(dateChanged(QDate)),this,SLOT(create_employee_appt()));
    QObject::connect(this->ui->stylist_comboBox,SIGNAL(currentTextChanged(QString)),this,SLOT(create_employee_appt()));
    QObject::connect(this->ui->tabWidget,SIGNAL(currentChanged(int)),this,SLOT(tabChanged(int)));
    QObject::connect(this->ui->daily_appt_tableWidget->selectionModel(),SIGNAL(selectionChanged(QItemSelection,QItemSelection)),this,SLOT(tableSelectionChanged(QItemSelection,QItemSelection)));
    QObject::connect(this->ui->employee_day_tableWidget->selectionModel(),SIGNAL(selectionChanged(QItemSelection,QItemSelection)),this,SLOT(tableSelectionChanged(QItemSelection,QItemSelection)));
    QObject::connect(this->ui->actionDelete_Record,SIGNAL(triggered()),this,SLOT(deleteSelectedAppt()));
    QObject::connect(this->ui->actionEdit_Record,SIGNAL(triggered()),this,SLOT(editSelectedAppt()));
    QObject::connect(this->ui->actionAdd,SIGNAL(triggered()),this,SLOT(addAppt()));
    QObject::connect(this->ui->actionSend_Email,SIGNAL(triggered()),this,SLOT(SendEmail()));
    QObject::connect(this->ui->actionEmail,SIGNAL(triggered()),this,SLOT(showEmailConfigDialog()));
    QObject::connect(this->ui->actionCompany,SIGNAL(triggered()),this,SLOT(showCompanyConfigDialog()));
    QObject::connect(this->ui->actionAbout_Tech_Scheduler,SIGNAL(triggered()),this,SLOT(showAboutDialog()));
    QObject::connect(this->ui->action_report_Appointments_Stylist,SIGNAL(triggered()),this,SLOT(showApptStylistReportDialog()));


}
void MainWindow::SendEmail(){
    QStringList files;
    QFile tmp_file;
    QString curStylistName = this->ui->stylist_comboBox->currentText();
    QString curDate = this->ui->employee_day_date_dateEdit->date().toString(global_config.date_format);
    QString curReportFileName = QString("ApptsbyStylist").append(".pdf");
    this->generateApptbyStylistReport(curDate,curStylistName,curReportFileName);
    tmp_file.setFileName(curReportFileName);

    if(tmp_file.open(QIODevice::ReadOnly)){
        files<<curReportFileName;
        tmp_file.close();
    }
    //qDebug()<<files;
    Smtp* smtp = new Smtp(this->username, this->password, this->serverName, this->port);
    connect(smtp, SIGNAL(status(QString)), this, SLOT(mailSent(QString)));
    //QObject::connect(smtp,SIGNAL(log(QString)),this,SLOT(updateLog(QString)));
    this->ui->statusBar->showMessage("Sending Email...");
    if(!files.isEmpty())
        smtp->sendMail(this->username, this->rcpt , this->subject,this->message, files );
    else
        smtp->sendMail(this->username, this->rcpt , this->subject,this->message);
    this->ui->statusBar->clearMessage();
}

void MainWindow::tabChanged(int current){
    this->ui->daily_appt_tableWidget->selectionModel()->clear();
    this->ui->employee_day_tableWidget->selectionModel()->clear();
    if(current==0){
        create_daily_appt();
        this->ui->actionSend_Email->setEnabled(false);
    }else{
        create_employee_appt();
        this->ui->actionSend_Email->setEnabled(true);
    }
}

void MainWindow::showEditCustomerDialog(){
    EditCustumerDialog dialog;
    dialog.exec();
    //Update the Daily Appoitments Table
    this->create_daily_appt();
}
void MainWindow::showEditStylistDialog(){
    EditStylistDialog dialog;
    dialog.exec();
    //Update the Daily Appoitments Table
    this->stylist_model->select();
    this->create_daily_appt();
}
void MainWindow::showEditServiceDialog(){
    EditServiceDialog dialog;
    dialog.exec();
    //Update the Daily Appoitments Table
    this->create_daily_appt();
}
void MainWindow::showEditUserDialog(){
    EditUserDialog dialog;
    dialog.exec();
}
void MainWindow::showEditAppointmentDialog(){
    EditAppointmentDialog dialog;
    dialog.exec();
    //Update the Daily Appoitments Table
    this->create_daily_appt();
}



void MainWindow::saveUserPreferences(){
    QSettings toSave(QSettings::IniFormat,QSettings::UserScope,VER_COMPANYNAME_STR,VER_FILEDESCRIPTION_STR);
    toSave.beginGroup("MainWindow");
    toSave.setValue("geometry", saveGeometry());
    toSave.endGroup();
    toSave.beginGroup("EmailConfig");
    toSave.setValue("serverName",this->serverName);
    toSave.sync();
    toSave.setValue("serverPort",this->port);
    toSave.sync();
    toSave.setValue("userName",this->username);
    toSave.sync();
    toSave.setValue("password",this->password);
    toSave.sync();
    toSave.setValue("recipient",this->rcpt);
    toSave.sync();
    toSave.setValue("subject",this->subject);
    toSave.sync();
    toSave.setValue("message",this->message);
    toSave.sync();
    toSave.endGroup();
    toSave.beginGroup("CompanyConfig");
    toSave.setValue("companyName",this->companyName);
    toSave.sync();
    toSave.setValue("companyAddress",this->companyAddress);
    toSave.sync();
    toSave.setValue("companyCity",this->companyCity);
    toSave.sync();
    toSave.setValue("companyEmail",this->companyEmail);
    toSave.sync();
    toSave.setValue("companyLogoFile",this->companyLogoFile);
    toSave.sync();
    toSave.setValue("companyPhone",this->companyPhone);
    toSave.sync();
    toSave.setValue("companySlogan",this->companySlogan);
    toSave.sync();
    toSave.setValue("companyState",this->companyState);
    toSave.sync();
    toSave.endGroup();
}
void MainWindow::loadUerPreferences(){
    QSettings toLoad(QSettings::IniFormat,QSettings::UserScope,VER_COMPANYNAME_STR,VER_FILEDESCRIPTION_STR);
    toLoad.beginGroup("MainWindow");
    this->restoreGeometry(toLoad.value("geometry").toByteArray());
    toLoad.endGroup();
    toLoad.beginGroup("EmailConfig");
    this->serverName = toLoad.value("servername","").toString();
    this->port = toLoad.value("serverPort",465).toInt();
    this->username = toLoad.value("username","").toString();
    this->password = toLoad.value("password","").toString();
    this->rcpt = toLoad.value("recipient","").toString();
    this->subject = toLoad.value("subject","Tech Scheduler Notification Email").toString();
    this->message = toLoad.value("message","").toString();
    toLoad.endGroup();
    toLoad.beginGroup("CompanyConfig");
    this->companyAddress = toLoad.value("companyAddress","").toString();
    this->companyCity = toLoad.value("companyCity","Fort Wayne").toString();
    this->companyName = toLoad.value("companyName","").toString();
    this->companyEmail = toLoad.value("companyEmail","techscheduler@indianatech.net").toString();
    this->companyLogoFile = toLoad.value("companyLogoFile","").toString();
    this->companyPhone = toLoad.value("companyPhone","").toString();
    this->companySlogan = toLoad.value("companySlogan","").toString();
    this->companyState = toLoad.value("companyState","").toString();
    toLoad.endGroup();
}
void MainWindow::closeEvent(QCloseEvent *event)
{
    saveUserPreferences();
    event->accept();

}
int MainWindow::getApptDurationInMinutes(QTime begin_time, QTime end_time){
    int durationInMinutes = (end_time.msecsSinceStartOfDay()/1000/60)-(begin_time.msecsSinceStartOfDay()/1000/60);
    return durationInMinutes;
}
