#include "LectoresPage.h"
#include "ui_LectoresPage.h"
#include "../dialogs/LectorDialog.h"
#include <QPushButton>

#include <QHeaderView>
#include <QAbstractItemView>

LectoresPage::LectoresPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::LectoresPage)
{
    ui->setupUi(this);

    connect(ui->btnNuevoLector, &QPushButton::clicked,
            this, [this]() {
                LectorDialog dialog(this);
                dialog.exec();
            });

    // 1. CONFIGURAR LA TABLA DE LECTORES
    ui->tablaLectores->horizontalHeader()->setSectionResizeMode(
        QHeaderView::Stretch
        );

    ui->tablaLectores->verticalHeader()->setVisible(false);

    ui->tablaLectores->setSelectionBehavior(
        QAbstractItemView::SelectRows
        );

    ui->tablaLectores->setEditTriggers(
        QAbstractItemView::NoEditTriggers
        );

    ui->tablaLectores->setAlternatingRowColors(false);
}

LectoresPage::~LectoresPage()
{
    delete ui;
}
