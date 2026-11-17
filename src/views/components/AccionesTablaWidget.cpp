
#include "AccionesTablaWidget.h"

#include <QHBoxLayout>
#include <QPushButton>
#include <QSizePolicy>

AccionesTablaWidget::AccionesTablaWidget(
    int idRegistro,
    QWidget *parent,
    TipoAcciones tipo
    )
    : QWidget(parent),
    m_idRegistro(idRegistro),
    btnEditar(nullptr),
    btnEliminar(nullptr),
    btnDevolver(nullptr)
{
    setObjectName("accionesTablaWidget");

    auto *layout = new QHBoxLayout(this);

    layout->setContentsMargins(4, 0, 4, 0);
    layout->setSpacing(8);
    layout->setAlignment(Qt::AlignCenter);

    // Acción Editar: disponible en todas las configuraciones
    btnEditar = new QPushButton("Editar", this);
    btnEditar->setObjectName("btnEditarAccion");
    btnEditar->setCursor(Qt::PointingHandCursor);
    btnEditar->setToolTip("Editar registro");
    btnEditar->setMinimumHeight(30);

    btnEditar->setSizePolicy(
        QSizePolicy::Preferred,
        QSizePolicy::Fixed
        );

    layout->addWidget(btnEditar);

    connect(btnEditar, &QPushButton::clicked,
            this, [this]() {
                emit editarSolicitado(m_idRegistro);
            });

    // Acción Eliminar
    if (tipo == TipoAcciones::EditarEliminar) {

        btnEliminar = new QPushButton("Eliminar", this);
        btnEliminar->setObjectName("btnEliminarAccion");
        btnEliminar->setCursor(Qt::PointingHandCursor);
        btnEliminar->setToolTip("Eliminar registro");
        btnEliminar->setMinimumHeight(30);

        btnEliminar->setSizePolicy(
            QSizePolicy::Preferred,
            QSizePolicy::Fixed
            );

        layout->addWidget(btnEliminar);

        connect(btnEliminar, &QPushButton::clicked,
                this, [this]() {
                    emit eliminarSolicitado(m_idRegistro);
                });
    }

    // Acción Devolver
    if (tipo == TipoAcciones::EditarDevolver) {

        btnDevolver = new QPushButton("Devolver", this);
        btnDevolver->setObjectName("btnDevolverAccion");
        btnDevolver->setCursor(Qt::PointingHandCursor);
        btnDevolver->setToolTip("Registrar devolución del préstamo");
        btnDevolver->setMinimumHeight(30);

        btnDevolver->setSizePolicy(
            QSizePolicy::Preferred,
            QSizePolicy::Fixed
            );

        layout->addWidget(btnDevolver);

        connect(btnDevolver, &QPushButton::clicked,
                this, [this]() {
                    emit devolverSolicitado(m_idRegistro);
                });
    }
}

int AccionesTablaWidget::obtenerIdRegistro() const
{
    return m_idRegistro;
}
