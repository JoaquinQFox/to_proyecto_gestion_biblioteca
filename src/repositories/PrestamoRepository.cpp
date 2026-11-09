#include "PrestamoRepository.h"
#include "../database/Database.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <stdexcept>

int PrestamoRepository::agregar(const Prestamo &entidad)
{
    QSqlQuery query(Database::obtenerConexion());
    query.prepare("INSERT INTO prestamos (id_ejemplar, id_lector, fecha_prestamo, fecha_vencimiento, fecha_devolucion) "
                  "VALUES (:idEjemplar, :idLector, :fechaPrestamo, :fechaVencimiento, :fechaDevolucion)");
    query.bindValue(":idEjemplar", entidad.getIdEjemplar());
    query.bindValue(":idLector", entidad.getIdLector());
    query.bindValue(":fechaPrestamo", entidad.getFechaPrestamo().toString(Qt::ISODate));
    query.bindValue(":fechaVencimiento", entidad.getFechaVencimiento().toString(Qt::ISODate));

    if (entidad.getFechaDevolucion().isValid()) {
        query.bindValue(":fechaDevolucion", entidad.getFechaDevolucion().toString(Qt::ISODate));
    } else {
        query.bindValue(":fechaDevolucion", QVariant());
    }

    if (!query.exec()) {
        throw std::runtime_error("Error al agregar prestamo: " + query.lastError().text().toStdString());
    }
    return query.lastInsertId().toInt();
}

Prestamo PrestamoRepository::obtenerPorId(int id)
{
    QSqlQuery query(Database::obtenerConexion());
    query.prepare("SELECT id, id_ejemplar, id_lector, fecha_prestamo, fecha_vencimiento, fecha_devolucion "
                  "FROM prestamos WHERE id = :id");
    query.bindValue(":id", id);

    if (!query.exec()) {
        throw std::runtime_error("Error al obtener prestamo por ID: " + query.lastError().text().toStdString());
    }

    if (query.next()) {
        QString fechaDevStr = query.value("fecha_devolucion").toString();
        QDate fechaDevolucion = fechaDevStr.isEmpty()
                                    ? QDate()
                                    : QDate::fromString(fechaDevStr, Qt::ISODate);

        return Prestamo(
            query.value("id").toInt(),
            query.value("id_ejemplar").toInt(),
            query.value("id_lector").toInt(),
            QDate::fromString(query.value("fecha_prestamo").toString(), Qt::ISODate),
            QDate::fromString(query.value("fecha_vencimiento").toString(), Qt::ISODate),
            fechaDevolucion
            );
    }
    throw std::runtime_error("Prestamo no encontrado con ID: " + std::to_string(id));
}

QList<Prestamo> PrestamoRepository::obtenerTodos()
{
    QList<Prestamo> lista;
    QSqlQuery query(Database::obtenerConexion());
    query.prepare("SELECT id, id_ejemplar, id_lector, fecha_prestamo, fecha_vencimiento, fecha_devolucion FROM prestamos");

    if (!query.exec()) {
        throw std::runtime_error("Error al obtener prestamos: " + query.lastError().text().toStdString());
    }

    while (query.next()) {
        QString fechaDevStr = query.value("fecha_devolucion").toString();
        QDate fechaDevolucion = fechaDevStr.isEmpty()
                                    ? QDate()
                                    : QDate::fromString(fechaDevStr, Qt::ISODate);

        lista.append(Prestamo(
            query.value("id").toInt(),
            query.value("id_ejemplar").toInt(),
            query.value("id_lector").toInt(),
            QDate::fromString(query.value("fecha_prestamo").toString(), Qt::ISODate),
            QDate::fromString(query.value("fecha_vencimiento").toString(), Qt::ISODate),
            fechaDevolucion
            ));
    }
    return lista;
}

bool PrestamoRepository::actualizar(const Prestamo &entidad)
{
    QSqlQuery query(Database::obtenerConexion());
    query.prepare("UPDATE prestamos SET id_ejemplar = :idEjemplar, id_lector = :idLector, "
                  "fecha_prestamo = :fechaPrestamo, fecha_vencimiento = :fechaVencimiento, "
                  "fecha_devolucion = :fechaDevolucion WHERE id = :id");
    query.bindValue(":idEjemplar", entidad.getIdEjemplar());
    query.bindValue(":idLector", entidad.getIdLector());
    query.bindValue(":fechaPrestamo", entidad.getFechaPrestamo().toString(Qt::ISODate));
    query.bindValue(":fechaVencimiento", entidad.getFechaVencimiento().toString(Qt::ISODate));

    if (entidad.getFechaDevolucion().isValid()) {
        query.bindValue(":fechaDevolucion", entidad.getFechaDevolucion().toString(Qt::ISODate));
    } else {
        query.bindValue(":fechaDevolucion", QVariant());
    }

    query.bindValue(":id", entidad.getId());

    if (!query.exec()) {
        throw std::runtime_error("Error al actualizar prestamo: " + query.lastError().text().toStdString());
    }
    return query.numRowsAffected() > 0;
}

bool PrestamoRepository::eliminar(int id)
{
    QSqlQuery query(Database::obtenerConexion());
    query.prepare("DELETE FROM prestamos WHERE id = :id");
    query.bindValue(":id", id);

    if (!query.exec()) {
        throw std::runtime_error("Error al eliminar prestamo: " + query.lastError().text().toStdString());
    }
    return query.numRowsAffected() > 0;
}