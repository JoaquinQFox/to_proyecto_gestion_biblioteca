
#ifndef CATEGORIASPAGE_H
#define CATEGORIASPAGE_H

#include <QWidget>
#include <QList>

#include "../../models/Categoria.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class CategoriasPage;
}
QT_END_NAMESPACE

class CategoriasPage : public QWidget
{
    Q_OBJECT

public:
    explicit CategoriasPage(QWidget *parent = nullptr);
    ~CategoriasPage() override;

    void mostrarCategorias(const QList<Categoria> &categorias);

signals:
    void editarCategoriaSolicitado(int idCategoria);
    void eliminarCategoriaSolicitado(int idCategoria);

private:
    Ui::CategoriasPage *ui;
};

#endif // CATEGORIASPAGE_H
