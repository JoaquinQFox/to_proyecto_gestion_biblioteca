
#include "LectorDialog.h"
#include "ui_LectorDialog.h"

#include <QFile>
#include <QDebug>
#include <QPushButton>
#include <QStringList>

LectorDialog::LectorDialog(QWidget *parent)
    : QDialog(parent),
    ui(new Ui::LectorDialog)
{
    ui->setupUi(this);

    // Cargar estilos comunes y específicos del formulario
    QString estilos;

    const QStringList archivosEstilos = {
        ":/styles/dialog_common.qss",
        ":/styles/lector_dialog.qss"
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

LectorDialog::~LectorDialog()
{
    delete ui;
}
