#ifndef DIALOG_BENEFICIARIS_H
#define DIALOG_BENEFICIARIS_H

#include <QDialog>

#include "bbdd/auxiliar_bbdd.h"


namespace Ui {
class dialog_beneficiaris;
}

class dialog_beneficiaris : public QDialog
{
    Q_OBJECT

public:
    explicit dialog_beneficiaris(QWidget *parent = nullptr);
    ~dialog_beneficiaris();

private slots:
    void on_pushButton_clicked();
    void inici();

private:
    Ui::dialog_beneficiaris *ui;
};

#endif // DIALOG_BENEFICIARIS_H
