#ifndef DIALOG_CONSULTAPRODUCTES_H
#define DIALOG_CONSULTAPRODUCTES_H

#include <QDialog>

namespace Ui {
class dialog_consultaProductes;
}

class dialog_consultaProductes : public QDialog
{
    Q_OBJECT

public:
    explicit dialog_consultaProductes(QWidget *parent = nullptr);
    ~dialog_consultaProductes();

private:
    Ui::dialog_consultaProductes *ui;
};

#endif // DIALOG_CONSULTAPRODUCTES_H
