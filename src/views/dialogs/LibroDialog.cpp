
#include "LibroDialog.h"
#include "ui_LibroDialog.h"

#include <QFile>
#include <QDebug>
#include <QPushButton>
#include <QStringList>

LibroDialog::LibroDialog(QWidget *parent)
    : QDialog(parent),
    ui(new Ui::LibroDialog)
{
    ui->setupUi(this);

    // Cargar estilos comunes y específicos del formulario
    QString estilos;

    const QStringList archivosEstilos = {
        ":/styles/dialog_common.qss",
        ":/styles/libro_dialog.qss"
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

LibroDialog::~LibroDialog()
{
    delete ui;
}
