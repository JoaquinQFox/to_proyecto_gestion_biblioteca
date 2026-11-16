
#include "CategoriasPage.h"
#include "ui_CategoriasPage.h"

#include "../dialogs/CategoriaDialog.h"
#include "../components/AccionesTablaWidget.h"

#include <QPushButton>
#include <QHeaderView>
#include <QAbstractItemView>
#include <QTableWidgetItem>


CategoriasPage::CategoriasPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::CategoriasPage)
{
    ui->setupUi(this);

    // Abrir formulario de nueva categoría
    connect(ui->btnNuevaCategoria, &QPushButton::clicked,
            this, [this]() {
                CategoriaDialog dialog(this);
                dialog.exec();
            });

    // Configurar tabla
    ui->tablaCategorias->horizontalHeader()->setSectionResizeMode(
        QHeaderView::Stretch
        );

    ui->tablaCategorias->verticalHeader()->setVisible(false);

    ui->tablaCategorias->setSelectionBehavior(
        QAbstractItemView::SelectRows
        );

    ui->tablaCategorias->setEditTriggers(
        QAbstractItemView::NoEditTriggers
        );

    ui->tablaCategorias->setAlternatingRowColors(false);

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

        // Columna ID
        auto *itemId = new QTableWidgetItem(
            QString::number(categoria.getId())
            );

        itemId->setTextAlignment(Qt::AlignCenter);

        ui->tablaCategorias->setItem(fila, 0, itemId);

        // Columna Nombre
        auto *itemNombre = new QTableWidgetItem(
            categoria.getNombre()
            );

        ui->tablaCategorias->setItem(fila, 1, itemNombre);

        // Columna Acciones
        auto *acciones = new AccionesTablaWidget(
            categoria.getId(),
            ui->tablaCategorias
            );

        // Señal Editar
        connect(acciones, &AccionesTablaWidget::editarSolicitado,
                this, &CategoriasPage::editarCategoriaSolicitado);

        // Señal Eliminar
        connect(acciones, &AccionesTablaWidget::eliminarSolicitado,
                this, &CategoriasPage::eliminarCategoriaSolicitado);

        ui->tablaCategorias->setCellWidget(fila, 2, acciones);

        // Altura de fila
        ui->tablaCategorias->setRowHeight(fila, 52);
    }

    // Actualizar contador
    ui->lblTotal->setText(
        QString("Total de categorías: %1").arg(categorias.size())
        );
}
