
#include "LectorDialog.h"
#include "ui_LectorDialog.h"

#include <QFile>
#include <QDebug>
#include <QPushButton>
#include <QStringList>
#include <QLabel>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QStyle>
#include <QRegularExpression>
#include <QList>
#include <QLayout>
#include <QSizePolicy>
#include <QTimer>
#include <QWidget>

LectorDialog::LectorDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::LectorDialog)
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

    // Límites de caracteres
    ui->txtNombre->setMaxLength(100);
    ui->txtDNI->setMaxLength(8);
    ui->txtCorreo->setMaxLength(150);
    ui->txtTelefono->setMaxLength(9);

    // Configuración de campos numéricos
    ui->txtDNI->setInputMethodHints(
        Qt::ImhDigitsOnly
        );

    ui->txtTelefono->setInputMethodHints(
        Qt::ImhDigitsOnly
        );

    // =========================================
    // MENSAJES DE VALIDACIÓN
    // =========================================

    lblErrorNombre = new QLabel(this);
    lblErrorDNI = new QLabel(this);
    lblErrorCorreo = new QLabel(this);
    lblErrorTelefono = new QLabel(this);

    lblErrorNombre->setObjectName("lblErrorNombre");
    lblErrorDNI->setObjectName("lblErrorDNI");
    lblErrorCorreo->setObjectName("lblErrorCorreo");
    lblErrorTelefono->setObjectName("lblErrorTelefono");

    const QList<QLabel *> etiquetas = {
        lblErrorNombre,
        lblErrorDNI,
        lblErrorCorreo,
        lblErrorTelefono
    };

    for (QLabel *etiqueta : etiquetas) {
        etiqueta->setWordWrap(true);

        etiqueta->setSizePolicy(
            QSizePolicy::Preferred,
            QSizePolicy::Minimum
            );

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

    insertarError(ui->txtNombre, lblErrorNombre);
    insertarError(ui->txtDNI, lblErrorDNI);
    insertarError(ui->txtCorreo, lblErrorCorreo);
    insertarError(ui->txtTelefono, lblErrorTelefono);

    // =========================================
    // BOTONES
    // =========================================

    connect(
        ui->btnCancelar,
        &QPushButton::clicked,
        this,
        &QDialog::reject
        );

    connect(
        ui->btnGuardar,
        &QPushButton::clicked,
        this,
        [this]() {
            if (!validarFormulario()) {
                return;
            }

            emit guardarSolicitado(
                ui->txtNombre->text().trimmed(),
                ui->txtDNI->text().trimmed(),
                ui->txtCorreo->text().trimmed(),
                ui->txtTelefono->text().trimmed()
                );
        }
        );

    // =========================================
    // LIMPIEZA AUTOMÁTICA DE ERRORES
    // =========================================

    connect(
        ui->txtNombre,
        &QLineEdit::textChanged,
        this,
        [this](const QString &) {
            limpiarError(
                lblErrorNombre,
                ui->txtNombre
                );
        }
        );

    connect(
        ui->txtDNI,
        &QLineEdit::textChanged,
        this,
        [this](const QString &) {
            limpiarError(
                lblErrorDNI,
                ui->txtDNI
                );
        }
        );

    connect(
        ui->txtCorreo,
        &QLineEdit::textChanged,
        this,
        [this](const QString &) {
            limpiarError(
                lblErrorCorreo,
                ui->txtCorreo
                );
        }
        );

    connect(
        ui->txtTelefono,
        &QLineEdit::textChanged,
        this,
        [this](const QString &) {
            limpiarError(
                lblErrorTelefono,
                ui->txtTelefono
                );
        }
        );

    // =========================================
    // CARGA DE ESTILOS
    // =========================================

    QString estilos;

    const QStringList archivosEstilos = {
        ":/styles/dialog_common.qss",
        ":/styles/lector_dialog.qss"
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

    // Ajuste inicial del tamaño
    QTimer::singleShot(0, this, [this]() {
        ui->layoutPrincipal->invalidate();
        adjustSize();
    });
}

LectorDialog::~LectorDialog()
{
    delete ui;
}

void LectorDialog::cargarLector(const Lector &lector)
{
    setWindowTitle("Editar lector");
    ui->lblTitulo->setText("Editar lector");
    ui->lblDescripcion->setText("Actualiza la información del lector.");
    ui->btnGuardar->setText("Guardar cambios");
    ui->txtNombre->setText(
        (lector.getNombres() + " " + lector.getApellidos()).trimmed()
        );
    ui->txtDNI->setText(lector.getDni());
    ui->txtCorreo->setText(lector.getCorreo());
    ui->txtTelefono->setText(lector.getTelefono());
}

// =========================================
// VALIDACIÓN DEL FORMULARIO
// =========================================

bool LectorDialog::validarFormulario()
{
    bool valido = true;
    QWidget *primerCampoInvalido = nullptr;

    const QString nombre =
        ui->txtNombre->text().trimmed();

    const QString dni =
        ui->txtDNI->text().trimmed();

    const QString correo =
        ui->txtCorreo->text().trimmed();

    const QString telefono =
        ui->txtTelefono->text().trimmed();

    // Validar nombre
    if (nombre.isEmpty()) {
        mostrarError(
            lblErrorNombre,
            "Ingresa el nombre completo del lector.",
            ui->txtNombre
            );

        valido = false;
        primerCampoInvalido = ui->txtNombre;
    } else {
        limpiarError(
            lblErrorNombre,
            ui->txtNombre
            );
    }

    // Validar DNI
    const QRegularExpression regexDNI(
        "^[0-9]{8}$"
        );

    if (!regexDNI.match(dni).hasMatch()) {
        mostrarError(
            lblErrorDNI,
            "El DNI debe contener exactamente 8 dígitos.",
            ui->txtDNI
            );

        if (!primerCampoInvalido) {
            primerCampoInvalido = ui->txtDNI;
        }

        valido = false;
    } else {
        limpiarError(
            lblErrorDNI,
            ui->txtDNI
            );
    }

    // Validar correo electrónico
    const QRegularExpression regexCorreo(
        R"(^[A-Za-z0-9._%+\-]+@[A-Za-z0-9.\-]+\.[A-Za-z]{2,}$)"
        );

    if (!regexCorreo.match(correo).hasMatch()) {
        mostrarError(
            lblErrorCorreo,
            "Ingresa un correo electrónico válido.",
            ui->txtCorreo
            );

        if (!primerCampoInvalido) {
            primerCampoInvalido = ui->txtCorreo;
        }

        valido = false;
    } else {
        limpiarError(
            lblErrorCorreo,
            ui->txtCorreo
            );
    }

    // Validar teléfono
    const QRegularExpression regexTelefono(
        "^[0-9]{9}$"
        );

    if (!regexTelefono.match(telefono).hasMatch()) {
        mostrarError(
            lblErrorTelefono,
            "El teléfono debe contener exactamente 9 dígitos.",
            ui->txtTelefono
            );

        if (!primerCampoInvalido) {
            primerCampoInvalido = ui->txtTelefono;
        }

        valido = false;
    } else {
        limpiarError(
            lblErrorTelefono,
            ui->txtTelefono
            );
    }

    // Enfocar el primer campo incorrecto
    if (primerCampoInvalido) {
        primerCampoInvalido->setFocus();
    }

    // Actualizar tamaño según errores visibles
    ui->layoutPrincipal->invalidate();
    adjustSize();

    return valido;
}

// =========================================
// MOSTRAR ERROR
// =========================================

void LectorDialog::mostrarError(
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

void LectorDialog::limpiarError(
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
