#include "CategoriaDialog.h"
#include "ui_CategoriaDialog.h"

#include <QFile>

CategoriaDialog::CategoriaDialog(QWidget *parent)
    : QDialog(parent),
    ui(new Ui::CategoriaDialog)
{
    ui->setupUi(this);

    connect(ui->btnCancelar, &QPushButton::clicked,
            this, &QDialog::reject);

    QFile archivo(":/styles/categoria_dialog.qss");

    if (archivo.open(QIODevice::ReadOnly | QIODevice::Text)) {
        setStyleSheet(QString::fromUtf8(archivo.readAll()));
    }

    setModal(true);
}

CategoriaDialog::~CategoriaDialog()
{
    delete ui;
}
