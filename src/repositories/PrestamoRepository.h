#ifndef PRESTAMOREPOSITORY_H
#define PRESTAMOREPOSITORY_H

#include "../interfaces/IRepository.h"
#include "../models/Prestamo.h"

class PrestamoRepository : public IRepository<Prestamo>
{
public:
    PrestamoRepository() = default;
    ~PrestamoRepository() override = default;

    int agregar(const Prestamo &entidad) override;
    Prestamo obtenerPorId(int id) override;
    QList<Prestamo> obtenerTodos() override;
    bool actualizar(const Prestamo &entidad) override;
    bool eliminar(int id) override;
};

#endif // PRESTAMOREPOSITORY_H