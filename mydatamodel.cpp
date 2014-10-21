#include "mydatamodel.h"
#include <QSqlTableModel>
#include <QSqlRecord>
#include <QSqlField>
#include <QSqlError>
#include <QDebug>
#include <QSqlQueryModel>
#include <QSqlDatabase>
MyDataModel::MyDataModel(QString table_name, QSqlDatabase *db, QObject *parent) :
    QObject(parent)
{
    QSqlQueryModel query_model;
    if(db){
        this->table_model = new QSqlTableModel(parent, *db);
        query_model.setQuery(QString("SELECT * FROM %1 ASC LIMIT 1").arg(table_name),*db);
    }else{
        this->table_model = new QSqlTableModel(parent);
        query_model.setQuery(QString("SELECT * FROM %1 ASC LIMIT 1").arg(table_name));
    }
    this->table_model->setTable(table_name);
    this->table_model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    this->table_model->select();
    qDebug()<<this->table_model->database().tables();
    //Recovery default Record with all fiedls name
    this->record = new QSqlRecord();
    *this->record  = query_model.record();
}
void MyDataModel::setValue(QString field, QVariant value){
    /*
    this->fields.append(field);
    this->values.append(value);
    */
    this->record->setValue(field,value);
}
bool MyDataModel::submitAll(){
    /*
    QSqlRecord record;
    for(int i=0;i<this->fields.length();++i){
        record.append(QSqlField(this->fields.at(i)));
        record.setValue(this->fields.at(i),this->values.at(i));
    }
    */
    if(!this->table_model->insertRecord(-1,*this->record)){
        qDebug()<<this->table_model->lastError();
        return false;
    }
    if(!this->table_model->submitAll()){
        qDebug()<<this->table_model->lastError();
        return false;
    }
    return true;
}
