#ifndef PRESTAMO_H
#define PRESTAMO_H

#include <QDate>

class Prestamo
{
private:
    int id;
    int idEjemplar;
    int idLector;
    QDate fechaPrestamo;
    QDate fechaVencimiento;
    QDate fechaDevolucion;

public:
    Prestamo();
    Prestamo(int id, int idEjemplar, int idLector, const QDate &fechaPrestamo,
             const QDate &fechaVencimiento, const QDate &fechaDevolucion = QDate());

    int getId() const;
    void setId(int id);

    int getIdEjemplar() const;
    void setIdEjemplar(int idEjemplar);

    int getIdLector() const;
    void setIdLector(int idLector);

    QDate getFechaPrestamo() const;
    void setFechaPrestamo(const QDate &fechaPrestamo);

    QDate getFechaVencimiento() const;
    void setFechaVencimiento(const QDate &fechaVencimiento);

    QDate getFechaDevolucion() const;
    void setFechaDevolucion(const QDate &fechaDevolucion);

    bool operator<(const Prestamo &otro) const;
    bool operator==(const Prestamo &otro) const;
};

#endif // PRESTAMO_H