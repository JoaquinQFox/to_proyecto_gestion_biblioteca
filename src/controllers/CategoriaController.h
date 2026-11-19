#ifndef CATEGORIACONTROLLER_H
#define CATEGORIACONTROLLER_H

#include "../models/Categoria.h"
#include "../repositories/CategoriaRepository.h"

#include <QList>
#include <QString>

class CategoriaController
{
public:
    QList<Categoria> listar();
    Categoria obtenerPorId(int id);
    int crear(const QString &nombre);
    void actualizar(int id, const QString &nombre);
    void eliminar(int id);

private:
    CategoriaRepository repository;

    void validarNombre(const QString &nombre, int idExcluido = 0);
};

#endif // CATEGORIACONTROLLER_H
