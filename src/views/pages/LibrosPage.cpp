
#include "LibrosPage.h"
#include "ui_LibrosPage.h"

#include "../dialogs/LibroDialog.h"
#include "../components/AccionesTablaWidget.h"

#include <QPushButton>
#include <QHeaderView>
#include <QAbstractItemView>
#include <QTableWidgetItem>

LibrosPage::LibrosPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::LibrosPage)
{
    ui->setupUi(this);

    // Abrir formulario de nuevo libro
    connect(ui->btnNuevoLibro, &QPushButton::clicked,
            this, [this]() {
                LibroDialog dialog(this);
                dialog.exec();
            });

    // Configurar tabla
    ui->tablaLibros->horizontalHeader()->setSectionResizeMode(
        QHeaderView::Stretch
        );

    ui->tablaLibros->verticalHeader()->setVisible(false);

    ui->tablaLibros->setSelectionBehavior(
        QAbstractItemView::SelectRows
        );

    ui->tablaLibros->setEditTriggers(
        QAbstractItemView::NoEditTriggers
        );

    ui->tablaLibros->setAlternatingRowColors(false);
}

LibrosPage::~LibrosPage()
{
    delete ui;
}

void LibrosPage::mostrarLibros(const QList<Libro> &libros)
{
    // Limpiar registros anteriores
    ui->tablaLibros->setRowCount(0);

    for (const Libro &libro : libros) {

        const int fila = ui->tablaLibros->rowCount();

        ui->tablaLibros->insertRow(fila);

        // ID
        auto *itemId = new QTableWidgetItem(
            QString::number(libro.getId())
            );

        itemId->setTextAlignment(Qt::AlignCenter);

        ui->tablaLibros->setItem(fila, 0, itemId);

        // Título
        auto *itemTitulo = new QTableWidgetItem(
            libro.getTitulo()
            );

        ui->tablaLibros->setItem(fila, 1, itemTitulo);

        // Autor
        auto *itemAutor = new QTableWidgetItem(
            libro.getAutor()
            );

        ui->tablaLibros->setItem(fila, 2, itemAutor);

        // ISBN
        auto *itemIsbn = new QTableWidgetItem(
            libro.getIsbn()
            );

        ui->tablaLibros->setItem(fila, 3, itemIsbn);

        // Categoría
        // Temporalmente se muestra el ID de categoría.
        // El nombre se resolverá al integrar los datos.
        auto *itemCategoria = new QTableWidgetItem(
            QString::number(libro.getIdCategoria())
            );

        itemCategoria->setTextAlignment(Qt::AlignCenter);

        ui->tablaLibros->setItem(fila, 4, itemCategoria);

        // Botones de acciones
        auto *acciones = new AccionesTablaWidget(
            libro.getId(),
            ui->tablaLibros
            );

        connect(acciones, &AccionesTablaWidget::editarSolicitado,
                this, &LibrosPage::editarLibroSolicitado);

        connect(acciones, &AccionesTablaWidget::eliminarSolicitado,
                this, &LibrosPage::eliminarLibroSolicitado);

        ui->tablaLibros->setCellWidget(fila, 5, acciones);

        // Altura de fila
        ui->tablaLibros->setRowHeight(fila, 52);
    }

    // Actualizar contador
    ui->lblTotal->setText(
        QString("Total de libros: %1").arg(libros.size())
        );
}
