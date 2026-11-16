
#include "RecomendacionDialog.h"
#include "ui_RecomendacionDialog.h"

#include <QFile>
#include <QDebug>
#include <QPushButton>
#include <QStringList>

RecomendacionDialog::RecomendacionDialog(QWidget *parent)
    : QDialog(parent),
    ui(new Ui::RecomendacionDialog)
{
    ui->setupUi(this);

    // Cargar estilos comunes y específicos del formulario
    QString estilos;

    const QStringList archivosEstilos = {
        ":/styles/dialog_common.qss",
        ":/styles/recomendacion_dialog.qss"
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

RecomendacionDialog::~RecomendacionDialog()
{
    delete ui;
}
