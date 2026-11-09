#include "DatabaseInitializer.h"
#include "Database.h"

#include <QSqlDatabase>
#include <QSqlQuery>

bool DatabaseInitializer::incializar()
{
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
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                nombre TEXT NOT NULL
            )
        )",

        // Tabla: LIBROS
        R"(
            CREATE TABLE IF NOT EXISTS libros (
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                id_categoria INTEGER NOT NULL,
                titulo TEXT NOT NULL,
                autor TEXT NOT NULL,
                year INTEGER,
                editorial TEXT,
                isbn TEXT UNIQUE,
                activo INTEGER NOT NULL DEFAULT 1
                    CHECK (activo IN (0, 1)),

                FOREIGN KEY (id_categoria)
                    REFERENCES categorias(id)
                    ON DELETE RESTRICT
                    ON UPDATE CASCADE
            )
        )",

       // Tabla: EJEMPLARES
       R"(
            CREATE TABLE IF NOT EXISTS ejemplares (
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                id_libro INTEGER NOT NULL,
                codigo TEXT NOT NULL UNIQUE,
                estado TEXT NOT NULL
                    CHECK (estado IN (
                        'Disponible',
                        'Prestado',
                        'Dañado',
                        'Perdido',
                        'En mantenimiento',
                        'Retirado'
                    )),

                FOREIGN KEY (id_libro)
                    REFERENCES libros(id)
                    ON DELETE RESTRICT
                    ON UPDATE CASCADE
            )
        )",

       // Tabla: LECTORES
       R"(
            CREATE TABLE IF NOT EXISTS lectores (
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                dni TEXT NOT NULL UNIQUE,
                nombres TEXT NOT NULL,
                apellidos TEXT NOT NULL,
                telefono TEXT,
                direccion TEXT,
                activo INTEGER NOT NULL DEFAULT 1
                    CHECK (activo IN (0, 1))
            )
        )",

       // Tabla: PRESTAMOS
       R"(
            CREATE TABLE IF NOT EXISTS prestamos (
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                id_ejemplar INTEGER NOT NULL,
                id_lector INTEGER NOT NULL,
                fecha_prestamo TEXT NOT NULL,
                fecha_vencimiento TEXT NOT NULL,
                fecha_devolucion TEXT,

                FOREIGN KEY (id_ejemplar)
                    REFERENCES ejemplares(id)
                    ON DELETE RESTRICT
                    ON UPDATE CASCADE,

                FOREIGN KEY (id_lector)
                    REFERENCES lectores(id)
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
