
#ifndef ACCIONESTABLAWIDGET_H
#define ACCIONESTABLAWIDGET_H

#include <QWidget>

class QPushButton;

class AccionesTablaWidget : public QWidget
{
    Q_OBJECT

public:
    enum class TipoAcciones {
        EditarEliminar,
        EditarDevolver,
        SoloEditar
    };

    explicit AccionesTablaWidget(
        int idRegistro,
        QWidget *parent = nullptr,
        TipoAcciones tipo = TipoAcciones::EditarEliminar
        );

    int obtenerIdRegistro() const;

signals:
    void editarSolicitado(int idRegistro);
    void eliminarSolicitado(int idRegistro);
    void devolverSolicitado(int idRegistro);

private:
    int m_idRegistro;

    QPushButton *btnEditar;
    QPushButton *btnEliminar;
    QPushButton *btnDevolver;
};

#endif // ACCIONESTABLAWIDGET_H
