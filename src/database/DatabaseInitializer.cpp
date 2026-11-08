#include "DatabaseInitializer.h"
#include "Database.h"

#include <QSqlDatabase>
#include <QSqlQuery>

bool DatabaseInitializer::incializar() {
    QSqlDatabase db = Database::obtenerConexion();

    if (!db.isOpen()) {
        qDebug() << "La base de datos no esta conectada";
        return false;
    }

    QSqlQuery query(db);

    QStringList sentencias = {
        // Tabla: CATEGORIAS
        R"(
            CREATE TABLE IF NOT EXISTS categorias (
                id_categoria INTEGER PRIMARY KEY AUTOINCREMENT,
                nombre TEXT NOT NULL,
            )
        )",

        // Tabla: LIBROS
        R"(
            CREATE TABLE IF NOT EXISTS libros (
                id_libro INTEGER PRIMARY KEY AUTOINCREMENT,
                id_categoria INTEGER NOT NULL,
                titulo TEXT NOT NULL,
                autor TEXT NOT NULL,
                year INTEGER,
                editorial TEXT,
                isbn TEXT,

                FOREIGN KEY (id_categoria)
                    REFERENCES categorias(id_categoria)
                    ON DELETE RESTRICT
                    ON UPDATE CASCADE
            )
        )",

        // Tabla: EJEMPLARES
        R"(
            CREATE TABLE IF NOT EXISTS ejemplares (
                id_ejemplar INTEGER PRIMARY KEY AUTOINCREMENT,
                id_libro INTEGER NOT NULL,
                codigo TEXT NOT NULL UNIQUE,
                estado TEXT NOT NULL,

                FOREIGN KEY (id_libro)
                    REFERENCES libros(id_libro)
                    ON DELETE RESTRICT
                    ON UPDATE CASCADE
            )
        )",

        // Tabla: LECTORES
        R"(
            CREATE TABLE IF NOT EXISTS lectores (
                id_lector INTEGER PRIMARY KEY AUTOINCREMENT,
                dni TEXT NOT NULL UNIQUE,
                nombres TEXT NOT NULL,
                apellidos TEXT NOT NULL,
                telefono TEXT,
                direccion TEXT
            )
        )",

        // Tabla: PRESTAMOS
        R"(
            CREATE TABLE IF NOT EXISTS prestamos (
                id_prestamo INTEGER PRIMARY KEY AUTOINCREMENT,
                id_ejemplar INTEGER NOT NULL,
                id_lector INTEGER NOT NULL,
                fecha_prestamo TEXT NOT NULL,
                fecha_vencimiento TEXT NOT NULL,
                fecha_devolucion TEXT,

                FOREIGN KEY (id_ejemplar)
                    REFERENCES ejemplares(id_ejemplar)
                    ON DELETE RESTRICT
                    ON UPDATE CASCADE,

                FOREIGN KEY (id_lector)
                    REFERENCES lectores(id_lector)
                    ON DELETE RESTRICT
                    ON UPDATE CASCADE
            )
        )",
    };

    if (!db.transaction()) {
        return false;
    }

    for (const QString &sentencia : sentencias) {
        if (!query.exec(sentencia)) {
            db.rollback();
            return false;
        }
    }

    if (!db.commit()) {
        db.rollback();
        return false;
    }

    return true;
}
