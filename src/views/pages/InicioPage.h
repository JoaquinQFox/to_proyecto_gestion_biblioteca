#ifndef INICIOPAGE_H
#define INICIOPAGE_H

#include <QWidget>
#include "../../controllers/PrestamoController.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class InicioPage;
}
QT_END_NAMESPACE

class InicioPage : public QWidget
{
    Q_OBJECT

public:
    explicit InicioPage(PrestamoController *controller = nullptr, QWidget *parent = nullptr);
    ~InicioPage() override;

public slots:
    void actualizarDashboard();

private:
    Ui::InicioPage *ui;
    PrestamoController *m_controller = nullptr;
    bool m_ownsController = false;

    void cargarPrestamosRecientes();
};

#endif // INICIOPAGE_H
