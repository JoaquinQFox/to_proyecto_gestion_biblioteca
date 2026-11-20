#include "RecomendacionController.h"
#include "../database/Database.h"
#include "../repositories/LibroRepository.h"

#include <QCoreApplication>
#include <QHash>
#include <QThread>
#include <QtConcurrent>
#include <exception>
#include <stdexcept>

RecomendacionController::RecomendacionController(QObject *parent)
    : QObject(parent)
{
    // ... se ejecuta en el hilo de la interfaz cuando termina el hilo secundario
    connect(&m_watcher, &QFutureWatcher<ResultadoRecomendacion>::finished,
            this, [this]() {
                const ResultadoRecomendacion resultado = m_watcher.result();
                if (resultado.error.isEmpty()) {
                    emit recomendacionesListas(resultado.items);
                } else {
                    emit errorOcurrido(resultado.error);
                }
            });
}

QList<Lector> RecomendacionController::listarLectores()
{
    return m_lectorRepo.obtenerActivos();
}

QList<Categoria> RecomendacionController::listarCategorias()
{
    return m_categoriaRepo.obtenerTodos();
}

bool RecomendacionController::estaOcupado() const
{
    return m_watcher.isRunning();
}

void RecomendacionController::generarAsync(TipoEstrategia tipo, int idLector,
                                           int idCategoria, int cantidad)
{
    if (estaOcupado()) {
        throw std::logic_error("Ya se está generando una recomendación.");
    }
    if (idLector <= 0) {
        throw std::invalid_argument("Debe seleccionar un lector válido.");
    }
    if (cantidad < 1 || cantidad > 20) {
        throw std::invalid_argument("La cantidad debe estar entre 1 y 20.");
    }
    if (tipo == TipoEstrategia::Categoria && idCategoria <= 0) {
        throw std::invalid_argument("Debe seleccionar una categoría.");
    }

    emit generacionIniciada();

    // ... el cálculo corre en un hilo del pool para no congelar la GUI
    m_watcher.setFuture(QtConcurrent::run([=]() {
        return RecomendacionController::calcular(tipo, idLector, idCategoria, cantidad);
    }));
}

ResultadoRecomendacion RecomendacionController::calcular(TipoEstrategia tipo, int idLector,
                                                         int idCategoria, int cantidad)
{
    ResultadoRecomendacion resultado;

    // ... conexión propia solo si estamos fuera del hilo principal
    const bool enHiloSecundario =
        QThread::currentThread() != QCoreApplication::instance()->thread();
    const bool conexionPropia = enHiloSecundario && Database::abrirConexionHilo();

    try {
        auto estrategia = RecommendationFactory::crearEstrategia(tipo);
        const QList<Libro> libros = estrategia->recomendar(idLector, idCategoria);

        CategoriaRepository categoriaRepo;
        QHash<int, QString> nombres;
        for (const Categoria &c : categoriaRepo.obtenerTodos()) {
            nombres.insert(c.getId(), c.getNombre());
        }

        // ... se recorta a la cantidad pedida
        const int limite = qMin(cantidad, static_cast<int>(libros.size()));
        for (int i = 0; i < limite; ++i) {
            RecomendacionItem item;
            item.idLibro = libros[i].getId();
            item.titulo = libros[i].getTitulo();
            item.autor = libros[i].getAutor();
            item.categoria = nombres.value(libros[i].getIdCategoria());
            item.motivo = estrategia->motivo();
            resultado.items.append(item);
        }
    } catch (const std::exception &e) {
        resultado.error = QString::fromUtf8(e.what());
    }

    if (conexionPropia) {
        Database::cerrarConexionHilo();
    }
    return resultado;
}
