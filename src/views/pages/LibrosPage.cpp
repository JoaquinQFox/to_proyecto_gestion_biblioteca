#include "LibrosPage.h"
#include "ui_LibrosPage.h"

#include <QHeaderView>
#include <QAbstractItemView>

LibrosPage::LibrosPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::LibrosPage)
{
    ui->setupUi(this);

    // 1. CONFIGURAR LA TABLA DE LIBROS
    ui->tablaLibros->horizontalHeader()->setSectionResizeMode(
        QHeaderView::Stretch
        );

    ui->tablaLibros->verticalHeader()->setVisible(false);

    ui->tablaLibros->setSelectionBehavior(
        QAbstractItemView::SelectRows
        );

    ui->tablaLibros->setEditTriggers(
        QAbstractItemView::NoEditTriggers
        );

    ui->tablaLibros->setAlternatingRowColors(false);
}

LibrosPage::~LibrosPage()
{
    delete ui;
}
