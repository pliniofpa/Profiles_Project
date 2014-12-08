#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H
#include <QDialog>
/**
 * @file aboutdialog.h
 * This Class defines application About dialog window.
 * @brief Header file for AboutDialog class definition.
 *
 * It's a simple QDialog derived class that defines application About window.
 *
 * @author Plinio Andrade &lt;PAndrade@fele.com&gt;
 * @version 1.0.0.0 (Qt: 5.3.1)
 */

namespace Ui {
class AboutDialog;
}
/**
 * @brief The AboutDialog class defines application About window.
 *
 * \image html about-dialog.png
 * It's a simple QDialog derived class that defines application About dialog window.
 */
class AboutDialog : public QDialog
{
    Q_OBJECT

public:
    /**
     * @brief Constructs this class with the given parent widget.
     *
     * @param parent A pointer to parent widget.
     */
    explicit AboutDialog(QWidget *parent = 0);

    /**
     * @brief Sets application version text in the created dialog to the given text.
     *
     * @param text The version text.
     */
    void setVersionText(QString text);

    /**
     * @brief Destructs this class.
     *
     */
    ~AboutDialog();

private:
    /**
     * @brief ui object was created by Qt Creator.
     *
     * It contains the graphical elements created using Qt Design.
     */
    Ui::AboutDialog *ui;
};

#endif // ABOUTDIALOG_H
