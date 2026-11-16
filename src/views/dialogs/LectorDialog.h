
#ifndef LECTORDIALOG_H
#define LECTORDIALOG_H

#include <QDialog>

namespace Ui {
class LectorDialog;
}

class LectorDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LectorDialog(QWidget *parent = nullptr);
    ~LectorDialog();

private:
    Ui::LectorDialog *ui;
};

#endif
