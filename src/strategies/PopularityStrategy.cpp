#include "PopularityStrategy.h"

#include <QHash>
#include <algorithm>
#include <iterator>

QList<Libro> PopularityStrategy::recomendar(int idLector, int idCategoria)
{
    Q_UNUSED(idLector);

    QHash<int, int> libroPorEjemplar;
    for (const Ejemplar &e : m_ejemplarRepo.obtenerTodos()) {
        libroPorEjemplar.insert(e.getId(), e.getIdLibro());
    }

    // cantidad de préstamos históricos por libro
    QHash<int, int> conteo;
    for (const Prestamo &p : m_prestamoRepo.obtenerTodos()) {
        ++conteo[libroPorEjemplar.value(p.getIdEjemplar())];
    }

    // libros activos
    const QList<Libro> activos = m_libroRepo.obtenerActivos();
    QList<Libro> resultado;
    std::copy_if(activos.begin(), activos.end(), std::back_inserter(resultado),
                 [idCategoria](const Libro &l) {
                     return idCategoria <= 0 || l.getIdCategoria() == idCategoria;
                 });

    // más prestados primero
    std::sort(resultado.begin(), resultado.end(),
              [&conteo](const Libro &a, const Libro &b) {
                  const int ca = conteo.value(a.getId());
                  const int cb = conteo.value(b.getId());
                  if (ca != cb) {
                      return ca > cb;
                  }
                  return a.getTitulo() < b.getTitulo();
              });

    return resultado;
}

QString PopularityStrategy::motivo() const
{
    return "Entre los más prestados";
}
