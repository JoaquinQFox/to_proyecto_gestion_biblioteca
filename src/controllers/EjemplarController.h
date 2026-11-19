#ifndef EJEMPLARCONTROLLER_H
#define EJEMPLARCONTROLLER_H

#include "../models/Ejemplar.h"
#include "../repositories/EjemplarRepository.h"
#include "../repositories/LibroRepository.h"

#include <QList>
#include <QString>

class EjemplarController
{
public:
    QList<Ejemplar> listar();
    Ejemplar obtenerPorId(int id);
    int crear(int idLibro, const QString &codigo, const QString &estado);
    void actualizar(int id, int idLibro, const QString &codigo,
                    const QString &estado);
    void eliminar(int id);

private:
    EjemplarRepository repository;
    LibroRepository libroRepository;

    Ejemplar::Estado validarCampos(int idLibro, const QString &codigo,
                                   const QString &estado, int idExcluido = 0);
};

#endif // EJEMPLARCONTROLLER_H
