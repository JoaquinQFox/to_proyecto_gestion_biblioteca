
#include "EjemplaresPage.h"
#include "ui_EjemplaresPage.h"

#include "../dialogs/EjemplarDialog.h"
#include "../dialogs/ConfirmacionDialog.h"
#include "../components/AccionesTablaWidget.h"
#include "../components/TablaHelper.h"

#include <QPushButton>
#include <QLineEdit>
#include <QMessageBox>
#include <QTableWidgetItem>
#include <exception>

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
            try {
                const QList<Libro> libros = libroController.listarActivos();
                if (libros.isEmpty()) {
                    QMessageBox::information(
                        this,
                        "No hay libros",
                        "Primero registra un libro para poder crear un ejemplar."
                        );
                    return;
                }

                EjemplarDialog dialog(this);
                dialog.establecerLibros(libros);
                connect(
                    &dialog,
                    &EjemplarDialog::guardarSolicitado,
                    this,
                    [this, &dialog](
                        int idLibro,
                        const QString &codigo,
                        const QString &estado
                        ) {
                        try {
                            controller.crear(idLibro, codigo, estado);
                            cargarEjemplares();
                            dialog.accept();
                        } catch (const std::exception &error) {
                            QMessageBox::warning(
                                &dialog,
                                "No se pudo guardar el ejemplar",
                                QString::fromUtf8(error.what())
                                );
                        }
                    }
                    );
                dialog.exec();
            } catch (const std::exception &error) {
                QMessageBox::warning(
                    this,
                    "No se pudieron cargar los libros",
                    QString::fromUtf8(error.what())
                    );
            }
        }
        );

    connect(
        this,
        &EjemplaresPage::editarEjemplarSolicitado,
        this,
        &EjemplaresPage::editarEjemplar
        );

    connect(
        this,
        &EjemplaresPage::eliminarEjemplarSolicitado,
        this,
        &EjemplaresPage::eliminarEjemplar
        );

    connect(
        ui->txtBuscar,
        &QLineEdit::textChanged,
        this,
        [this](const QString &texto) {
            TablaHelper::filtrar(ui->tablaEjemplares, texto);
        }
        );

    // Configuración reutilizable de la tabla
    // Columna 4: Acciones, ancho de 180 px
    TablaHelper::configurar(ui->tablaEjemplares, 4, 180);

    cargarEjemplares();
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
        auto *itemLibro = new QTableWidgetItem(
            titulosLibros.value(
                ejemplar.getIdLibro(),
                QString::number(ejemplar.getIdLibro())
                )
            );

        itemLibro->setTextAlignment(Qt::AlignVCenter | Qt::AlignLeft);
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

void EjemplaresPage::cargarEjemplares()
{
    try {
        titulosLibros.clear();
        const QList<Libro> libros = libroController.listarActivos();
        for (const Libro &libro : libros) {
            titulosLibros.insert(libro.getId(), libro.getTitulo());
        }

        mostrarEjemplares(controller.listar());
        TablaHelper::filtrar(ui->tablaEjemplares, ui->txtBuscar->text());
    } catch (const std::exception &error) {
        QMessageBox::warning(
            this,
            "No se pudieron cargar los ejemplares",
            QString::fromUtf8(error.what())
            );
    }
}

void EjemplaresPage::editarEjemplar(int idEjemplar)
{
    try {
        const Ejemplar ejemplar = controller.obtenerPorId(idEjemplar);
        const QList<Libro> libros = libroController.listarActivos();
        EjemplarDialog dialog(this);
        dialog.establecerLibros(libros, ejemplar.getIdLibro());
        dialog.cargarEjemplar(ejemplar);

        connect(
            &dialog,
            &EjemplarDialog::guardarSolicitado,
            this,
            [this, &dialog, idEjemplar](
                int idLibro,
                const QString &codigo,
                const QString &estado
                ) {
                try {
                    controller.actualizar(
                        idEjemplar,
                        idLibro,
                        codigo,
                        estado
                        );
                    cargarEjemplares();
                    dialog.accept();
                } catch (const std::exception &error) {
                    QMessageBox::warning(
                        &dialog,
                        "No se pudo editar el ejemplar",
                        QString::fromUtf8(error.what())
                        );
                }
            }
            );

        dialog.exec();
    } catch (const std::exception &error) {
        QMessageBox::warning(
            this,
            "No se pudo abrir el ejemplar",
            QString::fromUtf8(error.what())
            );
    }
}

void EjemplaresPage::eliminarEjemplar(int idEjemplar)
{
    try {
        controller.eliminar(idEjemplar);
        cargarEjemplares();
    } catch (const std::exception &error) {
        QMessageBox::warning(
            this,
            "No se pudo eliminar el ejemplar",
            QString::fromUtf8(error.what())
            );
    }
}
