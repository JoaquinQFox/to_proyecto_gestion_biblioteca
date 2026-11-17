
#ifndef PRESTAMODIALOG_H
#define PRESTAMODIALOG_H

#include <QDialog>
#include <QString>

class QLabel;
class QWidget;

namespace Ui {
class PrestamoDialog;
}

class PrestamoDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PrestamoDialog(QWidget *parent = nullptr);
    ~PrestamoDialog();

private:
    Ui::PrestamoDialog *ui;

    QLabel *lblErrorLector = nullptr;
    QLabel *lblErrorEjemplar = nullptr;
    QLabel *lblErrorFechaPrestamo = nullptr;
    QLabel *lblErrorFechaDevolucion = nullptr;

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

    void actualizarValidacionFechas();
};

#endif // PRESTAMODIALOG_H
