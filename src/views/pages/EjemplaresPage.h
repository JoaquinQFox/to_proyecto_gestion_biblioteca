
#ifndef EJEMPLARESPAGE_H
#define EJEMPLARESPAGE_H

#include <QWidget>
#include <QList>

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
};

#endif // EJEMPLARESPAGE_H
