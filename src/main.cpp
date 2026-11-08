#include "views/MainWindow.h"
#include "database/Database.h"

#include <QApplication>
#include <QSqlDatabase>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    if (!Database::conectar())
        return 1;

    MainWindow w;
    w.show();

    int res = a.exec();
    Database::cerrarConexion();
    return res;
}
