
#include "EjemplaresPage.h"
#include "ui_EjemplaresPage.h"

#include "../dialogs/EjemplarDialog.h"
#include "../components/AccionesTablaWidget.h"

#include <QPushButton>
#include <QHeaderView>
#include <QAbstractItemView>
#include <QTableWidgetItem>

EjemplaresPage::EjemplaresPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::EjemplaresPage)
{
    ui->setupUi(this);

    // Abrir formulario de nuevo ejemplar
    connect(ui->btnNuevoEjemplar, &QPushButton::clicked,
            this, [this]() {
                EjemplarDialog dialog(this);
                dialog.exec();
            });

    // Configurar tabla
    ui->tablaEjemplares->horizontalHeader()->setSectionResizeMode(
        QHeaderView::Stretch
        );

    ui->tablaEjemplares->verticalHeader()->setVisible(false);

    ui->tablaEjemplares->setSelectionBehavior(
        QAbstractItemView::SelectRows
        );

    ui->tablaEjemplares->setEditTriggers(
        QAbstractItemView::NoEditTriggers
        );

    ui->tablaEjemplares->setAlternatingRowColors(false);
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

        // Columna ID
        auto *itemId = new QTableWidgetItem(
            QString::number(ejemplar.getId())
            );

        itemId->setTextAlignment(Qt::AlignCenter);

        ui->tablaEjemplares->setItem(fila, 0, itemId);

        // Columna Código
        auto *itemCodigo = new QTableWidgetItem(
            ejemplar.getCodigo()
            );

        ui->tablaEjemplares->setItem(fila, 1, itemCodigo);

        // Columna Libro
        // Temporalmente mostramos el ID del libro.
        auto *itemLibro = new QTableWidgetItem(
            QString::number(ejemplar.getIdLibro())
            );

        itemLibro->setTextAlignment(Qt::AlignCenter);

        ui->tablaEjemplares->setItem(fila, 2, itemLibro);

        // Columna Estado
        auto *itemEstado = new QTableWidgetItem(
            ejemplar.getEstadoString()
            );

        itemEstado->setTextAlignment(Qt::AlignCenter);

        ui->tablaEjemplares->setItem(fila, 3, itemEstado);

        // Componente reutilizable de acciones
        auto *acciones = new AccionesTablaWidget(
            ejemplar.getId(),
            ui->tablaEjemplares
            );

        connect(acciones, &AccionesTablaWidget::editarSolicitado,
                this, &EjemplaresPage::editarEjemplarSolicitado);

        connect(acciones, &AccionesTablaWidget::eliminarSolicitado,
                this, &EjemplaresPage::eliminarEjemplarSolicitado);

        ui->tablaEjemplares->setCellWidget(fila, 4, acciones);

        // Altura de fila
        ui->tablaEjemplares->setRowHeight(fila, 52);
    }

    // Actualizar contador
    ui->lblTotal->setText(
        QString("Total de ejemplares: %1").arg(ejemplares.size())
        );
}
