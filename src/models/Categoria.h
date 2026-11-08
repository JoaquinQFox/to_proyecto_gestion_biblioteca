#ifndef CATEGORIA_H
#define CATEGORIA_H

#include <QString>

class Categoria
{
private:
    int id;
    QString nombre;

public:
    Categoria();
    Categoria(const QString& nombre);
    Categoria(int id, const QString& nombre);

    int getId() const;
    QString getNombre() const;

    void setNombre(const QString& nombre);
};

#endif // CATEGORIA_H
