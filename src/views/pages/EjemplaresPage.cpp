#include "EjemplaresPage.h"
#include "ui_EjemplaresPage.h"

EjemplaresPage::EjemplaresPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::EjemplaresPage)
{
    ui->setupUi(this);
}

EjemplaresPage::~EjemplaresPage()
{
    delete ui;
}
