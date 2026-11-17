#ifndef CATEGORIADIALOG_H
#define CATEGORIADIALOG_H

#include <QDialog>
#include <QString>

namespace Ui {
class CategoriaDialog;
}

class CategoriaDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CategoriaDialog(QWidget *parent = nullptr);
    ~CategoriaDialog();

signals:
    void guardarSolicitado(const QString &nombre);

private:
    Ui::CategoriaDialog *ui;

    bool validarFormulario();
    void mostrarError(const QString &mensaje);
    void limpiarError();
};

#endif