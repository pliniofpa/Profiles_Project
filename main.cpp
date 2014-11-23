#include "mainwindow.h"
#include <QApplication>
#include "connection.h"
#include "version.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName(VER_FILEDESCRIPTION_STR);
    app.setOrganizationName(VER_COMPANYNAME_STR);
    app.setOrganizationDomain(VER_COMPANYDOMAIN_STR);
    if (!createConnection())
        return 1;
    MainWindow w;
    w.show();

    return app.exec();
}
