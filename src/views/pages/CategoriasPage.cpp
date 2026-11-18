
#include "CategoriasPage.h"
#include "ui_CategoriasPage.h"

#include "../dialogs/CategoriaDialog.h"
#include "../dialogs/ConfirmacionDialog.h"
#include "../components/AccionesTablaWidget.h"
#include "../components/TablaHelper.h"

#include <QPushButton>
#include <QLineEdit>
#include <QMessageBox>
#include <QTableWidgetItem>
#include <exception>

CategoriasPage::CategoriasPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::CategoriasPage)
{
    ui->setupUi(this);

    // Abrir formulario de nueva categoría
    connect(
        ui->btnNuevaCategoria,
        &QPushButton::clicked,
        this,
        [this]() {
            CategoriaDialog dialog(this);
            connect(
                &dialog,
                &CategoriaDialog::guardarSolicitado,
                this,
                [this, &dialog](const QString &nombre) {
                    try {
                        controller.crear(nombre);
                        cargarCategorias();
                        dialog.accept();
                    } catch (const std::exception &error) {
                        QMessageBox::warning(
                            &dialog,
                            "No se pudo guardar la categoría",
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
        &CategoriasPage::editarCategoriaSolicitado,
        this,
        &CategoriasPage::editarCategoria
        );

    connect(
        this,
        &CategoriasPage::eliminarCategoriaSolicitado,
        this,
        &CategoriasPage::eliminarCategoria
        );

    connect(
        ui->txtBuscar,
        &QLineEdit::textChanged,
        this,
        [this](const QString &texto) {
            TablaHelper::filtrar(ui->tablaCategorias, texto);
        }
        );

    // Configuración reutilizable de la tabla
    // Columna 2: Acciones, ancho de 180 px
    TablaHelper::configurar(ui->tablaCategorias, 2, 180);

    cargarCategorias();
}

CategoriasPage::~CategoriasPage()
{
    delete ui;
}

void CategoriasPage::mostrarCategorias(
    const QList<Categoria> &categorias
    )
{
    // Limpiar registros anteriores
    ui->tablaCategorias->setRowCount(0);

    // Agregar cada categoría
    for (const Categoria &categoria : categorias) {

        const int fila = ui->tablaCategorias->rowCount();
        ui->tablaCategorias->insertRow(fila);

        // Columna 0: ID
        auto *itemId = new QTableWidgetItem(
            QString::number(categoria.getId())
            );

        itemId->setTextAlignment(Qt::AlignCenter);
        ui->tablaCategorias->setItem(fila, 0, itemId);

        // Columna 1: Nombre
        auto *itemNombre = new QTableWidgetItem(
            categoria.getNombre()
            );

        itemNombre->setTextAlignment(
            Qt::AlignVCenter | Qt::AlignLeft
            );

        ui->tablaCategorias->setItem(fila, 1, itemNombre);

        // Columna 2: Acciones reutilizables
        auto *acciones = new AccionesTablaWidget(
            categoria.getId(),
            ui->tablaCategorias
            );

        // Solicitud de edición
        connect(
            acciones,
            &AccionesTablaWidget::editarSolicitado,
            this,
            &CategoriasPage::editarCategoriaSolicitado
            );

        // Confirmación antes de solicitar eliminación
        connect(
            acciones,
            &AccionesTablaWidget::eliminarSolicitado,
            this,
            [this](int idCategoria) {

                const bool confirmado =
                    ConfirmacionDialog::confirmar(
                        "Eliminar categoría",
                        QString(
                            "¿Estás seguro de que deseas eliminar "
                            "la categoría con ID %1?\n\n"
                            "Esta acción no se puede deshacer."
                            ).arg(idCategoria),
                        this
                        );

                if (confirmado) {
                    emit eliminarCategoriaSolicitado(idCategoria);
                }
            }
            );

        ui->tablaCategorias->setCellWidget(
            fila, 2, acciones
            );
    }

    // Actualizar contador
    ui->lblTotal->setText(
        QString("Total de categorías: %1").arg(categorias.size())
        );
}

void CategoriasPage::cargarCategorias()
{
    try {
        mostrarCategorias(controller.listar());
        TablaHelper::filtrar(ui->tablaCategorias, ui->txtBuscar->text());
    } catch (const std::exception &error) {
        QMessageBox::warning(
            this,
            "No se pudieron cargar las categorías",
            QString::fromUtf8(error.what())
            );
    }
}

void CategoriasPage::editarCategoria(int idCategoria)
{
    try {
        const Categoria categoria = controller.obtenerPorId(idCategoria);
        CategoriaDialog dialog(categoria.getNombre(), this);

        connect(
            &dialog,
            &CategoriaDialog::guardarSolicitado,
            this,
            [this, &dialog, idCategoria](const QString &nombre) {
                try {
                    controller.actualizar(idCategoria, nombre);
                    cargarCategorias();
                    dialog.accept();
                } catch (const std::exception &error) {
                    QMessageBox::warning(
                        &dialog,
                        "No se pudo editar la categoría",
                        QString::fromUtf8(error.what())
                        );
                }
            }
            );

        dialog.exec();
    } catch (const std::exception &error) {
        QMessageBox::warning(
            this,
            "No se pudo abrir la categoría",
            QString::fromUtf8(error.what())
            );
    }
}

void CategoriasPage::eliminarCategoria(int idCategoria)
{
    try {
        controller.eliminar(idCategoria);
        cargarCategorias();
    } catch (const std::exception &error) {
        QMessageBox::warning(
            this,
            "No se pudo eliminar la categoría",
            QString::fromUtf8(error.what())
            );
    }
}
