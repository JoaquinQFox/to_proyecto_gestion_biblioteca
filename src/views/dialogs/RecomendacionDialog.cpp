
#include "RecomendacionDialog.h"
#include "ui_RecomendacionDialog.h"

#include <QFile>
#include <QDebug>
#include <QPushButton>
#include <QComboBox>
#include <QSpinBox>
#include <QLabel>
#include <QStyle>
#include <QStringList>
#include <QWidget>

RecomendacionDialog::RecomendacionDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::RecomendacionDialog)
{
    ui->setupUi(this);

    // Configurar cantidad permitida
    ui->spinCantidad->setRange(1, 20);
    ui->spinCantidad->setValue(5);

    // Crear etiquetas de validación
    lblErrorLector = new QLabel(this);
    lblErrorCantidad = new QLabel(this);

    lblErrorLector->setObjectName("lblErrorLector");
    lblErrorCantidad->setObjectName("lblErrorCantidad");

    lblErrorLector->setWordWrap(true);
    lblErrorCantidad->setWordWrap(true);

    lblErrorLector->hide();
    lblErrorCantidad->hide();

    // Insertar mensajes debajo de cada campo
    auto insertarError = [this](
                             QWidget *campo,
                             QLabel *etiqueta
                             ) {
        const int indice =
            ui->layoutPrincipal->indexOf(campo);

        if (indice >= 0) {
            ui->layoutPrincipal->insertWidget(
                indice + 1,
                etiqueta
                );
        }
    };

    insertarError(ui->cmbLector, lblErrorLector);

    insertarError(
        ui->spinCantidad,
        lblErrorCantidad
        );

    // Cancelar
    connect(
        ui->btnCancelar,
        &QPushButton::clicked,
        this,
        &QDialog::reject
        );

    // Generar: únicamente validación visual
    connect(
        ui->btnGenerar,
        &QPushButton::clicked,
        this,
        [this]() {
            validarFormulario();
        }
        );

    // Limpiar error al seleccionar lector
    connect(
        ui->cmbLector,
        QOverload<int>::of(
            &QComboBox::currentIndexChanged
            ),
        this,
        [this](int) {
            limpiarError(
                lblErrorLector,
                ui->cmbLector
                );
        }
        );

    // Limpiar error al modificar cantidad
    connect(
        ui->spinCantidad,
        QOverload<int>::of(
            &QSpinBox::valueChanged
            ),
        this,
        [this](int) {
            limpiarError(
                lblErrorCantidad,
                ui->spinCantidad
                );
        }
        );

    // Cargar estilos QSS
    QString estilos;

    const QStringList archivosEstilos = {
        ":/styles/dialog_common.qss",
        ":/styles/recomendacion_dialog.qss"
    };

    for (const QString &ruta : archivosEstilos) {
        QFile archivo(ruta);

        if (archivo.open(
                QIODevice::ReadOnly | QIODevice::Text
                )) {
            estilos += QString::fromUtf8(
                archivo.readAll()
                );

            estilos += "\n";
        } else {
            qWarning() << "No se pudo cargar:" << ruta;
        }
    }

    setStyleSheet(estilos);
    setModal(true);
}

RecomendacionDialog::~RecomendacionDialog()
{
    delete ui;
}

bool RecomendacionDialog::validarFormulario()
{
    bool valido = true;
    QWidget *primerCampoInvalido = nullptr;

    // Validar lector obligatorio
    if (ui->cmbLector->currentIndex() <= 0) {
        mostrarError(
            lblErrorLector,
            "Selecciona un lector para generar recomendaciones.",
            ui->cmbLector
            );

        valido = false;
        primerCampoInvalido = ui->cmbLector;
    } else {
        limpiarError(
            lblErrorLector,
            ui->cmbLector
            );
    }

    // Validar cantidad
    const int cantidad =
        ui->spinCantidad->value();

    if (cantidad < 1 || cantidad > 20) {
        mostrarError(
            lblErrorCantidad,
            "La cantidad debe estar entre 1 y 20.",
            ui->spinCantidad
            );

        if (!primerCampoInvalido) {
            primerCampoInvalido = ui->spinCantidad;
        }

        valido = false;
    } else {
        limpiarError(
            lblErrorCantidad,
            ui->spinCantidad
            );
    }

    // Categoría opcional:
    // índice 0 significa todas las categorías.

    if (primerCampoInvalido) {
        primerCampoInvalido->setFocus();
    }

    return valido;
}

void RecomendacionDialog::mostrarError(
    QLabel *etiqueta,
    const QString &mensaje,
    QWidget *campo
    )
{
    etiqueta->setText(mensaje);
    etiqueta->show();

    campo->setProperty("error", true);
    campo->style()->unpolish(campo);
    campo->style()->polish(campo);
    campo->update();
}

void RecomendacionDialog::limpiarError(
    QLabel *etiqueta,
    QWidget *campo
    )
{
    etiqueta->clear();
    etiqueta->hide();

    campo->setProperty("error", false);
    campo->style()->unpolish(campo);
    campo->style()->polish(campo);
    campo->update();
}
