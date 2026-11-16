
#include "LectorDialog.h"
#include "ui_LectorDialog.h"

#include <QFile>
#include <QPushButton>

LectorDialog::LectorDialog(QWidget *parent)
    : QDialog(parent),
    ui(new Ui::LectorDialog)
{
    ui->setupUi(this);

    QFile archivo(":/styles/lector_dialog.qss");

    if (archivo.open(QIODevice::ReadOnly | QIODevice::Text)) {
        setStyleSheet(QString::fromUtf8(archivo.readAll()));
    }

    connect(ui->btnCancelar, &QPushButton::clicked,
            this, &QDialog::reject);

    setModal(true);
}

LectorDialog::~LectorDialog()
{
    delete ui;
}
