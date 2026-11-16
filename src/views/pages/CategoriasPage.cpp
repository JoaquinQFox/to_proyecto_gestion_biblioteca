#include "CategoriasPage.h"
#include "ui_CategoriasPage.h"

#include <QHeaderView>
#include <QAbstractItemView>

CategoriasPage::CategoriasPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::CategoriasPage)
{
    ui->setupUi(this);

    // 1. CONFIGURAR LA TABLA DE CATEGORÍAS
    ui->tablaCategorias->horizontalHeader()->setSectionResizeMode(
        QHeaderView::Stretch
        );

    ui->tablaCategorias->verticalHeader()->setVisible(false);

    ui->tablaCategorias->setSelectionBehavior(
        QAbstractItemView::SelectRows
        );

    ui->tablaCategorias->setEditTriggers(
        QAbstractItemView::NoEditTriggers
        );

    ui->tablaCategorias->setAlternatingRowColors(false);
}

CategoriasPage::~CategoriasPage()
{
    delete ui;
}
