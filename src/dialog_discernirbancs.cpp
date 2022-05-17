#include "dialog_discernirbancs.h"
#include "ui_dialog_discernirbancs.h"

dialog_discernirBancs::dialog_discernirBancs(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dialog_discernirBancs)
{
    ui->setupUi(this);
}

dialog_discernirBancs::~dialog_discernirBancs()
{
    delete ui;
}

void dialog_discernirBancs::on_pushButton_ConsultaComptesBancs_clicked()
{

}

void dialog_discernirBancs::on_pushButtonInserirBanc_clicked()
{
    nouDialog_Banks = new dialog_banks(this);
    nouDialog_Banks->setWindowTitle("Nou Banc");
    QPixmap imatgeGranCash(":/icones/rsc/bank.png");
    nouDialog_Banks->setWindowIcon(imatgeGranCash);
    nouDialog_Banks->show();
}
