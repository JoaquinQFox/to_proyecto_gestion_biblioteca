
#ifndef PRESTAMODIALOG_H
#define PRESTAMODIALOG_H

#include <QDialog>

namespace Ui {
class PrestamoDialog;
}

class PrestamoDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PrestamoDialog(QWidget *parent = nullptr);
    ~PrestamoDialog();

private:
    Ui::PrestamoDialog *ui;
};

#endif
