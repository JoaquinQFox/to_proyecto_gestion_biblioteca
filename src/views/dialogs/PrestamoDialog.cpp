#include "PrestamoDialog.h"
#include "ui_PrestamoDialog.h"

#include "../../controllers/LectorController.h"
#include "../../controllers/EjemplarController.h"
#include "../../controllers/LibroController.h"

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
#include <QLayout>
#include <QSizePolicy>
#include <QTimer>

PrestamoDialog::PrestamoDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::PrestamoDialog)
{
    ui->setupUi(this);

    ui->layoutPrincipal->setSizeConstraint(QLayout::SetFixedSize);
    ui->layoutPrincipal->setSpacing(10);
    ui->layoutPrincipal->setContentsMargins(28, 24, 28, 24);

    ui->datePrestamo->setDate(QDate::currentDate());
    ui->dateDevolucion->setDate(QDate::currentDate().addDays(7));

    // Cargar lectores activos y ejemplares disponibles desde la BD
    cargarDatosCombos();

    lblErrorLector = new QLabel(this);
    lblErrorEjemplar = new QLabel(this);
    lblErrorFechaPrestamo = new QLabel(this);
    lblErrorFechaDevolucion = new QLabel(this);

    lblErrorLector->setObjectName("lblErrorLector");
    lblErrorEjemplar->setObjectName("lblErrorEjemplar");
    lblErrorFechaPrestamo->setObjectName("lblErrorFechaPrestamo");
    lblErrorFechaDevolucion->setObjectName("lblErrorFechaDevolucion");

    const QList<QLabel *> etiquetas = {
        lblErrorLector,
        lblErrorEjemplar,
        lblErrorFechaPrestamo,
        lblErrorFechaDevolucion
    };

    for (QLabel *etiqueta : etiquetas) {
        etiqueta->setWordWrap(true);
        etiqueta->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
        etiqueta->hide();
    }

    auto insertarError = [this](QWidget *campo, QLabel *etiqueta) {
        const int indice = ui->layoutPrincipal->indexOf(campo);
        if (indice >= 0) {
            ui->layoutPrincipal->insertWidget(indice + 1, etiqueta);
        }
    };

    insertarError(ui->cmbLector, lblErrorLector);
    insertarError(ui->cmbEjemplar, lblErrorEjemplar);
    insertarError(ui->datePrestamo, lblErrorFechaPrestamo);
    insertarError(ui->dateDevolucion, lblErrorFechaDevolucion);

    connect(ui->btnCancelar, &QPushButton::clicked, this, &QDialog::reject);

    connect(ui->btnGuardar, &QPushButton::clicked, this, [this]() {
        if (validarFormulario()) {
            accept();
        }
    });

    connect(ui->cmbLector, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this](int) {
        limpiarError(lblErrorLector, ui->cmbLector);
    });

    connect(ui->cmbEjemplar, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this](int) {
        limpiarError(lblErrorEjemplar, ui->cmbEjemplar);
    });

    connect(ui->datePrestamo, &QDateEdit::dateChanged, this, [this](const QDate &) {
        actualizarValidacionFechas();
    });

    connect(ui->dateDevolucion, &QDateEdit::dateChanged, this, [this](const QDate &) {
        actualizarValidacionFechas();
    });

    QString estilos;
    const QStringList archivosEstilos = {
        ":/styles/dialog_common.qss",
        ":/styles/prestamo_dialog.qss"
    };

    for (const QString &ruta : archivosEstilos) {
        QFile archivo(ruta);
        if (archivo.open(QIODevice::ReadOnly | QIODevice::Text)) {
            estilos += QString::fromUtf8(archivo.readAll()) + "\n";
        } else {
            qWarning() << "No se pudo cargar:" << ruta;
        }
    }

    setStyleSheet(estilos);
    setModal(true);

    QTimer::singleShot(0, this, [this]() {
        ui->layoutPrincipal->invalidate();
        adjustSize();
    });
}

PrestamoDialog::~PrestamoDialog()
{
    delete ui;
}

void PrestamoDialog::cargarDatosCombos()
{
    ui->cmbLector->clear();
    ui->cmbLector->addItem("Seleccione un lector...", 0);

    try {
        LectorController lectorCtrl;
        const QList<Lector> lectores = lectorCtrl.listar();
        for (const Lector &lector : lectores) {
            if (lector.isActivo()) {
                QString texto = QString("%1 %2 (DNI: %3)")
                                    .arg(lector.getNombres(), lector.getApellidos(), lector.getDni());
                ui->cmbLector->addItem(texto, lector.getId());
            }
        }
    } catch (const std::exception &e) {
        qWarning() << "Error al cargar lectores:" << e.what();
    }

    ui->cmbEjemplar->clear();
    ui->cmbEjemplar->addItem("Seleccione un ejemplar disponible...", 0);

    try {
        EjemplarController ejemplarCtrl;
        LibroController libroCtrl;
        const QList<Ejemplar> ejemplares = ejemplarCtrl.listar();
        for (const Ejemplar &ejemplar : ejemplares) {
            if (ejemplar.getEstado() == Ejemplar::Estado::DISPONIBLE) {
                QString tituloLibro = "Libro #" + QString::number(ejemplar.getIdLibro());
                try {
                    Libro libro = libroCtrl.obtenerPorId(ejemplar.getIdLibro());
                    if (libro.getId() > 0) {
                        tituloLibro = libro.getTitulo();
                    }
                } catch (...) {}

                QString texto = QString("[%1] %2").arg(ejemplar.getCodigo(), tituloLibro);
                ui->cmbEjemplar->addItem(texto, ejemplar.getId());
            }
        }
    } catch (const std::exception &e) {
        qWarning() << "Error al cargar ejemplares:" << e.what();
    }
}

int PrestamoDialog::getIdLectorSeleccionado() const
{
    return ui->cmbLector->currentData().toInt();
}

int PrestamoDialog::getIdEjemplarSeleccionado() const
{
    return ui->cmbEjemplar->currentData().toInt();
}

QDate PrestamoDialog::getFechaPrestamo() const
{
    return ui->datePrestamo->date();
}

QDate PrestamoDialog::getFechaVencimiento() const
{
    return ui->dateDevolucion->date();
}

bool PrestamoDialog::validarFormulario()
{
    bool valido = true;
    QWidget *primerCampoInvalido = nullptr;

    if (ui->cmbLector->currentIndex() <= 0) {
        mostrarError(lblErrorLector, "Selecciona un lector.", ui->cmbLector);
        valido = false;
        primerCampoInvalido = ui->cmbLector;
    } else {
        limpiarError(lblErrorLector, ui->cmbLector);
    }

    if (ui->cmbEjemplar->currentIndex() <= 0) {
        mostrarError(lblErrorEjemplar, "Selecciona un ejemplar disponible.", ui->cmbEjemplar);
        if (!primerCampoInvalido) primerCampoInvalido = ui->cmbEjemplar;
        valido = false;
    } else {
        limpiarError(lblErrorEjemplar, ui->cmbEjemplar);
    }

    const QDate fechaPrestamo = ui->datePrestamo->date();
    const QDate fechaDevolucion = ui->dateDevolucion->date();

    if (!fechaPrestamo.isValid()) {
        mostrarError(lblErrorFechaPrestamo, "Selecciona una fecha de préstamo válida.", ui->datePrestamo);
        if (!primerCampoInvalido) primerCampoInvalido = ui->datePrestamo;
        valido = false;
    } else {
        limpiarError(lblErrorFechaPrestamo, ui->datePrestamo);
    }

    if (!fechaDevolucion.isValid() || fechaDevolucion < fechaPrestamo) {
        mostrarError(lblErrorFechaDevolucion, "La fecha de vencimiento debe ser igual o posterior al préstamo.", ui->dateDevolucion);
        if (!primerCampoInvalido) primerCampoInvalido = ui->dateDevolucion;
        valido = false;
    } else {
        limpiarError(lblErrorFechaDevolucion, ui->dateDevolucion);
    }

    if (primerCampoInvalido) {
        primerCampoInvalido->setFocus();
    }

    ui->layoutPrincipal->invalidate();
    adjustSize();

    return valido;
}

void PrestamoDialog::mostrarError(QLabel *etiqueta, const QString &mensaje, QWidget *campo)
{
    if (!etiqueta || !campo) return;
    etiqueta->setText(mensaje);
    etiqueta->show();
    campo->setProperty("error", true);
    campo->style()->unpolish(campo);
    campo->style()->polish(campo);
    campo->update();
}

void PrestamoDialog::limpiarError(QLabel *etiqueta, QWidget *campo)
{
    if (!etiqueta || !campo) return;
    etiqueta->hide();
    etiqueta->clear();
    campo->setProperty("error", false);
    campo->style()->unpolish(campo);
    campo->style()->polish(campo);
    campo->update();
}

void PrestamoDialog::actualizarValidacionFechas()
{
    const QDate fechaPrestamo = ui->datePrestamo->date();
    const QDate fechaDevolucion = ui->dateDevolucion->date();

    if (fechaPrestamo.isValid() && fechaDevolucion.isValid() && fechaDevolucion >= fechaPrestamo) {
        limpiarError(lblErrorFechaPrestamo, ui->datePrestamo);
        limpiarError(lblErrorFechaDevolucion, ui->dateDevolucion);
    }
}
