#include "CategoryStrategy.h"

#include <algorithm>
#include <iterator>
#include <stdexcept>

QList<Libro> CategoryStrategy::recomendar(int idLector, int idCategoria)
{
    Q_UNUSED(idLector);

    if (idCategoria <= 0) {
        throw std::invalid_argument("Debe seleccionar una categoría.");
    }

    // solo libros activos de la categoría elegida
    const QList<Libro> activos = m_libroRepo.obtenerActivos();
    QList<Libro> resultado;
    std::copy_if(activos.begin(), activos.end(), std::back_inserter(resultado),
                 [idCategoria](const Libro &l) {
                     return l.getIdCategoria() == idCategoria;
                 });

    std::sort(resultado.begin(), resultado.end(),
              [](const Libro &a, const Libro &b) {
                  return a.getTitulo() < b.getTitulo();
              });

    return resultado;
}

QString CategoryStrategy::motivo() const
{
    return "Pertenece a la categoría elegida";
}
