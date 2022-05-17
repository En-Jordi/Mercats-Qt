#include "dialog_beneficiaris.h"
#include "ui_dialog_beneficiaris.h"

#include <QDebug>

QStringList CAP_Table = {"Comerç"};


dialog_beneficiaris::dialog_beneficiaris(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dialog_beneficiaris)
{
    ui->setupUi(this);

    /*
      Codi per a mostrar els comerços existents
    */
    inici();

}

dialog_beneficiaris::~dialog_beneficiaris()
{
    delete ui;
}

void dialog_beneficiaris::inici()
{
    QStringList llista = retornaComercos();
    ui->tableWidget->setColumnCount(1);
    ui->tableWidget->setHorizontalHeaderLabels(CAP_Table);

    for (int i = 0; i < llista.length(); i++)
    {
        ui->tableWidget->insertRow(ui->tableWidget->rowCount());
        ui->tableWidget->setItem(i,0, new QTableWidgetItem(llista.at(i)));
    }
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget->resizeColumnsToContents();
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

}

void dialog_beneficiaris::on_pushButton_clicked()
{
    if (!ui->lineEdit->text().isEmpty() && !ui->lineEdit->text().isNull())
    {
        inserirComerc(ui->lineEdit->text().toUtf8());

        ui->tableWidget->reset();
        ui->tableWidget->clear();
        inici();
    }
}
