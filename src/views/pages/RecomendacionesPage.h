#ifndef RECOMENDACIONESPAGE_H
#define RECOMENDACIONESPAGE_H

#include <QWidget>
#include <QList>

#include "../../controllers/RecomendacionController.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class RecomendacionesPage;
}
QT_END_NAMESPACE

class RecomendacionesPage : public QWidget
{
    Q_OBJECT

public:
    explicit RecomendacionesPage(RecomendacionController *controller = nullptr,
                                 QWidget *parent = nullptr);
    ~RecomendacionesPage() override;

private:
    Ui::RecomendacionesPage *ui;
    RecomendacionController *m_controller = nullptr;

    void abrirFormulario();
    void mostrarRecomendaciones(const QList<RecomendacionItem> &items);
    void establecerCargando(bool cargando);
};

#endif // RECOMENDACIONESPAGE_H
