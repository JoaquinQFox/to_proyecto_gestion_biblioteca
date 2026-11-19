#include "LibroController.h"

#include <QRegularExpression>
#include <stdexcept>

QList<Libro> LibroController::listar()
{
    return repository.obtenerActivos();
}

QList<Libro> LibroController::listarActivos()
{
    return repository.obtenerActivos();
}

Libro LibroController::obtenerPorId(int id)
{
    if (id <= 0) {
        throw std::invalid_argument("El ID del libro no es válido.");
    }

    return repository.obtenerPorId(id);
}

int LibroController::crear(
    int idCategoria,
    const QString &titulo,
    const QString &autor,
    const QString &isbn
    )
{
    const QString isbnNormalizado =
        validarCampos(idCategoria, titulo, autor, isbn);

    return repository.agregar(
        Libro(
            0,
            idCategoria,
            titulo.trimmed(),
            autor.trimmed(),
            0,
            QString(),
            isbnNormalizado
            )
        );
}

void LibroController::actualizar(
    int id,
    int idCategoria,
    const QString &titulo,
    const QString &autor,
    const QString &isbn
    )
{
    if (id <= 0) {
        throw std::invalid_argument("El ID del libro no es válido.");
    }

    const QString isbnNormalizado =
        validarCampos(idCategoria, titulo, autor, isbn, id);

    Libro libro = repository.obtenerPorId(id);
    libro.setIdCategoria(idCategoria);
    libro.setTitulo(titulo.trimmed());
    libro.setAutor(autor.trimmed());
    libro.setIsbn(isbnNormalizado);

    if (!repository.actualizar(libro)) {
        throw std::invalid_argument("No se encontró el libro que deseas editar.");
    }
}

void LibroController::eliminar(int id)
{
    if (id <= 0) {
        throw std::invalid_argument("El ID del libro no es válido.");
    }

    if (!repository.eliminar(id)) {
        throw std::invalid_argument("No se encontró el libro que deseas eliminar.");
    }
}

QString LibroController::validarCampos(
    int idCategoria,
    const QString &titulo,
    const QString &autor,
    const QString &isbn,
    int idExcluido
    )
{
    const QString tituloLimpio = titulo.trimmed();
    const QString autorLimpio = autor.trimmed();

    if (tituloLimpio.isEmpty()) {
        throw std::invalid_argument("El título del libro es obligatorio.");
    }
    if (tituloLimpio.size() > 150) {
        throw std::invalid_argument("El título no puede superar los 150 caracteres.");
    }
    if (autorLimpio.isEmpty()) {
        throw std::invalid_argument("El autor del libro es obligatorio.");
    }
    if (autorLimpio.size() > 100) {
        throw std::invalid_argument("El autor no puede superar los 100 caracteres.");
    }
    if (idCategoria <= 0) {
        throw std::invalid_argument("Selecciona una categoría válida.");
    }

    bool categoriaEncontrada = false;
    const QList<Categoria> categorias = categoriaRepository.obtenerTodos();
    for (const Categoria &categoria : categorias) {
        if (categoria.getId() == idCategoria) {
            categoriaEncontrada = true;
            break;
        }
    }
    if (!categoriaEncontrada) {
        throw std::invalid_argument("La categoría seleccionada no existe.");
    }

    QString isbnNormalizado = isbn.trimmed().toUpper();
    isbnNormalizado.remove(QRegularExpression("[\\s-]"));
    static const QRegularExpression formatoISBN(
        "^(?:[0-9]{9}[0-9X]|[0-9]{13})$"
        );
    if (!formatoISBN.match(isbnNormalizado).hasMatch()) {
        throw std::invalid_argument("El ISBN debe tener 10 o 13 caracteres válidos.");
    }

    const QList<Libro> libros = repository.obtenerTodos();
    for (const Libro &libro : libros) {
        if (libro.getId() == idExcluido) {
            continue;
        }

        QString isbnExistente = libro.getIsbn().trimmed().toUpper();
        isbnExistente.remove(QRegularExpression("[\\s-]"));
        if (isbnExistente == isbnNormalizado) {
            throw std::invalid_argument("Ya existe un libro con ese ISBN.");
        }
    }

    return isbnNormalizado;
}
