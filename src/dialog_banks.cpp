#include "dialog_banks.h"
#include "ui_dialog_banks.h"

dialog_banks::dialog_banks(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dialog_banks)
{
    ui->setupUi(this);

    QPixmap imatgeGranCash(":/icones/rsc/bank.png");
    ui->label->setPixmap(imatgeGranCash);
}

dialog_banks::~dialog_banks()
{
    delete ui;
}

/*
  Funcions per a inserir calés als bancs i fer el seguiment
*/
