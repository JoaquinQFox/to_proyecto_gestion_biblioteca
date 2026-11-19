
#ifndef LECTORESPAGE_H
#define LECTORESPAGE_H

#include <QWidget>
#include <QList>

#include "../../controllers/LectorController.h"
#include "../../models/Lector.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class LectoresPage;
}
QT_END_NAMESPACE

class LectoresPage : public QWidget
{
    Q_OBJECT

public:
    explicit LectoresPage(QWidget *parent = nullptr);
    ~LectoresPage() override;

    void mostrarLectores(const QList<Lector> &lectores);

signals:
    void editarLectorSolicitado(int idLector);
    void eliminarLectorSolicitado(int idLector);

private:
    Ui::LectoresPage *ui;
    LectorController controller;

    void cargarLectores();
    void editarLector(int idLector);
    void eliminarLector(int idLector);
};

#endif // LECTORESPAGE_H
