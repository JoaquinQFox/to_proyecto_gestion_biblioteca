
#include "PrestamoDialog.h"
#include "ui_PrestamoDialog.h"

#include <QFile>
#include <QPushButton>
#include <QDate>

PrestamoDialog::PrestamoDialog(QWidget *parent)
    : QDialog(parent),
    ui(new Ui::PrestamoDialog)
{
    ui->setupUi(this);

    ui->datePrestamo->setDate(QDate::currentDate());
    ui->dateDevolucion->setDate(QDate::currentDate().addDays(7));

    QFile archivo(":/styles/prestamo_dialog.qss");

    if (archivo.open(QIODevice::ReadOnly | QIODevice::Text)) {
        setStyleSheet(QString::fromUtf8(archivo.readAll()));
    }

    connect(ui->btnCancelar, &QPushButton::clicked,
            this, &QDialog::reject);

    setModal(true);
}

PrestamoDialog::~PrestamoDialog()
{
    delete ui;
}
