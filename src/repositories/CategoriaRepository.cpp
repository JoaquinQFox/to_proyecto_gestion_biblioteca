#include "CategoriaRepository.h"
#include "../models/Categoria.h"
#include "../database/Database.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QDebug>

bool CategoriaRepository::agregar(const Categoria& categoria)
{
    QSqlQuery query(Database::obtenerConexion());

    query.prepare(
        "INSERT INTO categorias (nombre) "
        "VALUES (:nombre)"
        );

    query.bindValue(":nombre", categoria.getNombre());

    if (!query.exec()) {
        qDebug() << "Error al agregar categoría:"
                 << query.lastError().text();
        return false;
    }

    return true;
}

bool CategoriaRepository::obtenerPorId(int id, Categoria& categoria)
{
    QSqlQuery query(Database::obtenerConexion());

    query.prepare(
        "SELECT id, nombre "
        "FROM categorias "
        "WHERE id = :id"
    );

    query.bindValue(":id", id);

    if (!query.exec()) {
        qDebug() << "Error al obtener categoría:"
                 << query.lastError().text();
        return false;
    }

    if (!query.next())
        return false;

    categoria = Categoria(
        query.value("nombre").toString()
    );

    return true;
}

QList<Categoria> CategoriaRepository::obtenerTodos()
{
    QList<Categoria> categorias;

    QSqlQuery query(Database::obtenerConexion());

    query.prepare(
        "SELECT id, nombre "
        "FROM categorias "
        "ORDER BY nombre"
    );

    if (!query.exec()) {
        qDebug() << "Error al obtener categorías:"
                 << query.lastError().text();
        return categorias;
    }

    while (query.next()) {
        Categoria categoria(
            query.value("id").toInt(),
            query.value("nombre").toString()
        );

        categorias.append(categoria);
    }
    return categorias;
}

bool CategoriaRepository::actualizar(const Categoria& categoria)
{
    QSqlQuery query(Database::obtenerConexion());

    query.prepare(
        "UPDATE categorias "
        "SET nombre = :nombre, "
        "WHERE id = :id"
    );

    query.bindValue(":id", categoria.getId());
    query.bindValue(":nombre", categoria.getNombre());

    if (!query.exec()) {
        qDebug() << "Error al actualizar categoría:"
                 << query.lastError().text();
        return false;
    }

    return query.numRowsAffected() > 0;
}

bool CategoriaRepository::eliminar(int id)
{
    QSqlQuery query(Database::obtenerConexion());

    query.prepare(
        "DELETE FROM categorias "
        "WHERE id = :id"
        );

    query.bindValue(":id", id);

    if (!query.exec()) {
        qDebug() << "Error al desactivar categoría:"
                 << query.lastError().text();
        return false;
    }

    return query.numRowsAffected() > 0;
}