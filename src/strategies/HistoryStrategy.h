#ifndef HISTORYSTRATEGY_H
#define HISTORYSTRATEGY_H

#include "../interfaces/IRecommendationStrategy.h"
#include "../repositories/LibroRepository.h"
#include "../repositories/EjemplarRepository.h"
#include "../repositories/PrestamoRepository.h"

class HistoryStrategy : public IRecommendationStrategy
{
public:
    QList<Libro> recomendar(int idLector, int idCategoria = 0) override;
    QString motivo() const override;

private:
    LibroRepository m_libroRepo;
    EjemplarRepository m_ejemplarRepo;
    PrestamoRepository m_prestamoRepo;

    QString m_motivo = "Basado en tus lecturas anteriores";
};

#endif // HISTORYSTRATEGY_H
