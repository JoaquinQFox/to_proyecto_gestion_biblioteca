#ifndef INICIOPAGE_H
#define INICIOPAGE_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class InicioPage;
}
QT_END_NAMESPACE

class InicioPage : public QWidget
{
    Q_OBJECT

public:
    explicit InicioPage(QWidget *parent = nullptr);
    ~InicioPage() override;

private:
    Ui::InicioPage *ui;
};

#endif // INICIOPAGE_H
