
#include "LibrosPage.h"
#include "ui_LibrosPage.h"

#include "../dialogs/LibroDialog.h"
#include "../dialogs/ConfirmacionDialog.h"
#include "../components/AccionesTablaWidget.h"
#include "../components/TablaHelper.h"

#include <QPushButton>
#include <QLineEdit>
#include <QMessageBox>
#include <QTableWidgetItem>
#include <exception>

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
            try {
                const QList<Categoria> categorias =
                    categoriaController.listar();
                if (categorias.isEmpty()) {
                    QMessageBox::information(
                        this,
                        "No hay categorías",
                        "Primero registra una categoría para poder crear un libro."
                        );
                    return;
                }

                LibroDialog dialog(this);
                dialog.establecerCategorias(categorias);
                connect(
                    &dialog,
                    &LibroDialog::guardarSolicitado,
                    this,
                    [this, &dialog](
                        const QString &titulo,
                        const QString &autor,
                        const QString &isbn,
                        int idCategoria
                        ) {
                        try {
                            controller.crear(
                                idCategoria,
                                titulo,
                                autor,
                                isbn
                                );
                            cargarLibros();
                            dialog.accept();
                        } catch (const std::exception &error) {
                            QMessageBox::warning(
                                &dialog,
                                "No se pudo guardar el libro",
                                QString::fromUtf8(error.what())
                                );
                        }
                    }
                    );
                dialog.exec();
            } catch (const std::exception &error) {
                QMessageBox::warning(
                    this,
                    "No se pudieron cargar las categorías",
                    QString::fromUtf8(error.what())
                    );
            }
        }
        );

    connect(
        this,
        &LibrosPage::editarLibroSolicitado,
        this,
        &LibrosPage::editarLibro
        );

    connect(
        this,
        &LibrosPage::eliminarLibroSolicitado,
        this,
        &LibrosPage::eliminarLibro
        );

    connect(
        ui->txtBuscar,
        &QLineEdit::textChanged,
        this,
        [this](const QString &texto) {
            TablaHelper::filtrar(ui->tablaLibros, texto);
        }
        );

    // Configuración reutilizable de la tabla
    // Columna 5: Acciones, ancho de 180 px
    TablaHelper::configurar(ui->tablaLibros, 5, 180);

    cargarLibros();
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
        auto *itemCategoria = new QTableWidgetItem(
            nombresCategorias.value(
                libro.getIdCategoria(),
                QString::number(libro.getIdCategoria())
                )
            );

        itemCategoria->setTextAlignment(Qt::AlignVCenter | Qt::AlignLeft);
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

void LibrosPage::cargarLibros()
{
    try {
        nombresCategorias.clear();
        const QList<Categoria> categorias = categoriaController.listar();
        for (const Categoria &categoria : categorias) {
            nombresCategorias.insert(categoria.getId(), categoria.getNombre());
        }

        mostrarLibros(controller.listar());
        TablaHelper::filtrar(ui->tablaLibros, ui->txtBuscar->text());
    } catch (const std::exception &error) {
        QMessageBox::warning(
            this,
            "No se pudieron cargar los libros",
            QString::fromUtf8(error.what())
            );
    }
}

void LibrosPage::editarLibro(int idLibro)
{
    try {
        const Libro libro = controller.obtenerPorId(idLibro);
        const QList<Categoria> categorias = categoriaController.listar();
        LibroDialog dialog(this);
        dialog.establecerCategorias(categorias, libro.getIdCategoria());
        dialog.cargarLibro(libro);

        connect(
            &dialog,
            &LibroDialog::guardarSolicitado,
            this,
            [this, &dialog, idLibro](
                const QString &titulo,
                const QString &autor,
                const QString &isbn,
                int idCategoria
                ) {
                try {
                    controller.actualizar(
                        idLibro,
                        idCategoria,
                        titulo,
                        autor,
                        isbn
                        );
                    cargarLibros();
                    dialog.accept();
                } catch (const std::exception &error) {
                    QMessageBox::warning(
                        &dialog,
                        "No se pudo editar el libro",
                        QString::fromUtf8(error.what())
                        );
                }
            }
            );

        dialog.exec();
    } catch (const std::exception &error) {
        QMessageBox::warning(
            this,
            "No se pudo abrir el libro",
            QString::fromUtf8(error.what())
            );
    }
}

void LibrosPage::eliminarLibro(int idLibro)
{
    try {
        controller.eliminar(idLibro);
        cargarLibros();
    } catch (const std::exception &error) {
        QMessageBox::warning(
            this,
            "No se pudo eliminar el libro",
            QString::fromUtf8(error.what())
            );
    }
}
