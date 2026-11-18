
#ifndef LIBROSPAGE_H
#define LIBROSPAGE_H

#include <QWidget>
#include <QList>
#include <QHash>

#include "../../controllers/CategoriaController.h"
#include "../../controllers/LibroController.h"
#include "../../models/Libro.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class LibrosPage;
}
QT_END_NAMESPACE

class LibrosPage : public QWidget
{
    Q_OBJECT

public:
    explicit LibrosPage(QWidget *parent = nullptr);
    ~LibrosPage() override;

    void mostrarLibros(const QList<Libro> &libros);

signals:
    void editarLibroSolicitado(int idLibro);
    void eliminarLibroSolicitado(int idLibro);

private:
    Ui::LibrosPage *ui;
    LibroController controller;
    CategoriaController categoriaController;
    QHash<int, QString> nombresCategorias;

    void cargarLibros();
    void editarLibro(int idLibro);
    void eliminarLibro(int idLibro);
};

#endif // LIBROSPAGE_H
