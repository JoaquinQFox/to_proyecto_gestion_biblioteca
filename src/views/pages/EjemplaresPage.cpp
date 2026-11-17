
#include "EjemplaresPage.h"
#include "ui_EjemplaresPage.h"

#include "../dialogs/EjemplarDialog.h"
#include "../dialogs/ConfirmacionDialog.h"
#include "../components/AccionesTablaWidget.h"
#include "../components/TablaHelper.h"

#include <QPushButton>
#include <QTableWidgetItem>

EjemplaresPage::EjemplaresPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::EjemplaresPage)
{
    ui->setupUi(this);

    // Abrir formulario de nuevo ejemplar
    connect(
        ui->btnNuevoEjemplar,
        &QPushButton::clicked,
        this,
        [this]() {
            EjemplarDialog dialog(this);
            dialog.exec();
        }
        );

    // Configuración reutilizable de la tabla
    // Columna 4: Acciones, ancho de 180 px
    TablaHelper::configurar(ui->tablaEjemplares, 4, 180);
}

EjemplaresPage::~EjemplaresPage()
{
    delete ui;
}

void EjemplaresPage::mostrarEjemplares(
    const QList<Ejemplar> &ejemplares
    )
{
    // Limpiar registros anteriores
    ui->tablaEjemplares->setRowCount(0);

    for (const Ejemplar &ejemplar : ejemplares) {

        const int fila = ui->tablaEjemplares->rowCount();
        ui->tablaEjemplares->insertRow(fila);

        // Columna 0: ID
        auto *itemId = new QTableWidgetItem(
            QString::number(ejemplar.getId())
            );

        itemId->setTextAlignment(Qt::AlignCenter);
        ui->tablaEjemplares->setItem(fila, 0, itemId);

        // Columna 1: Código
        auto *itemCodigo = new QTableWidgetItem(
            ejemplar.getCodigo()
            );

        itemCodigo->setTextAlignment(
            Qt::AlignVCenter | Qt::AlignLeft
            );

        ui->tablaEjemplares->setItem(fila, 1, itemCodigo);

        // Columna 2: Libro
        // Temporalmente se muestra el ID del libro
        auto *itemLibro = new QTableWidgetItem(
            QString::number(ejemplar.getIdLibro())
            );

        itemLibro->setTextAlignment(Qt::AlignCenter);
        ui->tablaEjemplares->setItem(fila, 2, itemLibro);

        // Columna 3: Estado
        auto *itemEstado = new QTableWidgetItem(
            ejemplar.getEstadoString()
            );

        itemEstado->setTextAlignment(Qt::AlignCenter);
        ui->tablaEjemplares->setItem(fila, 3, itemEstado);

        // Columna 4: Acciones reutilizables
        auto *acciones = new AccionesTablaWidget(
            ejemplar.getId(),
            ui->tablaEjemplares
            );

        // Solicitud de edición
        connect(
            acciones,
            &AccionesTablaWidget::editarSolicitado,
            this,
            &EjemplaresPage::editarEjemplarSolicitado
            );

        // Confirmación antes de solicitar eliminación
        connect(
            acciones,
            &AccionesTablaWidget::eliminarSolicitado,
            this,
            [this](int idEjemplar) {

                const bool confirmado =
                    ConfirmacionDialog::confirmar(
                        "Eliminar ejemplar",
                        QString(
                            "¿Estás seguro de que deseas eliminar "
                            "el ejemplar con ID %1?\n\n"
                            "Esta acción no se puede deshacer."
                            ).arg(idEjemplar),
                        this
                        );

                if (confirmado) {
                    emit eliminarEjemplarSolicitado(idEjemplar);
                }
            }
            );

        ui->tablaEjemplares->setCellWidget(
            fila, 4, acciones
            );
    }

    // Actualizar contador
    ui->lblTotal->setText(
        QString("Total de ejemplares: %1").arg(ejemplares.size())
        );
}
