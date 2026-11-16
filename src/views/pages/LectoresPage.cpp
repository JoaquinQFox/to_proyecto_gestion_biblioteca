
#include "LectoresPage.h"
#include "ui_LectoresPage.h"

#include "../dialogs/LectorDialog.h"
#include "../components/AccionesTablaWidget.h"
#include "../components/TablaHelper.h"

#include <QPushButton>
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

    // Configuración reutilizable de la tabla
    // Columna 5: Acciones, ancho de 180 px
    TablaHelper::configurar(ui->tablaLectores, 5, 180);
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

        // Columna 0: ID
        auto *itemId = new QTableWidgetItem(
            QString::number(lector.getId())
            );

        itemId->setTextAlignment(Qt::AlignCenter);
        ui->tablaLectores->setItem(fila, 0, itemId);

        // Columna 1: Nombre completo
        const QString nombreCompleto =
            lector.getNombres() + " " + lector.getApellidos();

        auto *itemNombre = new QTableWidgetItem(
            nombreCompleto.trimmed()
            );

        itemNombre->setTextAlignment(
            Qt::AlignVCenter | Qt::AlignLeft
            );

        ui->tablaLectores->setItem(fila, 1, itemNombre);

        // Columna 2: DNI
        auto *itemDni = new QTableWidgetItem(
            lector.getDni()
            );

        itemDni->setTextAlignment(Qt::AlignCenter);
        ui->tablaLectores->setItem(fila, 2, itemDni);

        // Columna 3: Correo
        // El modelo Lector aún no dispone de este atributo
        auto *itemCorreo = new QTableWidgetItem("—");

        itemCorreo->setTextAlignment(Qt::AlignCenter);
        ui->tablaLectores->setItem(fila, 3, itemCorreo);

        // Columna 4: Teléfono
        auto *itemTelefono = new QTableWidgetItem(
            lector.getTelefono()
            );

        itemTelefono->setTextAlignment(Qt::AlignCenter);
        ui->tablaLectores->setItem(fila, 4, itemTelefono);

        // Columna 5: Acciones reutilizables
        auto *acciones = new AccionesTablaWidget(
            lector.getId(),
            ui->tablaLectores
            );

        // Solicitud de edición
        connect(
            acciones,
            &AccionesTablaWidget::editarSolicitado,
            this,
            &LectoresPage::editarLectorSolicitado
            );

        // Solicitud de eliminación
        connect(
            acciones,
            &AccionesTablaWidget::eliminarSolicitado,
            this,
            &LectoresPage::eliminarLectorSolicitado
            );

        ui->tablaLectores->setCellWidget(
            fila, 5, acciones
            );
    }

    // Actualizar contador
    ui->lblTotal->setText(
        QString("Total de lectores: %1").arg(lectores.size())
        );
}
