// Problemes línia:  85

#include "dialog_selecciooperacions.h"
#include "ui_dialog_selecciooperacions.h"

#include <QDebug>
#include <QLineEdit>
#include <QTableWidgetItem>
#include <QAbstractItemView>
#include <QVector>

#include <QMessageBox>
#include <QString>

#define NO_OFERTES 0
#define SI_OFERTES 1

QStringList header_Productes = {"nom", "família", "descripció", "ean", "pvp"};
QStringList header_LlistaCompra = {"Producte", "Quantitat"};

QVector<QVector<QString>> llistaProductes_noOfertes;
QVector<QVector<QString>> llistaProductes_siOfertes;

QVector<QVector<QString>> llistaCompra;
QVector<uint16_t> indexProductes;
QVector<uint16_t> TEMPORAL;

//Fer que tablewidget_LlistaCompra la segona columna només accepti enters
//Guardar la tablewidget_LlistaCompra a llistaCompra

dialog_selecciooperacions::dialog_selecciooperacions(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::dialog_selecciooperacions)
{
    ui->setupUi(this);

    // Mostrar la data actual
    ui->label_6->setText(ui->calendarWidget->selectedDate().toString("yyyy-MM-dd"));

    ui->lineEdit_descomptes->setDisabled(true);

    // Omplir la taula de productes
    ui->tableWidget_Productes->setColumnCount(5);
    ui->tableWidget_Productes->setHorizontalHeaderLabels(header_Productes);
    ui->tableWidget_Productes->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // Crear els encapçalaments de la llista de la compra
    ui->tableWidget_LlistaCompra->setColumnCount(2);
    ui->tableWidget_LlistaCompra->setHorizontalHeaderLabels(header_LlistaCompra);

    // Cerca els comerços i omple'ls en el comboBox
    QStringList llistaComercos = retornaComercos();
    ui->comboBox->addItem("");
    ui->comboBox->addItems(llistaComercos);

    // Carreguem els productes
    carregaLlistaInici();
    connect(ui->comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(carregaLlista()));
    // SI seleccionem mostrar ofertes, es carrega la llista
    connect(ui->checkBox_Ofertes, SIGNAL(stateChanged(int)), this, SLOT(discriminadorOfertes()));

    connect(ui->calendarWidget, SIGNAL(selectionChanged()), this, SLOT(canviaData()));

    connect(ui->tableWidget_Productes, SIGNAL(cellClicked()), this, SLOT(carregaCompra()));

    connect(ui->checkBox_descomptes, SIGNAL(stateChanged(int)), this, SLOT(canviaEstatDescomptes_lineEdit()));
}

dialog_selecciooperacions::~dialog_selecciooperacions()
{
    delete ui;
}

/*
   Funció que crida el dialog per inserir productes.
   S'ha de crear a dins del auxiliar_bbdd

   A desenvolupar
*/
void dialog_selecciooperacions::on_actionProducte_general_triggered()
{
    nouDialog_Productes = new dialog_inserirproductes(this);
    nouDialog_Productes->setWindowTitle("Inserir producte");
    QPixmap imatgeGranCash(":/icones/rsc/rolled-cloth.png");
    nouDialog_Productes->setWindowIcon(imatgeGranCash);
    nouDialog_Productes->show();
}

// Funció per entrar algun descompte al preu final (producte que ens hagin fet un descompte puntual)
void dialog_selecciooperacions::canviaEstatDescomptes_lineEdit()
{
    if (ui->tableWidget_LlistaCompra->rowCount() != 0)
    {
        if (ui->checkBox_descomptes->isChecked())
        {
            /*
               v1: Activem el lineEdit per escriure el descompte
               v2: (no implementat encara) mostrar la llista i indicar quin element té la segona unitat a meitat de preu
            */
            /* v1
            ui->lineEdit_descomptes->setEnabled(true);
            ui->lineEdit_descomptes->setAlignment(Qt::AlignCenter);

            ui->lineEdit_descomptes->setValidator( new QDoubleValidator(0, 100, 2, this) );
            */
            /* v2 */
            QDialog *nouDialog = new QDialog(this);
            QTableWidget *nouTable = new QTableWidget(this);
            nouTable->setColumnCount(1);
            nouTable->setHorizontalHeaderItem(0, new QTableWidgetItem("Descompte"));
            QGridLayout *nouGrid = new QGridLayout();
            QLabel *nouLabel = new QLabel();


            nouTable->horizontalHeader()->setStretchLastSection(true);
            nouTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

            uint64_t id_oferta_2a = 0;

            //qDebug() << "Productes temp long:\t" << indexProductes.length();
            for (int i=0; i< ui->tableWidget_LlistaCompra->rowCount(); i++)
            {
                nouTable->insertRow(i);
                nouTable->setItem(i, 0, new QTableWidgetItem(ui->tableWidget_LlistaCompra->item(i,0)->text() ) );
            }

            QVector<QVector<int>> matriu;
            for (int i = 0; i < indexProductes.length() ; i++)
            {
                QVector<int> tempVector;
                qDebug() << "quantiTat: " << QString::number(ui->tableWidget_LlistaCompra->item(i,1)->text().toInt()).toInt() << "\tfkey_producte: " << indexProductes.at(i);
                if (QString::number(ui->tableWidget_LlistaCompra->item(i,1)->text().toInt()).toInt() >= 2)
                {
                    /* Buscar el "id" del historicpreus on
                    select id from historicpreus where ofertes = '2a' and fkey_producte = (select fkey_producte from historicpreus where id = <indexProductes.at(i)> )

                    i afegir el "id" de la oferta '2a' con un nou producte.
                    */

                    id_oferta_2a = troba_id_oferta_segona_unitat(indexProductes.at(i), ui->comboBox->currentIndex());


                    qDebug() << "hooooola\t" << indexProductes.at(i) << "\tID:\t" << id_oferta_2a;
                }

                /*
                for (int j = 0; j < 1; j++)
                {
                    //Dóna problemes duplica els valors afegint-hi dos de més
                    tempVector.push_back(ui->tableWidget_LlistaCompra->item(i,1)->text().toInt());
                    tempVector.push_back(indexProductes.at(i));
                }
                matriu.push_back(tempVector);
                */
            }

            nouGrid->addWidget(nouTable);
            nouGrid->addWidget(nouLabel);
            nouDialog->setLayout(nouGrid);
            nouDialog->exec();

            /*
               7/11/2021
            */
            QMessageBox::information(nouDialog, "Hola", "Element seleccionat:\t" + QString::number(nouTable->currentRow()) );

            qDebug() << "Current row:\t" << nouTable->currentRow();

            QVector<int> fila;
            fila.push_back(nouTable->currentRow());

            //qDebug() << "longitud vector: " << fila.length();
            qDebug() << "ID 2a:\t" << id_oferta_2a;

            /*
               Codi que només llistarà els productes etiquetats com a
               ofertes = '2a'

               (Primer verificar que hi ha algun element que tingui 2 unitats. (Vigilar si element:
                    waffer
                    galetes
                    tutti
               )
               (Funció sql que cerqui entre l'array de nombres que li passem, quin té com a oferta '2a')
             */
            //TODO
            //la següent línia és orientativa. HA D'ELIMINAR-SE
            //QMessageBox::information(this, "Descomptes", "Selecció:\t" + ui->tableWidget_LlistaCompra->item(fila.at(0),0)->text() );
        }
    }
}

// La següent funció mostra el dia que es selecciona al calendari.
void dialog_selecciooperacions::canviaData()
{
    ui->label_6->setText(ui->calendarWidget->selectedDate().toString("yyyy-MM-dd"));
}

void dialog_selecciooperacions::carregaLlistaInici()
{
    QVector<QVector<QString>> hola;
    /* Cridem a AUXILIAR_BBDD on fem la crida de DO $$ */
    hola = creaInici_taula_temporal_hostia();

    //ui->tableWidget_Productes->setRowCount(0);
    ui->tableWidget_Productes->model()->removeRows(0, ui->tableWidget_Productes->rowCount());

    for (int i = 0; i < hola.length(); i++)
    {
        ui->tableWidget_Productes->insertRow(ui->tableWidget_Productes->rowCount());
        for (int j=0; j < 6; j++)
        {
            ui->tableWidget_Productes->setItem(i,j, new QTableWidgetItem(hola.at(i).value(j)));
        }
    }
    ui->tableWidget_Productes->resizeColumnsToContents();
}

void dialog_selecciooperacions::carregaLlista()
{
    ui->tableWidget_Productes->reset();
    ui->tableWidget_Productes->clear();

    //ui->tableWidget_Productes->setRowCount(0);
    ui->tableWidget_Productes->model()->removeRows(0, ui->tableWidget_Productes->rowCount());
    ui->tableWidget_Productes->setColumnCount(5);
    ui->tableWidget_Productes->setHorizontalHeaderLabels(header_Productes);
    ui->tableWidget_Productes->setEditTriggers(QAbstractItemView::NoEditTriggers);


    ui->tableWidget_LlistaCompra->reset();
    ui->tableWidget_LlistaCompra->clear();

    ui->tableWidget_LlistaCompra->setRowCount(0);
    ui->tableWidget_LlistaCompra->model()->removeRows(0, ui->tableWidget_Productes->rowCount());
    ui->tableWidget_LlistaCompra->setColumnCount(2);
    ui->tableWidget_LlistaCompra->setHorizontalHeaderLabels(header_LlistaCompra);
    //ui->tableWidget_LlistaCompra->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // NO ELIMINAR AQUESTES DUES LÍNIES!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    disconnect(ui->tableWidget_Productes, SIGNAL(cellChanged(int, int)), this, SLOT(posicioProducte()));
    disconnect(ui->checkBox_edicioPreu, SIGNAL(stateChanged(int)), this, SLOT(edicioPreus_TableWidget()));
    disconnect(ui->checkBox_Ofertes, SIGNAL(stateChanged(int)), this, SLOT(discriminadorOfertes()));


    if (llistaProductes_noOfertes.empty() == false)
    {
        llistaProductes_noOfertes.clear();
        if (indexProductes.empty() == false)
            indexProductes.clear();
    }
    if (llistaProductes_siOfertes.empty() == false)
    {
        llistaProductes_siOfertes.clear();
        if (indexProductes.empty() == false)
            indexProductes.clear();
    }


    QVector<QVector<QString>> hola;

    if (!ui->checkBox_Ofertes->isChecked())
    {
        hola = retornaProductesGeneral(ui->comboBox->currentIndex(), NO_OFERTES);
        llistaProductes_noOfertes = hola;
    }
    else if (ui->checkBox_Ofertes->isChecked())
    {
        hola = retornaProductesGeneral(ui->comboBox->currentIndex(), SI_OFERTES);
        llistaProductes_siOfertes = hola;
    }

    for (int i = 0; i < hola.length(); i++)
    {
        ui->tableWidget_Productes->insertRow(ui->tableWidget_Productes->rowCount());
        for (int j=1; j < 6; j++)
        {
            ui->tableWidget_Productes->setItem(i,j-1, new QTableWidgetItem(hola.at(i).value(j)));
        }
    }
    ui->tableWidget_Productes->resizeColumnsToContents();

    //https://stackoverflow.com/questions/13310183/how-to-emit-a-signal-when-cell-item-is-edited
    //https://stackoverflow.com/questions/28524925/how-to-disconnect-a-signal-with-a-slot-temporarily-in-qt
    connect(ui->checkBox_edicioPreu, SIGNAL(stateChanged(int)), this, SLOT(edicioPreus_TableWidget()));
    connect(ui->checkBox_Ofertes, SIGNAL(stateChanged(int)), this, SLOT(discriminadorOfertes()));
}

void dialog_selecciooperacions::discriminadorOfertes()
{
    ui->tableWidget_Productes->reset();
    ui->tableWidget_Productes->clear();

    //ui->tableWidget_Productes->setRowCount(0);
    ui->tableWidget_Productes->model()->removeRows(0, ui->tableWidget_Productes->rowCount());
    ui->tableWidget_Productes->setColumnCount(5);
    ui->tableWidget_Productes->setHorizontalHeaderLabels(header_Productes);
    ui->tableWidget_Productes->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // NO ELIMINAR AQUESTES TRES LÍNIES!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    disconnect(ui->tableWidget_Productes, SIGNAL(cellChanged(int, int)), this, SLOT(posicioProducte()));
    disconnect(ui->checkBox_edicioPreu, SIGNAL(stateChanged(int)), this, SLOT(edicioPreus_TableWidget()));
    disconnect(ui->checkBox_Ofertes, SIGNAL(stateChanged(int)), this, SLOT(discriminadorOfertes()));

    QMessageBox::information(this, "oju", "Ha sigut activat");

    if (ui->checkBox_Ofertes->isChecked()) {
        QVector<QVector<QString>> hola;

        hola = retornaProductesGeneral(ui->comboBox->currentIndex(), SI_OFERTES);
        llistaProductes_siOfertes = hola;

        for (int i = 0; i < hola.length(); i++)
        {
            ui->tableWidget_Productes->insertRow(ui->tableWidget_Productes->rowCount());
            for (int j=1; j < 6; j++)
            {
                ui->tableWidget_Productes->setItem(i,j-1, new QTableWidgetItem(hola.at(i).value(j)));
            }
        }
        ui->tableWidget_Productes->resizeColumnsToContents();
        //https://stackoverflow.com/questions/13310183/how-to-emit-a-signal-when-cell-item-is-edited
        //https://stackoverflow.com/questions/28524925/how-to-disconnect-a-signal-with-a-slot-temporarily-in-qt
        connect(ui->checkBox_edicioPreu, SIGNAL(stateChanged(int)), this, SLOT(edicioPreus_TableWidget()));
        connect(ui->checkBox_Ofertes, SIGNAL(stateChanged(int)), this, SLOT(discriminadorOfertes()));
    }
    else if (!ui->checkBox_Ofertes->isChecked())
    {
        QVector<QVector<QString>> hola;

        hola = retornaProductesGeneral(ui->comboBox->currentIndex(), NO_OFERTES);
        llistaProductes_noOfertes = hola;

        for (int i = 0; i < hola.length(); i++)
        {
            ui->tableWidget_Productes->insertRow(ui->tableWidget_Productes->rowCount());
            for (int j=1; j < 6; j++)
            {
                ui->tableWidget_Productes->setItem(i,j-1, new QTableWidgetItem(hola.at(i).value(j)));
            }
        }
        ui->tableWidget_Productes->resizeColumnsToContents();

        //https://stackoverflow.com/questions/13310183/how-to-emit-a-signal-when-cell-item-is-edited
        //https://stackoverflow.com/questions/28524925/how-to-disconnect-a-signal-with-a-slot-temporarily-in-qt
        connect(ui->checkBox_edicioPreu, SIGNAL(stateChanged(int)), this, SLOT(edicioPreus_TableWidget()));
        connect(ui->checkBox_Ofertes, SIGNAL(stateChanged(int)), this, SLOT(discriminadorOfertes()));
    }

}

void dialog_selecciooperacions::on_actionConsulta_triggered()
{
    on_actionInserir_triggered();
}

void dialog_selecciooperacions::on_actionInserir_triggered()
{
    nouDialog_Beneficiaris = new dialog_beneficiaris(this);
    nouDialog_Beneficiaris->setWindowTitle("Llista comerços");
    QPixmap imatgeGranCash(":/icones/rsc/wanted-reward.png");
    nouDialog_Beneficiaris->setWindowIcon(imatgeGranCash);
    nouDialog_Beneficiaris->show();
}

void dialog_selecciooperacions::on_actionConsulta_Productes_triggered()
{
    nouDialog_consultaProductes = new dialog_consultaProductes(this);
    nouDialog_consultaProductes->setWindowTitle("Consulta productes");
    QPixmap imatgeGranCash(":/icones/rsc/rolled-cloth.png");
    nouDialog_consultaProductes->setWindowIcon(imatgeGranCash);
    nouDialog_consultaProductes->show();
}

void dialog_selecciooperacions::on_tableWidget_Productes_itemDoubleClicked(QTableWidgetItem *item)
{
    /* Si el comboBox està a 0 => Cap beneficiari seleccionat
     * no permetre que s'afegeixin productes a la llista
     *
     * El indexProducte que afegeix fent pushBack és el "id" del historic preus
     */
    if (ui->comboBox->currentIndex() == 0)
    {
        QMessageBox::information(this, "Avís informatiu","No es poden afegir productes sense\nsaber qui és el comerç (beneficiari).");
    }
    else {
        ui->tableWidget_LlistaCompra->insertRow(ui->tableWidget_LlistaCompra->rowCount());

        ui->tableWidget_LlistaCompra->setItem(ui->tableWidget_LlistaCompra->rowCount() - 1, 1, new QTableWidgetItem(QString::number(1)));
        ui->tableWidget_LlistaCompra->setItem(ui->tableWidget_LlistaCompra->rowCount() - 1, 0, new QTableWidgetItem(ui->tableWidget_Productes->item(item->row(), 2)->text()));
        ui->tableWidget_LlistaCompra->resizeColumnsToContents();

        if (!ui->checkBox_Ofertes->isChecked())
        {
            // No oferta
            indexProductes.push_back(llistaProductes_noOfertes.at(ui->tableWidget_Productes->currentRow()).at(0).toInt());
        }
        else if (ui->checkBox_Ofertes->isChecked())
        {
            // Ofertes
            indexProductes.push_back(llistaProductes_siOfertes.at(ui->tableWidget_Productes->currentRow()).at(0).toInt());
        }
    }
}

void dialog_selecciooperacions::edicioPreus_TableWidget()
{   /* Aquesta funció s'activa quan s'ha de canviar el preu de la taula de productes */
    ui->tableWidget_Productes->model()->blockSignals(true);
    disconnect(ui->tableWidget_Productes, SIGNAL(cellChanged(int, int)), this, SLOT(posicioProducte()));
    if (ui->checkBox_edicioPreu->isChecked())
    {
        QMessageBox::warning(this, "AVÍS!!","Escriu directament el nou preu per actualitzar-lo");
        /* La següent línia permet editar sobreescribint a la casella */
        ui->tableWidget_Productes->setEditTriggers(QAbstractItemView::AnyKeyPressed);
        // PROVA
        ui->tableWidget_Productes->model()->blockSignals(true);
        connect(ui->tableWidget_Productes, SIGNAL(cellChanged(int, int)), this, SLOT(posicioProducte()));
        ui->tableWidget_Productes->model()->blockSignals(false);
    }
    ui->tableWidget_Productes->model()->blockSignals(false);
}

void dialog_selecciooperacions::posicioProducte()
{   /* aquesta funció només s'activa quan s'ha canviat una cel·la de la taula Productes */
    ui->tableWidget_Productes->model()->blockSignals(true);
    QMessageBox::information(this, "a", "holaaaaaa cel·la");

    //qDebug() << "Producte:\t" << llistaProductes_noOfertes.at(ui->tableWidget_Productes->currentRow()).at(0);
    //qDebug() << "Nou preu:\t" << ui->tableWidget_Productes->item(ui->tableWidget_Productes->currentRow(), 4)->text();

    if (ui->checkBox_Ofertes->isChecked())
    {
        float preu = ui->tableWidget_Productes->item(ui->tableWidget_Productes->currentRow(), 4)->text().toFloat();
        uint32_t producte = llistaProductes_siOfertes.at(ui->tableWidget_Productes->currentRow()).at(0).toUInt();
        uint16_t beneficiari = ui->comboBox->currentIndex();
        actualitzaPreuOfertes(preu, producte, beneficiari);
        carregaLlista();
    }
    else if (!ui->checkBox_Ofertes->isChecked())
    {
        float preu = ui->tableWidget_Productes->item(ui->tableWidget_Productes->currentRow(), 4)->text().toFloat();
        uint32_t producte = llistaProductes_noOfertes.at(ui->tableWidget_Productes->currentRow()).at(0).toUInt();
        uint16_t beneficiari = ui->comboBox->currentIndex();
        actualitzaPreu(preu, producte, beneficiari);
        carregaLlista();
    }

    ui->tableWidget_Productes->model()->blockSignals(false);
}

void dialog_selecciooperacions::on_pushButton_eliminaProducte_clicked()
{   /* Aquesta funció s'activa quan s'ha d'eliminar un producte de la taula "llista productes" */
    indexProductes.removeAt(ui->tableWidget_LlistaCompra->currentRow());
    ui->tableWidget_LlistaCompra->removeRow(ui->tableWidget_LlistaCompra->currentRow());
    ui->tableWidget_LlistaCompra->selectionModel()->clearCurrentIndex();
}

void dialog_selecciooperacions::on_pushButton_crearTiquet_clicked()
{
    // vector per poder afegir al cistell
    QVector<int> idQuantitat;

    // Matriu on hi ha el producte i la quantitat respectiva
    QVector<QVector<int>> matriu;
    if (ui->tableWidget_LlistaCompra->rowCount() == 0)
    {
        QMessageBox::warning(this, "Avís!", "No es pot crear tiquet de compra sense productes.");
    }
    else
    {
        for (int i = 0; i < indexProductes.length() ; i++)
        {
            QVector<int> tempVector;
            qDebug() << "quantitat: " << QString::number(ui->tableWidget_LlistaCompra->item(i,1)->text().toInt()).toInt() << "\tfkey_producte: " << indexProductes.at(i);
            for (int j = 0; j < 1; j++)
            {
                //Dóna problemes duplica els valors afegint-hi dos de més
                tempVector.push_back(ui->tableWidget_LlistaCompra->item(i,1)->text().toInt());
                tempVector.push_back(indexProductes.at(i));
            }
            matriu.push_back(tempVector);
        }
    }

    for (int i=0; i < indexProductes.length(); i++)
    {
        std::swap(matriu[i][0], matriu[i][1]);
    }

    std::sort(matriu.begin(),matriu.end() ,[](const QVector<int>& left,const QVector<int>& right)->bool{
                if(left.empty() && right.empty())
                    return false;
                if(left.empty())
                    return true;
                if(right.empty())
                    return false;
                return left.first()<right.first();
            }
        );

    for (int i=0; i < indexProductes.length(); i++)
    {
        std::swap(matriu[i][0], matriu[i][1]);
    }

    std::vector<uint32_t> vectorQuantitat;
    vectorQuantitat = troba_id_quantitat(matriu);
    std::sort(vectorQuantitat.begin(), vectorQuantitat.end());

    uint32_t resultat_id_cistell;
    resultat_id_cistell = troba_id_cistell(troba_id_quantitat(matriu));

    QVector<QVector<QString>> tiquetProds = mostrar_ultim_tiquet();
    missatges_qmessagebox("Tiquet " + QString::number(resultat_id_cistell),
                          "Beneficiari ("
                          + QString::number(ui->comboBox->currentIndex()) + ") "
                          + ui->comboBox->currentText()
                          + "\n\nProductes:\n"
                          + tiquetProds.at(0).at(0), "");
    qDebug() << "La data de la compra: " << ui->label_6->text();
    qDebug() << "ID cistell:\t" << resultat_id_cistell;
    qDebug() << "Beneficiari:\t" << ui->comboBox->currentIndex();

}


void dialog_selecciooperacions::on_pushButton_clicked()
{
    float OOOU = mostrar_preu_ultim_tiquet();
    qDebug() << "Preu del últim tiquet:\t" << OOOU;
}
