
#ifndef LIBRODIALOG_H
#define LIBRODIALOG_H

#include <QDialog>
#include <QString>

namespace Ui {
class LibroDialog;
}

class LibroDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LibroDialog(QWidget *parent = nullptr);
    ~LibroDialog();

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
