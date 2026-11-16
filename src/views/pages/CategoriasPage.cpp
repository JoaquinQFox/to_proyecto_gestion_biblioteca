
#include "CategoriasPage.h"
#include "ui_CategoriasPage.h"

#include "../dialogs/CategoriaDialog.h"
#include "../dialogs/ConfirmacionDialog.h"
#include "../components/AccionesTablaWidget.h"
#include "../components/TablaHelper.h"

#include <QPushButton>
#include <QTableWidgetItem>

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
            dialog.exec();
        }
        );

    // Configuración reutilizable de la tabla
    // Columna 2: Acciones, ancho de 180 px
    TablaHelper::configurar(ui->tablaCategorias, 2, 180);
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
