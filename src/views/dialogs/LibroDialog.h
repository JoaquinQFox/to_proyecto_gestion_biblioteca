
#ifndef LIBRODIALOG_H
#define LIBRODIALOG_H

#include <QDialog>
#include <QList>
#include <QString>

#include "../../models/Categoria.h"
#include "../../models/Libro.h"

namespace Ui {
class LibroDialog;
}

class LibroDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LibroDialog(QWidget *parent = nullptr);
    ~LibroDialog();

    void establecerCategorias(const QList<Categoria> &categorias,
                              int idCategoriaSeleccionada = 0);
    void cargarLibro(const Libro &libro);

signals:
    void guardarSolicitado(const QString &titulo, const QString &autor,
                           const QString &isbn, int idCategoria);

private:
    Ui::LibroDialog *ui;

    bool validarFormulario();
    bool validarISBN(const QString &isbn);

    void mostrarError(
        const QString &nombreCampo,
        const QString &mensaje
        );

    void limpiarError(const QString &nombreCampo);
    void limpiarTodosLosErrores();
};

#endif
