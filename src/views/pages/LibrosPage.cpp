#include "LibrosPage.h"
#include "ui_LibrosPage.h"
#include "../dialogs/LibroDialog.h"
#include <QPushButton>

#include <QHeaderView>
#include <QAbstractItemView>

LibrosPage::LibrosPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::LibrosPage)
{
    ui->setupUi(this);

    connect(ui->btnNuevoLibro, &QPushButton::clicked,
            this, [this]() {
                LibroDialog dialog(this);
                dialog.exec();
            });

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
