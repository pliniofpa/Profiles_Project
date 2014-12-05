#ifndef GLOBALCONFIG_H
#define GLOBALCONFIG_H

#include <QObject>
#include <QStringList>
class GlobalConfig : public QObject
{
    Q_OBJECT
public:
    explicit GlobalConfig(QObject *parent = 0);
    QString time_format,date_format;
    //Sets USA States List
    QStringList usa_states;

signals:

public slots:

};

#endif // GLOBALCONFIG_H
