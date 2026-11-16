#include "InicioPage.h"
#include "ui_InicioPage.h"

#include <QHeaderView>
#include <QAbstractItemView>

InicioPage::InicioPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::InicioPage)
{
    ui->setupUi(this);

    // 1. CONFIGURAR LA TABLA DE PRÉSTAMOS RECIENTES
    ui->tablaPrestamosRecientes->horizontalHeader()->setSectionResizeMode(
        QHeaderView::Stretch
        );

    ui->tablaPrestamosRecientes->verticalHeader()->setVisible(false);

    ui->tablaPrestamosRecientes->setSelectionBehavior(
        QAbstractItemView::SelectRows
        );

    ui->tablaPrestamosRecientes->setEditTriggers(
        QAbstractItemView::NoEditTriggers
        );

    ui->tablaPrestamosRecientes->setAlternatingRowColors(false);
}

InicioPage::~InicioPage()
{
    delete ui;
}
