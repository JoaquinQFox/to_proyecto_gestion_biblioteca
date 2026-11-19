#ifndef LECTORCONTROLLER_H
#define LECTORCONTROLLER_H

#include "../models/Lector.h"
#include "../repositories/LectorRepository.h"

#include <QList>
#include <QString>

class LectorController
{
public:
    QList<Lector> listar();
    Lector obtenerPorId(int id);
    int crear(const QString &nombreCompleto, const QString &dni,
              const QString &correo, const QString &telefono);
    void actualizar(int id, const QString &nombreCompleto, const QString &dni,
                    const QString &correo, const QString &telefono);
    void eliminar(int id);

private:
    LectorRepository repository;

    void validarDni(const QString &dni, int idExcluido = 0);
    void validarDatos(const QString &nombreCompleto, const QString &correo,
                      const QString &telefono);
};

#endif // LECTORCONTROLLER_H
