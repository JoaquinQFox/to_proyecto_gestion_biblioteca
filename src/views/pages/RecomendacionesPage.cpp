#include "RecomendacionesPage.h"
#include "ui_RecomendacionesPage.h"
#include "../dialogs/RecomendacionDialog.h"
#include <QPushButton>

#include <QHeaderView>
#include <QAbstractItemView>

RecomendacionesPage::RecomendacionesPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::RecomendacionesPage)
{
    ui->setupUi(this);

    connect(ui->btnGenerarRecomendaciones, &QPushButton::clicked,
            this, [this]() {
                RecomendacionDialog dialog(this);
                dialog.exec();
            });

    // 1. CONFIGURAR LA TABLA DE RECOMENDACIONES
    ui->tablaRecomendaciones->horizontalHeader()->setSectionResizeMode(
        QHeaderView::Stretch
        );

    ui->tablaRecomendaciones->verticalHeader()->setVisible(false);

    ui->tablaRecomendaciones->setSelectionBehavior(
        QAbstractItemView::SelectRows
        );

    ui->tablaRecomendaciones->setEditTriggers(
        QAbstractItemView::NoEditTriggers
        );

    ui->tablaRecomendaciones->setAlternatingRowColors(false);
}

RecomendacionesPage::~RecomendacionesPage()
{
    delete ui;
}
