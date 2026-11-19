#include "InicioPage.h"
#include "ui_InicioPage.h"

#include "../components/TablaHelper.h"
#include "../../controllers/LectorController.h"
#include "../../controllers/EjemplarController.h"
#include "../../controllers/LibroController.h"

#include <QTableWidgetItem>
#include <QDate>
#include <algorithm>

InicioPage::InicioPage(PrestamoController *controller, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::InicioPage)
{
    ui->setupUi(this);

    if (controller) {
        m_controller = controller;
    } else {
        m_controller = new PrestamoController(this);
        m_ownsController = true;
    }

    TablaHelper::configurar(ui->tablaPrestamosRecientes);

    // Conectar las señales en tiempo real del PrestamoController
    connect(m_controller, &PrestamoController::prestamoRegistrado, this, &InicioPage::actualizarDashboard);
    connect(m_controller, &PrestamoController::prestamoDevuelto, this, &InicioPage::actualizarDashboard);
    connect(m_controller, &PrestamoController::datosActualizados, this, &InicioPage::actualizarDashboard);

    actualizarDashboard();
}

InicioPage::~InicioPage()
{
    delete ui;
}

void InicioPage::actualizarDashboard()
{
    try {
        ui->lblTotalLibros->setText(QString::number(m_controller->contarTotalLibros()));
        ui->lblTotalEjemplares->setText(QString::number(m_controller->contarTotalEjemplares()));
        ui->lblTotalLectores->setText(QString::number(m_controller->contarTotalLectores()));
        ui->lblTotalPrestamos->setText(QString::number(m_controller->contarPrestamosActivos()));

        cargarPrestamosRecientes();
    } catch (...) {
        // Evitar caída de interfaz si ocurre un error puntual de lectura
    }
}

void InicioPage::cargarPrestamosRecientes()
{
    ui->tablaPrestamosRecientes->setRowCount(0);

    QList<Prestamo> todos = m_controller->obtenerTodos();
    // Mostrar los últimos 10 préstamos (los más recientes primero)
    const int inicio = std::max(0, static_cast<int>(todos.size()) - 10);

    LectorController lectorCtrl;
    EjemplarController ejemplarCtrl;
    LibroController libroCtrl;

    for (int i = todos.size() - 1; i >= inicio; --i) {
        const Prestamo &p = todos[i];
        const int fila = ui->tablaPrestamosRecientes->rowCount();
        ui->tablaPrestamosRecientes->insertRow(fila);

        auto *itemId = new QTableWidgetItem(QString::number(p.getId()));
        itemId->setTextAlignment(Qt::AlignCenter);
        ui->tablaPrestamosRecientes->setItem(fila, 0, itemId);

        QString nombreLector = QString("Lector #%1").arg(p.getIdLector());
        try {
            Lector l = lectorCtrl.obtenerPorId(p.getIdLector());
            if (l.getId() > 0) nombreLector = QString("%1 %2").arg(l.getNombres(), l.getApellidos());
        } catch (...) {}

        auto *itemLector = new QTableWidgetItem(nombreLector);
        itemLector->setTextAlignment(Qt::AlignCenter);
        ui->tablaPrestamosRecientes->setItem(fila, 1, itemLector);

        QString infoLibro = QString("Ejemplar #%1").arg(p.getIdEjemplar());
        try {
            Ejemplar ej = ejemplarCtrl.obtenerPorId(p.getIdEjemplar());
            if (ej.getId() > 0) {
                Libro lib = libroCtrl.obtenerPorId(ej.getIdLibro());
                infoLibro = (lib.getId() > 0)
                                ? QString("%1 (%2)").arg(lib.getTitulo(), ej.getCodigo())
                                : ej.getCodigo();
            }
        } catch (...) {}

        auto *itemLibro = new QTableWidgetItem(infoLibro);
        itemLibro->setTextAlignment(Qt::AlignCenter);
        ui->tablaPrestamosRecientes->setItem(fila, 2, itemLibro);

        auto *itemFecha = new QTableWidgetItem(p.getFechaPrestamo().toString("dd/MM/yyyy"));
        itemFecha->setTextAlignment(Qt::AlignCenter);
        ui->tablaPrestamosRecientes->setItem(fila, 3, itemFecha);

        QString estado;
        if (p.getFechaDevolucion().isValid()) {
            estado = "Devuelto";
        } else if (p.getFechaVencimiento().isValid() && p.getFechaVencimiento() < QDate::currentDate()) {
            estado = "Vencido";
        } else {
            estado = "Activo";
        }

        auto *itemEstado = new QTableWidgetItem(estado);
        itemEstado->setTextAlignment(Qt::AlignCenter);
        ui->tablaPrestamosRecientes->setItem(fila, 4, itemEstado);
    }
}
