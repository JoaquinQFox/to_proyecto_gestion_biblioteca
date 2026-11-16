#include "LibrosPage.h"
#include "ui_LibrosPage.h"

LibrosPage::LibrosPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::LibrosPage)
{
    ui->setupUi(this);
}

LibrosPage::~LibrosPage()
{
    delete ui;
}
