#ifndef IREPOSITORY_H
#define IREPOSITORY_H

#include <QList>

template <typename T>
class IRepository
{
public:
    virtual int agregar(const T& entidad) = 0;
    virtual T obtenerPorId(int id) = 0;
    virtual QList<T> obtenerTodos() = 0;
    virtual bool actualizar(const T& entidad) = 0;
    virtual bool eliminar(int id) = 0;

    virtual ~IRepository() = default;
};

#endif // IREPOSITORY_H