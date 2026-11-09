#include "Ejemplar.h"

Ejemplar::Ejemplar()
    : id(0), idLibro(0), codigo(""), estado("Disponible") {}

Ejemplar::Ejemplar(int id, int idLibro, const QString &codigo, const QString &estado)
    : id(id), idLibro(idLibro), codigo(codigo), estado(estado) {}

int Ejemplar::getId() const
{
    return id;
}

void Ejemplar::setId(int id)
{
    this->id = id;
}

int Ejemplar::getIdLibro() const
{
    return idLibro;
}

void Ejemplar::setIdLibro(int idLibro)
{
    this->idLibro = idLibro;
}

QString Ejemplar::getCodigo() const
{
    return codigo;
}

void Ejemplar::setCodigo(const QString &codigo)
{
    this->codigo = codigo;
}

QString Ejemplar::getEstado() const
{
    return estado;
}

void Ejemplar::setEstado(const QString &estado)
{
    this->estado = estado;
}