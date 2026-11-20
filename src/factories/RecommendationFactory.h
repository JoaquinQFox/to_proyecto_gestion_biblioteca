#ifndef RECOMMENDATIONFACTORY_H
#define RECOMMENDATIONFACTORY_H

#include "../interfaces/IRecommendationStrategy.h"

#include <memory>

enum class TipoEstrategia {
    Popularidad,
    Categoria,
    Historial
};

class RecommendationFactory
{
public:
    // lanza std::out_of_range si el tipo no es válido
    static std::unique_ptr<IRecommendationStrategy> crearEstrategia(TipoEstrategia tipo);
};

#endif // RECOMMENDATIONFACTORY_H
