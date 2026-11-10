#ifndef EJEMPLAR_H
#define EJEMPLAR_H

#include <QString>

class Ejemplar
{
public:
    enum class Estado {
        DISPONIBLE,
        PRESTADO
    };

private:
    int id;
    int idLibro;
    QString codigo;
    Estado estado;

public:
    Ejemplar();
    Ejemplar(int id, int idLibro, const QString &codigo, const Estado &estado);

    int getId() const;
    void setId(int id);

    int getIdLibro() const;
    void setIdLibro(int idLibro);

    QString getCodigo() const;
    void setCodigo(const QString &codigo);

    Estado getEstado() const;
    void setEstado(const Estado &estado);

    QString getEstadoString() const;

    static Estado stringToEstado(const QString &estado);
};

#endif // EJEMPLAR_H