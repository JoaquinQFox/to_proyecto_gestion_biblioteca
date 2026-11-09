#ifndef EJEMPLAR_H
#define EJEMPLAR_H

#include <QString>

class Ejemplar
{
private:
    int id;
    int idLibro;
    QString codigo;
    QString estado;

public:
    Ejemplar();
    Ejemplar(int id, int idLibro, const QString &codigo, const QString &estado);

    int getId() const;
    void setId(int id);

    int getIdLibro() const;
    void setIdLibro(int idLibro);

    QString getCodigo() const;
    void setCodigo(const QString &codigo);

    QString getEstado() const;
    void setEstado(const QString &estado);
};

#endif // EJEMPLAR_H