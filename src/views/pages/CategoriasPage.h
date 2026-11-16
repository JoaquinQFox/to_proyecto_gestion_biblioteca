#ifndef CATEGORIASPAGE_H
#define CATEGORIASPAGE_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class CategoriasPage;
}
QT_END_NAMESPACE

class CategoriasPage : public QWidget
{
    Q_OBJECT

public:
    explicit CategoriasPage(QWidget *parent = nullptr);
    ~CategoriasPage() override;

private:
    Ui::CategoriasPage *ui;
};

#endif // CATEGORIASPAGE_H
