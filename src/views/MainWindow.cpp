#include "MainWindow.h"
#include "./ui_MainWindow.h"

#include <QFile>
#include <QDebug>
#include <QPushButton>
#include <QList>
#include <QStyle>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QFile archivoEstilos(":/styles/main.qss");

    if (archivoEstilos.open(QIODevice::ReadOnly | QIODevice::Text)) {
        setStyleSheet(QString::fromUtf8(archivoEstilos.readAll()));
    } else {
        qWarning() << "No se pudo cargar main.qss";
    }

    configurarNavegacion();
    cambiarPagina(0, ui->btnInicio);
}

MainWindow::~MainWindow()
{
    delete ui;
}

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

void MainWindow::cambiarPagina(int indice, QPushButton *botonActivo)
{
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
        boton->setProperty("activo", boton == botonActivo);

        boton->style()->unpolish(boton);
        boton->style()->polish(boton);
        boton->update();
    }
}
