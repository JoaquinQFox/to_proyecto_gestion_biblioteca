#include "MainWindow.h"
#include "./ui_MainWindow.h"

#include <QFile>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QFile archivoEstilos(":/styles/main.qss");

    if (archivoEstilos.open(QIODevice::ReadOnly | QIODevice::Text)) {
        setStyleSheet(QString::fromUtf8(archivoEstilos.readAll()));
        archivoEstilos.close();
    } else {
        qWarning() << "No se pudo cargar main.qss";
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}
