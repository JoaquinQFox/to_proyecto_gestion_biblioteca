#include "InicioPage.h"
#include "ui_InicioPage.h"

InicioPage::InicioPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::InicioPage)
{
    ui->setupUi(this);
}

InicioPage::~InicioPage()
{
    delete ui;
}
