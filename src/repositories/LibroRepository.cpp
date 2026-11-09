#include "LibroRepository.h"
#include "../database/Database.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <stdexcept>

int LibroRepository::agregar(const Libro &entidad)
{
    QSqlQuery query(Database::obtenerConexion());
    query.prepare("INSERT INTO libros (id_categoria, titulo, autor, year, editorial, isbn, activo) "
                  "VALUES (:idCategoria, :titulo, :autor, :year, :editorial, :isbn, :activo)");
    query.bindValue(":idCategoria", entidad.getIdCategoria());
    query.bindValue(":titulo", entidad.getTitulo());
    query.bindValue(":autor", entidad.getAutor());
    query.bindValue(":year", entidad.getYear());
    query.bindValue(":editorial", entidad.getEditorial());
    query.bindValue(":isbn", entidad.getIsbn());
    query.bindValue(":activo", entidad.isActivo() ? 1 : 0);

    if (!query.exec()) {
        throw std::runtime_error("Error al agregar libro: " + query.lastError().text().toStdString());
    }
    return query.lastInsertId().toInt();
}

Libro LibroRepository::obtenerPorId(int id)
{
    QSqlQuery query(Database::obtenerConexion());
    query.prepare("SELECT id, id_categoria, titulo, autor, year, editorial, isbn, activo "
                  "FROM libros WHERE id = :id");
    query.bindValue(":id", id);

    if (!query.exec()) {
        throw std::runtime_error("Error al obtener libro por ID: " + query.lastError().text().toStdString());
    }

    if (query.next()) {
        return Libro(
            query.value("id").toInt(),
            query.value("id_categoria").toInt(),
            query.value("titulo").toString(),
            query.value("autor").toString(),
            query.value("year").toInt(),
            query.value("editorial").toString(),
            query.value("isbn").toString(),
            query.value("activo").toInt() == 1
            );
    }
    throw std::runtime_error("Libro no encontrado con ID: " + std::to_string(id));
}

QList<Libro> LibroRepository::obtenerTodos() {
    QList<Libro> lista;
    QSqlQuery query(Database::obtenerConexion());
    query.prepare("SELECT id, id_categoria, titulo, autor, year, editorial, isbn, activo FROM libros");

    if (!query.exec()) {
        throw std::runtime_error("Error al obtener libros: " + query.lastError().text().toStdString());
    }

    while (query.next()) {
        lista.append(Libro(
            query.value("id").toInt(),
            query.value("id_categoria").toInt(),
            query.value("titulo").toString(),
            query.value("autor").toString(),
            query.value("year").toInt(),
            query.value("editorial").toString(),
            query.value("isbn").toString(),
            query.value("activo").toInt() == 1
            ));
    }
    return lista;
}

bool LibroRepository::actualizar(const Libro &entidad)
{
    QSqlQuery query(Database::obtenerConexion());
    query.prepare("UPDATE libros SET id_categoria = :idCategoria, titulo = :titulo, autor = :autor, "
                  "year = :year, editorial = :editorial, isbn = :isbn, activo = :activo "
                  "WHERE id = :id");
    query.bindValue(":idCategoria", entidad.getIdCategoria());
    query.bindValue(":titulo", entidad.getTitulo());
    query.bindValue(":autor", entidad.getAutor());
    query.bindValue(":year", entidad.getYear());
    query.bindValue(":editorial", entidad.getEditorial());
    query.bindValue(":isbn", entidad.getIsbn());
    query.bindValue(":activo", entidad.isActivo() ? 1 : 0);
    query.bindValue(":id", entidad.getId());

    if (!query.exec()) {
        throw std::runtime_error("Error al actualizar libro: " + query.lastError().text().toStdString());
    }
    return query.numRowsAffected() > 0;
}

bool LibroRepository::eliminar(int id)
{
    QSqlQuery query(Database::obtenerConexion());
    query.prepare("UPDATE libros SET activo = 0 WHERE id = :id");
    query.bindValue(":id", id);

    if (!query.exec()) {
        throw std::runtime_error("Error al desactivar libro: " + query.lastError().text().toStdString());
    }
    return query.numRowsAffected() > 0;
}

QList<Libro> LibroRepository::obtenerActivos()
{
    QList<Libro> lista;
    QSqlQuery query(Database::obtenerConexion());
    query.prepare("SELECT id, id_categoria, titulo, autor, year, editorial, isbn, activo "
                  "FROM libros WHERE activo = 1");

    if (!query.exec()) {
        throw std::runtime_error("Error al obtener libros activos: " + query.lastError().text().toStdString());
    }

    while (query.next()) {
        lista.append(Libro(
            query.value("id").toInt(),
            query.value("id_categoria").toInt(),
            query.value("titulo").toString(),
            query.value("autor").toString(),
            query.value("year").toInt(),
            query.value("editorial").toString(),
            query.value("isbn").toString(),
            true
            ));
    }
    return lista;
}