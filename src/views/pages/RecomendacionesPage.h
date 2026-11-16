#ifndef RECOMENDACIONESPAGE_H
#define RECOMENDACIONESPAGE_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class RecomendacionesPage;
}
QT_END_NAMESPACE

class RecomendacionesPage : public QWidget
{
    Q_OBJECT

public:
    explicit RecomendacionesPage(QWidget *parent = nullptr);
    ~RecomendacionesPage() override;

private:
    Ui::RecomendacionesPage *ui;
};

#endif // RECOMENDACIONESPAGE_H
