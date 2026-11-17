
#include "PrestamoDialog.h"
#include "ui_PrestamoDialog.h"

#include <QFile>
#include <QDebug>
#include <QPushButton>
#include <QDate>
#include <QDateEdit>
#include <QComboBox>
#include <QLabel>
#include <QStyle>
#include <QStringList>
#include <QList>
#include <QWidget>

PrestamoDialog::PrestamoDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::PrestamoDialog)
{
    ui->setupUi(this);

    // Fechas predeterminadas
    ui->datePrestamo->setDate(QDate::currentDate());

    ui->dateDevolucion->setDate(
        QDate::currentDate().addDays(7)
        );

    // Crear mensajes de validación
    lblErrorLector = new QLabel(this);
    lblErrorEjemplar = new QLabel(this);
    lblErrorFechaPrestamo = new QLabel(this);
    lblErrorFechaDevolucion = new QLabel(this);

    lblErrorLector->setObjectName("lblErrorLector");
    lblErrorEjemplar->setObjectName("lblErrorEjemplar");

    lblErrorFechaPrestamo->setObjectName(
        "lblErrorFechaPrestamo"
        );

    lblErrorFechaDevolucion->setObjectName(
        "lblErrorFechaDevolucion"
        );

    const QList<QLabel *> etiquetas = {
        lblErrorLector,
        lblErrorEjemplar,
        lblErrorFechaPrestamo,
        lblErrorFechaDevolucion
    };

    for (QLabel *etiqueta : etiquetas) {
        etiqueta->setWordWrap(true);
        etiqueta->hide();
    }

    // Insertar cada error debajo de su campo
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
    insertarError(ui->cmbEjemplar, lblErrorEjemplar);

    insertarError(
        ui->datePrestamo,
        lblErrorFechaPrestamo
        );

    insertarError(
        ui->dateDevolucion,
        lblErrorFechaDevolucion
        );

    // Botón Cancelar
    connect(
        ui->btnCancelar,
        &QPushButton::clicked,
        this,
        &QDialog::reject
        );

    // Botón Guardar: solo validación visual
    connect(
        ui->btnGuardar,
        &QPushButton::clicked,
        this,
        [this]() {
            validarFormulario();
        }
        );

    // Limpiar errores de los selectores
    connect(
        ui->cmbLector,
        QOverload<int>::of(&QComboBox::currentIndexChanged),
        this,
        [this](int) {
            limpiarError(
                lblErrorLector,
                ui->cmbLector
                );
        }
        );

    connect(
        ui->cmbEjemplar,
        QOverload<int>::of(&QComboBox::currentIndexChanged),
        this,
        [this](int) {
            limpiarError(
                lblErrorEjemplar,
                ui->cmbEjemplar
                );
        }
        );

    // Revisar coherencia al cambiar las fechas
    connect(
        ui->datePrestamo,
        &QDateEdit::dateChanged,
        this,
        [this](const QDate &) {
            actualizarValidacionFechas();
        }
        );

    connect(
        ui->dateDevolucion,
        &QDateEdit::dateChanged,
        this,
        [this](const QDate &) {
            actualizarValidacionFechas();
        }
        );

    // Cargar estilos
    QString estilos;

    const QStringList archivosEstilos = {
        ":/styles/dialog_common.qss",
        ":/styles/prestamo_dialog.qss"
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

PrestamoDialog::~PrestamoDialog()
{
    delete ui;
}

bool PrestamoDialog::validarFormulario()
{
    bool valido = true;
    QWidget *primerCampoInvalido = nullptr;

    // Validar lector
    if (ui->cmbLector->currentIndex() <= 0) {
        mostrarError(
            lblErrorLector,
            "Selecciona un lector.",
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

    // Validar ejemplar
    if (ui->cmbEjemplar->currentIndex() <= 0) {
        mostrarError(
            lblErrorEjemplar,
            "Selecciona un ejemplar disponible.",
            ui->cmbEjemplar
            );

        if (!primerCampoInvalido) {
            primerCampoInvalido = ui->cmbEjemplar;
        }

        valido = false;
    } else {
        limpiarError(
            lblErrorEjemplar,
            ui->cmbEjemplar
            );
    }

    // Validar fecha de préstamo
    const QDate fechaPrestamo =
        ui->datePrestamo->date();

    const QDate fechaDevolucion =
        ui->dateDevolucion->date();

    if (!fechaPrestamo.isValid()) {
        mostrarError(
            lblErrorFechaPrestamo,
            "Selecciona una fecha de préstamo válida.",
            ui->datePrestamo
            );

        if (!primerCampoInvalido) {
            primerCampoInvalido = ui->datePrestamo;
        }

        valido = false;
    } else {
        limpiarError(
            lblErrorFechaPrestamo,
            ui->datePrestamo
            );
    }

    // La devolución debe ser posterior
    if (!fechaDevolucion.isValid() ||
        (fechaPrestamo.isValid() &&
         fechaDevolucion <= fechaPrestamo)) {

        mostrarError(
            lblErrorFechaDevolucion,
            "La devolución debe ser posterior "
            "a la fecha de préstamo.",
            ui->dateDevolucion
            );

        if (!primerCampoInvalido) {
            primerCampoInvalido = ui->dateDevolucion;
        }

        valido = false;
    } else {
        limpiarError(
            lblErrorFechaDevolucion,
            ui->dateDevolucion
            );
    }

    if (primerCampoInvalido) {
        primerCampoInvalido->setFocus();
    }

    return valido;
}

void PrestamoDialog::actualizarValidacionFechas()
{
    const QDate fechaPrestamo =
        ui->datePrestamo->date();

    const QDate fechaDevolucion =
        ui->dateDevolucion->date();

    if (!fechaPrestamo.isValid()) {
        mostrarError(
            lblErrorFechaPrestamo,
            "Selecciona una fecha de préstamo válida.",
            ui->datePrestamo
            );
    } else {
        limpiarError(
            lblErrorFechaPrestamo,
            ui->datePrestamo
            );
    }

    if (!fechaDevolucion.isValid() ||
        (fechaPrestamo.isValid() &&
         fechaDevolucion <= fechaPrestamo)) {

        mostrarError(
            lblErrorFechaDevolucion,
            "La devolución debe ser posterior "
            "a la fecha de préstamo.",
            ui->dateDevolucion
            );
    } else {
        limpiarError(
            lblErrorFechaDevolucion,
            ui->dateDevolucion
            );
    }
}

void PrestamoDialog::mostrarError(
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

void PrestamoDialog::limpiarError(
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
