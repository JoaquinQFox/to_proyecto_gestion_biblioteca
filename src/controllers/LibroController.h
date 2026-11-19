#ifndef LIBROCONTROLLER_H
#define LIBROCONTROLLER_H

#include "../models/Libro.h"
#include "../repositories/CategoriaRepository.h"
#include "../repositories/LibroRepository.h"

#include <QList>
#include <QString>

class LibroController
{
public:
    QList<Libro> listar();
    QList<Libro> listarActivos();
    Libro obtenerPorId(int id);
    int crear(int idCategoria, const QString &titulo, const QString &autor,
              const QString &isbn);
    void actualizar(int id, int idCategoria, const QString &titulo,
                    const QString &autor, const QString &isbn);
    void eliminar(int id);

private:
    LibroRepository repository;
    CategoriaRepository categoriaRepository;

    QString validarCampos(int idCategoria, const QString &titulo,
                          const QString &autor, const QString &isbn,
                          int idExcluido = 0);
};

#endif // LIBROCONTROLLER_H
