#ifndef PRESTAMODIALOG_H
#define PRESTAMODIALOG_H

#include <QDialog>
#include <QString>
#include <QDate>

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

    int getIdLectorSeleccionado() const;
    int getIdEjemplarSeleccionado() const;
    QDate getFechaPrestamo() const;
    QDate getFechaVencimiento() const;

private:
    Ui::PrestamoDialog *ui;

    QLabel *lblErrorLector = nullptr;
    QLabel *lblErrorEjemplar = nullptr;
    QLabel *lblErrorFechaPrestamo = nullptr;
    QLabel *lblErrorFechaDevolucion = nullptr;

    void cargarDatosCombos();
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
