
#ifndef TABLAHELPER_H
#define TABLAHELPER_H

class QString;
class QTableWidget;

class TablaHelper
{
public:
    static void configurar(
        QTableWidget *tabla,
        int columnaAcciones = -1,
        int anchoAcciones = 180
        );

    static void ajustarFilas(
        QTableWidget *tabla,
        int altura = 52
        );

    static void filtrar(
        QTableWidget *tabla,
        const QString &texto
        );
};

#endif // TABLAHELPER_H
