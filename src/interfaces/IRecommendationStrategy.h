#ifndef IRECOMMENDATIONSTRATEGY_H
#define IRECOMMENDATIONSTRATEGY_H

#include "../models/Libro.h"

#include <QList>
#include <QString>

class IRecommendationStrategy
{
public:
    virtual ~IRecommendationStrategy() = default;

    // idCategoria = 0 significa todas las categorías
    virtual QList<Libro> recomendar(int idLector, int idCategoria = 0) = 0;

    virtual QString motivo() const = 0;
};

#endif // IRECOMMENDATIONSTRATEGY_H
