
#include "EjemplarDialog.h"
#include "ui_EjemplarDialog.h"

#include <QFile>
#include <QPushButton>

EjemplarDialog::EjemplarDialog(QWidget *parent)
    : QDialog(parent),
    ui(new Ui::EjemplarDialog)
{
    ui->setupUi(this);

    QFile archivo(":/styles/ejemplar_dialog.qss");

    if (archivo.open(QIODevice::ReadOnly | QIODevice::Text)) {
        setStyleSheet(QString::fromUtf8(archivo.readAll()));
    }

    connect(ui->btnCancelar, &QPushButton::clicked,
            this, &QDialog::reject);

    setModal(true);
}

EjemplarDialog::~EjemplarDialog()
{
    delete ui;
}
