
#include "RecomendacionDialog.h"
#include "ui_RecomendacionDialog.h"

#include "../../controllers/RecomendacionController.h"

#include <QFile>
#include <QDebug>
#include <QPushButton>
#include <QComboBox>
#include <QSpinBox>
#include <QLabel>
#include <QStyle>
#include <QStringList>
#include <QWidget>
#include <QLayout>
#include <QSizePolicy>
#include <QTimer>
#include <QMessageBox>
#include <exception>

RecomendacionDialog::RecomendacionDialog(RecomendacionController *controller,
                                         QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::RecomendacionDialog)
    , m_controller(controller)
{
    ui->setupUi(this);

    // =========================================
    // TIPO DE RECOMENDACIÓN (ComboBox)
    // =========================================

    cmbEstrategia = new QComboBox(this);
    cmbEstrategia->setMinimumHeight(42);
    cmbEstrategia->addItem("Por popularidad", static_cast<int>(TipoEstrategia::Popularidad));
    cmbEstrategia->addItem("Por categoría", static_cast<int>(TipoEstrategia::Categoria));
    cmbEstrategia->addItem("Por historial del lector", static_cast<int>(TipoEstrategia::Historial));

    auto *lblEstrategia = new QLabel("Tipo de recomendación", this);
    lblEstrategia->setObjectName("lblEstrategia");

    const int posLector = ui->layoutPrincipal->indexOf(ui->lblLector);
    ui->layoutPrincipal->insertWidget(posLector, lblEstrategia);
    ui->layoutPrincipal->insertWidget(posLector + 1, cmbEstrategia);

    lblErrorCategoria = new QLabel(this);
    lblErrorCategoria->setObjectName("lblErrorCategoria");
    lblErrorCategoria->setWordWrap(true);
    lblErrorCategoria->hide();

    // =========================================
    // CONFIGURACIÓN DEL FORMULARIO
    // =========================================

    // Ajustar el tamaño según el contenido visible
    ui->layoutPrincipal->setSizeConstraint(
        QLayout::SetFixedSize
        );

    // Espaciado uniforme
    ui->layoutPrincipal->setSpacing(10);

    // Márgenes internos
    ui->layoutPrincipal->setContentsMargins(
        28, 24, 28, 24
        );

    // Cantidad permitida de recomendaciones
    ui->spinCantidad->setRange(1, 20);
    ui->spinCantidad->setValue(5);

    // =========================================
    // MENSAJES DE VALIDACIÓN
    // =========================================

    lblErrorLector = new QLabel(this);
    lblErrorCantidad = new QLabel(this);

    lblErrorLector->setObjectName(
        "lblErrorLector"
        );

    lblErrorCantidad->setObjectName(
        "lblErrorCantidad"
        );

    lblErrorLector->setWordWrap(true);
    lblErrorCantidad->setWordWrap(true);

    lblErrorLector->setSizePolicy(
        QSizePolicy::Preferred,
        QSizePolicy::Minimum
        );

    lblErrorCantidad->setSizePolicy(
        QSizePolicy::Preferred,
        QSizePolicy::Minimum
        );

    lblErrorLector->hide();
    lblErrorCantidad->hide();

    // Insertar errores debajo de cada campo
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

    insertarError(
        ui->cmbLector,
        lblErrorLector
        );

    insertarError(
        ui->spinCantidad,
        lblErrorCantidad
        );

    insertarError(
        ui->cmbCategoria,
        lblErrorCategoria
        );

    // =========================================
    // BOTONES
    // =========================================

    connect(
        ui->btnCancelar,
        &QPushButton::clicked,
        this,
        &QDialog::reject
        );

    // Generar: si el formulario es válido se cierra con Accepted
    connect(
        ui->btnGenerar,
        &QPushButton::clicked,
        this,
        [this]() {
            if (validarFormulario()) {
                accept();
            }
        }
        );

    // La categoría solo es obligatoria con la estrategia "Por categoría"
    connect(
        cmbEstrategia,
        QOverload<int>::of(&QComboBox::currentIndexChanged),
        this,
        [this](int) {
            limpiarError(lblErrorCategoria, ui->cmbCategoria);
            actualizarCategoriaObligatoria();
        }
        );

    connect(
        ui->cmbCategoria,
        QOverload<int>::of(&QComboBox::currentIndexChanged),
        this,
        [this](int) {
            limpiarError(lblErrorCategoria, ui->cmbCategoria);
        }
        );

    cargarLectoresYCategorias();
    actualizarCategoriaObligatoria();

    // =========================================
    // LIMPIEZA AUTOMÁTICA DE ERRORES
    // =========================================

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

    // =========================================
    // CARGA DE ESTILOS
    // =========================================

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
            qWarning()
            << "No se pudo cargar:"
            << ruta;
        }
    }

    setStyleSheet(estilos);
    setModal(true);

    // Ajuste inicial del tamaño
    QTimer::singleShot(0, this, [this]() {
        ui->layoutPrincipal->invalidate();
        adjustSize();
    });
}

RecomendacionDialog::~RecomendacionDialog()
{
    delete ui;
}

// =========================================
// VALIDACIÓN DEL FORMULARIO
// =========================================

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

    // Categoría: obligatoria solo con la estrategia "Por categoría"
    if (tipoSeleccionado() == TipoEstrategia::Categoria
        && ui->cmbCategoria->currentIndex() <= 0) {
        mostrarError(
            lblErrorCategoria,
            "Selecciona una categoría para esta recomendación.",
            ui->cmbCategoria
            );

        if (!primerCampoInvalido) {
            primerCampoInvalido = ui->cmbCategoria;
        }

        valido = false;
    } else {
        limpiarError(
            lblErrorCategoria,
            ui->cmbCategoria
            );
    }

    // Enfocar el primer campo incorrecto
    if (primerCampoInvalido) {
        primerCampoInvalido->setFocus();
    }

    // Recalcular tamaño
    ui->layoutPrincipal->invalidate();
    adjustSize();

    return valido;
}

// =========================================
// MOSTRAR ERROR
// =========================================

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

    ui->layoutPrincipal->invalidate();
    adjustSize();
}

// =========================================
// LIMPIAR ERROR
// =========================================

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

    ui->layoutPrincipal->invalidate();
    adjustSize();
}

// =========================================
// DATOS ELEGIDOS
// =========================================

TipoEstrategia RecomendacionDialog::tipoSeleccionado() const
{
    return static_cast<TipoEstrategia>(cmbEstrategia->currentData().toInt());
}

int RecomendacionDialog::idLector() const
{
    return ui->cmbLector->currentData().toInt();
}

int RecomendacionDialog::idCategoria() const
{
    return ui->cmbCategoria->currentData().toInt();
}

int RecomendacionDialog::cantidad() const
{
    return ui->spinCantidad->value();
}

// =========================================
// CARGA DE COMBOS
// =========================================

void RecomendacionDialog::cargarLectoresYCategorias()
{
    if (!m_controller) {
        return;
    }

    try {
        ui->cmbLector->clear();
        ui->cmbLector->addItem("Seleccionar lector", 0);
        for (const Lector &lector : m_controller->listarLectores()) {
            ui->cmbLector->addItem(
                lector.getNombres() + " " + lector.getApellidos()
                    + " (" + lector.getDni() + ")",
                lector.getId()
                );
        }

        ui->cmbCategoria->clear();
        ui->cmbCategoria->addItem("Todas las categorías", 0);
        for (const Categoria &categoria : m_controller->listarCategorias()) {
            ui->cmbCategoria->addItem(categoria.getNombre(), categoria.getId());
        }
    } catch (const std::exception &error) {
        QMessageBox::warning(this, "Recomendaciones", QString::fromUtf8(error.what()));
    }
}

void RecomendacionDialog::actualizarCategoriaObligatoria()
{
    // ... "Seleccionar categoría" en lugar de "Todas" cuando es obligatoria
    const bool obligatoria = tipoSeleccionado() == TipoEstrategia::Categoria;
    ui->cmbCategoria->setItemText(
        0, obligatoria ? "Seleccionar categoría" : "Todas las categorías");
    ui->lblCategoria->setText(
        obligatoria ? "Categoría (obligatoria)" : "Categoría de interés (opcional)");
}
