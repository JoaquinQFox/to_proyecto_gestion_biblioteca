#ifndef PRESTAMOSPAGE_H
#define PRESTAMOSPAGE_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class PrestamosPage;
}
QT_END_NAMESPACE

class PrestamosPage : public QWidget
{
    Q_OBJECT

public:
    explicit PrestamosPage(QWidget *parent = nullptr);
    ~PrestamosPage() override;

private:
    Ui::PrestamosPage *ui;
};

#endif // PRESTAMOSPAGE_H
