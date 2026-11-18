#include "EjemplarController.h"

#include <stdexcept>

QList<Ejemplar> EjemplarController::listar()
{
    return repository.obtenerTodos();
}

Ejemplar EjemplarController::obtenerPorId(int id)
{
    if (id <= 0) {
        throw std::invalid_argument("El ID del ejemplar no es válido.");
    }

    return repository.obtenerPorId(id);
}

int EjemplarController::crear(
    int idLibro,
    const QString &codigo,
    const QString &estado
    )
{
    const Ejemplar::Estado estadoValidado =
        validarCampos(idLibro, codigo, estado);

    return repository.agregar(
        Ejemplar(
            0,
            idLibro,
            codigo.trimmed(),
            estadoValidado
            )
        );
}

void EjemplarController::actualizar(
    int id,
    int idLibro,
    const QString &codigo,
    const QString &estado
    )
{
    if (id <= 0) {
        throw std::invalid_argument("El ID del ejemplar no es válido.");
    }

    const Ejemplar::Estado estadoValidado =
        validarCampos(idLibro, codigo, estado, id);

    if (!repository.actualizar(
            Ejemplar(id, idLibro, codigo.trimmed(), estadoValidado)
            )) {
        throw std::invalid_argument("No se encontró el ejemplar que deseas editar.");
    }
}

void EjemplarController::eliminar(int id)
{
    if (id <= 0) {
        throw std::invalid_argument("El ID del ejemplar no es válido.");
    }

    if (!repository.eliminar(id)) {
        throw std::invalid_argument("No se encontró el ejemplar que deseas eliminar.");
    }
}

Ejemplar::Estado EjemplarController::validarCampos(
    int idLibro,
    const QString &codigo,
    const QString &estado,
    int idExcluido
    )
{
    if (idLibro <= 0) {
        throw std::invalid_argument("Selecciona un libro válido.");
    }

    bool libroEncontrado = false;
    const QList<Libro> libros = libroRepository.obtenerActivos();
    for (const Libro &libro : libros) {
        if (libro.getId() == idLibro) {
            libroEncontrado = true;
            break;
        }
    }
    if (!libroEncontrado) {
        throw std::invalid_argument("El libro seleccionado no existe o está inactivo.");
    }

    const QString codigoLimpio = codigo.trimmed();
    if (codigoLimpio.isEmpty()) {
        throw std::invalid_argument("El código del ejemplar es obligatorio.");
    }
    if (codigoLimpio.size() > 30) {
        throw std::invalid_argument("El código no puede superar los 30 caracteres.");
    }

    Ejemplar::Estado estadoValidado;
    if (estado == "DISPONIBLE") {
        estadoValidado = Ejemplar::Estado::DISPONIBLE;
    } else if (estado == "PRESTADO") {
        estadoValidado = Ejemplar::Estado::PRESTADO;
    } else if (estado == "RETIRADO") {
        estadoValidado = Ejemplar::Estado::RETIRADO;
    } else {
        throw std::invalid_argument("Selecciona un estado válido para el ejemplar.");
    }

    const QList<Ejemplar> ejemplares = repository.obtenerTodos();
    for (const Ejemplar &ejemplar : ejemplares) {
        if (ejemplar.getId() != idExcluido &&
            QString::compare(
                ejemplar.getCodigo().trimmed(),
                codigoLimpio,
                Qt::CaseInsensitive
                ) == 0) {
            throw std::invalid_argument("Ya existe un ejemplar con ese código.");
        }
    }

    return estadoValidado;
}
