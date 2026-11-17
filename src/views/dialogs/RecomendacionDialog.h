
#ifndef RECOMENDACIONDIALOG_H
#define RECOMENDACIONDIALOG_H

#include <QDialog>
#include <QString>

class QLabel;
class QWidget;

namespace Ui {
class RecomendacionDialog;
}

class RecomendacionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RecomendacionDialog(QWidget *parent = nullptr);
    ~RecomendacionDialog();

private:
    Ui::RecomendacionDialog *ui;

    QLabel *lblErrorLector = nullptr;
    QLabel *lblErrorCantidad = nullptr;

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

#endif // RECOMENDACIONDIALOG_H
