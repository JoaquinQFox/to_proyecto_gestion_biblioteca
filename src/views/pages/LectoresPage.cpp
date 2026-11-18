
#include "LectoresPage.h"
#include "ui_LectoresPage.h"

#include "../dialogs/LectorDialog.h"
#include "../dialogs/ConfirmacionDialog.h"
#include "../components/AccionesTablaWidget.h"
#include "../components/TablaHelper.h"

#include <QPushButton>
#include <QLineEdit>
#include <QMessageBox>
#include <QTableWidgetItem>
#include <exception>

LectoresPage::LectoresPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::LectoresPage)
{
    ui->setupUi(this);

    // Abrir formulario de nuevo lector
    connect(
        ui->btnNuevoLector,
        &QPushButton::clicked,
        this,
        [this]() {
            LectorDialog dialog(this);
            connect(
                &dialog,
                &LectorDialog::guardarSolicitado,
                this,
                [this, &dialog](
                    const QString &nombreCompleto,
                    const QString &dni,
                    const QString &correo,
                    const QString &telefono
                    ) {
                    try {
                        controller.crear(
                            nombreCompleto,
                            dni,
                            correo,
                            telefono
                            );
                        cargarLectores();
                        dialog.accept();
                    } catch (const std::exception &error) {
                        QMessageBox::warning(
                            &dialog,
                            "No se pudo guardar el lector",
                            QString::fromUtf8(error.what())
                            );
                    }
                }
                );
            dialog.exec();
        }
        );

    connect(
        this,
        &LectoresPage::editarLectorSolicitado,
        this,
        &LectoresPage::editarLector
        );

    connect(
        this,
        &LectoresPage::eliminarLectorSolicitado,
        this,
        &LectoresPage::eliminarLector
        );

    connect(
        ui->txtBuscar,
        &QLineEdit::textChanged,
        this,
        [this](const QString &texto) {
            TablaHelper::filtrar(ui->tablaLectores, texto);
        }
        );

    // Configuración reutilizable de la tabla
    // Columna 5: Acciones, ancho de 180 px
    TablaHelper::configurar(ui->tablaLectores, 5, 180);

    cargarLectores();
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
        auto *itemCorreo = new QTableWidgetItem(lector.getCorreo());

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

        // Confirmación antes de solicitar eliminación
        connect(
            acciones,
            &AccionesTablaWidget::eliminarSolicitado,
            this,
            [this](int idLector) {

                const bool confirmado =
                    ConfirmacionDialog::confirmar(
                        "Eliminar lector",
                        QString(
                            "¿Estás seguro de que deseas eliminar "
                            "el lector con ID %1?\n\n"
                            "Esta acción no se puede deshacer."
                            ).arg(idLector),
                        this
                        );

                if (confirmado) {
                    emit eliminarLectorSolicitado(idLector);
                }
            }
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

void LectoresPage::cargarLectores()
{
    try {
        mostrarLectores(controller.listar());
        TablaHelper::filtrar(ui->tablaLectores, ui->txtBuscar->text());
    } catch (const std::exception &error) {
        QMessageBox::warning(
            this,
            "No se pudieron cargar los lectores",
            QString::fromUtf8(error.what())
            );
    }
}

void LectoresPage::editarLector(int idLector)
{
    try {
        const Lector lector = controller.obtenerPorId(idLector);
        LectorDialog dialog(this);
        dialog.cargarLector(lector);

        connect(
            &dialog,
            &LectorDialog::guardarSolicitado,
            this,
            [this, &dialog, idLector](
                const QString &nombreCompleto,
                const QString &dni,
                const QString &correo,
                const QString &telefono
                ) {
                try {
                    controller.actualizar(
                        idLector,
                        nombreCompleto,
                        dni,
                        correo,
                        telefono
                        );
                    cargarLectores();
                    dialog.accept();
                } catch (const std::exception &error) {
                    QMessageBox::warning(
                        &dialog,
                        "No se pudo editar el lector",
                        QString::fromUtf8(error.what())
                        );
                }
            }
            );

        dialog.exec();
    } catch (const std::exception &error) {
        QMessageBox::warning(
            this,
            "No se pudo abrir el lector",
            QString::fromUtf8(error.what())
            );
    }
}

void LectoresPage::eliminarLector(int idLector)
{
    try {
        controller.eliminar(idLector);
        cargarLectores();
    } catch (const std::exception &error) {
        QMessageBox::warning(
            this,
            "No se pudo eliminar el lector",
            QString::fromUtf8(error.what())
            );
    }
}
