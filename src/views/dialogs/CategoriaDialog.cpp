
#include "CategoriaDialog.h"
#include "ui_CategoriaDialog.h"

#include <QFile>
#include <QDebug>
#include <QPushButton>
#include <QLineEdit>
#include <QStringList>
#include <QLabel>
#include <QVBoxLayout>
#include <QLayout>
#include <QSizePolicy>
#include <QStyle>
#include <QTimer>

CategoriaDialog::CategoriaDialog(QWidget *parent)
    : CategoriaDialog(QString(), parent)
{
}

CategoriaDialog::CategoriaDialog(
    const QString &nombreInicial,
    QWidget *parent
    )
    : QDialog(parent)
    , ui(new Ui::CategoriaDialog)
{
    ui->setupUi(this);

    if (!nombreInicial.isNull()) {
        setWindowTitle("Editar categoría");
        ui->lblTitulo->setText("Editar categoría");
        ui->lblDescripcion->setText(
            "Actualiza el nombre de la categoría."
            );
        ui->btnGuardar->setText("Guardar cambios");
        ui->txtNombre->setText(nombreInicial);
    }

    // =========================================
    // CONFIGURACIÓN DEL FORMULARIO
    // =========================================

    // Ajustar la ventana al contenido visible
    ui->layoutPrincipal->setSizeConstraint(
        QLayout::SetFixedSize
        );

    // Espaciado y márgenes uniformes
    ui->layoutPrincipal->setSpacing(10);

    ui->layoutPrincipal->setContentsMargins(
        28, 24, 28, 24
        );

    // Configuración del campo
    ui->txtNombre->setMaxLength(60);

    // =========================================
    // MENSAJE DE VALIDACIÓN
    // =========================================

    auto *lblError = new QLabel(this);

    lblError->setObjectName("lblErrorValidacion");
    lblError->setWordWrap(true);

    lblError->setSizePolicy(
        QSizePolicy::Preferred,
        QSizePolicy::Minimum
        );

    lblError->hide();

    // Insertar el mensaje debajo del campo Nombre
    const int indiceNombre =
        ui->layoutPrincipal->indexOf(ui->txtNombre);

    if (indiceNombre >= 0) {
        ui->layoutPrincipal->insertWidget(
            indiceNombre + 1,
            lblError
            );
    }

    // =========================================
    // CONEXIONES
    // =========================================

    connect(
        ui->btnCancelar,
        &QPushButton::clicked,
        this,
        &QDialog::reject
        );

    // Mantener la señal existente
    connect(
        ui->btnGuardar,
        &QPushButton::clicked,
        this,
        [this]() {
            if (!validarFormulario()) {
                return;
            }

            emit guardarSolicitado(
                ui->txtNombre->text().trimmed()
                );
        }
        );

    // Limpiar error al modificar el nombre
    connect(
        ui->txtNombre,
        &QLineEdit::textChanged,
        this,
        [this](const QString &) {
            limpiarError();
        }
        );

    // =========================================
    // CARGA DE ESTILOS
    // =========================================

    QString estilos;

    const QStringList archivosEstilos = {
        ":/styles/dialog_common.qss",
        ":/styles/categoria_dialog.qss"
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

    // Ajustar tamaño después de construir la interfaz
    QTimer::singleShot(0, this, [this]() {
        ui->layoutPrincipal->invalidate();
        adjustSize();
    });
}

CategoriaDialog::~CategoriaDialog()
{
    delete ui;
}

// =========================================
// VALIDACIÓN DEL FORMULARIO
// =========================================

bool CategoriaDialog::validarFormulario()
{
    const QString nombre =
        ui->txtNombre->text().trimmed();

    if (nombre.isEmpty()) {
        mostrarError(
            "Ingresa el nombre de la categoría."
            );

        ui->txtNombre->setFocus();
        return false;
    }

    limpiarError();
    return true;
}

// =========================================
// MOSTRAR ERROR
// =========================================

void CategoriaDialog::mostrarError(
    const QString &mensaje
    )
{
    auto *lblError = findChild<QLabel *>(
        "lblErrorValidacion"
        );

    if (lblError) {
        lblError->setText(mensaje);
        lblError->show();
    }

    ui->txtNombre->setProperty("error", true);

    ui->txtNombre->style()->unpolish(
        ui->txtNombre
        );

    ui->txtNombre->style()->polish(
        ui->txtNombre
        );

    ui->txtNombre->update();

    // Recalcular tamaño con el error visible
    ui->layoutPrincipal->invalidate();
    adjustSize();
}

// =========================================
// LIMPIAR ERROR
// =========================================

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

    ui->txtNombre->style()->unpolish(
        ui->txtNombre
        );

    ui->txtNombre->style()->polish(
        ui->txtNombre
        );

    ui->txtNombre->update();

    // Recalcular tamaño sin el error
    ui->layoutPrincipal->invalidate();
    adjustSize();
}
