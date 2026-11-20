#ifndef RECOMENDACIONCONTROLLER_H
#define RECOMENDACIONCONTROLLER_H

#include "../factories/RecommendationFactory.h"
#include "../models/Lector.h"
#include "../models/Categoria.h"
#include "../repositories/LectorRepository.h"
#include "../repositories/CategoriaRepository.h"

#include <QObject>
#include <QList>
#include <QString>
#include <QFutureWatcher>

// ... una fila de la tabla de recomendaciones
struct RecomendacionItem {
    int idLibro = 0;
    QString titulo;
    QString autor;
    QString categoria;
    QString motivo;
};

// ... resultado del hilo secundario (error vacío = todo bien)
struct ResultadoRecomendacion {
    QList<RecomendacionItem> items;
    QString error;
};

class RecomendacionController : public QObject
{
    Q_OBJECT

public:
    explicit RecomendacionController(QObject *parent = nullptr);

    QList<Lector> listarLectores();
    QList<Categoria> listarCategorias();

    // ... lanza std::invalid_argument si los datos no son válidos
    void generarAsync(TipoEstrategia tipo, int idLector, int idCategoria, int cantidad);

    bool estaOcupado() const;

    // ... cálculo completo; se ejecuta en el hilo secundario
    static ResultadoRecomendacion calcular(TipoEstrategia tipo, int idLector,
                                           int idCategoria, int cantidad);

signals:
    void generacionIniciada();
    void recomendacionesListas(const QList<RecomendacionItem> &items);
    void errorOcurrido(const QString &mensaje);

private:
    LectorRepository m_lectorRepo;
    CategoriaRepository m_categoriaRepo;
    QFutureWatcher<ResultadoRecomendacion> m_watcher;
};

#endif // RECOMENDACIONCONTROLLER_H
