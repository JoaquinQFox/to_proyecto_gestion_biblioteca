#ifndef PRESTAMOSPAGE_H
#define PRESTAMOSPAGE_H

#include <QWidget>
#include <QList>

#include "../../models/Prestamo.h"
#include "../../controllers/PrestamoController.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class PrestamosPage;
}
QT_END_NAMESPACE

class PrestamosPage : public QWidget
{
    Q_OBJECT

public:
    explicit PrestamosPage(PrestamoController *controller = nullptr, QWidget *parent = nullptr);
    ~PrestamosPage() override;

    void cargarPrestamos();
    void mostrarPrestamos(const QList<Prestamo> &prestamos);

signals:
    void editarPrestamoSolicitado(int idPrestamo);
    void devolverPrestamoSolicitado(int idPrestamo);

private slots:
    void onNuevoPrestamo();
    void onDevolverPrestamo(int idPrestamo);
    void onEditarPrestamo(int idPrestamo);
    void onBuscarTextoChanged(const QString &texto);

private:
    Ui::PrestamosPage *ui;
    PrestamoController *m_controller = nullptr;
    bool m_ownsController = false;
    QList<Prestamo> m_listaCompleta;

    QString obtenerNombreLector(int idLector) const;
    QString obtenerInfoLibro(int idEjemplar) const;
};

#endif // PRESTAMOSPAGE_H
