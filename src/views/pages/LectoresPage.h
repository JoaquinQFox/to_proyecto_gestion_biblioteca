#ifndef LECTORESPAGE_H
#define LECTORESPAGE_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class LectoresPage;
}
QT_END_NAMESPACE

class LectoresPage : public QWidget
{
    Q_OBJECT

public:
    explicit LectoresPage(QWidget *parent = nullptr);
    ~LectoresPage() override;

private:
    Ui::LectoresPage *ui;
};

#endif // LECTORESPAGE_H
