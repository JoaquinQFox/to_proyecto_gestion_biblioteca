#include "LibroDialog.h"
#include "ui_LibroDialog.h"

#include <QFile>
#include <QPushButton>

LibroDialog::LibroDialog(QWidget *parent)
    : QDialog(parent),
    ui(new Ui::LibroDialog)
{
    ui->setupUi(this);

    QFile archivo(":/styles/libro_dialog.qss");

    if (archivo.open(QIODevice::ReadOnly | QIODevice::Text)) {
        setStyleSheet(QString::fromUtf8(archivo.readAll()));
    }

    connect(ui->btnCancelar, &QPushButton::clicked,
            this, &QDialog::reject);

    setModal(true);
}

LibroDialog::~LibroDialog()
{
    delete ui;
}
