
#include "LectoresPage.h"
#include "ui_LectoresPage.h"

#include "../dialogs/LectorDialog.h"
#include "../components/AccionesTablaWidget.h"

#include <QPushButton>
#include <QHeaderView>
#include <QAbstractItemView>
#include <QTableWidgetItem>

LectoresPage::LectoresPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::LectoresPage)
{
    ui->setupUi(this);

    // Abrir formulario de nuevo lector
    connect(ui->btnNuevoLector, &QPushButton::clicked,
            this, [this]() {
                LectorDialog dialog(this);
                dialog.exec();
            });

    // Configurar tabla
    ui->tablaLectores->horizontalHeader()->setSectionResizeMode(
        QHeaderView::Stretch
        );

    ui->tablaLectores->verticalHeader()->setVisible(false);

    ui->tablaLectores->setSelectionBehavior(
        QAbstractItemView::SelectRows
        );

    ui->tablaLectores->setEditTriggers(
        QAbstractItemView::NoEditTriggers
        );

    ui->tablaLectores->setAlternatingRowColors(false);
}

LectoresPage::~LectoresPage()
{
    delete ui;
}

void LectoresPage::mostrarLectores(
    const QList<Lector> &lectores
    )
{
    // Limpiar registros anteriores
    ui->tablaLectores->setRowCount(0);

    for (const Lector &lector : lectores) {

        const int fila = ui->tablaLectores->rowCount();

        ui->tablaLectores->insertRow(fila);

        // Columna ID
        auto *itemId = new QTableWidgetItem(
            QString::number(lector.getId())
            );

        itemId->setTextAlignment(Qt::AlignCenter);

        ui->tablaLectores->setItem(fila, 0, itemId);

        // Columna Nombre completo
        const QString nombreCompleto =
            lector.getNombres() + " " + lector.getApellidos();

        auto *itemNombre = new QTableWidgetItem(
            nombreCompleto.trimmed()
            );

        ui->tablaLectores->setItem(fila, 1, itemNombre);

        // Columna DNI
        auto *itemDni = new QTableWidgetItem(
            lector.getDni()
            );

        itemDni->setTextAlignment(Qt::AlignCenter);

        ui->tablaLectores->setItem(fila, 2, itemDni);

        // Columna Correo
        // El modelo Lector no dispone de este atributo.
        auto *itemCorreo = new QTableWidgetItem("—");

        itemCorreo->setTextAlignment(Qt::AlignCenter);

        ui->tablaLectores->setItem(fila, 3, itemCorreo);

        // Columna Teléfono
        auto *itemTelefono = new QTableWidgetItem(
            lector.getTelefono()
            );

        itemTelefono->setTextAlignment(Qt::AlignCenter);

        ui->tablaLectores->setItem(fila, 4, itemTelefono);

        // Componente reutilizable de acciones
        auto *acciones = new AccionesTablaWidget(
            lector.getId(),
            ui->tablaLectores
            );

        connect(acciones, &AccionesTablaWidget::editarSolicitado,
                this, &LectoresPage::editarLectorSolicitado);

        connect(acciones, &AccionesTablaWidget::eliminarSolicitado,
                this, &LectoresPage::eliminarLectorSolicitado);

        ui->tablaLectores->setCellWidget(fila, 5, acciones);

        // Altura de fila
        ui->tablaLectores->setRowHeight(fila, 52);
    }

    // Actualizar contador
    ui->lblTotal->setText(
        QString("Total de lectores: %1").arg(lectores.size())
        );
}
