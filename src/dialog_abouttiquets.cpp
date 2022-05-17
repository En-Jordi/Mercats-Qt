#include "dialog_abouttiquets.h"
#include "ui_dialog_abouttiquets.h"

dialog_AboutTiquets::dialog_AboutTiquets(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dialog_AboutTiquets)
{
    ui->setupUi(this);

    ui->label_2->setText(titolPrograma_NOM);
    ui->textBrowser->setText(QString(titolPrograma_NOM) + "\n\nGuarda els tiquets i les compres en format digital");


    QPixmap imatgeGranCash(":/icones/rsc/cash.png");
    ui->label->setPixmap(imatgeGranCash);
}

dialog_AboutTiquets::~dialog_AboutTiquets()
{
    delete ui;
}
