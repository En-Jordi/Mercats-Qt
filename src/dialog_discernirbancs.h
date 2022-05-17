#ifndef DIALOG_DISCERNIRBANCS_H
#define DIALOG_DISCERNIRBANCS_H

#include "dialog_banks.h"

#include <QDialog>

namespace Ui {
class dialog_discernirBancs;
}

class dialog_discernirBancs : public QDialog
{
    Q_OBJECT

public:
    explicit dialog_discernirBancs(QWidget *parent = nullptr);
    ~dialog_discernirBancs();

private slots:
    void on_pushButton_ConsultaComptesBancs_clicked();

    void on_pushButtonInserirBanc_clicked();

private:
    Ui::dialog_discernirBancs *ui;

    dialog_banks *nouDialog_Banks;
};

#endif // DIALOG_DISCERNIRBANCS_H
