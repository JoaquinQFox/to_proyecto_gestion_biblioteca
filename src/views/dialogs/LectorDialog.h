
#ifndef LECTORDIALOG_H
#define LECTORDIALOG_H

#include <QDialog>
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
