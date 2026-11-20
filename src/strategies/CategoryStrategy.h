#ifndef CATEGORYSTRATEGY_H
#define CATEGORYSTRATEGY_H

#include "../interfaces/IRecommendationStrategy.h"
#include "../repositories/LibroRepository.h"

class CategoryStrategy : public IRecommendationStrategy
{
public:
    QList<Libro> recomendar(int idLector, int idCategoria = 0) override;
    QString motivo() const override;

private:
    LibroRepository m_libroRepo;
};

#endif // CATEGORYSTRATEGY_H
