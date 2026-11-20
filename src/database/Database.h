#ifndef DATABASE_H
#define DATABASE_H

#include <QSqlDatabase>
#include <QString>

class Database
{
private:
    static QSqlDatabase conexion;
    static QString rutaBD;

public:
    static bool conectar();

    // Solo para pruebas
    static bool conectarEnMemoria();

    static QSqlDatabase obtenerConexion();
    static void cerrarConexion();

    // conexión propia para hilos secundarios
    static bool abrirConexionHilo();
    static void cerrarConexionHilo();
};

#endif // DATABASE_H
