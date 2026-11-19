#include "PrestamosPage.h"
#include "ui_PrestamosPage.h"

#include "../dialogs/PrestamoDialog.h"
#include "../components/AccionesTablaWidget.h"
#include "../components/TablaHelper.h"
#include "../../controllers/LectorController.h"
#include "../../controllers/EjemplarController.h"
#include "../../controllers/LibroController.h"

#include <QPushButton>
#include <QLineEdit>
#include <QTableWidgetItem>
#include <QMessageBox>
#include <QDate>
#include <exception>

PrestamosPage::PrestamosPage(PrestamoController *controller, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PrestamosPage)
{
    ui->setupUi(this);

    if (controller) {
        m_controller = controller;
    } else {
        m_controller = new PrestamoController(this);
        m_ownsController = true;
    }

    // Configuración de tabla: Columna 6 Acciones, ancho 180 px
    TablaHelper::configurar(ui->tablaPrestamos, 6, 180);

    // Conectar botones y buscador
    connect(ui->btnNuevoPrestamo, &QPushButton::clicked, this, &PrestamosPage::onNuevoPrestamo);
    connect(ui->txtBuscar, &QLineEdit::textChanged, this, &PrestamosPage::onBuscarTextoChanged);

    // Conectar acciones de la tabla (Devolver y Editar)
    connect(this, &PrestamosPage::devolverPrestamoSolicitado, this, &PrestamosPage::onDevolverPrestamo);
    connect(this, &PrestamosPage::editarPrestamoSolicitado, this, &PrestamosPage::onEditarPrestamo);

    // Refrescar automáticamente cuando el controlador emita señales
    connect(m_controller, &PrestamoController::datosActualizados, this, &PrestamosPage::cargarPrestamos);

    cargarPrestamos();
}

PrestamosPage::~PrestamosPage()
{
    delete ui;
}

void PrestamosPage::cargarPrestamos()
{
    try {
        m_listaCompleta = m_controller->obtenerTodos();
        onBuscarTextoChanged(ui->txtBuscar->text());
    } catch (const std::exception &e) {
        QMessageBox::warning(this, "Error al cargar préstamos", QString::fromUtf8(e.what()));
    }
}

void PrestamosPage::onNuevoPrestamo()
{
    PrestamoDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        try {
            m_controller->registrarPrestamo(
                dialog.getIdEjemplarSeleccionado(),
                dialog.getIdLectorSeleccionado(),
                dialog.getFechaPrestamo(),
                dialog.getFechaVencimiento()
            );
            QMessageBox::information(this, "Préstamo registrado", "El préstamo se registró correctamente.");
        } catch (const std::exception &e) {
            QMessageBox::warning(this, "No se pudo registrar el préstamo", QString::fromUtf8(e.what()));
        }
    }
}

void PrestamosPage::onDevolverPrestamo(int idPrestamo)
{
    const auto respuesta = QMessageBox::question(
        this,
        "Confirmar devolución",
        "¿Deseas registrar la devolución de este préstamo con fecha de hoy?",
        QMessageBox::Yes | QMessageBox::No
    );

    if (respuesta == QMessageBox::Yes) {
        try {
            m_controller->registrarDevolucion(idPrestamo, QDate::currentDate());
            QMessageBox::information(this, "Devolución registrada", "El ejemplar ha sido devuelto y marcado como Disponible.");
        } catch (const std::exception &e) {
            QMessageBox::warning(this, "Error al devolver", QString::fromUtf8(e.what()));
        }
    }
}

void PrestamosPage::onEditarPrestamo(int idPrestamo)
{
    try {
        Prestamo p = m_controller->obtenerPorId(idPrestamo);
        QString detalle = QString("Préstamo #%1\nLector: %2\nLibro: %3\nFecha préstamo: %4\nFecha vencimiento: %5\nEstado: %6")
                              .arg(p.getId())
                              .arg(obtenerNombreLector(p.getIdLector()))
                              .arg(obtenerInfoLibro(p.getIdEjemplar()))
                              .arg(p.getFechaPrestamo().toString("dd/MM/yyyy"))
                              .arg(p.getFechaVencimiento().toString("dd/MM/yyyy"))
                              .arg(p.getFechaDevolucion().isValid() ? "Devuelto el " + p.getFechaDevolucion().toString("dd/MM/yyyy") : "Activo");

        QMessageBox::information(this, "Detalle del préstamo", detalle);
    } catch (const std::exception &e) {
        QMessageBox::warning(this, "Error", QString::fromUtf8(e.what()));
    }
}

void PrestamosPage::onBuscarTextoChanged(const QString &texto)
{
    const QString filtro = texto.trimmed().toLower();
    if (filtro.isEmpty()) {
        mostrarPrestamos(m_listaCompleta);
        return;
    }

    QList<Prestamo> filtrados;
    for (const Prestamo &p : m_listaCompleta) {
        const QString nombreLector = obtenerNombreLector(p.getIdLector()).toLower();
        const QString infoLibro = obtenerInfoLibro(p.getIdEjemplar()).toLower();
        const QString idStr = QString::number(p.getId());

        if (nombreLector.contains(filtro) || infoLibro.contains(filtro) || idStr.contains(filtro)) {
            filtrados.append(p);
        }
    }
    mostrarPrestamos(filtrados);
}

QString PrestamosPage::obtenerNombreLector(int idLector) const
{
    try {
        LectorController lectorCtrl;
        Lector lector = lectorCtrl.obtenerPorId(idLector);
        if (lector.getId() > 0) {
            return QString("%1 %2").arg(lector.getNombres(), lector.getApellidos());
        }
    } catch (...) {}
    return QString("Lector #%1").arg(idLector);
}

QString PrestamosPage::obtenerInfoLibro(int idEjemplar) const
{
    try {
        EjemplarController ejemplarCtrl;
        Ejemplar ejemplar = ejemplarCtrl.obtenerPorId(idEjemplar);
        if (ejemplar.getId() > 0) {
            LibroController libroCtrl;
            Libro libro = libroCtrl.obtenerPorId(ejemplar.getIdLibro());
            if (libro.getId() > 0) {
                return QString("%1 (%2)").arg(libro.getTitulo(), ejemplar.getCodigo());
            }
            return QString("Ejemplar %1").arg(ejemplar.getCodigo());
        }
    } catch (...) {}
    return QString("Ejemplar #%1").arg(idEjemplar);
}

void PrestamosPage::mostrarPrestamos(const QList<Prestamo> &prestamos)
{
    ui->tablaPrestamos->setRowCount(0);

    for (const Prestamo &prestamo : prestamos) {
        const int fila = ui->tablaPrestamos->rowCount();
        ui->tablaPrestamos->insertRow(fila);

        auto *itemId = new QTableWidgetItem(QString::number(prestamo.getId()));
        itemId->setTextAlignment(Qt::AlignCenter);
        ui->tablaPrestamos->setItem(fila, 0, itemId);

        auto *itemLector = new QTableWidgetItem(obtenerNombreLector(prestamo.getIdLector()));
        itemLector->setTextAlignment(Qt::AlignCenter);
        ui->tablaPrestamos->setItem(fila, 1, itemLector);

        auto *itemLibro = new QTableWidgetItem(obtenerInfoLibro(prestamo.getIdEjemplar()));
        itemLibro->setTextAlignment(Qt::AlignCenter);
        ui->tablaPrestamos->setItem(fila, 2, itemLibro);

        auto *itemFechaPrestamo = new QTableWidgetItem(prestamo.getFechaPrestamo().toString("dd/MM/yyyy"));
        itemFechaPrestamo->setTextAlignment(Qt::AlignCenter);
        ui->tablaPrestamos->setItem(fila, 3, itemFechaPrestamo);

        const bool devuelto = prestamo.getFechaDevolucion().isValid();

        auto *itemFechaDevolucion = new QTableWidgetItem(
            devuelto ? prestamo.getFechaDevolucion().toString("dd/MM/yyyy") : "Pendiente"
        );
        itemFechaDevolucion->setTextAlignment(Qt::AlignCenter);
        ui->tablaPrestamos->setItem(fila, 4, itemFechaDevolucion);

        QString estado;
        if (devuelto) {
            estado = "Devuelto";
        } else if (prestamo.getFechaVencimiento().isValid() && prestamo.getFechaVencimiento() < QDate::currentDate()) {
            estado = "Vencido";
        } else {
            estado = "Activo";
        }

        auto *itemEstado = new QTableWidgetItem(estado);
        itemEstado->setTextAlignment(Qt::AlignCenter);
        ui->tablaPrestamos->setItem(fila, 5, itemEstado);

        const auto tipoAcciones = devuelto
                                      ? AccionesTablaWidget::TipoAcciones::SoloEditar
                                      : AccionesTablaWidget::TipoAcciones::EditarDevolver;

        auto *acciones = new AccionesTablaWidget(prestamo.getId(), ui->tablaPrestamos, tipoAcciones);

        connect(acciones, &AccionesTablaWidget::editarSolicitado, this, &PrestamosPage::editarPrestamoSolicitado);
        connect(acciones, &AccionesTablaWidget::devolverSolicitado, this, &PrestamosPage::devolverPrestamoSolicitado);

        ui->tablaPrestamos->setCellWidget(fila, 6, acciones);
    }

    ui->lblTotal->setText(QString("Total de préstamos: %1").arg(prestamos.size()));
}
