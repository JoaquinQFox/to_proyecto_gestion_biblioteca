#include "PrestamosPage.h"
#include "ui_PrestamosPage.h"
#include "../dialogs/PrestamoDialog.h"
#include <QPushButton>

#include <QHeaderView>
#include <QAbstractItemView>

PrestamosPage::PrestamosPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PrestamosPage)
{
    ui->setupUi(this);

    connect(ui->btnNuevoPrestamo, &QPushButton::clicked,
            this, [this]() {
                PrestamoDialog dialog(this);
                dialog.exec();
            });

    // 1. CONFIGURAR LA TABLA DE PRÉSTAMOS
    ui->tablaPrestamos->horizontalHeader()->setSectionResizeMode(
        QHeaderView::Stretch
        );

    ui->tablaPrestamos->verticalHeader()->setVisible(false);

    ui->tablaPrestamos->setSelectionBehavior(
        QAbstractItemView::SelectRows
        );

    ui->tablaPrestamos->setEditTriggers(
        QAbstractItemView::NoEditTriggers
        );

    ui->tablaPrestamos->setAlternatingRowColors(false);

    // 2. CONFIGURAR EL TAMAÑO DE LOS ENCABEZADOS
    ui->tablaPrestamos->horizontalHeader()->setMinimumSectionSize(65);

    ui->tablaPrestamos->horizontalHeader()->setDefaultAlignment(
        Qt::AlignCenter
        );
}

PrestamosPage::~PrestamosPage()
{
    delete ui;
}
