
#include "TablaHelper.h"

#include <QTableWidget>
#include <QHeaderView>
#include <QAbstractItemView>

void TablaHelper::configurar(
    QTableWidget *tabla,
    int columnaAcciones,
    int anchoAcciones
    )
{
    if (!tabla) {
        return;
    }

    // Encabezados
    auto *encabezado = tabla->horizontalHeader();

    encabezado->setSectionResizeMode(
        QHeaderView::Stretch
        );

    encabezado->setDefaultAlignment(
        Qt::AlignCenter
        );

    encabezado->setMinimumSectionSize(65);
    encabezado->setHighlightSections(false);

    // Ocultar numeración vertical
    tabla->verticalHeader()->setVisible(false);

    // Selección de filas completas
    tabla->setSelectionBehavior(
        QAbstractItemView::SelectRows
        );

    tabla->setSelectionMode(
        QAbstractItemView::SingleSelection
        );

    // Evitar edición directa de celdas
    tabla->setEditTriggers(
        QAbstractItemView::NoEditTriggers
        );

    // Apariencia
    tabla->setAlternatingRowColors(false);
    tabla->setShowGrid(false);
    tabla->setWordWrap(false);

    // Desplazamiento fluido
    tabla->setHorizontalScrollMode(
        QAbstractItemView::ScrollPerPixel
        );

    tabla->setVerticalScrollMode(
        QAbstractItemView::ScrollPerPixel
        );

    // Altura predeterminada de filas
    tabla->verticalHeader()->setDefaultSectionSize(52);

    // Columna de acciones, cuando exista
    if (columnaAcciones >= 0 &&
        columnaAcciones < tabla->columnCount()) {

        encabezado->setSectionResizeMode(
            columnaAcciones,
            QHeaderView::Fixed
            );

        tabla->setColumnWidth(
            columnaAcciones,
            anchoAcciones
            );
    }
}

void TablaHelper::ajustarFilas(
    QTableWidget *tabla,
    int altura
    )
{
    if (!tabla || altura <= 0) {
        return;
    }

    for (int fila = 0; fila < tabla->rowCount(); ++fila) {
        tabla->setRowHeight(fila, altura);
    }
}
