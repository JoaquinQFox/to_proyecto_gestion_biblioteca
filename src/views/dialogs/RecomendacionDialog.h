
#ifndef RECOMENDACIONDIALOG_H
#define RECOMENDACIONDIALOG_H

#include <QDialog>

namespace Ui {
class RecomendacionDialog;
}

class RecomendacionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RecomendacionDialog(QWidget *parent = nullptr);
    ~RecomendacionDialog();

private:
    Ui::RecomendacionDialog *ui;
};

#endif
