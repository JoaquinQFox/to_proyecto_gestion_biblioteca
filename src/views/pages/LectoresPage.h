
#ifndef LECTORESPAGE_H
#define LECTORESPAGE_H

#include <QWidget>
#include <QList>

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
};

#endif // LECTORESPAGE_H
