#include "Categoria.h"

Categoria::Categoria()
    : id(0), nombre("") {}

Categoria::Categoria(int id, const QString &nombre)
    : id(id), nombre(nombre) {}

int Categoria::getId() const {
    return id;
}

void Categoria::setId(int id) {
    this->id = id;
}

QString Categoria::getNombre() const {
    return nombre;
}
 void Categoria::setNombre(const QString &nombre) {
    this->nombre = nombre;
}