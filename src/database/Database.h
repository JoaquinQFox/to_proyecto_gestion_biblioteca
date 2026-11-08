#ifndef DATABASE_H
#define DATABASE_H

#include <QSqlDatabase>
#include <QString>

class Database
{
private:
    static QSqlDatabase conexion;

public:
    static bool conectar();
    static QSqlDatabase obtenerConexion();
    static void cerrarConexion();
};


#endif // DATABASE_H
