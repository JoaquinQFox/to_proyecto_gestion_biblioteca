#include "Lector.h"

Lector::Lector()
    : id(0), dni(""), nombres(""), apellidos(""), telefono(""), direccion(""), activo(true) {}

Lector::Lector(int id, const QString &dni, const QString &nombres, const QString &apellidos,
               const QString &telefono, const QString &direccion, bool activo)
    : id(id), dni(dni), nombres(nombres), apellidos(apellidos),
    telefono(telefono), direccion(direccion), activo(activo) {}

int Lector::getId() const {
    return id;
}

void Lector::setId(int id) {
    this->id = id;
}

QString Lector::getDni() const {
    return dni;
}

void Lector::setDni(const QString &dni) {
    this->dni = dni;
}

QString Lector::getNombres() const {
    return nombres;
}

void Lector::setNombres(const QString &nombres) {
    this->nombres = nombres;
}

QString Lector::getApellidos() const {
    return apellidos;
}

void Lector::setApellidos(const QString &apellidos) {
    this->apellidos = apellidos;
}

QString Lector::getTelefono() const {
    return telefono;
}

void Lector::setTelefono(const QString &telefono) {
    this->telefono = telefono;
}

QString Lector::getDireccion() const {
    return direccion;
}

void Lector::setDireccion(const QString &direccion) {
    this->direccion = direccion;
}

bool Lector::isActivo() const {
    return activo;
}

void Lector::setActivo(bool activo) {
    this->activo = activo;
}