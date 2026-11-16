#include "CategoriasPage.h"
#include "ui_CategoriasPage.h"
#include "../dialogs/CategoriaDialog.h"
#include <QPushButton>

#include <QHeaderView>
#include <QAbstractItemView>

CategoriasPage::CategoriasPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::CategoriasPage)
{
    ui->setupUi(this);

    connect(ui->btnNuevaCategoria, &QPushButton::clicked,
            this, [this]() {
                CategoriaDialog dialog(this);
                dialog.exec();
            });

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
