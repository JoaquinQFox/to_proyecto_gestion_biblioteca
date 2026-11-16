#include "PrestamosPage.h"
#include "ui_PrestamosPage.h"

PrestamosPage::PrestamosPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PrestamosPage)
{
    ui->setupUi(this);
}

PrestamosPage::~PrestamosPage()
{
    delete ui;
}
