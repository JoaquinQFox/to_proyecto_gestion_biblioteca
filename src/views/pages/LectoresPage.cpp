#include "LectoresPage.h"
#include "ui_LectoresPage.h"

LectoresPage::LectoresPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::LectoresPage)
{
    ui->setupUi(this);
}

LectoresPage::~LectoresPage()
{
    delete ui;
}
