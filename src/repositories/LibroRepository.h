#ifndef LIBROREPOSITORY_H
#define LIBROREPOSITORY_H

#include "../interfaces/IRepository.h"
#include "../models/Libro.h"

class LibroRepository : public IRepository<Libro>
{
public:
    LibroRepository() = default;
    ~LibroRepository() override = default;

    int agregar(const Libro &entidad) override;
    Libro obtenerPorId(int id) override;
    QList<Libro> obtenerTodos() override;
    bool actualizar(const Libro &entidad) override;
    bool eliminar(int id) override;
};

#endif // LIBROREPOSITORY_H