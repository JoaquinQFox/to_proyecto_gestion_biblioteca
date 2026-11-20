#include "RecomendacionesPage.h"
#include "ui_RecomendacionesPage.h"

#include "../dialogs/RecomendacionDialog.h"
#include "../components/TablaHelper.h"

#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QTableWidgetItem>
#include <exception>

RecomendacionesPage::RecomendacionesPage(RecomendacionController *controller, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::RecomendacionesPage)
    , m_controller(controller)
{
    ui->setupUi(this);

    // si no recibe controlador, crea el suyo
    if (!m_controller) {
        m_controller = new RecomendacionController(this);
    }

    // Configuración reutilizable de la tabla
    // No existe columna de acciones
    TablaHelper::configurar(ui->tablaRecomendaciones);

    // Abrir formulario para generar recomendaciones
    connect(ui->btnGenerarRecomendaciones, &QPushButton::clicked,
            this, &RecomendacionesPage::abrirFormulario);

    // Buscador
    connect(ui->txtBuscar, &QLineEdit::textChanged,
            this, [this](const QString &texto) {
                TablaHelper::filtrar(ui->tablaRecomendaciones, texto);
            });

    // resultados que llegan desde el hilo secundario
    connect(m_controller, &RecomendacionController::generacionIniciada,
            this, [this]() { establecerCargando(true); });

    connect(m_controller, &RecomendacionController::recomendacionesListas,
            this, [this](const QList<RecomendacionItem> &items) {
                establecerCargando(false);
                mostrarRecomendaciones(items);
            });

    connect(m_controller, &RecomendacionController::errorOcurrido,
            this, [this](const QString &mensaje) {
                establecerCargando(false);
                QMessageBox::warning(this, "Recomendaciones", mensaje);
            });
}

RecomendacionesPage::~RecomendacionesPage()
{
    delete ui;
}

void RecomendacionesPage::abrirFormulario()
{
    RecomendacionDialog dialog(m_controller, this);
    if (dialog.exec() != QDialog::Accepted) {
        return;
    }

    try {
        m_controller->generarAsync(
            dialog.tipoSeleccionado(),
            dialog.idLector(),
            dialog.idCategoria(),
            dialog.cantidad()
            );
    } catch (const std::exception &error) {
        QMessageBox::warning(this, "Recomendaciones", QString::fromUtf8(error.what()));
    }
}

void RecomendacionesPage::mostrarRecomendaciones(const QList<RecomendacionItem> &items)
{
    ui->txtBuscar->clear();
    ui->tablaRecomendaciones->setRowCount(0);
    ui->tablaRecomendaciones->setRowCount(items.size());

    int fila = 0;
    for (const RecomendacionItem &item : items) {
        auto *itemId = new QTableWidgetItem(QString::number(item.idLibro));
        itemId->setTextAlignment(Qt::AlignCenter);

        auto *itemTitulo = new QTableWidgetItem(item.titulo);
        itemTitulo->setTextAlignment(Qt::AlignVCenter | Qt::AlignLeft);

        auto *itemAutor = new QTableWidgetItem(item.autor);
        itemAutor->setTextAlignment(Qt::AlignVCenter | Qt::AlignLeft);

        auto *itemCategoria = new QTableWidgetItem(item.categoria);
        itemCategoria->setTextAlignment(Qt::AlignVCenter | Qt::AlignLeft);

        auto *itemMotivo = new QTableWidgetItem(item.motivo);
        itemMotivo->setTextAlignment(Qt::AlignVCenter | Qt::AlignLeft);

        ui->tablaRecomendaciones->setItem(fila, 0, itemId);
        ui->tablaRecomendaciones->setItem(fila, 1, itemTitulo);
        ui->tablaRecomendaciones->setItem(fila, 2, itemAutor);
        ui->tablaRecomendaciones->setItem(fila, 3, itemCategoria);
        ui->tablaRecomendaciones->setItem(fila, 4, itemMotivo);
        ++fila;
    }

    TablaHelper::ajustarFilas(ui->tablaRecomendaciones);
    ui->lblTotal->setText(QString("Total: %1 recomendaciones").arg(items.size()));
}

void RecomendacionesPage::establecerCargando(bool cargando)
{
    // evita pedir otra recomendación mientras se calcula
    ui->btnGenerarRecomendaciones->setEnabled(!cargando);
    if (cargando) {
        ui->lblTotal->setText("Generando recomendaciones...");
    }
}
