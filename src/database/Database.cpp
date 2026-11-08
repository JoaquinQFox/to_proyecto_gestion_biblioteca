#include "Database.h"

#include <QSqlError>
#include <QDir>
#include <QDebug>
#include <QCoreApplication>
#include <QSqlQuery>

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

    QSqlQuery query(conexion);
    if (!query.exec("PRAGMA foreign_keys = ON")) {
        qDebug() << "Error al activar claves foráneas:"
                 << query.lastError().text();
        return false;
    }

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