#include "RecomendacionesPage.h"
#include "ui_RecomendacionesPage.h"

#include "../dialogs/RecomendacionDialog.h"
#include "../components/TablaHelper.h"

#include <QPushButton>

RecomendacionesPage::RecomendacionesPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::RecomendacionesPage)
{
    ui->setupUi(this);

    // Abrir formulario para generar recomendaciones
    connect(
        ui->btnGenerarRecomendaciones,
        &QPushButton::clicked,
        this,
        [this]() {
            RecomendacionDialog dialog(this);
            dialog.exec();
        }
        );

    // Configuración reutilizable de la tabla
    // No existe columna de acciones
    TablaHelper::configurar(ui->tablaRecomendaciones);
}

RecomendacionesPage::~RecomendacionesPage()
{
    delete ui;
}
