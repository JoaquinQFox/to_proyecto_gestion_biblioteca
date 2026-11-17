
#ifndef CONFIRMACIONDIALOG_H
#define CONFIRMACIONDIALOG_H

#include <QDialog>
#include <QString>

class QLabel;
class QPushButton;

class ConfirmacionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ConfirmacionDialog(
        const QString &titulo,
        const QString &mensaje,
        QWidget *parent = nullptr
        );

    static bool confirmar(
        const QString &titulo,
        const QString &mensaje,
        QWidget *parent = nullptr
        );

private:
    void configurarInterfaz();

    QString m_titulo;
    QString m_mensaje;

    QLabel *m_lblTitulo = nullptr;
    QLabel *m_lblMensaje = nullptr;

    QPushButton *m_btnCancelar = nullptr;
    QPushButton *m_btnConfirmar = nullptr;
};

#endif // CONFIRMACIONDIALOG_H
