#ifndef CATEGORIAREPOSITORY_H
#define CATEGORIAREPOSITORY_H

#include "../interfaces/IRepository.h"
#include "../models/Categoria.h"

class CategoriaRepository : public IRepository<Categoria>
{
public:
    bool agregar(const Categoria& entidad) override;

    bool obtenerPorId(int id, Categoria& entidad) override;

    QList<Categoria> obtenerTodos() override;

    bool actualizar(const Categoria& entidad) override;

    bool eliminar(int id) override;
};

#endif // CATEGORIAREPOSITORY_H
