
#include "CategoriasPage.h"
#include "ui_CategoriasPage.h"

CategoriasPage::CategoriasPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::CategoriasPage)
{
    ui->setupUi(this);
}

CategoriasPage::~CategoriasPage()
{
    delete ui;
}
