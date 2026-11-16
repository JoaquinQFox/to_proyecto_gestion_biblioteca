
#ifndef PRESTAMOSPAGE_H
#define PRESTAMOSPAGE_H

#include <QWidget>
#include <QList>

#include "../../models/Prestamo.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class PrestamosPage;
}
QT_END_NAMESPACE

class PrestamosPage : public QWidget
{
    Q_OBJECT

public:
    explicit PrestamosPage(QWidget *parent = nullptr);
    ~PrestamosPage() override;

    void mostrarPrestamos(const QList<Prestamo> &prestamos);

signals:
    void editarPrestamoSolicitado(int idPrestamo);
    void devolverPrestamoSolicitado(int idPrestamo);

private:
    Ui::PrestamosPage *ui;
};

#endif // PRESTAMOSPAGE_H
