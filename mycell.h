/**
 * @file mycell.h
 * This Class defines a table cell item.
 * @brief Header file for MyCell class definition.
 *
 * This class implements a QTableWidgetItem with a specific tags, text alignment and color.
 *
 * @author Plinio Andrade &lt;PAndrade@fele.com&gt;
 * @version 1.0.0.0 (Qt: 5.3.1)
 */

#ifndef MYCELL_H
#define MYCELL_H
#include <QTableWidgetItem>
#include <QString>
#include <QList>

/**
 * @brief The MyCell class defines a table cell item.
 *
 * This class implements a QTableWidgetItem with a specific tags, text alignment and color.
 */
class MyCell: public QTableWidgetItem
{

public:
    /**
     * @brief Creates a cell item with the given string.
     *
     * @param text The given string.
     */
    MyCell(const QString &text);
    //void dropEvent(QDropEvent *event);
    //void dragMoveEvent(QDragMoveEvent *event);
    //void dragEnterEvent(QDragEnterEvent *event);
    //bool dropMimeData(int row, int column, const QMimeData *data, Qt::DropAction action);

    void setApptID(int id);
    void setApptStylistID(int stylistID);
    void setApptStylistName(QString stylistName);
    void setApptServiceName(QString serviceName);
    void setApptDetails(QString details);
    void setApptCustomerName(QString customerName);
    void setApptBeginTime (QString beginTime);
    void setApptEndTime (QString endTime);
    void setMyCellParent(MyCell *parent){
        this->MyCellParent = parent;
    }
    void addMyCellChild(MyCell *child){
        this->children->append(child);
    }
    QList<MyCell *> * getChildenList(){
        return this->children;
    }

    int getApptID();
    int getApptStylistID();
    QString getApptStylistName();
    QString getApptServiceName();
    QString getApptDetails();
    QString getApptCustomerName();
    QString getApptBeginTime();
    QString getApptEndTime();
private:
    MyCell *MyCellParent;
    QList<MyCell *> *children;
    int apptID, apptStylistID;
    QString *apptServiceName,*apptStylistName, *apptDetails, *apptCustomerName, *apptBeginTime, *apptEndTime;
};

#endif // MYCELL_H
