#ifndef LIBRODIALOG_H
#define LIBRODIALOG_H

#include <QDialog>

namespace Ui {
class LibroDialog;
}

class LibroDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LibroDialog(QWidget *parent = nullptr);
    ~LibroDialog();

private:
    Ui::LibroDialog *ui;
};

#endif
