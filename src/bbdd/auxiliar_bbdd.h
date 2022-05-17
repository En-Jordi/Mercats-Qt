#ifndef AUXILIAR_BBDD_H
#define AUXILIAR_BBDD_H

#include "parametres_Configuracio.h"

#include <QSqlDatabase>
#include <QSqlRecord>
#include <QSqlError>
#include <QSqlQuery>
#include <QVariant>

#include <QMessageBox>
#include <QStringList>
#include <QString>
#include <QVector>

#include <cstdint>

// Pels missatges
void missatges_qmessagebox(QString titol, QString missatge, QString aux);

// Connecta la Base de Dades
QSqlDatabase obrirBBDD();
void tancaBBDD(QSqlDatabase *db);

QVector<QVector<QString>> creaInici_taula_temporal_hostia();

// Afegir un comerç
void inserirComerc(QString nom);

// Retorna la llista de comerços (Beneficiaris)
QStringList retornaComercos();

// Retorna TOTS els productes de la taula PRODUCTE
QVector<QVector<QString>> retornaProductesGeneral(uint8_t CONTROL, uint8_t OFERTES);

// Actualitza preu NO ofertes
void actualitzaPreu(float preu, uint32_t id_Producte, uint16_t beneficiari);

// Actualitza preu SI ofertes
void actualitzaPreuOfertes(float preu, uint32_t id_Producte, uint16_t beneficiari);

// Funció per crear a quantitat
uint32_t creaQuantitat(uint32_t quantitat, uint32_t fkey_producte, float llast, uint8_t CONTROL_COMERC, QSqlDatabase *db);

// Troba el id de la taula quantitat
std::vector<uint32_t> troba_id_quantitat(QVector<QVector<int>> matrix);

// Troba el id de la taula cistell
// vectorProductes és el vector dels identificadors de la f(x) troba_id_quantitat
uint32_t troba_id_cistell(std::vector<uint32_t> vectorProductes);

// Per a crear el cistell de la compra
uint32_t crea_cistell_compra(std::vector<uint32_t> vector, QSqlDatabase *db);

// Funció que retornarà la quantitat, descripció i el preu del cistell de la compra
QVector<QVector<QString>> mostrar_ultim_tiquet();

// Funció per mostrar el preu del últim tiquet.
float mostrar_preu_ultim_tiquet();

// Funció per trobar el ID del producte en 2a oferta
uint32_t troba_id_oferta_segona_unitat(uint32_t producte, uint32_t comerc);

#endif // AUXILIAR_BBDD_H
