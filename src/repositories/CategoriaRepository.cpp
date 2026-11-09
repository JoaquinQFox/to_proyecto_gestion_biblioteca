#include "CategoriaRepository.h"
#include "../database/Database.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <stdexcept>

int CategoriaRepository::agregar(const Categoria &entidad)
{
    QSqlQuery query(Database::obtenerConexion());
    query.prepare("INSERT INTO categorias (nombre) VALUES (:nombre)");
    query.bindValue(":nombre", entidad.getNombre());

    if (!query.exec()) {
        throw std::runtime_error("Error al agregar categoria: " + query.lastError().text().toStdString());
    }
    return query.lastInsertId().toInt();
}

Categoria CategoriaRepository::obtenerPorId(int id)
{
    QSqlQuery query(Database::obtenerConexion());
    query.prepare("SELECT id, nombre FROM categorias WHERE id = :id");
    query.bindValue(":id", id);

    if (!query.exec()) {
        throw std::runtime_error("Error al obtener categoria por ID: " + query.lastError().text().toStdString());
    }

    if (query.next()) {
        return Categoria(
            query.value("id").toInt(),
            query.value("nombre").toString()
            );
    }
    throw std::runtime_error("Categoria no encontrada con ID: " + std::to_string(id));
}

QList<Categoria> CategoriaRepository::obtenerTodos()
{
    QList<Categoria> lista;
    QSqlQuery query(Database::obtenerConexion());
    query.prepare("SELECT id, nombre FROM categorias");

    if (!query.exec()) {
        throw std::runtime_error("Error al obtener categorias: " + query.lastError().text().toStdString());
    }

    while (query.next()) {
        lista.append(Categoria(
            query.value("id").toInt(),
            query.value("nombre").toString()
        ));
    }
    return lista;
}

bool CategoriaRepository::actualizar(const Categoria &entidad)
{
    QSqlQuery query(Database::obtenerConexion());
    query.prepare("UPDATE categorias SET nombre = :nombre WHERE id = :id");
    query.bindValue(":nombre", entidad.getNombre());
    query.bindValue(":id", entidad.getId());

    if (!query.exec()) {
        throw std::runtime_error("Error al actualizar categoria: " + query.lastError().text().toStdString());
    }
    return query.numRowsAffected() > 0;
}

bool CategoriaRepository::eliminar(int id)
{
    QSqlQuery query(Database::obtenerConexion());
    query.prepare("DELETE FROM categorias WHERE id = :id");
    query.bindValue(":id", id);

    if (!query.exec()) {
        throw std::runtime_error("Error al eliminar categoria: " + query.lastError().text().toStdString());
    }
    return query.numRowsAffected() > 0;
}