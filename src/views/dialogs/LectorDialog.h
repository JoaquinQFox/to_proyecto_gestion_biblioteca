
#ifndef LECTORDIALOG_H
#define LECTORDIALOG_H

#include <QDialog>
#include <QString>

#include "../../models/Lector.h"
#include <QString>

class QLabel;
class QWidget;

namespace Ui {
class LectorDialog;
}

class LectorDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LectorDialog(QWidget *parent = nullptr);
    ~LectorDialog();

    void cargarLector(const Lector &lector);

signals:
    void guardarSolicitado(const QString &nombreCompleto,
                           const QString &dni,
                           const QString &correo,
                           const QString &telefono);

private:
    Ui::LectorDialog *ui;

    QLabel *lblErrorNombre = nullptr;
    QLabel *lblErrorDNI = nullptr;
    QLabel *lblErrorCorreo = nullptr;
    QLabel *lblErrorTelefono = nullptr;

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
};

#endif // LECTORDIALOG_H
