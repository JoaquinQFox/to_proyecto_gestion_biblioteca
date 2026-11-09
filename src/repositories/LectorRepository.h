#ifndef LECTORREPOSITORY_H
#define LECTORREPOSITORY_H

#include "../interfaces/IRepository.h"
#include "../models/Lector.h"

class LectorRepository : public IRepository<Lector>
{
public:
    LectorRepository() = default;
    ~LectorRepository() override = default;

    int agregar(const Lector &entidad) override;
    Lector obtenerPorId(int id) override;
    QList<Lector> obtenerTodos() override;
    bool actualizar(const Lector &entidad) override;
    bool eliminar(int id) override;

    QList<Lector> obtenerActivos();
};

#endif // LECTORREPOSITORY_H