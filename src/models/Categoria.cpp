#include "Categoria.h"

Categoria::Categoria() :
    id(0) {}

Categoria::Categoria(const QString& nombre) :
    nombre(nombre) {}

Categoria::Categoria(int id, const QString& nombre) :
    id(id), nombre(nombre) {}

int Categoria::getId() const
{
    return id;
}

QString Categoria::getNombre() const
{
    return nombre;
}

void Categoria::setNombre(const QString& nombre)
{
    this->nombre = nombre;
}