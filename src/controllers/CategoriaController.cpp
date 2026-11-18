#include "CategoriaController.h"

#include <stdexcept>

QList<Categoria> CategoriaController::listar()
{
    return repository.obtenerTodos();
}

Categoria CategoriaController::obtenerPorId(int id)
{
    if (id <= 0) {
        throw std::invalid_argument("El ID de la categoría no es válido.");
    }

    return repository.obtenerPorId(id);
}

int CategoriaController::crear(const QString &nombre)
{
    const QString nombreLimpio = nombre.trimmed();
    validarNombre(nombreLimpio);

    return repository.agregar(Categoria(0, nombreLimpio));
}

void CategoriaController::actualizar(int id, const QString &nombre)
{
    if (id <= 0) {
        throw std::invalid_argument("El ID de la categoría no es válido.");
    }

    const QString nombreLimpio = nombre.trimmed();
    validarNombre(nombreLimpio, id);

    if (!repository.actualizar(Categoria(id, nombreLimpio))) {
        throw std::invalid_argument("No se encontró la categoría que deseas editar.");
    }
}

void CategoriaController::eliminar(int id)
{
    if (id <= 0) {
        throw std::invalid_argument("El ID de la categoría no es válido.");
    }

    if (!repository.eliminar(id)) {
        throw std::invalid_argument("No se encontró la categoría que deseas eliminar.");
    }
}

void CategoriaController::validarNombre(
    const QString &nombre,
    int idExcluido
    )
{
    if (nombre.isEmpty()) {
        throw std::invalid_argument("El nombre de la categoría es obligatorio.");
    }

    if (nombre.size() > 60) {
        throw std::invalid_argument(
            "El nombre de la categoría no puede superar los 60 caracteres."
            );
    }

    const QList<Categoria> categorias = repository.obtenerTodos();
    for (const Categoria &categoria : categorias) {
        if (categoria.getId() != idExcluido &&
            QString::compare(
                categoria.getNombre().trimmed(),
                nombre,
                Qt::CaseInsensitive
                ) == 0) {
            throw std::invalid_argument(
                "Ya existe una categoría con ese nombre."
                );
        }
    }
}
