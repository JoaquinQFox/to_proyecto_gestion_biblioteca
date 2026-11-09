#ifndef EJEMPLARREPOSITORY_H
#define EJEMPLARREPOSITORY_H

#include "../interfaces/IRepository.h"
#include "../models/Ejemplar.h"

class EjemplarRepository : public IRepository<Ejemplar>
{
public:
    EjemplarRepository() = default;
    ~EjemplarRepository() override = default;

    int agregar(const Ejemplar &entidad) override;
    Ejemplar obtenerPorId(int id) override;
    QList<Ejemplar> obtenerTodos() override;
    bool actualizar(const Ejemplar &entidad) override;
    bool eliminar(int id) override;
};

#endif // EJEMPLARREPOSITORY_H