#ifndef POPULARITYSTRATEGY_H
#define POPULARITYSTRATEGY_H

#include "../interfaces/IRecommendationStrategy.h"
#include "../repositories/LibroRepository.h"
#include "../repositories/EjemplarRepository.h"
#include "../repositories/PrestamoRepository.h"

class PopularityStrategy : public IRecommendationStrategy
{
public:
    QList<Libro> recomendar(int idLector, int idCategoria = 0) override;
    QString motivo() const override;

private:
    LibroRepository m_libroRepo;
    EjemplarRepository m_ejemplarRepo;
    PrestamoRepository m_prestamoRepo;
};

#endif // POPULARITYSTRATEGY_H
