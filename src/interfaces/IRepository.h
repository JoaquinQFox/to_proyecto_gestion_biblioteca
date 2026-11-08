#ifndef IREPOSITORY_H
#define IREPOSITORY_H

#include <QList>

template <typename T>
class IRepository
{
public:
    virtual ~IRepository() = default;

    virtual bool agregar(const T& entidad) = 0;

    virtual bool obtenerPorId(int id, T& entidad) = 0;

    virtual QList<T> obtenerTodos() = 0;

    virtual bool actualizar(const T& entidad) = 0;

    virtual bool eliminar(int id) = 0;
};

#endif // IREPOSITORY_H
