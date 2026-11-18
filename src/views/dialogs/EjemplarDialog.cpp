
#include "EjemplarDialog.h"
#include "ui_EjemplarDialog.h"

#include <QFile>
#include <QDebug>
#include <QPushButton>
#include <QStringList>
#include <QLabel>
#include <QComboBox>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QStyle>
#include <QLayout>
#include <QTimer>
#include <QWidget>
#include <QList>

EjemplarDialog::EjemplarDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::EjemplarDialog)
    , lblErrorLibro(new QLabel(this))
    , lblErrorCodigo(new QLabel(this))
    , lblErrorEstado(new QLabel(this))
{
    ui->setupUi(this);

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

    // Configuración del campo Código
    ui->txtCodigo->setMaxLength(30);

    // =========================================
    // ETIQUETAS DE VALIDACIÓN
    // =========================================

    lblErrorLibro->setObjectName("lblErrorLibro");
    lblErrorCodigo->setObjectName("lblErrorCodigo");
    lblErrorEstado->setObjectName("lblErrorEstado");

    const QList<QLabel *> etiquetas = {
        lblErrorLibro,
        lblErrorCodigo,
        lblErrorEstado
    };

    for (QLabel *etiqueta : etiquetas) {
        etiqueta->setWordWrap(true);
        etiqueta->setSizePolicy(
            QSizePolicy::Preferred,
            QSizePolicy::Minimum
            );
        etiqueta->hide();
    }

    // Insertar cada mensaje debajo de su campo
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

    insertarError(ui->cmbLibro, lblErrorLibro);
    insertarError(ui->txtCodigo, lblErrorCodigo);
    insertarError(ui->cmbEstado, lblErrorEstado);

    // =========================================
    // CONEXIONES DE BOTONES
    // =========================================

    connect(
        ui->btnCancelar,
        &QPushButton::clicked,
        this,
        &QDialog::reject
        );

    // Guardar: comprobar únicamente los campos
    connect(
        ui->btnGuardar,
        &QPushButton::clicked,
        this,
        [this]() {
            if (!validarFormulario()) {
                return;
            }

            emit guardarSolicitado(
                ui->cmbLibro->currentData().toInt(),
                ui->txtCodigo->text().trimmed(),
                ui->cmbEstado->currentData().toString()
                );
        }
        );

    // =========================================
    // LIMPIEZA AUTOMÁTICA DE ERRORES
    // =========================================

    connect(
        ui->cmbLibro,
        QOverload<int>::of(&QComboBox::currentIndexChanged),
        this,
        [this](int) {
            limpiarError(
                lblErrorLibro,
                ui->cmbLibro
                );
        }
        );

    connect(
        ui->txtCodigo,
        &QLineEdit::textChanged,
        this,
        [this](const QString &) {
            limpiarError(
                lblErrorCodigo,
                ui->txtCodigo
                );
        }
        );

    connect(
        ui->cmbEstado,
        QOverload<int>::of(&QComboBox::currentIndexChanged),
        this,
        [this](int) {
            limpiarError(
                lblErrorEstado,
                ui->cmbEstado
                );
        }
        );

    // =========================================
    // CARGA DE ESTILOS
    // =========================================

    QString estilos;

    const QStringList archivosEstilos = {
        ":/styles/dialog_common.qss",
        ":/styles/ejemplar_dialog.qss"
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

    // Ajustar geometría después de construir la interfaz
    QTimer::singleShot(0, this, [this]() {
        ui->layoutPrincipal->invalidate();
        adjustSize();
    });
}

EjemplarDialog::~EjemplarDialog()
{
    delete ui;
}

void EjemplarDialog::establecerLibros(
    const QList<Libro> &libros,
    int idLibroSeleccionado
    )
{
    ui->cmbLibro->clear();
    ui->cmbLibro->addItem("Seleccionar libro", 0);
    for (const Libro &libro : libros) {
        ui->cmbLibro->addItem(libro.getTitulo(), libro.getId());
    }

    const int indiceLibro = ui->cmbLibro->findData(idLibroSeleccionado);
    if (indiceLibro >= 0) {
        ui->cmbLibro->setCurrentIndex(indiceLibro);
    }

    ui->cmbEstado->clear();
    ui->cmbEstado->addItem("Seleccionar estado", QString());
    ui->cmbEstado->addItem("Disponible", "DISPONIBLE");
    ui->cmbEstado->addItem("Prestado", "PRESTADO");
    ui->cmbEstado->addItem("Retirado", "RETIRADO");
}

void EjemplarDialog::cargarEjemplar(const Ejemplar &ejemplar)
{
    setWindowTitle("Editar ejemplar");
    ui->lblTitulo->setText("Editar ejemplar");
    ui->lblDescripcion->setText("Actualiza la información del ejemplar.");
    ui->btnGuardar->setText("Guardar cambios");
    ui->txtCodigo->setText(ejemplar.getCodigo());
    ui->cmbLibro->setCurrentIndex(
        ui->cmbLibro->findData(ejemplar.getIdLibro())
        );
    ui->cmbEstado->setCurrentIndex(
        ui->cmbEstado->findData(ejemplar.getEstadoString())
        );
}

// =========================================
// VALIDACIÓN DEL FORMULARIO
// =========================================

bool EjemplarDialog::validarFormulario()
{
    bool valido = true;
    QWidget *primerCampoInvalido = nullptr;

    // Validar libro
    if (ui->cmbLibro->currentIndex() <= 0) {
        mostrarError(
            lblErrorLibro,
            "Selecciona un libro.",
            ui->cmbLibro
            );

        valido = false;
        primerCampoInvalido = ui->cmbLibro;
    } else {
        limpiarError(
            lblErrorLibro,
            ui->cmbLibro
            );
    }

    // Validar código
    if (ui->txtCodigo->text().trimmed().isEmpty()) {
        mostrarError(
            lblErrorCodigo,
            "Ingresa el código del ejemplar.",
            ui->txtCodigo
            );

        if (!primerCampoInvalido) {
            primerCampoInvalido = ui->txtCodigo;
        }

        valido = false;
    } else {
        limpiarError(
            lblErrorCodigo,
            ui->txtCodigo
            );
    }

    // Validar estado
    if (ui->cmbEstado->currentIndex() <= 0) {
        mostrarError(
            lblErrorEstado,
            "Selecciona el estado del ejemplar.",
            ui->cmbEstado
            );

        if (!primerCampoInvalido) {
            primerCampoInvalido = ui->cmbEstado;
        }

        valido = false;
    } else {
        limpiarError(
            lblErrorEstado,
            ui->cmbEstado
            );
    }

    // Enfocar el primer campo incorrecto
    if (primerCampoInvalido) {
        primerCampoInvalido->setFocus();
    }

    // Actualizar geometría del formulario
    ui->layoutPrincipal->invalidate();
    adjustSize();

    return valido;
}

// =========================================
// MOSTRAR ERROR
// =========================================

void EjemplarDialog::mostrarError(
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

void EjemplarDialog::limpiarError(
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
// LIMPIAR TODOS LOS ERRORES
// =========================================

void EjemplarDialog::limpiarErrores()
{
    limpiarError(
        lblErrorLibro,
        ui->cmbLibro
        );

    limpiarError(
        lblErrorCodigo,
        ui->txtCodigo
        );

    limpiarError(
        lblErrorEstado,
        ui->cmbEstado
        );
}
