#include "RecommendationFactory.h"
#include "../strategies/PopularityStrategy.h"
#include "../strategies/CategoryStrategy.h"
#include "../strategies/HistoryStrategy.h"

#include <stdexcept>

std::unique_ptr<IRecommendationStrategy> RecommendationFactory::crearEstrategia(TipoEstrategia tipo)
{
    switch (tipo) {
    case TipoEstrategia::Popularidad:
        return std::make_unique<PopularityStrategy>();
    case TipoEstrategia::Categoria:
        return std::make_unique<CategoryStrategy>();
    case TipoEstrategia::Historial:
        return std::make_unique<HistoryStrategy>();
    }

    throw std::out_of_range("Tipo de estrategia de recomendación no válido.");
}
