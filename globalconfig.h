#ifndef GLOBALCONFIG_H
#define GLOBALCONFIG_H

#include <QObject>
#include <QStringList>
#include <QTime>
class GlobalConfig : public QObject
{
    Q_OBJECT
public:
    explicit GlobalConfig(QObject *parent = 0);
    QString time_format,date_format;
    int appointments_interval;
    //Sets USA States List
    QStringList usa_states;
    QTime *first_time,*last_time;
    int verticalSectionSize;
    int per_column_appts;
    int default_column_size;

signals:

public slots:

};

#endif // GLOBALCONFIG_H
