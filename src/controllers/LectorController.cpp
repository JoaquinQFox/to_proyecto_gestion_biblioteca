#include "LectorController.h"

#include <QRegularExpression>
#include <QStringList>
#include <stdexcept>

QList<Lector> LectorController::listar()
{
    return repository.obtenerActivos();
}

Lector LectorController::obtenerPorId(int id)
{
    if (id <= 0) {
        throw std::invalid_argument("El ID del lector no es válido.");
    }

    return repository.obtenerPorId(id);
}

int LectorController::crear(
    const QString &nombreCompleto,
    const QString &dni,
    const QString &correo,
    const QString &telefono
    )
{
    validarDatos(nombreCompleto, correo, telefono);
    validarDni(dni);

    const QStringList partes =
        nombreCompleto.simplified().split(' ', Qt::SkipEmptyParts);
    const QString nombres = partes.first();
    const QString apellidos = partes.mid(1).join(' ');

    return repository.agregar(
        Lector(
            0,
            dni.trimmed(),
            nombres,
            apellidos,
            telefono.trimmed(),
            QString(),
            true,
            correo.trimmed()
            )
        );
}

void LectorController::actualizar(
    int id,
    const QString &nombreCompleto,
    const QString &dni,
    const QString &correo,
    const QString &telefono
    )
{
    if (id <= 0) {
        throw std::invalid_argument("El ID del lector no es válido.");
    }

    validarDatos(nombreCompleto, correo, telefono);
    validarDni(dni, id);

    const QStringList partes =
        nombreCompleto.simplified().split(' ', Qt::SkipEmptyParts);

    Lector lector = repository.obtenerPorId(id);
    lector.setNombres(partes.first());
    lector.setApellidos(partes.mid(1).join(' '));
    lector.setDni(dni.trimmed());
    lector.setCorreo(correo.trimmed());
    lector.setTelefono(telefono.trimmed());

    if (!repository.actualizar(lector)) {
        throw std::invalid_argument("No se encontró el lector que deseas editar.");
    }
}

void LectorController::eliminar(int id)
{
    if (id <= 0) {
        throw std::invalid_argument("El ID del lector no es válido.");
    }

    if (!repository.eliminar(id)) {
        throw std::invalid_argument("No se encontró el lector que deseas eliminar.");
    }
}

void LectorController::validarDni(const QString &dni, int idExcluido)
{
    const QString dniLimpio = dni.trimmed();
    static const QRegularExpression formatoDni("^[0-9]{8}$");
    if (!formatoDni.match(dniLimpio).hasMatch()) {
        throw std::invalid_argument("El DNI debe contener exactamente 8 dígitos.");
    }

    const QList<Lector> lectores = repository.obtenerTodos();
    for (const Lector &lector : lectores) {
        if (lector.getId() != idExcluido &&
            lector.getDni().trimmed() == dniLimpio) {
            throw std::invalid_argument("Ya existe un lector con ese DNI.");
        }
    }
}

void LectorController::validarDatos(
    const QString &nombreCompleto,
    const QString &correo,
    const QString &telefono
    )
{
    const QString nombreLimpio = nombreCompleto.simplified();
    const QString correoLimpio = correo.trimmed();
    const QString telefonoLimpio = telefono.trimmed();

    const QStringList partes =
        nombreLimpio.split(' ', Qt::SkipEmptyParts);
    if (partes.size() < 2) {
        throw std::invalid_argument(
            "Ingresa nombres y apellidos del lector."
            );
    }
    if (nombreLimpio.size() > 100) {
        throw std::invalid_argument(
            "El nombre completo no puede superar los 100 caracteres."
            );
    }

    static const QRegularExpression formatoCorreo(
        R"(^[A-Za-z0-9._%+\-]+@[A-Za-z0-9.\-]+\.[A-Za-z]{2,}$)"
        );
    if (!formatoCorreo.match(correoLimpio).hasMatch()) {
        throw std::invalid_argument("Ingresa un correo electrónico válido.");
    }

    static const QRegularExpression formatoTelefono("^[0-9]{9}$");
    if (!formatoTelefono.match(telefonoLimpio).hasMatch()) {
        throw std::invalid_argument(
            "El teléfono debe contener exactamente 9 dígitos."
            );
    }
}
