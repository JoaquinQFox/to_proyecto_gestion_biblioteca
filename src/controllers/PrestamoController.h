#ifndef PRESTAMOCONTROLLER_H
#define PRESTAMOCONTROLLER_H

#include <QObject>
#include <QList>
#include <QDate>
#include "../models/Prestamo.h"
#include "../repositories/PrestamoRepository.h"
#include "../repositories/EjemplarRepository.h"
#include "../repositories/LectorRepository.h"
#include "../repositories/LibroRepository.h"

class PrestamoController : public QObject
{
    Q_OBJECT

public:
    explicit PrestamoController(QObject *parent = nullptr);

    // Operaciones principales (lanzan std::invalid_argument o std::logic_error si hay error)
    bool registrarPrestamo(int idEjemplar, int idLector, const QDate &fechaPrestamo, const QDate &fechaVencimiento);
    bool registrarDevolucion(int idPrestamo, const QDate &fechaDevolucion = QDate::currentDate());

    // Consultas para las vistas y el Dashboard
    QList<Prestamo> obtenerTodos();
    QList<Prestamo> obtenerActivos();
    QList<Prestamo> obtenerVencidos();
    Prestamo obtenerPorId(int id);

    // Métricas rápidas para InicioPage (Dashboard)
    int contarPrestamosActivos();
    int contarPrestamosVencidos();
    int contarTotalLibros();
    int contarTotalEjemplares();
    int contarTotalLectores();

signals:
    // Señales para actualizar PrestamosPage e InicioPage en tiempo real
    void prestamoRegistrado();
    void prestamoDevuelto();
    void datosActualizados();

private:
    PrestamoRepository m_prestamoRepo;
    EjemplarRepository m_ejemplarRepo;
    LectorRepository m_lectorRepo;
    LibroRepository m_libroRepo;
};

#endif // PRESTAMOCONTROLLER_H
