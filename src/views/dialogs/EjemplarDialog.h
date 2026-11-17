#ifndef EJEMPLARDIALOG_H
#define EJEMPLARDIALOG_H

#include <QDialog>

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