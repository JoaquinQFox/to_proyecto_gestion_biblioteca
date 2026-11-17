
#include "LibroDialog.h"
#include "ui_LibroDialog.h"

#include <QFile>
#include <QDebug>
#include <QPushButton>
#include <QStringList>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QStyle>
#include <QMessageBox>
#include <QLayout>
#include <QSizePolicy>
#include <QTimer>
#include <QList>
#include <QPair>
#include <QWidget>

LibroDialog::LibroDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::LibroDialog)
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
    ui->txtTitulo->setMaxLength(150);
    ui->txtAutor->setMaxLength(100);
    ui->txtISBN->setMaxLength(17);

    // =========================================
    // MENSAJES DE VALIDACIÓN
    // =========================================

    const QList<QPair<QString, QString>> campos = {
        {"txtTitulo", "lblErrorTitulo"},
        {"txtAutor", "lblErrorAutor"},
        {"txtISBN", "lblErrorISBN"},
        {"cmbCategoria", "lblErrorCategoria"}
    };

    for (const auto &campo : campos) {

        auto *error = new QLabel(this);

        error->setObjectName(campo.second);
        error->setWordWrap(true);

        error->setSizePolicy(
            QSizePolicy::Preferred,
            QSizePolicy::Minimum
            );

        error->hide();

        QWidget *campoWidget =
            findChild<QWidget *>(campo.first);

        if (campoWidget) {
            const int indice =
                ui->layoutPrincipal->indexOf(campoWidget);

            if (indice >= 0) {
                ui->layoutPrincipal->insertWidget(
                    indice + 1,
                    error
                    );
            }
        }
    }

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

            // Prueba visual: no guarda en la base de datos
            QMessageBox::information(
                this,
                "Validación correcta",
                "Los datos del libro son válidos.\n\n"
                "El guardado se integrará posteriormente."
                );
        }
        );

    // =========================================
    // LIMPIEZA AUTOMÁTICA DE ERRORES
    // =========================================

    connect(
        ui->txtTitulo,
        &QLineEdit::textChanged,
        this,
        [this](const QString &) {
            limpiarError("txtTitulo");
        }
        );

    connect(
        ui->txtAutor,
        &QLineEdit::textChanged,
        this,
        [this](const QString &) {
            limpiarError("txtAutor");
        }
        );

    connect(
        ui->txtISBN,
        &QLineEdit::textChanged,
        this,
        [this](const QString &) {
            limpiarError("txtISBN");
        }
        );

    connect(
        ui->cmbCategoria,
        QOverload<int>::of(
            &QComboBox::currentIndexChanged
            ),
        this,
        [this](int) {
            limpiarError("cmbCategoria");
        }
        );

    // =========================================
    // CARGA DE ESTILOS
    // =========================================

    QString estilos;

    const QStringList archivosEstilos = {
        ":/styles/dialog_common.qss",
        ":/styles/libro_dialog.qss"
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

    // Ajuste inicial de geometría
    QTimer::singleShot(0, this, [this]() {
        ui->layoutPrincipal->invalidate();
        adjustSize();
    });
}

LibroDialog::~LibroDialog()
{
    delete ui;
}

// =========================================
// VALIDACIÓN DEL ISBN
// =========================================

bool LibroDialog::validarISBN(const QString &isbn)
{
    QString limpio = isbn;

    limpio.remove('-');
    limpio.remove(' ');

    if (limpio.length() != 10 &&
        limpio.length() != 13) {
        return false;
    }

    for (int i = 0; i < limpio.length(); ++i) {

        const QChar caracter = limpio[i];

        if (i == limpio.length() - 1 &&
            limpio.length() == 10 &&
            caracter == 'X') {
            continue;
        }

        if (!caracter.isDigit()) {
            return false;
        }
    }

    return true;
}

// =========================================
// VALIDACIÓN DEL FORMULARIO
// =========================================

bool LibroDialog::validarFormulario()
{
    limpiarTodosLosErrores();

    bool valido = true;

    const QString titulo =
        ui->txtTitulo->text().trimmed();

    const QString autor =
        ui->txtAutor->text().trimmed();

    const QString isbn =
        ui->txtISBN->text().trimmed();

    // Validar título
    if (titulo.isEmpty()) {
        mostrarError(
            "txtTitulo",
            "Ingresa el título del libro."
            );

        valido = false;
    }

    // Validar autor
    if (autor.isEmpty()) {
        mostrarError(
            "txtAutor",
            "Ingresa el nombre del autor."
            );

        valido = false;
    }

    // Validar ISBN
    if (isbn.isEmpty()) {
        mostrarError(
            "txtISBN",
            "Ingresa el ISBN del libro."
            );

        valido = false;
    }
    else if (!validarISBN(isbn)) {
        mostrarError(
            "txtISBN",
            "El ISBN debe contener 10 o 13 dígitos."
            );

        valido = false;
    }

    // Validar categoría
    if (ui->cmbCategoria->currentIndex() <= 0) {
        mostrarError(
            "cmbCategoria",
            "Selecciona una categoría."
            );

        valido = false;
    }

    // Enfocar el primer campo incorrecto
    if (!valido) {

        if (titulo.isEmpty()) {
            ui->txtTitulo->setFocus();
        }
        else if (autor.isEmpty()) {
            ui->txtAutor->setFocus();
        }
        else if (isbn.isEmpty() ||
                 !validarISBN(isbn)) {
            ui->txtISBN->setFocus();
        }
        else {
            ui->cmbCategoria->setFocus();
        }
    }

    // Actualizar tamaño según los errores visibles
    ui->layoutPrincipal->invalidate();
    adjustSize();

    return valido;
}

// =========================================
// MOSTRAR ERROR
// =========================================

void LibroDialog::mostrarError(
    const QString &nombreCampo,
    const QString &mensaje
    )
{
    QLabel *error = findChild<QLabel *>(
        "lblError" + nombreCampo.mid(3)
        );

    if (!error) {
        return;
    }

    error->setText(mensaje);
    error->show();

    QWidget *campo =
        findChild<QWidget *>(nombreCampo);

    if (campo) {
        campo->setProperty("error", true);

        campo->style()->unpolish(campo);
        campo->style()->polish(campo);
        campo->update();
    }

    ui->layoutPrincipal->invalidate();
    adjustSize();
}

// =========================================
// LIMPIAR ERROR
// =========================================

void LibroDialog::limpiarError(
    const QString &nombreCampo
    )
{
    QLabel *error = findChild<QLabel *>(
        "lblError" + nombreCampo.mid(3)
        );

    if (error) {
        error->clear();
        error->hide();
    }

    QWidget *campo =
        findChild<QWidget *>(nombreCampo);

    if (campo) {
        campo->setProperty("error", false);

        campo->style()->unpolish(campo);
        campo->style()->polish(campo);
        campo->update();
    }

    ui->layoutPrincipal->invalidate();
    adjustSize();
}

// =========================================
// LIMPIAR TODOS LOS ERRORES
// =========================================

void LibroDialog::limpiarTodosLosErrores()
{
    limpiarError("txtTitulo");
    limpiarError("txtAutor");
    limpiarError("txtISBN");
    limpiarError("cmbCategoria");
}
