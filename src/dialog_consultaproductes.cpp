#include "dialog_consultaproductes.h"
#include "ui_dialog_consultaproductes.h"

dialog_consultaProductes::dialog_consultaProductes(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dialog_consultaProductes)
{
    ui->setupUi(this);
}

dialog_consultaProductes::~dialog_consultaProductes()
{
    delete ui;
}
