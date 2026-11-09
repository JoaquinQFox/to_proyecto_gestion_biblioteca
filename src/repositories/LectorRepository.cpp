#include "LectorRepository.h"
#include "../database/Database.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <stdexcept>

int LectorRepository::agregar(const Lector &entidad)
{
    QSqlQuery query(Database::obtenerConexion());
    query.prepare("INSERT INTO lectores (dni, nombres, apellidos, telefono, direccion, activo) "
                  "VALUES (:dni, :nombres, :apellidos, :telefono, :direccion, :activo)");
    query.bindValue(":dni", entidad.getDni());
    query.bindValue(":nombres", entidad.getNombres());
    query.bindValue(":apellidos", entidad.getApellidos());
    query.bindValue(":telefono", entidad.getTelefono());
    query.bindValue(":direccion", entidad.getDireccion());
    query.bindValue(":activo", entidad.isActivo() ? 1 : 0);

    if (!query.exec()) {
        throw std::runtime_error("Error al agregar lector: " + query.lastError().text().toStdString());
    }
    return query.lastInsertId().toInt();
}

Lector LectorRepository::obtenerPorId(int id)
{
    QSqlQuery query(Database::obtenerConexion());
    query.prepare("SELECT id, dni, nombres, apellidos, telefono, direccion, activo "
                  "FROM lectores WHERE id = :id");
    query.bindValue(":id", id);

    if (!query.exec()) {
        throw std::runtime_error("Error al obtener lector por ID: " + query.lastError().text().toStdString());
    }

    if (query.next()) {
        return Lector(
            query.value("id").toInt(),
            query.value("dni").toString(),
            query.value("nombres").toString(),
            query.value("apellidos").toString(),
            query.value("telefono").toString(),
            query.value("direccion").toString(),
            query.value("activo").toInt() == 1
            );
    }
    throw std::runtime_error("Lector no encontrado con ID: " + std::to_string(id));
}

QList<Lector> LectorRepository::obtenerTodos() {
    QList<Lector> lista;
    QSqlQuery query(Database::obtenerConexion());
    query.prepare("SELECT id, dni, nombres, apellidos, telefono, direccion, activo FROM lectores");

    if (!query.exec()) {
        throw std::runtime_error("Error al obtener lectores: " + query.lastError().text().toStdString());
    }

    while (query.next()) {
        lista.append(Lector(
            query.value("id").toInt(),
            query.value("dni").toString(),
            query.value("nombres").toString(),
            query.value("apellidos").toString(),
            query.value("telefono").toString(),
            query.value("direccion").toString(),
            query.value("activo").toInt() == 1
            ));
    }
    return lista;
}

bool LectorRepository::actualizar(const Lector &entidad)
{
    QSqlQuery query(Database::obtenerConexion());
    query.prepare("UPDATE lectores SET dni = :dni, nombres = :nombres, apellidos = :apellidos, "
                  "telefono = :telefono, direccion = :direccion, activo = :activo "
                  "WHERE id = :id");
    query.bindValue(":dni", entidad.getDni());
    query.bindValue(":nombres", entidad.getNombres());
    query.bindValue(":apellidos", entidad.getApellidos());
    query.bindValue(":telefono", entidad.getTelefono());
    query.bindValue(":direccion", entidad.getDireccion());
    query.bindValue(":activo", entidad.isActivo() ? 1 : 0);
    query.bindValue(":id", entidad.getId());

    if (!query.exec()) {
        throw std::runtime_error("Error al actualizar lector: " + query.lastError().text().toStdString());
    }
    return query.numRowsAffected() > 0;
}

bool LectorRepository::eliminar(int id)
{
    QSqlQuery query(Database::obtenerConexion());
    query.prepare("UPDATE lectores SET activo = 0 WHERE id = :id");
    query.bindValue(":id", id);

    if (!query.exec()) {
        throw std::runtime_error("Error al desactivar lector: " + query.lastError().text().toStdString());
    }
    return query.numRowsAffected() > 0;
}

QList<Lector> LectorRepository::obtenerActivos()
{
    QList<Lector> lista;
    QSqlQuery query(Database::obtenerConexion());
    query.prepare("SELECT id, dni, nombres, apellidos, telefono, direccion, activo "
                  "FROM lectores WHERE activo = 1");

    if (!query.exec()) {
        throw std::runtime_error("Error al obtener lectores activos: " + query.lastError().text().toStdString());
    }

    while (query.next()) {
        lista.append(Lector(
            query.value("id").toInt(),
            query.value("dni").toString(),
            query.value("nombres").toString(),
            query.value("apellidos").toString(),
            query.value("telefono").toString(),
            query.value("direccion").toString(),
            true
            ));
    }
    return lista;
}