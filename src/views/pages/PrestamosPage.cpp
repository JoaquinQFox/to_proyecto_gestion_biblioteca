
#include "PrestamosPage.h"
#include "ui_PrestamosPage.h"

#include "../dialogs/PrestamoDialog.h"
#include "../components/AccionesTablaWidget.h"
#include "../components/TablaHelper.h"

#include <QPushButton>
#include <QTableWidgetItem>
#include <QDate>

PrestamosPage::PrestamosPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PrestamosPage)
{
    ui->setupUi(this);

    // Abrir formulario de nuevo préstamo
    connect(ui->btnNuevoPrestamo, &QPushButton::clicked,
            this, [this]() {
                PrestamoDialog dialog(this);
                dialog.exec();
            });

    // Configuración reutilizable de la tabla
    // Columna 6: Acciones, ancho de 180 px
    TablaHelper::configurar(ui->tablaPrestamos, 6, 180);
}

PrestamosPage::~PrestamosPage()
{
    delete ui;
}

void PrestamosPage::mostrarPrestamos(
    const QList<Prestamo> &prestamos
    )
{
    // Limpiar registros anteriores
    ui->tablaPrestamos->setRowCount(0);

    // Mostrar cada préstamo
    for (const Prestamo &prestamo : prestamos) {

        const int fila = ui->tablaPrestamos->rowCount();
        ui->tablaPrestamos->insertRow(fila);

        // Columna 0: ID
        auto *itemId = new QTableWidgetItem(
            QString::number(prestamo.getId())
            );

        itemId->setTextAlignment(Qt::AlignCenter);
        ui->tablaPrestamos->setItem(fila, 0, itemId);

        // Columna 1: Lector
        // Temporalmente se muestra su ID
        auto *itemLector = new QTableWidgetItem(
            QString::number(prestamo.getIdLector())
            );

        itemLector->setTextAlignment(Qt::AlignCenter);
        ui->tablaPrestamos->setItem(fila, 1, itemLector);

        // Columna 2: Libro
        // Temporalmente se muestra el ID del ejemplar
        auto *itemLibro = new QTableWidgetItem(
            QString::number(prestamo.getIdEjemplar())
            );

        itemLibro->setTextAlignment(Qt::AlignCenter);
        ui->tablaPrestamos->setItem(fila, 2, itemLibro);

        // Columna 3: Fecha de préstamo
        auto *itemFechaPrestamo = new QTableWidgetItem(
            prestamo.getFechaPrestamo().toString("dd/MM/yyyy")
            );

        itemFechaPrestamo->setTextAlignment(Qt::AlignCenter);
        ui->tablaPrestamos->setItem(fila, 3, itemFechaPrestamo);

        // Verificar si el préstamo fue devuelto
        const bool devuelto =
            prestamo.getFechaDevolucion().isValid();

        // Columna 4: Fecha de devolución
        auto *itemFechaDevolucion = new QTableWidgetItem(
            devuelto
                ? prestamo.getFechaDevolucion().toString("dd/MM/yyyy")
                : "Pendiente"
            );

        itemFechaDevolucion->setTextAlignment(Qt::AlignCenter);
        ui->tablaPrestamos->setItem(fila, 4, itemFechaDevolucion);

        // Columna 5: Estado del préstamo
        QString estado;

        if (devuelto) {
            estado = "Devuelto";
        }
        else if (
            prestamo.getFechaVencimiento().isValid() &&
            prestamo.getFechaVencimiento() < QDate::currentDate()
            ) {
            estado = "Vencido";
        }
        else {
            estado = "Activo";
        }

        auto *itemEstado = new QTableWidgetItem(estado);
        itemEstado->setTextAlignment(Qt::AlignCenter);

        ui->tablaPrestamos->setItem(fila, 5, itemEstado);

        // Columna 6: Acciones reutilizables
        // Devuelto: solo Editar
        // Activo o vencido: Editar y Devolver
        const auto tipoAcciones = devuelto
                                      ? AccionesTablaWidget::TipoAcciones::SoloEditar
                                      : AccionesTablaWidget::TipoAcciones::EditarDevolver;

        auto *acciones = new AccionesTablaWidget(
            prestamo.getId(),
            ui->tablaPrestamos,
            tipoAcciones
            );

        // Solicitud de edición
        connect(
            acciones,
            &AccionesTablaWidget::editarSolicitado,
            this,
            &PrestamosPage::editarPrestamoSolicitado
            );

        // Solicitud de devolución
        connect(
            acciones,
            &AccionesTablaWidget::devolverSolicitado,
            this,
            &PrestamosPage::devolverPrestamoSolicitado
            );

        ui->tablaPrestamos->setCellWidget(
            fila, 6, acciones
            );
    }

    // Actualizar contador de préstamos
    ui->lblTotal->setText(
        QString("Total de préstamos: %1").arg(prestamos.size())
        );
}
