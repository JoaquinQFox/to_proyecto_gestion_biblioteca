#ifndef CATEGORIA_H
#define CATEGORIA_H

#include <QString>

class Categoria {
private:
    int id;
    QString nombre;

public:
    Categoria();
    Categoria(int id, const QString &nombre);

    int getId() const;
    void setId(int id);

    QString getNombre() const;
    void setNombre(const QString &nombre);
};

#endif // CATEGORIA_H