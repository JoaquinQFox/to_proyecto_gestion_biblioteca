
#include "LibrosPage.h"
#include "ui_LibrosPage.h"

#include "../dialogs/LibroDialog.h"
#include "../dialogs/ConfirmacionDialog.h"
#include "../components/AccionesTablaWidget.h"
#include "../components/TablaHelper.h"

#include <QPushButton>
#include <QTableWidgetItem>

LibrosPage::LibrosPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::LibrosPage)
{
    ui->setupUi(this);

    // Abrir formulario de nuevo libro
    connect(
        ui->btnNuevoLibro,
        &QPushButton::clicked,
        this,
        [this]() {
            LibroDialog dialog(this);
            dialog.exec();
        }
        );

    // Configuración reutilizable de la tabla
    // Columna 5: Acciones, ancho de 180 px
    TablaHelper::configurar(ui->tablaLibros, 5, 180);
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

        // Columna 0: ID
        auto *itemId = new QTableWidgetItem(
            QString::number(libro.getId())
            );

        itemId->setTextAlignment(Qt::AlignCenter);
        ui->tablaLibros->setItem(fila, 0, itemId);

        // Columna 1: Título
        auto *itemTitulo = new QTableWidgetItem(
            libro.getTitulo()
            );

        itemTitulo->setTextAlignment(
            Qt::AlignVCenter | Qt::AlignLeft
            );

        ui->tablaLibros->setItem(fila, 1, itemTitulo);

        // Columna 2: Autor
        auto *itemAutor = new QTableWidgetItem(
            libro.getAutor()
            );

        itemAutor->setTextAlignment(
            Qt::AlignVCenter | Qt::AlignLeft
            );

        ui->tablaLibros->setItem(fila, 2, itemAutor);

        // Columna 3: ISBN
        auto *itemIsbn = new QTableWidgetItem(
            libro.getIsbn()
            );

        itemIsbn->setTextAlignment(Qt::AlignCenter);
        ui->tablaLibros->setItem(fila, 3, itemIsbn);

        // Columna 4: Categoría
        // Temporalmente se muestra el ID de categoría
        auto *itemCategoria = new QTableWidgetItem(
            QString::number(libro.getIdCategoria())
            );

        itemCategoria->setTextAlignment(Qt::AlignCenter);
        ui->tablaLibros->setItem(fila, 4, itemCategoria);

        // Columna 5: Acciones reutilizables
        auto *acciones = new AccionesTablaWidget(
            libro.getId(),
            ui->tablaLibros
            );

        // Solicitud de edición
        connect(
            acciones,
            &AccionesTablaWidget::editarSolicitado,
            this,
            &LibrosPage::editarLibroSolicitado
            );

        // Confirmación antes de solicitar eliminación
        connect(
            acciones,
            &AccionesTablaWidget::eliminarSolicitado,
            this,
            [this](int idLibro) {

                const bool confirmado =
                    ConfirmacionDialog::confirmar(
                        "Eliminar libro",
                        QString(
                            "¿Estás seguro de que deseas eliminar "
                            "el libro con ID %1?\n\n"
                            "Esta acción no se puede deshacer."
                            ).arg(idLibro),
                        this
                        );

                if (confirmado) {
                    emit eliminarLibroSolicitado(idLibro);
                }
            }
            );

        ui->tablaLibros->setCellWidget(
            fila, 5, acciones
            );
    }

    // Actualizar contador
    ui->lblTotal->setText(
        QString("Total de libros: %1").arg(libros.size())
        );
}
