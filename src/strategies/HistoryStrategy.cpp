#include "HistoryStrategy.h"
#include "PopularityStrategy.h"

#include <QHash>
#include <QSet>
#include <algorithm>
#include <iterator>

QList<Libro> HistoryStrategy::recomendar(int idLector, int idCategoria)
{
    QHash<int, int> libroPorEjemplar;
    for (const Ejemplar &e : m_ejemplarRepo.obtenerTodos()) {
        libroPorEjemplar.insert(e.getId(), e.getIdLibro());
    }

    QHash<int, int> categoriaPorLibro;
    for (const Libro &l : m_libroRepo.obtenerTodos()) {
        categoriaPorLibro.insert(l.getId(), l.getIdCategoria());
    }

    // libros leídos y categorías preferidas del lector
    QSet<int> leidos;
    QHash<int, int> conteoCategorias;
    for (const Prestamo &p : m_prestamoRepo.obtenerTodos()) {
        if (p.getIdLector() != idLector) {
            continue;
        }
        const int idLibro = libroPorEjemplar.value(p.getIdEjemplar());
        leidos.insert(idLibro);
        ++conteoCategorias[categoriaPorLibro.value(idLibro)];
    }

    // sin historial se recomienda por popularidad
    if (leidos.isEmpty()) {
        PopularityStrategy popularidad;
        m_motivo = "Popular (aún no tienes historial)";
        return popularidad.recomendar(idLector, idCategoria);
    }
    m_motivo = "Basado en tus lecturas anteriores";

    // libros nuevos de las categorías preferidas
    const QList<Libro> activos = m_libroRepo.obtenerActivos();
    QList<Libro> resultado;
    std::copy_if(activos.begin(), activos.end(), std::back_inserter(resultado),
                 [&](const Libro &l) {
                     return conteoCategorias.contains(l.getIdCategoria())
                            && !leidos.contains(l.getId())
                            && (idCategoria <= 0 || l.getIdCategoria() == idCategoria);
                 });

    // categoría más leída primero
    std::sort(resultado.begin(), resultado.end(),
              [&conteoCategorias](const Libro &a, const Libro &b) {
                  const int ca = conteoCategorias.value(a.getIdCategoria());
                  const int cb = conteoCategorias.value(b.getIdCategoria());
                  if (ca != cb) {
                      return ca > cb;
                  }
                  return a.getTitulo() < b.getTitulo();
              });

    return resultado;
}

QString HistoryStrategy::motivo() const
{
    return m_motivo;
}
