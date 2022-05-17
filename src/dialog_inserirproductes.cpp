//  https://k2s.cc/file/97d0d59e6d844   Amy's Conquest 1
//  https://k2s.cc/file/c5aecccb1f941   Amy's Conquest 2

#include "dialog_inserirproductes.h"
#include "ui_dialog_inserirproductes.h"

#include <QDoubleValidator>
#include <QProcess>
#include <QDate>

#include <QDebug>

#define PVP_MIN 0
#define PVP_MAX 25000

QStringList llistaComercos = retornaComercos();

dialog_inserirproductes::dialog_inserirproductes(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dialog_inserirproductes)
{
    ui->setupUi(this);

    // Cerca els comerços i omple'ls en el comboBox

    ui->comboBox_comerc->addItem("");
    ui->comboBox_comerc->addItems(llistaComercos);

    QDoubleValidator *dblVal = new QDoubleValidator(PVP_MIN, PVP_MAX, 2, ui->edit_pvp);
    dblVal->setNotation(QDoubleValidator::StandardNotation);
    dblVal->setLocale(QLocale::Catalan);
    ui->edit_pvp->setValidator(dblVal);

    // Set date validator for date lineedit
    // modificar. Error en mes i dia. es pot entrar en una data passada
    QRegExp R_date("(20[2-9][1-9])-(0[1-9]|[1][0-2])-(0[1-9]|[12][0-9]|3[01])");
    QRegExpValidator *valida = new QRegExpValidator(R_date, this);
    ui->edit_data->setValidator(valida);
    ui->edit_data->setText( QDate().currentDate().toString("yyyy-MM-dd") );

    // Set place holder for date lineedit
    ui->edit_data->setPlaceholderText("Example : 2021-01-01");

    connect(ui->r_2a, SIGNAL(clicked()), this, SLOT(missatge_checkBox_2a()));
    connect(ui->r_si, SIGNAL(clicked()), this, SLOT(missatge_checkBox_si()));

    server = new QTcpServer(this);
    connect(server, SIGNAL(newConnection()), this, SLOT(newConnection()));
    if (!server->listen(QHostAddress::LocalHost, PORT_TCP))
    {
        missatges_qmessagebox("Error TCP localserver", "No s'ha pogut iniciar el servidor local", "");
    }

    connect(ui->edit_URL, SIGNAL(textChanged(const QString &)), this, SLOT(provaNovaFuncio()));
}

dialog_inserirproductes::~dialog_inserirproductes()
{
    delete ui;
}

void dialog_inserirproductes::missatge_checkBox_2a()
{
    missatges_qmessagebox("Alerta", "Introduir el PVP normal","");
}

void dialog_inserirproductes::missatge_checkBox_si()
{
    missatges_qmessagebox("Alerta", "Introduir el PVP del descompte","");
}

void dialog_inserirproductes::on_buttonBox_accepted()
{
    if (ui->comboBox_comerc->currentIndex() == 0)
    {
        missatges_qmessagebox("Error", "No es poden afegir els camps sense sel·leccionar abans un comerç.", "");
    }
    else
    {
        // Comprobar que els camps no són nuls.
        /*
        BEGIN;
        SAVEPOINT c1;
        INSERT INTO producte (nom,familia,descripcio,ean) SELECT 'Xocolata blanca','Xocolata','Xocolata blanca dulcinea 200gr','8410510014602' WHERE NOT EXISTS ( SELECT * FROM producte WHERE nom = 'Xocolata blanca' AND familia = 'Xocolata' AND descripcio = 'Xocolata blanca dulcinea 200gr' AND ean = '8410510014602');
        INSERT INTO historicpreus (pvp,data,ofertes,fkey_producte,fkey_beneficiari) SELECT '0.99','2021-11-02','no',(SELECT id FROM producte WHERE nom = 'Xocolata blanca' AND familia = 'Xocolata' AND descripcio = 'Xocolata blanca dulcinea 200gr' AND ean = '8410510014602'),(SELECT id FROM beneficiari WHERE comerc = 'Bon Area') WHERE NOT EXISTS (SELECT * FROM historicpreus WHERE pvp = '0.99' AND ofertes = 'no' AND fkey_beneficiari = (SELECT id FROM beneficiari WHERE comerc = 'Bon Area') AND fkey_producte = (SELECT id FROM producte WHERE nom = 'Xocolata blanca' AND familia = 'Xocolata' AND descripcio = 'Xocolata blanca dulcinea 200gr' AND ean = '8410510014602') );
        INSERT INTO url_bonarea (fkey_producte, url) SELECT (SELECT id FROM producte WHERE nom = 'Xocolata blanca' AND familia = 'Xocolata' AND descripcio = 'Xocolata blanca dulcinea 200gr' AND ean = '8410510014602'),'https://www.bonarea-online.com/online/producte/xocolata-blanca-dulcinea-postres-culinaria/13_0090336' WHERE NOT EXISTS (SELECT * FROM url_bonarea WHERE fkey_producte = (SELECT id FROM producte WHERE nom = 'Xocolata blanca' AND familia = 'Xocolata' AND descripcio = 'Xocolata blanca dulcinea 200gr' AND ean = '8410510014602') AND url = 'https://www.bonarea-online.com/online/producte/xocolata-blanca-dulcinea-postres-culinaria/13_0090336');
        */

        if ( ui->edit_URL->text().isEmpty() )// ui->edit_nom->text().isEmpty() || ui->edit_pvp->text().isEmpty() || ui->edit_data->text().isEmpty() || ui->edit_familia->text().isEmpty() || ui->edit_descripcio->text().isEmpty() )
        {
            missatges_qmessagebox("Error", "Revisa els camps. N'hi ha un de buit.", "");
        }
        else
        {   /*
            QProcess process;
            QString scriptFile = "/home/jordi/bbdd_SorliQT.py";

            QString pythonCommand = "python3 " + scriptFile + " " + ui->edit_URL->text();

            process.start(pythonCommand);
            process.execute(pythonCommand);
            process.waitForFinished(50000);
            auto returned = process.readAllStandardOutput();

            qDebug() << "Dades retornades:\n" << returned << "\nFinal dades retornades";
*/

            qDebug() << "Comerç:\t" << llistaComercos.at(ui->comboBox_comerc->currentIndex() - 1);
            qDebug() << "preu:\t" << ui->edit_pvp->text();
        }
    }
}

void dialog_inserirproductes::newConnection()
{
    QTcpSocket *socket = server->nextPendingConnection();

    socket->flush();
    socket->waitForBytesWritten(3000);
    qDebug() << "El puto servidor:\n" << socket->readAll();

    socket->close();
}

void dialog_inserirproductes::provaNovaFuncio()
{
    missatges_qmessagebox("HHHHHHHH", ui->edit_URL->text(), "");

    /*
       Funció que pot servir per a llegir el socket del servidor. O crear el proces a "bbdd_SorliQT.py"
    */
}
