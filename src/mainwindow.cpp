#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>

#include <QFrame>
#include <QToolBar>


/*
    Les funcions:
        - void MainWindow::on_pushButton_Comptes_clicked()
        - void MainWindow::on_actionBanc_triggered()
    obren LA finestra que ens demana:
        - INSERIR
        - CONSULTAR
    els comptes bancaris
*/


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->layout = new QVBoxLayout;

    ui->actionAbout_programa->setText("Sobre  " + QString(titolPrograma_NOM));

    ui->pushButton_Metges->setHidden(false);
    ui->pushButton_4->setHidden(true);
    ui->pushButton_5->setHidden(true);

    /* Fem que l'amplada de la columna sigui de 240 pixels */
    //ui->tableWidget->setColumnWidth(0,240);
    //ui->tableWidget->setColumnWidth(1,240);
    ui->tableWidget->resizeColumnsToContents();

    /* Fem que el text de la columna 1 de totes les files estigui centrat */
    for (qint8 filesTableWidget = 0; filesTableWidget < ui->tableWidget->rowCount(); filesTableWidget++)
    {
        ui->tableWidget->model()->setData(ui->tableWidget->model()->index(filesTableWidget,1),Qt::AlignCenter, Qt::TextAlignmentRole);
    }


    QPushButton *back = new QPushButton("<");
    back->setMaximumWidth(60);
    back->setMaximumHeight(40);
    QPushButton *next = new QPushButton(">");
    back->setMaximumWidth(60);
    back->setMaximumHeight(40);
    QLabel *label_date = new QLabel("MESOS");
    label_date->setAlignment(Qt::AlignCenter);
    label_date->setFixedWidth(400);
    label_date->setMaximumHeight(40);

    QHBoxLayout *horizontal_layout = new QHBoxLayout;
    horizontal_layout->addWidget(back, 1, Qt::AlignRight);
    horizontal_layout->addWidget(label_date, 1, Qt::AlignCenter);
    horizontal_layout->addWidget(next, 1, Qt::AlignLeft);
    //ui->widget->setLayout(horizontal_layout);
    this->layout->addLayout(horizontal_layout);

    QGridLayout *grid_layout = new QGridLayout;
    int i,j;
    for (j = 0; j < 7 ; j++)
    {
        QFrame *frame = new QFrame;
        QHBoxLayout *hl = new QHBoxLayout;
        hl->setAlignment(Qt::AlignCenter);
        frame->setFixedHeight(50);
        QLabel *wday_name = new QLabel(QString::number(j+1));
        wday_name->setObjectName("header");
        frame->setObjectName("header");
        hl->addWidget(wday_name);
        hl->setMargin(0);
        frame->setLayout(hl);
        grid_layout->addWidget(frame,0,j);
    }

    for ( i = 1; i < 7; i++)
    {
        for ( j = 0 ; j < 7 ; j++)
        {
            QFrameExtended *frame = createQFrameExtended(NULL);
            grid_layout->addWidget(frame, i, j);
            this->frames[7*(i-1)+j] = frame;
        }
    }
    grid_layout->setHorizontalSpacing(0);
    grid_layout->setVerticalSpacing(0);
    this->layout->addLayout(grid_layout);

    ui->widget->setLayout(this->layout);
}

MainWindow::~MainWindow()
{
    delete ui;
}


QFrameExtended* MainWindow::createQFrameExtended(Date * date)
{
    QFrameExtended *frame = new QFrameExtended;
    QVBoxLayout *vl = new QVBoxLayout;
    frame->setDate(date);
    frame->setObjectName("day");
    vl->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    vl->setMargin(0);
    vl->setSpacing(1);
    vl->addWidget(new QLabel);
    frame->setMinimumWidth(150);
    frame->setMinimumHeight(60);
    frame->setLayout(vl);

    frame->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    return frame;
}

void MainWindow::on_actionAbout_programa_triggered()
{
    nouDialog_About = new dialog_AboutTiquets(this);
    nouDialog_About->setWindowTitle("Sobre  " + QString(titolPrograma_NOM) + "  " + QString(titolPrograma_VERSIO) + "  " + QString(titolPrograma_RELEASE));
    // EL mostrem amb show perquè no retorna res
    nouDialog_About->show();
}

void MainWindow::on_pushButton_Comptes_clicked()
{
    triaBanks = new dialog_discernirBancs(this);
    triaBanks->setWindowTitle("Escull acció");
    QPixmap imatgeGranCash(":/iconesrsc/bank.png");
    triaBanks->setWindowIcon(imatgeGranCash);
    triaBanks->show();
}

void MainWindow::on_pushButton_Operacions_clicked()
{
    nouDialog_Operacions = new dialog_selecciooperacions(this);
    nouDialog_Operacions->setWindowTitle("Operacions");

    nouDialog_Operacions->show();
}


void MainWindow::on_actionBanc_triggered()
{
    nouDialog_Banks = new dialog_banks(this);
    nouDialog_Banks->setWindowTitle("Nou Banc");
    QPixmap imatgeGranCash(":/icones/rsc/bank.png");
    nouDialog_Banks->setWindowIcon(imatgeGranCash);
    nouDialog_Banks->show();
}

void MainWindow::on_actionBeneficiari_triggered()
{
    nouDialog_Beneficiaris = new dialog_beneficiaris(this);
    nouDialog_Beneficiaris->setWindowTitle("Nou Comerç");
    QPixmap imatgeGranCash(":/icones/rsc/wanted-reward.png");
    nouDialog_Beneficiaris->setWindowIcon(imatgeGranCash);
    nouDialog_Beneficiaris->show();
}
