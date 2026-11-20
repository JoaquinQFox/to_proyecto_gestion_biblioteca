#include "Database.h"

#include <QSqlError>
#include <QDir>
#include <QDebug>
#include <QCoreApplication>
#include <QSqlQuery>
#include <QThread>

QSqlDatabase Database::conexion;
QString Database::rutaBD;

static thread_local QString conexionHilo;

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

    rutaBD = ruta;
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

bool Database::conectarEnMemoria()
{
    const QString nombre = "BibliotecaConnection";

    if (QSqlDatabase::contains(nombre)) {
        conexion.close();
        conexion = QSqlDatabase();
        QSqlDatabase::removeDatabase(nombre);
    }

    conexion = QSqlDatabase::addDatabase("QSQLITE", nombre);
    conexion.setDatabaseName(":memory:");
    rutaBD.clear();

    if (!conexion.open()) {
        qDebug() << "Error al conectar con la base de datos en memoria:"
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
    if (!conexionHilo.isEmpty()) {
        return QSqlDatabase::database(conexionHilo);
    }
    return conexion;
}

void Database::cerrarConexion()
{
    if (conexion.isOpen()) {
        conexion.close();
    }
}

bool Database::abrirConexionHilo()
{
    if (rutaBD.isEmpty()) {
        return false;
    }

    const QString nombre = QString("BibliotecaHilo_%1")
                               .arg(reinterpret_cast<quintptr>(QThread::currentThreadId()));

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", nombre);
    db.setDatabaseName(rutaBD);

    if (!db.open()) {
        qDebug() << "Error al conectar en hilo:" << db.lastError().text();
        db = QSqlDatabase();
        QSqlDatabase::removeDatabase(nombre);
        return false;
    }

    conexionHilo = nombre;
    return true;
}

void Database::cerrarConexionHilo()
{
    if (conexionHilo.isEmpty()) {
        return;
    }

    const QString nombre = conexionHilo;
    conexionHilo.clear();

    {
        QSqlDatabase db = QSqlDatabase::database(nombre, false);
        if (db.isOpen()) {
            db.close();
        }
    }
    QSqlDatabase::removeDatabase(nombre);
}
