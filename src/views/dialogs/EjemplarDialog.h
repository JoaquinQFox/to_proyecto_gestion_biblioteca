
#ifndef EJEMPLARDIALOG_H
#define EJEMPLARDIALOG_H

#include <QDialog>

namespace Ui {
class EjemplarDialog;
}

class EjemplarDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EjemplarDialog(QWidget *parent = nullptr);
    ~EjemplarDialog();

private:
    Ui::EjemplarDialog *ui;
};

#endif
