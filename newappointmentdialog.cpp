#include "newappointmentdialog.h"
#include "ui_newappointmentdialog.h"

NewAppointmentDialog::NewAppointmentDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewAppointmentDialog)
{
    ui->setupUi(this);
    //Removes "What's it?" button
    this->setWindowFlags(Qt::Dialog | Qt::WindowTitleHint | Qt::WindowSystemMenuHint | Qt::WindowCloseButtonHint);
    QObject::connect(this->ui->duration_spinBox,SIGNAL(editingFinished()),this,SLOT(updateEndTime()));
    QObject::connect(this->ui->timeend_timeEdit,SIGNAL(editingFinished()),this,SLOT(updateDuration()));
    QObject::connect(this->ui->timebegin_timeEdit,SIGNAL(timeChanged(QTime)),this,SLOT(setRange(QTime)));
    this->ui->timebegin_timeEdit->setTimeRange(*this->global_config.first_time,*this->global_config.last_time);
}

NewAppointmentDialog::~NewAppointmentDialog()
{
    delete ui;
}
void NewAppointmentDialog::setRange(QTime beginTime){
    this->ui->timeend_timeEdit->setTimeRange(beginTime.addSecs(global_config.appointments_interval*60),*this->global_config.last_time);
}
void NewAppointmentDialog::updateDuration(){
     QTime curTime = this->ui->timeend_timeEdit->time();
    int minutes = curTime.minute();
    int rest = minutes%global_config.appointments_interval;
    if(rest){
        float result= (float)rest/(float)global_config.appointments_interval;
        curTime = curTime.addSecs(-minutes*60);
        if(result>=0.5){
            this->ui->timeend_timeEdit->setTime(curTime.addSecs(60*(minutes+(global_config.appointments_interval-rest))));
        }else{
            this->ui->timeend_timeEdit->setTime(curTime.addSecs((minutes-rest)*60));
        }
    }
    //Update End Time
    QTime end_time = this->ui->timeend_timeEdit->time();
    QTime begin_time = this->ui->timebegin_timeEdit->time();
    this->ui->duration_spinBox->setValue((end_time.msecsSinceStartOfDay()/1000/60)-(begin_time.msecsSinceStartOfDay()/1000/60));
}
void NewAppointmentDialog::updateEndTime(){
    int duration = this->ui->duration_spinBox->value();
    int rest = duration%global_config.appointments_interval;
    if(rest){
        float result= (float)rest/(float)global_config.appointments_interval;
        if(result>=0.5){
            this->ui->duration_spinBox->setValue(duration+(global_config.appointments_interval-rest));
        }else{
            this->ui->duration_spinBox->setValue(duration-rest);
        }
    }
    //Update End Time
    this->ui->timeend_timeEdit->setTime(this->ui->timebegin_timeEdit->time().addSecs(this->ui->duration_spinBox->value()*60));
}
