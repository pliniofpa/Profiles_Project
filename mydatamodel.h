#ifndef MYDATAMODEL_H
#define MYDATAMODEL_H

#include <QObject>
#include <QList>
#include <QVariant>
class QSqlTableModel;
class QSqlDatabase;
class QSqlRecord;
class MyDataModel : public QObject
{
    Q_OBJECT
public:
    explicit MyDataModel(QString tablename, QSqlDatabase *db = 0, QObject *parent = 0);
    void setValue(QString field, QVariant value);
    int submitAll();
private:
    QSqlTableModel *table_model;
    QList<QString> fields;
    QList<QVariant> values;
    QSqlRecord *record;

signals:

public slots:

};

#endif // MYDATAMODEL_H
