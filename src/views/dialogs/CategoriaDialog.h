#ifndef CATEGORIADIALOG_H
#define CATEGORIADIALOG_H

#include <QDialog>

namespace Ui {
class CategoriaDialog;
}

class CategoriaDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CategoriaDialog(QWidget *parent = nullptr);
    ~CategoriaDialog();

private:
    Ui::CategoriaDialog *ui;
};

#endif
