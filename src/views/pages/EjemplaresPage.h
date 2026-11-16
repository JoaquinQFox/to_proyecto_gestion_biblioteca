#ifndef EJEMPLARESPAGE_H
#define EJEMPLARESPAGE_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class EjemplaresPage;
}
QT_END_NAMESPACE

class EjemplaresPage : public QWidget
{
    Q_OBJECT

public:
    explicit EjemplaresPage(QWidget *parent = nullptr);
    ~EjemplaresPage() override;

private:
    Ui::EjemplaresPage *ui;
};

#endif // EJEMPLARESPAGE_H
