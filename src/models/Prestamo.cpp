#include "Prestamo.h"

Prestamo::Prestamo()
    : id(0), idEjemplar(0), idLector(0),
    fechaPrestamo(QDate()), fechaVencimiento(QDate()), fechaDevolucion(QDate()) {}

Prestamo::Prestamo(int id, int idEjemplar, int idLector, const QDate &fechaPrestamo,
                   const QDate &fechaVencimiento, const QDate &fechaDevolucion)
    : id(id), idEjemplar(idEjemplar), idLector(idLector),
    fechaPrestamo(fechaPrestamo), fechaVencimiento(fechaVencimiento), fechaDevolucion(fechaDevolucion) {}

int Prestamo::getId() const {
    return id;
}

void Prestamo::setId(int id) {
    this->id = id;
}

int Prestamo::getIdEjemplar() const {
    return idEjemplar;
}

void Prestamo::setIdEjemplar(int idEjemplar) {
    this->idEjemplar = idEjemplar;
}

int Prestamo::getIdLector() const {
    return idLector;
}

void Prestamo::setIdLector(int idLector) {
    this->idLector = idLector;
}

QDate Prestamo::getFechaPrestamo() const {
    return fechaPrestamo;
}

void Prestamo::setFechaPrestamo(const QDate &fechaPrestamo) {
    this->fechaPrestamo = fechaPrestamo;
}

QDate Prestamo::getFechaVencimiento() const {
    return fechaVencimiento;
}

void Prestamo::setFechaVencimiento(const QDate &fechaVencimiento) {
    this->fechaVencimiento = fechaVencimiento;
}

QDate Prestamo::getFechaDevolucion() const {
    return fechaDevolucion;
}

void Prestamo::setFechaDevolucion(const QDate &fechaDevolucion) {
    this->fechaDevolucion = fechaDevolucion;
}