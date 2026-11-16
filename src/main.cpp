
#include "views/MainWindow.h"
#include "database/Database.h"
#include "database/DatabaseInitializer.h"

#include <QApplication>
#include <QDebug>

#include <exception>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    try {
        if (!Database::conectar()) {
            qCritical() << "No se pudo conectar a la base de datos.";
            Database::cerrarConexion();
            return 1;
        }

        if (!DatabaseInitializer::incializar()) {
            qCritical() << "No se pudo inicializar la base de datos.";
            Database::cerrarConexion();
            return 1;
        }

        MainWindow w;
        w.show();

        int resultado = a.exec();

        Database::cerrarConexion();
        return resultado;
    }
    catch (const std::exception &e) {
        qCritical() << "Error inesperado:" << e.what();
    }
    catch (...) {
        qCritical() << "Ocurrió un error desconocido.";
    }

    Database::cerrarConexion();
    return 1;
}
