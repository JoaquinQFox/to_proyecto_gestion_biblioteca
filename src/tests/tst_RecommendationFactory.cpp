#include "../factories/RecommendationFactory.h"
#include "../strategies/PopularityStrategy.h"
#include "../strategies/CategoryStrategy.h"
#include "../strategies/HistoryStrategy.h"

#include <QtTest>
#include <stdexcept>

class tst_RecommendationFactory : public QObject
{
    Q_OBJECT

private slots:
    void crearPopularidad();
    void crearCategoria();
    void crearHistorial();
    void tipoInvalidoLanzaExcepcion();
};

void tst_RecommendationFactory::crearPopularidad()
{
    auto estrategia = RecommendationFactory::crearEstrategia(TipoEstrategia::Popularidad);
    QVERIFY(estrategia != nullptr);
    QVERIFY(dynamic_cast<PopularityStrategy *>(estrategia.get()) != nullptr);
}

void tst_RecommendationFactory::crearCategoria()
{
    auto estrategia = RecommendationFactory::crearEstrategia(TipoEstrategia::Categoria);
    QVERIFY(estrategia != nullptr);
    QVERIFY(dynamic_cast<CategoryStrategy *>(estrategia.get()) != nullptr);
}

void tst_RecommendationFactory::crearHistorial()
{
    auto estrategia = RecommendationFactory::crearEstrategia(TipoEstrategia::Historial);
    QVERIFY(estrategia != nullptr);
    QVERIFY(dynamic_cast<HistoryStrategy *>(estrategia.get()) != nullptr);
}

void tst_RecommendationFactory::tipoInvalidoLanzaExcepcion()
{
    // ... un valor fuera del enum debe lanzar std::out_of_range
    QVERIFY_EXCEPTION_THROWN(
        RecommendationFactory::crearEstrategia(static_cast<TipoEstrategia>(99)),
        std::out_of_range);
}

QTEST_GUILESS_MAIN(tst_RecommendationFactory)
#include "tst_RecommendationFactory.moc"
