#ifndef LIBROSPAGE_H
#define LIBROSPAGE_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class LibrosPage;
}
QT_END_NAMESPACE

class LibrosPage : public QWidget
{
    Q_OBJECT

public:
    explicit LibrosPage(QWidget *parent = nullptr);
    ~LibrosPage() override;

private:
    Ui::LibrosPage *ui;
};

#endif // LIBROSPAGE_H
