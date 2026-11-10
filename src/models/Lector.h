#ifndef LECTOR_H
#define LECTOR_H

#include <QString>

class Lector
{
private:
    int id;
    QString dni;
    QString nombres;
    QString apellidos;
    QString telefono;
    QString direccion;
    bool activo;

public:
    Lector();
    Lector(int id, const QString &dni, const QString &nombres, const QString &apellidos,
           const QString &telefono, const QString &direccion, bool activo = true);

    int getId() const;
    void setId(int id);

    QString getDni() const;
    void setDni(const QString &dni);

    QString getNombres() const;
    void setNombres(const QString &nombres);

    QString getApellidos() const;
    void setApellidos(const QString &apellidos);

    QString getTelefono() const;
    void setTelefono(const QString &telefono);

    QString getDireccion() const;
    void setDireccion(const QString &direccion);

    bool isActivo() const;
    void setActivo(bool activo);

    bool operator==(const Lector &otro) const;
};

#endif // LECTOR_H