
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

LibroDialog::LibroDialog(QWidget *parent)
    : QDialog(parent),
    ui(new Ui::LibroDialog)
{
    ui->setupUi(this);

    // Límites de caracteres
    ui->txtTitulo->setMaxLength(150);
    ui->txtAutor->setMaxLength(100);
    ui->txtISBN->setMaxLength(17);

    // Crear mensajes de error debajo de cada campo
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
        error->hide();

        QWidget *campoWidget = findChild<QWidget *>(campo.first);

        if (campoWidget) {
            const int indice =
                ui->layoutPrincipal->indexOf(campoWidget);

            ui->layoutPrincipal->insertWidget(
                indice + 1,
                error
                );
        }
    }

    // Botón cancelar
    connect(ui->btnCancelar, &QPushButton::clicked,
            this, &QDialog::reject);

    // Validar al pulsar guardar
    connect(ui->btnGuardar, &QPushButton::clicked,
            this, [this]() {

                if (!validarFormulario()) {
                    return;
                }

                // Prueba visual: todavía no se guarda en la base de datos.
                QMessageBox::information(
                    this,
                    "Validación correcta",
                    "Los datos del libro son válidos.\n\n"
                    "El guardado se integrará posteriormente."
                    );
            });

    // Limpiar errores al modificar los campos
    connect(ui->txtTitulo, &QLineEdit::textChanged,
            this, [this]() {
                limpiarError("txtTitulo");
            });

    connect(ui->txtAutor, &QLineEdit::textChanged,
            this, [this]() {
                limpiarError("txtAutor");
            });

    connect(ui->txtISBN, &QLineEdit::textChanged,
            this, [this]() {
                limpiarError("txtISBN");
            });

    connect(ui->cmbCategoria,
            QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, [this]() {
                limpiarError("cmbCategoria");
            });

    // Cargar estilos
    QString estilos;

    const QStringList archivosEstilos = {
        ":/styles/dialog_common.qss",
        ":/styles/libro_dialog.qss"
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

LibroDialog::~LibroDialog()
{
    delete ui;
}

bool LibroDialog::validarISBN(const QString &isbn)
{
    QString limpio = isbn;
    limpio.remove('-');
    limpio.remove(' ');

    if (limpio.length() != 10 && limpio.length() != 13) {
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

bool LibroDialog::validarFormulario()
{
    limpiarTodosLosErrores();

    bool valido = true;

    const QString titulo = ui->txtTitulo->text().trimmed();
    const QString autor = ui->txtAutor->text().trimmed();
    const QString isbn = ui->txtISBN->text().trimmed();

    if (titulo.isEmpty()) {
        mostrarError(
            "txtTitulo",
            "Ingresa el título del libro."
            );
        valido = false;
    }

    if (autor.isEmpty()) {
        mostrarError(
            "txtAutor",
            "Ingresa el nombre del autor."
            );
        valido = false;
    }

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

    if (ui->cmbCategoria->currentIndex() <= 0) {
        mostrarError(
            "cmbCategoria",
            "Selecciona una categoría."
            );
        valido = false;
    }

    if (!valido) {

        if (titulo.isEmpty()) {
            ui->txtTitulo->setFocus();
        }
        else if (autor.isEmpty()) {
            ui->txtAutor->setFocus();
        }
        else if (isbn.isEmpty() || !validarISBN(isbn)) {
            ui->txtISBN->setFocus();
        }
        else {
            ui->cmbCategoria->setFocus();
        }
    }

    return valido;
}

void LibroDialog::mostrarError(
    const QString &nombreCampo,
    const QString &mensaje)
{
    QLabel *error = findChild<QLabel *>(
        "lblError" +
        nombreCampo.mid(3)
        );

    if (!error) {
        return;
    }

    error->setText(mensaje);
    error->show();

    QWidget *campo = findChild<QWidget *>(nombreCampo);

    if (campo) {
        campo->setProperty("error", true);
        campo->style()->unpolish(campo);
        campo->style()->polish(campo);
        campo->update();
    }
}

void LibroDialog::limpiarError(const QString &nombreCampo)
{
    QLabel *error = findChild<QLabel *>(
        "lblError" +
        nombreCampo.mid(3)
        );

    if (error) {
        error->clear();
        error->hide();
    }

    QWidget *campo = findChild<QWidget *>(nombreCampo);

    if (campo) {
        campo->setProperty("error", false);
        campo->style()->unpolish(campo);
        campo->style()->polish(campo);
        campo->update();
    }
}

void LibroDialog::limpiarTodosLosErrores()
{
    limpiarError("txtTitulo");
    limpiarError("txtAutor");
    limpiarError("txtISBN");
    limpiarError("cmbCategoria");
}
