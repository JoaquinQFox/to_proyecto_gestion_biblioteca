
#include "PrestamoDialog.h"
#include "ui_PrestamoDialog.h"

#include <QFile>
#include <QDebug>
#include <QPushButton>
#include <QDate>
#include <QStringList>

PrestamoDialog::PrestamoDialog(QWidget *parent)
    : QDialog(parent),
    ui(new Ui::PrestamoDialog)
{
    ui->setupUi(this);

    // Configurar fechas predeterminadas
    ui->datePrestamo->setDate(QDate::currentDate());
    ui->dateDevolucion->setDate(QDate::currentDate().addDays(7));

    // Cargar estilos comunes y específicos del formulario
    QString estilos;

    const QStringList archivosEstilos = {
        ":/styles/dialog_common.qss",
        ":/styles/prestamo_dialog.qss"
    };

    for (const QString &ruta : archivosEstilos) {
        QFile archivo(ruta);

        if (archivo.open(QIODevice::ReadOnly | QIODevice::Text)) {
            estilos += QString::fromUtf8(archivo.readAll());
            estilos += "\n";
        } else {
            qWarning() << "No se pudo cargar:" << ruta;
        }
    }

    setStyleSheet(estilos);

    connect(ui->btnCancelar, &QPushButton::clicked,
            this, &QDialog::reject);

    setModal(true);
}

PrestamoDialog::~PrestamoDialog()
{
    delete ui;
}
