#include "PrestamoController.h"
#include "../models/Lector.h"
#include "../models/Ejemplar.h"
#include "../models/Libro.h"
#include <stdexcept>

PrestamoController::PrestamoController(QObject *parent)
    : QObject(parent)
{
}

bool PrestamoController::registrarPrestamo(int idEjemplar, int idLector, const QDate &fechaPrestamo, const QDate &fechaVencimiento)
{
    // 1. Validaciones básicas de parámetros
    if (idEjemplar <= 0) {
        throw std::invalid_argument("Debe seleccionar un ejemplar válido.");
    }
    if (idLector <= 0) {
        throw std::invalid_argument("Debe seleccionar un lector válido.");
    }
    if (!fechaPrestamo.isValid() || !fechaVencimiento.isValid()) {
        throw std::invalid_argument("Las fechas de préstamo y vencimiento deben ser válidas.");
    }
    if (fechaVencimiento < fechaPrestamo) {
        throw std::invalid_argument("La fecha de vencimiento no puede ser anterior a la fecha de préstamo.");
    }

    // 2. Validar existencia y estado del Lector
    Lector lector = m_lectorRepo.obtenerPorId(idLector);
    if (lector.getId() <= 0) {
        throw std::logic_error("El lector seleccionado no existe en el sistema.");
    }
    if (!lector.isActivo()) {
        throw std::logic_error("El lector se encuentra inactivo y no puede solicitar préstamos.");
    }

    // 3. Validar existencia y disponibilidad del Ejemplar
    Ejemplar ejemplar = m_ejemplarRepo.obtenerPorId(idEjemplar);
    if (ejemplar.getId() <= 0) {
        throw std::logic_error("El ejemplar seleccionado no existe.");
    }
    if (ejemplar.getEstado() != Ejemplar::Estado::DISPONIBLE) {
        throw std::logic_error("El ejemplar no está disponible para préstamo (Estado actual: " + ejemplar.getEstadoString().toStdString() + ").");
    }

    // 4. Validar que el Libro asociado esté activo
    Libro libro = m_libroRepo.obtenerPorId(ejemplar.getIdLibro());
    if (libro.getId() > 0 && !libro.isActivo()) {
        throw std::logic_error("El libro asociado a este ejemplar está dado de baja.");
    }

    // 5. Verificar que no exista un préstamo activo duplicado para el mismo ejemplar
    QList<Prestamo> prestamos = m_prestamoRepo.obtenerTodos();
    for (const Prestamo &p : prestamos) {
        if (p.getIdEjemplar() == idEjemplar && p.getFechaDevolucion().isNull()) {
            throw std::logic_error("Este ejemplar ya tiene un préstamo activo registrado.");
        }
    }

    // 6. Construir el modelo y persistir
    Prestamo nuevoPrestamo;
    nuevoPrestamo.setIdEjemplar(idEjemplar);
    nuevoPrestamo.setIdLector(idLector);
    nuevoPrestamo.setFechaPrestamo(fechaPrestamo);
    nuevoPrestamo.setFechaVencimiento(fechaVencimiento);
    nuevoPrestamo.setFechaDevolucion(QDate()); // Nula mientras esté activo

    if (m_prestamoRepo.agregar(nuevoPrestamo) <= 0) {
        throw std::runtime_error("Error en la base de datos al intentar registrar el préstamo.");
    }

    // 7. Notificar a las vistas en tiempo real
    emit prestamoRegistrado();
    emit datosActualizados();
    return true;
}

bool PrestamoController::registrarDevolucion(int idPrestamo, const QDate &fechaDevolucion)
{
    if (idPrestamo <= 0) {
        throw std::invalid_argument("ID de préstamo inválido.");
    }
    if (!fechaDevolucion.isValid()) {
        throw std::invalid_argument("La fecha de devolución no es válida.");
    }

    Prestamo prestamo = m_prestamoRepo.obtenerPorId(idPrestamo);
    if (prestamo.getId() <= 0) {
        throw std::logic_error("No se encontró el préstamo especificado.");
    }
    if (!prestamo.getFechaDevolucion().isNull()) {
        throw std::logic_error("Este préstamo ya fue devuelto anteriormente.");
    }

    prestamo.setFechaDevolucion(fechaDevolucion);

    // Actualizar préstamo en la base de datos
    if (!m_prestamoRepo.actualizar(prestamo)) {
        throw std::runtime_error("No se pudo registrar la devolución en la base de datos.");
    }

    // Notificar a las vistas en tiempo real
    emit prestamoDevuelto();
    emit datosActualizados();
    return true;
}

QList<Prestamo> PrestamoController::obtenerTodos()
{
    return m_prestamoRepo.obtenerTodos();
}

QList<Prestamo> PrestamoController::obtenerActivos()
{
    QList<Prestamo> activos;
    const QList<Prestamo> todos = m_prestamoRepo.obtenerTodos();
    for (const Prestamo &p : todos) {
        if (p.getFechaDevolucion().isNull()) {
            activos.append(p);
        }
    }
    return activos;
}

QList<Prestamo> PrestamoController::obtenerVencidos()
{
    QList<Prestamo> vencidos;
    const QList<Prestamo> todos = m_prestamoRepo.obtenerTodos();
    const QDate hoy = QDate::currentDate();
    for (const Prestamo &p : todos) {
        if (p.getFechaDevolucion().isNull() && p.getFechaVencimiento() < hoy) {
            vencidos.append(p);
        }
    }
    return vencidos;
}

Prestamo PrestamoController::obtenerPorId(int id)
{
    if (id <= 0) {
        throw std::invalid_argument("El ID del préstamo no es válido.");
    }
    return m_prestamoRepo.obtenerPorId(id);
}

int PrestamoController::contarPrestamosActivos()
{
    return obtenerActivos().size();
}

int PrestamoController::contarPrestamosVencidos()
{
    return obtenerVencidos().size();
}

int PrestamoController::contarTotalLibros()
{
    return m_libroRepo.obtenerTodos().size();
}

int PrestamoController::contarTotalEjemplares()
{
    return m_ejemplarRepo.obtenerTodos().size();
}


int PrestamoController::contarTotalLectores()
{
    return m_lectorRepo.obtenerTodos().size();
}
