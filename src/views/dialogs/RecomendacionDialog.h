
#ifndef RECOMENDACIONDIALOG_H
#define RECOMENDACIONDIALOG_H

#include <QDialog>
#include <QString>

#include "../../factories/RecommendationFactory.h"

class RecomendacionController;
class QLabel;
class QComboBox;
class QWidget;

namespace Ui {
class RecomendacionDialog;
}

class RecomendacionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RecomendacionDialog(RecomendacionController *controller,
                                 QWidget *parent = nullptr);
    ~RecomendacionDialog();

    // valores elegidos por el usuario
    TipoEstrategia tipoSeleccionado() const;
    int idLector() const;
    int idCategoria() const;
    int cantidad() const;

private:
    Ui::RecomendacionDialog *ui;
    RecomendacionController *m_controller = nullptr;

    QComboBox *cmbEstrategia = nullptr;
    QLabel *lblErrorCategoria = nullptr;

    void cargarLectoresYCategorias();
    void actualizarCategoriaObligatoria();

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
