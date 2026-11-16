
#ifndef LIBROSPAGE_H
#define LIBROSPAGE_H

#include <QWidget>
#include <QList>
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
};

#endif // LIBROSPAGE_H
