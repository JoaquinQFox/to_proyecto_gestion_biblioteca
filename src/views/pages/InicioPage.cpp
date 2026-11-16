
#include "InicioPage.h"
#include "ui_InicioPage.h"

#include "../components/TablaHelper.h"

InicioPage::InicioPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::InicioPage)
{
    ui->setupUi(this);

    // Configuración reutilizable de la tabla
    // de préstamos recientes.
    // No contiene columna de acciones.
    TablaHelper::configurar(ui->tablaPrestamosRecientes);
}

InicioPage::~InicioPage()
{
    delete ui;
}
