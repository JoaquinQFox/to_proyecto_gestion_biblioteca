
#ifndef CATEGORIASPAGE_H
#define CATEGORIASPAGE_H

#include <QWidget>
#include <QList>

#include "../../controllers/CategoriaController.h"
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
    CategoriaController controller;

    void cargarCategorias();
    void editarCategoria(int idCategoria);
    void eliminarCategoria(int idCategoria);
};

#endif // CATEGORIASPAGE_H
