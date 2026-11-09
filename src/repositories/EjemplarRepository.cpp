#include "EjemplarRepository.h"
#include "../database/Database.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <stdexcept>

int EjemplarRepository::agregar(const Ejemplar &entidad)
{
    QSqlQuery query(Database::obtenerConexion());
    query.prepare("INSERT INTO ejemplares (id_libro, codigo, estado) "
                  "VALUES (:idLibro, :codigo, :estado)");
    query.bindValue(":idLibro", entidad.getIdLibro());
    query.bindValue(":codigo", entidad.getCodigo());
    query.bindValue(":estado", entidad.getEstado());

    if (!query.exec()) {
        throw std::runtime_error("Error al agregar ejemplar: " + query.lastError().text().toStdString());
    }
    return query.lastInsertId().toInt();
}

Ejemplar EjemplarRepository::obtenerPorId(int id)
{
    QSqlQuery query(Database::obtenerConexion());
    query.prepare("SELECT id, id_libro, codigo, estado FROM ejemplares WHERE id = :id");
    query.bindValue(":id", id);

    if (!query.exec()) {
        throw std::runtime_error("Error al obtener ejemplar por ID: " + query.lastError().text().toStdString());
    }

    if (query.next()) {
        return Ejemplar(
            query.value("id").toInt(),
            query.value("id_libro").toInt(),
            query.value("codigo").toString(),
            query.value("estado").toString()
            );
    }
    throw std::runtime_error("Ejemplar no encontrado con ID: " + std::to_string(id));
}

QList<Ejemplar> EjemplarRepository::obtenerTodos() {
    QList<Ejemplar> lista;
    QSqlQuery query(Database::obtenerConexion());
    query.prepare("SELECT id, id_libro, codigo, estado FROM ejemplares");

    if (!query.exec()) {
        throw std::runtime_error("Error al obtener ejemplares: " + query.lastError().text().toStdString());
    }

    while (query.next()) {
        lista.append(Ejemplar(
            query.value("id").toInt(),
            query.value("id_libro").toInt(),
            query.value("codigo").toString(),
            query.value("estado").toString()
            ));
    }
    return lista;
}

bool EjemplarRepository::actualizar(const Ejemplar &entidad)
{
    QSqlQuery query(Database::obtenerConexion());
    query.prepare("UPDATE ejemplares SET id_libro = :idLibro, codigo = :codigo, estado = :estado "
                  "WHERE id = :id");
    query.bindValue(":idLibro", entidad.getIdLibro());
    query.bindValue(":codigo", entidad.getCodigo());
    query.bindValue(":estado", entidad.getEstado());
    query.bindValue(":id", entidad.getId());

    if (!query.exec()) {
        throw std::runtime_error("Error al actualizar ejemplar: " + query.lastError().text().toStdString());
    }
    return query.numRowsAffected() > 0;
}

bool EjemplarRepository::eliminar(int id)
{
    QSqlQuery query(Database::obtenerConexion());
    query.prepare("DELETE FROM ejemplares WHERE id = :id");
    query.bindValue(":id", id);

    if (!query.exec()) {
        throw std::runtime_error("Error al eliminar ejemplar: " + query.lastError().text().toStdString());
    }
    return query.numRowsAffected() > 0;
}