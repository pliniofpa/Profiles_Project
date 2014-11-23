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
    void dragEnterEvent(QDragEnterEvent *event);
    //bool dropMimeData(int row, int column, const QMimeData *data, Qt::DropAction action);

    void setApptID(int id);
    int getApptID();
    void setApptStylistID(int stylistID);
    int getApptStylistID();
private:
    int apptID;
    int apptStylistID;
};

#endif // MYCELL_H
