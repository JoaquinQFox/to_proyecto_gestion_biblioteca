#include "Libro.h"

Libro::Libro()
    : id(0), idCategoria(0), titulo(""), autor(""), year(0), editorial(""), isbn(""), activo(true) {}

Libro::Libro(int id, int idCategoria, const QString &titulo, const QString &autor,
             int year, const QString &editorial, const QString &isbn, bool activo)
    : id(id), idCategoria(idCategoria), titulo(titulo), autor(autor),
    year(year), editorial(editorial), isbn(isbn), activo(activo) {}

int Libro::getId() const
{
    return id;
}

void Libro::setId(int id)
{
    this->id = id;
}

int Libro::getIdCategoria() const
{
    return idCategoria;
}

void Libro::setIdCategoria(int idCategoria)
{
    this->idCategoria = idCategoria;
}

QString Libro::getTitulo() const
{
    return titulo;
}

void Libro::setTitulo(const QString &titulo)
{
    this->titulo = titulo;
}

QString Libro::getAutor() const
{
    return autor;
}

void Libro::setAutor(const QString &autor)
{
    this->autor = autor;
}

int Libro::getYear() const
{
    return year;
}

void Libro::setYear(int year)
{
    this->year = year;
}

QString Libro::getEditorial() const
{
    return editorial;
}

void Libro::setEditorial(const QString &editorial)
{
    this->editorial = editorial;
}

QString Libro::getIsbn() const
{
    return isbn;
}

void Libro::setIsbn(const QString &isbn)
{
    this->isbn = isbn;
}

bool Libro::isActivo() const
{
    return activo;
}

void Libro::setActivo(bool activo)
{
    this->activo = activo;
}

bool Libro::operator==(const Libro &otro) const
{
    return id == otro.id &&
           idCategoria == otro.idCategoria &&
           titulo == otro.titulo &&
           autor == otro.autor &&
           year == otro.year &&
           editorial == otro.editorial &&
           isbn == otro.isbn &&
           activo == otro.activo;
}