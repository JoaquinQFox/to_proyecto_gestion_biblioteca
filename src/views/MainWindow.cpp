#include "MainWindow.h"
#include "./ui_MainWindow.h"
#include "pages/CategoriasPage.h"
#include "pages/LibrosPage.h"
#include "pages/EjemplaresPage.h"
#include "pages/LectoresPage.h"
#include "pages/PrestamosPage.h"
#include "pages/RecomendacionesPage.h"

#include <QFile>
#include <QDebug>
#include <QPushButton>
#include <QList>
#include <QStyle>
#include <QStringList>
#include <QWidget>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 1. INTEGRAR LA PANTALLA DE CATEGORÍAS

    CategoriasPage *categoriasPage = new CategoriasPage(this);

    QWidget *paginaVacia = ui->pageCategorias;

    int indiceCategorias =
        ui->stackedWidget->indexOf(paginaVacia);

    if (indiceCategorias >= 0) {
        ui->stackedWidget->removeWidget(paginaVacia);

        ui->stackedWidget->insertWidget(
            indiceCategorias,
            categoriasPage
            );

        delete paginaVacia;
    } else {
        delete categoriasPage;
        qWarning() << "No se encontró pageCategorias";
    }

    // 2. INTEGRAR LA PANTALLA DE LIBROS

    LibrosPage *librosPage = new LibrosPage(this);

    QWidget *paginaLibrosVacia = ui->pageLibros;

    int indiceLibros =
        ui->stackedWidget->indexOf(paginaLibrosVacia);

    if (indiceLibros >= 0) {
        ui->stackedWidget->removeWidget(paginaLibrosVacia);

        ui->stackedWidget->insertWidget(
            indiceLibros,
            librosPage
            );

        delete paginaLibrosVacia;
    } else {
        delete librosPage;
        qWarning() << "No se encontró pageLibros";
    }

    // 3. INTEGRAR LA PANTALLA DE EJEMPLARES

    EjemplaresPage *ejemplaresPage = new EjemplaresPage(this);

    QWidget *paginaEjemplaresVacia = ui->pageEjemplares;

    int indiceEjemplares =
        ui->stackedWidget->indexOf(paginaEjemplaresVacia);

    if (indiceEjemplares >= 0) {
        ui->stackedWidget->removeWidget(paginaEjemplaresVacia);

        ui->stackedWidget->insertWidget(
            indiceEjemplares,
            ejemplaresPage
            );

        delete paginaEjemplaresVacia;
    } else {
        delete ejemplaresPage;
        qWarning() << "No se encontró pageEjemplares";
    }


    // 4. INTEGRAR LA PANTALLA DE LECTORES

    LectoresPage *lectoresPage = new LectoresPage(this);

    QWidget *paginaLectoresVacia = ui->pageLectores;

    int indiceLectores =
        ui->stackedWidget->indexOf(paginaLectoresVacia);

    if (indiceLectores >= 0) {
        ui->stackedWidget->removeWidget(paginaLectoresVacia);

        ui->stackedWidget->insertWidget(
            indiceLectores,
            lectoresPage
            );

        delete paginaLectoresVacia;
    } else {
        delete lectoresPage;
        qWarning() << "No se encontró pageLectores";
    }


    // 5. INTEGRAR LA PANTALLA DE PRÉSTAMOS

    PrestamosPage *prestamosPage = new PrestamosPage(this);

    QWidget *paginaPrestamosVacia = ui->pagePrestamos;

    int indicePrestamos =
        ui->stackedWidget->indexOf(paginaPrestamosVacia);

    if (indicePrestamos >= 0) {
        ui->stackedWidget->removeWidget(paginaPrestamosVacia);

        ui->stackedWidget->insertWidget(
            indicePrestamos,
            prestamosPage
            );

        delete paginaPrestamosVacia;
    } else {
        delete prestamosPage;
        qWarning() << "No se encontró pagePrestamos";
    }


    // 6. INTEGRAR LA PANTALLA DE RECOMENDACIONES

    RecomendacionesPage *recomendacionesPage =
        new RecomendacionesPage(this);

    QWidget *paginaRecomendacionesVacia =
        ui->pageRecomendaciones;

    int indiceRecomendaciones =
        ui->stackedWidget->indexOf(paginaRecomendacionesVacia);

    if (indiceRecomendaciones >= 0) {
        ui->stackedWidget->removeWidget(
            paginaRecomendacionesVacia
            );

        ui->stackedWidget->insertWidget(
            indiceRecomendaciones,
            recomendacionesPage
            );

        delete paginaRecomendacionesVacia;
    } else {
        delete recomendacionesPage;
        qWarning() << "No se encontró pageRecomendaciones";
    }


    // 7. CARGAR LOS ESTILOS QSS

    QString estilos;

    const QStringList archivosEstilos = {
        ":/styles/main.qss",
        ":/styles/categorias.qss"
    };

    for (const QString &ruta : archivosEstilos) {

        QFile archivo(ruta);

        if (archivo.open(QIODevice::ReadOnly | QIODevice::Text)) {

            estilos += QString::fromUtf8(
                archivo.readAll()
                );

            estilos += "\n";

        } else {

            qWarning() << "No se pudo cargar:" << ruta;

        }
    }

    setStyleSheet(estilos);

    // 8. CONFIGURAR NAVEGACIÓN

    configurarNavegacion();

    cambiarPagina(0, ui->btnInicio);
}

MainWindow::~MainWindow()
{
    delete ui;
}

// CONFIGURACIÓN DE NAVEGACIÓN

void MainWindow::configurarNavegacion()
{
    connect(ui->btnInicio, &QPushButton::clicked,
            this, [this]() {
                cambiarPagina(0, ui->btnInicio);
            });

    connect(ui->btnCategorias, &QPushButton::clicked,
            this, [this]() {
                cambiarPagina(1, ui->btnCategorias);
            });

    connect(ui->btnLibros, &QPushButton::clicked,
            this, [this]() {
                cambiarPagina(2, ui->btnLibros);
            });

    connect(ui->btnEjemplares, &QPushButton::clicked,
            this, [this]() {
                cambiarPagina(3, ui->btnEjemplares);
            });

    connect(ui->btnLectores, &QPushButton::clicked,
            this, [this]() {
                cambiarPagina(4, ui->btnLectores);
            });

    connect(ui->btnPrestamos, &QPushButton::clicked,
            this, [this]() {
                cambiarPagina(5, ui->btnPrestamos);
            });

    connect(ui->btnRecomendaciones, &QPushButton::clicked,
            this, [this]() {
                cambiarPagina(6, ui->btnRecomendaciones);
            });
}

// CAMBIAR DE PÁGINA Y ACTUALIZAR BOTONES

void MainWindow::cambiarPagina(
    int indice,
    QPushButton *botonActivo
    )
{
    if (indice < 0 ||
        indice >= ui->stackedWidget->count()) {

        qWarning() << "Índice de página inválido:" << indice;
        return;
    }

    ui->stackedWidget->setCurrentIndex(indice);

    const QList<QPushButton *> botones = {
        ui->btnInicio,
        ui->btnCategorias,
        ui->btnLibros,
        ui->btnEjemplares,
        ui->btnLectores,
        ui->btnPrestamos,
        ui->btnRecomendaciones
    };

    for (QPushButton *boton : botones) {

        boton->setProperty(
            "activo",
            boton == botonActivo
            );

        boton->style()->unpolish(boton);
        boton->style()->polish(boton);
        boton->update();
    }
}
