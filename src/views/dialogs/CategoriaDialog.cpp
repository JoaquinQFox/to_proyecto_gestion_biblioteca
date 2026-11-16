
#include "CategoriaDialog.h"
#include "ui_CategoriaDialog.h"

#include <QFile>
#include <QDebug>
#include <QPushButton>
#include <QStringList>

CategoriaDialog::CategoriaDialog(QWidget *parent)
    : QDialog(parent),
    ui(new Ui::CategoriaDialog)
{
    ui->setupUi(this);

    connect(ui->btnCancelar, &QPushButton::clicked,
            this, &QDialog::reject);

    // Cargar estilos comunes y específicos del formulario
    QString estilos;

    const QStringList archivosEstilos = {
        ":/styles/dialog_common.qss",
        ":/styles/categoria_dialog.qss"
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

    setModal(true);
}

CategoriaDialog::~CategoriaDialog()
{
    delete ui;
}
