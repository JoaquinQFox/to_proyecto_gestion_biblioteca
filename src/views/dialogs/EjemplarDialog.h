#ifndef EJEMPLARDIALOG_H
#define EJEMPLARDIALOG_H

#include <QDialog>
#include <QList>
#include <QString>

#include "../../models/Ejemplar.h"
#include "../../models/Libro.h"

class QLabel;

namespace Ui {
class EjemplarDialog;
}

class EjemplarDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EjemplarDialog(QWidget *parent = nullptr);
    ~EjemplarDialog();

    void establecerLibros(const QList<Libro> &libros,
                          int idLibroSeleccionado = 0);
    void cargarEjemplar(const Ejemplar &ejemplar);

signals:
    void guardarSolicitado(int idLibro, const QString &codigo,
                           const QString &estado);

private:
    Ui::EjemplarDialog *ui;

    QLabel *lblErrorLibro;
    QLabel *lblErrorCodigo;
    QLabel *lblErrorEstado;

    bool validarFormulario();

    void mostrarError(
        QLabel *etiqueta,
        const QString &mensaje,
        QWidget *campo
        );

    void limpiarError(
        QLabel *etiqueta,
        QWidget *campo
        );

    void limpiarErrores();
};

#endif