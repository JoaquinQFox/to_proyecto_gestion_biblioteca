
#ifndef EJEMPLARESPAGE_H
#define EJEMPLARESPAGE_H

#include <QWidget>
#include <QList>
#include <QHash>

#include "../../controllers/EjemplarController.h"
#include "../../controllers/LibroController.h"
#include "../../models/Ejemplar.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class EjemplaresPage;
}
QT_END_NAMESPACE

class EjemplaresPage : public QWidget
{
    Q_OBJECT

public:
    explicit EjemplaresPage(QWidget *parent = nullptr);
    ~EjemplaresPage() override;

    void mostrarEjemplares(const QList<Ejemplar> &ejemplares);

signals:
    void editarEjemplarSolicitado(int idEjemplar);
    void eliminarEjemplarSolicitado(int idEjemplar);

private:
    Ui::EjemplaresPage *ui;
    EjemplarController controller;
    LibroController libroController;
    QHash<int, QString> titulosLibros;

    void cargarEjemplares();
    void editarEjemplar(int idEjemplar);
    void eliminarEjemplar(int idEjemplar);
};

#endif // EJEMPLARESPAGE_H
