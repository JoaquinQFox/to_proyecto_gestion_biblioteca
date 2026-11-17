#include "CategoriaDialog.h"
#include "ui_CategoriaDialog.h"

#include <QFile>
#include <QDebug>
#include <QPushButton>
#include <QStringList>
#include <QLabel>
#include <QVBoxLayout>
#include <QStyle>

CategoriaDialog::CategoriaDialog(QWidget *parent)
    : QDialog(parent),
    ui(new Ui::CategoriaDialog)
{
    ui->setupUi(this);

    // Configuración del campo
    ui->txtNombre->setMaxLength(60);

    // Mensaje de validación
    auto *lblError = new QLabel(this);
    lblError->setObjectName("lblErrorValidacion");
    lblError->setWordWrap(true);
    lblError->hide();

    ui->layoutPrincipal->insertWidget(5, lblError);

    connect(ui->btnCancelar, &QPushButton::clicked,
            this, &QDialog::reject);

    // Validar al guardar
    connect(ui->btnGuardar, &QPushButton::clicked,
            this, [this]() {

                if (!validarFormulario()) {
                    return;
                }

                emit guardarSolicitado(
                    ui->txtNombre->text().trimmed()
                    );
            });

    // Limpiar error mientras se escribe
    connect(ui->txtNombre, &QLineEdit::textChanged,
            this, [this]() {
                limpiarError();
            });

    // Cargar estilos
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

bool CategoriaDialog::validarFormulario()
{
    const QString nombre = ui->txtNombre->text().trimmed();

    if (nombre.isEmpty()) {
        mostrarError("Ingresa el nombre de la categoría.");
        ui->txtNombre->setFocus();
        return false;
    }

    limpiarError();

    return true;
}

void CategoriaDialog::mostrarError(const QString &mensaje)
{
    auto *lblError = findChild<QLabel *>(
        "lblErrorValidacion"
        );

    if (lblError) {
        lblError->setText(mensaje);
        lblError->show();
    }

    ui->txtNombre->setProperty("error", true);
    ui->txtNombre->style()->unpolish(ui->txtNombre);
    ui->txtNombre->style()->polish(ui->txtNombre);
}

void CategoriaDialog::limpiarError()
{
    auto *lblError = findChild<QLabel *>(
        "lblErrorValidacion"
        );

    if (lblError) {
        lblError->clear();
        lblError->hide();
    }

    ui->txtNombre->setProperty("error", false);
    ui->txtNombre->style()->unpolish(ui->txtNombre);
    ui->txtNombre->style()->polish(ui->txtNombre);
}