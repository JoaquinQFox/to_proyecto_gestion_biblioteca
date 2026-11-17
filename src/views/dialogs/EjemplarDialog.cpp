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

EjemplarDialog::EjemplarDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::EjemplarDialog)
    , lblErrorLibro(new QLabel(this))
    , lblErrorCodigo(new QLabel(this))
    , lblErrorEstado(new QLabel(this))
{
    ui->setupUi(this);

    // Configuración del código
    ui->txtCodigo->setMaxLength(30);

    // Configuración de mensajes de validación
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
        etiqueta->hide();
    }

    // Insertar mensajes debajo de sus respectivos campos
    ui->layoutPrincipal->insertWidget(5, lblErrorLibro);
    ui->layoutPrincipal->insertWidget(8, lblErrorCodigo);
    ui->layoutPrincipal->insertWidget(11, lblErrorEstado);

    // Cancelar
    connect(ui->btnCancelar, &QPushButton::clicked,
            this, &QDialog::reject);

    // Validar al guardar
    connect(ui->btnGuardar, &QPushButton::clicked,
            this, [this]() {

                if (!validarFormulario()) {
                    return;
                }

                accept();
            });

    // Limpiar errores al modificar los campos
    connect(ui->cmbLibro,
            QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, [this]() {
                limpiarError(lblErrorLibro, ui->cmbLibro);
            });

    connect(ui->txtCodigo, &QLineEdit::textChanged,
            this, [this]() {
                limpiarError(lblErrorCodigo, ui->txtCodigo);
            });

    connect(ui->cmbEstado,
            QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, [this]() {
                limpiarError(lblErrorEstado, ui->cmbEstado);
            });

    // Cargar estilos
    QString estilos;

    const QStringList archivosEstilos = {
        ":/styles/dialog_common.qss",
        ":/styles/ejemplar_dialog.qss"
    };

    for (const QString &ruta : archivosEstilos) {
        QFile archivo(ruta);

        if (archivo.open(QIODevice::ReadOnly | QIODevice::Text)) {
            estilos += QString::fromUtf8(archivo.readAll());
            estilos += "\n";
            archivo.close();
        } else {
            qWarning() << "No se pudo cargar:" << ruta;
        }
    }

    setStyleSheet(estilos);

    setModal(true);
}

EjemplarDialog::~EjemplarDialog()
{
    delete ui;
}

// VALIDACIÓN DEL FORMULARIO

bool EjemplarDialog::validarFormulario()
{
    bool valido = true;

    // Validar libro
    if (ui->cmbLibro->currentIndex() <= 0) {
        mostrarError(
            lblErrorLibro,
            "Selecciona un libro.",
            ui->cmbLibro
            );

        valido = false;
    } else {
        limpiarError(lblErrorLibro, ui->cmbLibro);
    }

    // Validar código
    if (ui->txtCodigo->text().trimmed().isEmpty()) {
        mostrarError(
            lblErrorCodigo,
            "Ingresa el código del ejemplar.",
            ui->txtCodigo
            );

        if (valido) {
            ui->txtCodigo->setFocus();
        }

        valido = false;
    } else {
        limpiarError(lblErrorCodigo, ui->txtCodigo);
    }

    // Validar estado
    if (ui->cmbEstado->currentIndex() <= 0) {
        mostrarError(
            lblErrorEstado,
            "Selecciona el estado del ejemplar.",
            ui->cmbEstado
            );

        if (valido) {
            ui->cmbEstado->setFocus();
        }

        valido = false;
    } else {
        limpiarError(lblErrorEstado, ui->cmbEstado);
    }

    return valido;
}

// MOSTRAR ERROR

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
}

// LIMPIAR ERROR

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
}

// LIMPIAR TODOS LOS ERRORES

void EjemplarDialog::limpiarErrores()
{
    limpiarError(lblErrorLibro, ui->cmbLibro);
    limpiarError(lblErrorCodigo, ui->txtCodigo);
    limpiarError(lblErrorEstado, ui->cmbEstado);
}