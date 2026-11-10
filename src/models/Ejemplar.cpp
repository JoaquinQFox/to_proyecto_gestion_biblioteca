#include "Ejemplar.h"

Ejemplar::Ejemplar()
    : id(0), idLibro(0), codigo(""), estado(Estado::DISPONIBLE) {}

Ejemplar::Ejemplar(int id, int idLibro, const QString &codigo, const Estado &estado)
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

Ejemplar::Estado Ejemplar::getEstado() const
{
    return estado;
}

void Ejemplar::setEstado(const Estado &estado)
{
    this->estado = estado;
}

QString Ejemplar::getEstadoString() const
{
    switch(this->estado) {
    case Estado::DISPONIBLE:
        return "DISPONIBLE";
    case Estado::PRESTADO:
        return "PRESTADO";
    case Estado::RETIRADO:
        return "RETIRADO";
    }

    return "DESCONOCIDO";
}

Ejemplar::Estado Ejemplar::stringToEstado(const QString &estado)
{
    if (estado == "DISPONIBLE")
        return Estado::DISPONIBLE;

    if (estado == "PRESTADO")
        return Estado::PRESTADO;

    if (estado == "RETIRADO")
        return Estado::PRESTADO;

    throw std::invalid_argument("Estado de ejemplar no valido");
}

QString Ejemplar::estadoToString(const Estado &estado) {
    switch(estado) {
    case Estado::DISPONIBLE:
        return "DISPONIBLE";
    case Estado::PRESTADO:
        return "PRESTADO";
    case Estado::RETIRADO:
        return "RETIRADO";
    }

    return "DESCONOCIDO";
}

bool Ejemplar::operator==(const Ejemplar &otro) const
{
    return id == otro.id &&
           idLibro == otro.idLibro &&
           codigo == otro.codigo &&
           estado == otro.estado;
}