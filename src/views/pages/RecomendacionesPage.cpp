#include "RecomendacionesPage.h"
#include "ui_RecomendacionesPage.h"

RecomendacionesPage::RecomendacionesPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::RecomendacionesPage)
{
    ui->setupUi(this);
}

RecomendacionesPage::~RecomendacionesPage()
{
    delete ui;
}
