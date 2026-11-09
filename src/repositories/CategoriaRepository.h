#ifndef CATEGORIAREPOSITORY_H
#define CATEGORIAREPOSITORY_H

#include "../interfaces/IRepository.h"
#include "../models/Categoria.h"

class CategoriaRepository : public IRepository<Categoria>
{
public:
    CategoriaRepository() = default;
    ~CategoriaRepository() override = default;

    int agregar(const Categoria &entidad) override;
    Categoria obtenerPorId(int id) override;
    QList<Categoria> obtenerTodos() override;
    bool actualizar(const Categoria &entidad) override;
    bool eliminar(int id) override;
};

#endif // CATEGORIAREPOSITORY_H