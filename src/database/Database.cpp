#include "Database.h"

#include <QSqlError>
#include <QDir>
#include <QDebug>
#include <QCoreApplication>

QSqlDatabase Database::conexion;

bool Database::conectar()
{
    if (QSqlDatabase::contains("BibliotecaConnection")) {
        conexion = QSqlDatabase::database("BibliotecaConnection");

        if (!conexion.isOpen()) {
            return conexion.open();
        }

        return true;
    }

    QString ruta = QDir(QCoreApplication::applicationDirPath())
                       .filePath("biblioteca.db");

    conexion = QSqlDatabase::addDatabase(
        "QSQLITE",
        "BibliotecaConnection"
        );

    conexion.setDatabaseName(ruta);

    if (!conexion.open()) {
        qDebug() << "Error al conectar:"
                 << conexion.lastError().text();
        return false;
    }

    qDebug() << "Base de datos:" << ruta;

    return true;
}

QSqlDatabase Database::obtenerConexion()
{
    return conexion;
}

void Database::cerrarConexion()
{
    if (conexion.isOpen()) {
        conexion.close();
    }
}