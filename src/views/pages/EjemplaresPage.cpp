#include "EjemplaresPage.h"
#include "ui_EjemplaresPage.h"

#include <QHeaderView>
#include <QAbstractItemView>

EjemplaresPage::EjemplaresPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::EjemplaresPage)
{
    ui->setupUi(this);

    // 1. CONFIGURAR LA TABLA DE EJEMPLARES
    ui->tablaEjemplares->horizontalHeader()->setSectionResizeMode(
        QHeaderView::Stretch
        );

    ui->tablaEjemplares->verticalHeader()->setVisible(false);

    ui->tablaEjemplares->setSelectionBehavior(
        QAbstractItemView::SelectRows
        );

    ui->tablaEjemplares->setEditTriggers(
        QAbstractItemView::NoEditTriggers
        );

    ui->tablaEjemplares->setAlternatingRowColors(false);
}

EjemplaresPage::~EjemplaresPage()
{
    delete ui;
}
