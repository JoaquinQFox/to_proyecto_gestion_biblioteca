
#include "RecomendacionDialog.h"
#include "ui_RecomendacionDialog.h"

#include <QFile>
#include <QPushButton>

RecomendacionDialog::RecomendacionDialog(QWidget *parent)
    : QDialog(parent),
    ui(new Ui::RecomendacionDialog)
{
    ui->setupUi(this);

    QFile archivo(":/styles/recomendacion_dialog.qss");

    if (archivo.open(QIODevice::ReadOnly | QIODevice::Text)) {
        setStyleSheet(QString::fromUtf8(archivo.readAll()));
    }

    connect(ui->btnCancelar, &QPushButton::clicked,
            this, &QDialog::reject);

    setModal(true);
}

RecomendacionDialog::~RecomendacionDialog()
{
    delete ui;
}
