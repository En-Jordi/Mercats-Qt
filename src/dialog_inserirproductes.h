#ifndef DIALOG_INSERIRPRODUCTES_H
#define DIALOG_INSERIRPRODUCTES_H

#include "bbdd/auxiliar_bbdd.h"

#include <QDialog>


#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QTcpServer>

#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>

#ifndef PORT_TCP
#define PORT_TCP 64000
#endif

#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>

namespace Ui {
class dialog_inserirproductes;
}

class dialog_inserirproductes : public QDialog
{
    Q_OBJECT

public:
    explicit dialog_inserirproductes(QWidget *parent = nullptr);
    ~dialog_inserirproductes();

private slots:
    void on_buttonBox_accepted();

    // Funcions a ser cridades al clic del checkBox.
    void missatge_checkBox_2a();
    void missatge_checkBox_si();

    void newConnection();
    void provaNovaFuncio();

private:
    Ui::dialog_inserirproductes *ui;
    QTcpServer *server;
};

#endif // DIALOG_INSERIRPRODUCTES_H
