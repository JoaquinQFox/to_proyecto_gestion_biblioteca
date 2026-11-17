
#include "ConfirmacionDialog.h"

#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFrame>
#include <QStyle>
#include <QFile>
#include <QIODevice>
#include <QDebug>

ConfirmacionDialog::ConfirmacionDialog(
    const QString &titulo,
    const QString &mensaje,
    QWidget *parent
    )
    : QDialog(parent)
    , m_titulo(titulo)
    , m_mensaje(mensaje)
{
    configurarInterfaz();

    // Cargar estilos exclusivos del dialogo
    QFile archivo(":/styles/confirmacion_dialog.qss");

    if (archivo.open(QIODevice::ReadOnly | QIODevice::Text)) {
        setStyleSheet(QString::fromUtf8(archivo.readAll()));
    } else {
        qWarning() << "No se pudo cargar confirmacion_dialog.qss:"
                   << archivo.errorString();
    }
}

void ConfirmacionDialog::configurarInterfaz()
{
    setObjectName("confirmacionDialog");
    setWindowTitle(m_titulo);
    setModal(true);
    setMinimumWidth(420);
    setMaximumWidth(520);

    auto *layoutPrincipal = new QVBoxLayout(this);
    layoutPrincipal->setContentsMargins(28, 26, 28, 24);
    layoutPrincipal->setSpacing(18);

    // Encabezado
    auto *layoutEncabezado = new QHBoxLayout();
    layoutEncabezado->setSpacing(14);

    auto *lblIcono = new QLabel(this);
    lblIcono->setObjectName("lblIconoConfirmacion");
    lblIcono->setFixedSize(44, 44);
    lblIcono->setAlignment(Qt::AlignCenter);

    QIcon icono = style()->standardIcon(
        QStyle::SP_MessageBoxWarning
        );

    lblIcono->setPixmap(icono.pixmap(28, 28));

    m_lblTitulo = new QLabel(m_titulo, this);
    m_lblTitulo->setObjectName("lblTituloConfirmacion");
    m_lblTitulo->setWordWrap(true);

    layoutEncabezado->addWidget(lblIcono);
    layoutEncabezado->addWidget(m_lblTitulo, 1);

    layoutPrincipal->addLayout(layoutEncabezado);

    // Mensaje
    m_lblMensaje = new QLabel(m_mensaje, this);
    m_lblMensaje->setObjectName("lblMensajeConfirmacion");
    m_lblMensaje->setWordWrap(true);
    m_lblMensaje->setTextInteractionFlags(
        Qt::TextSelectableByMouse
        );

    layoutPrincipal->addWidget(m_lblMensaje);

    // Separador
    auto *separador = new QFrame(this);
    separador->setObjectName("separadorConfirmacion");
    separador->setFrameShape(QFrame::HLine);

    layoutPrincipal->addWidget(separador);

    // Botones
    auto *layoutBotones = new QHBoxLayout();
    layoutBotones->setSpacing(10);

    m_btnCancelar = new QPushButton("Cancelar", this);
    m_btnCancelar->setObjectName("btnCancelarConfirmacion");
    m_btnCancelar->setMinimumHeight(38);
    m_btnCancelar->setAutoDefault(false);

    m_btnConfirmar = new QPushButton("Confirmar", this);
    m_btnConfirmar->setObjectName("btnAceptarConfirmacion");
    m_btnConfirmar->setMinimumHeight(38);
    m_btnConfirmar->setDefault(true);

    layoutBotones->addStretch();
    layoutBotones->addWidget(m_btnCancelar);
    layoutBotones->addWidget(m_btnConfirmar);

    layoutPrincipal->addLayout(layoutBotones);

    // Eventos
    connect(
        m_btnCancelar,
        &QPushButton::clicked,
        this,
        &QDialog::reject
        );

    connect(
        m_btnConfirmar,
        &QPushButton::clicked,
        this,
        &QDialog::accept
        );
}

bool ConfirmacionDialog::confirmar(
    const QString &titulo,
    const QString &mensaje,
    QWidget *parent
    )
{
    ConfirmacionDialog dialog(titulo, mensaje, parent);

    return dialog.exec() == QDialog::Accepted;
}
