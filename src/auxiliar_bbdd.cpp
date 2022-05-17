#include "bbdd/auxiliar_bbdd.h"
#include <QDebug>
#include <QVector>

#define longitudDades 6

void missatges_qmessagebox(QString titol, QString missatge, QString aux)
{
    QMessageBox error_Obrint;
    error_Obrint.setWindowTitle(titol);
    error_Obrint.setIcon(QMessageBox::Warning);
    error_Obrint.setText(missatge +  aux);
    error_Obrint.setStandardButtons(QMessageBox::Ok);
    error_Obrint.exec();
}

QSqlDatabase obrirBBDD()
{
    QSqlDatabase db = QSqlDatabase::addDatabase(driverDatabase);
    db.setHostName(ip_bbdd);
    db.setPort(port_bbdd);
    db.setDatabaseName(bbdd);
    db.setUserName(usuari_bbdd);
    db.setPassword(contrasenya_bbdd);

    if (!db.open())
    {
        missatges_qmessagebox("Alerta FALLO!", db.lastError().text(), "");
        exit(1);
    }
    else
    {
        return db;
    }
}

void tancaBBDD(QSqlDatabase *db)
{
    //QSqlDatabase db = QSqlDatabase::database();
    db->close();
    QSqlDatabase::removeDatabase( QSqlDatabase::defaultConnection );
}

QStringList retornaComercos()
{
    QStringList comercos;
    QSqlDatabase db = obrirBBDD();

    QSqlQuery * queryID = new QSqlQuery(db);
    queryID->setForwardOnly(true);

    if (!queryID->prepare("SELECT comerc FROM beneficiari ORDER BY id ASC"))
    {
        missatges_qmessagebox("Alerta FALLO!", "Fallo:  ", db.lastError().text());
    }
    else
    {
        bool queryRes = queryID->exec();
        if (queryID->lastError().type() != QSqlError::NoError || !queryRes)
        {
            missatges_qmessagebox("Alerta!", "No processat!", "");
        }
        else
        {
            while (queryID->next())
            {
                comercos.append(queryID->value(0).toString());
            }
        }
    }

    tancaBBDD(&db);
    return comercos;
}

void inserirComerc(QString nom)
{
    QSqlDatabase db = obrirBBDD();

    QSqlQuery * inserir = new QSqlQuery(db);
    inserir->setForwardOnly(true);

    inserir->prepare("INSERT INTO beneficiari (comerc) SELECT :supers WHERE NOT EXISTS (SELECT * FROM beneficiari WHERE comerc = :supers)");
    inserir->bindValue(":supers", nom);
    inserir->exec();

    tancaBBDD(&db);
}

QVector<QVector<QString>> creaInici_taula_temporal_hostia()
{
    QVector<QVector<QString>> retornaTot;
    QSqlDatabase db = obrirBBDD();

    /*
     do $$ declare sortida1 record; unga refcursor; begin create temporary table hostia (nom text, familia text, descripcio text, ean text, pvp numeric(10,2)); for comptador in (select min(id) from beneficiari)..(select max(id) from beneficiari) loop open unga for execute 'SELECT DISTINCT ON (pr.descripcio) pr.nom,pr.familia,pr.descripcio,pr.ean,hi.pvp FROM producte AS pr INNER JOIN historicpreus AS hi ON hi.fkey_producte = pr.id INNER JOIN beneficiari AS bf ON bf.id = hi.fkey_beneficiari WHERE bf.id = ' || comptador || ' ORDER BY pr.descripcio,(SELECT NOW()) <-> hi.data ASC'; loop fetch unga into sortida1; exit when not found; insert into hostia values(sortida1.nom,sortida1.familia,sortida1.descripcio,sortida1.ean,sortida1.pvp); end loop; close unga; end loop; end;$$;
    */

    QSqlQuery * creaHostia = new QSqlQuery(db);
    creaHostia->setForwardOnly(true);
    creaHostia->exec("do $$ declare sortida1 record; unga refcursor; begin create temporary table hostia (nom text, familia text, descripcio text, ean text, pvp numeric(10,2)); for comptador in (select min(id) from beneficiari)..(select max(id) from beneficiari) loop open unga for execute 'SELECT DISTINCT ON (pr.descripcio) pr.nom,pr.familia,pr.descripcio,pr.ean,hi.pvp FROM producte AS pr INNER JOIN historicpreus AS hi ON hi.fkey_producte = pr.id INNER JOIN beneficiari AS bf ON bf.id = hi.fkey_beneficiari WHERE bf.id = ' || comptador || ' ORDER BY pr.descripcio,(SELECT NOW()) <-> hi.data ASC'; loop fetch unga into sortida1; exit when not found; insert into hostia values(sortida1.nom,sortida1.familia,sortida1.descripcio,sortida1.ean,sortida1.pvp); end loop; close unga; end loop; end;$$");

    QSqlQuery * queryID = new QSqlQuery(db);
    queryID->setForwardOnly(true);

    if (!queryID->prepare("SELECT nom,familia,descripcio,ean,pvp FROM hostia ORDER BY nom ASC"))
    {
        missatges_qmessagebox("Alerta FALLO!", "Fallo:  ", db.lastError().text());
    }
    else
    {
        bool queryRes = queryID->exec();
        if (queryID->lastError().type() != QSqlError::NoError || !queryRes)
        {
            missatges_qmessagebox("Alerta!", "No processat!", "");
        }
        else
        {
            while (queryID->next())
            {
                QVector<QString> tempVector;
                for (int i=0 ; i< 5; i++)
                {
                    tempVector.push_back(queryID->value(i).toString());
                }
                retornaTot.push_back(tempVector);
            }
        }
    }

    tancaBBDD(&db);
    return retornaTot;
}

/*
   CONTROL == 0  -->  TOTS els comerços
   CONTROL != 0  -->  discernim el comerç
*/
QVector<QVector<QString>> retornaProductesGeneral(uint8_t CONTROL, uint8_t OFERTES)
{
    /*
     * Retorna TOTS els productes
     */
    QVector<QVector<QString>> productesGenera;
    //https://evileg.com/en/post/111/

    QSqlDatabase db = obrirBBDD();

    QSqlQuery * queryID = new QSqlQuery(db);
    queryID->setForwardOnly(true);

    if (CONTROL == 0 && OFERTES == 2)
    {
        // NO discriminem per ofertes
        // es cridada per   dialog_seleccioOperacions::carregaLlista_Inici
        if (!queryID->prepare("SELECT hi.id,pr.nom,pr.familia,pr.descripcio,pr.ean,hi.pvp FROM producte as pr INNER JOIN historicpreus AS hi ON hi.fkey_producte = pr.id ORDER BY pr.nom ASC"))
        {
            missatges_qmessagebox("Alerta FALLO!", "Fallo:  ", db.lastError().text());
        }
        else
        {
            bool queryRes = queryID->exec();
            if (queryID->lastError().type() != QSqlError::NoError || !queryRes)
            {
                missatges_qmessagebox("Alerta!", "No processat!", "");
            }
            else
            {
                while (queryID->next())
                {
                    QVector<QString> tempVector;
                    for (int i=0 ; i< longitudDades; i++)
                    {
                        tempVector.push_back(queryID->value(i).toString());
                    }
                    productesGenera.push_back(tempVector);
                }
            }
        }
        return productesGenera;
    }
    else if (CONTROL == 0 && OFERTES == 0)
    {
        // SI discriminem per ofertes = no
        // Comerços: TOTS (CONTROL == 0)
        // es cridada per   dialog_seleccioOperacions::carregaLlista_Inici
        if (!queryID->prepare("SELECT hi.id,pr.nom,pr.familia,pr.descripcio,pr.ean,hi.pvp FROM producte as pr INNER JOIN historicpreus AS hi ON hi.fkey_producte = pr.id WHERE hi.ofertes = 'no' ORDER BY pr.nom ASC"))
        {
            missatges_qmessagebox("Alerta FALLO!", "Fallo:  ", db.lastError().text());
        }
        else
        {
            bool queryRes = queryID->exec();
            if (queryID->lastError().type() != QSqlError::NoError || !queryRes)
            {
                missatges_qmessagebox("Alerta!", "No processat!", "");
            }
            else
            {
                while (queryID->next())
                {
                    QVector<QString> tempVector;
                    for (int i=0 ; i< longitudDades; i++)
                    {
                        tempVector.push_back(queryID->value(i).toString());
                    }
                    productesGenera.push_back(tempVector);
                }
            }
        }
        return productesGenera;
    }
    else if (CONTROL == 0 && OFERTES == 1)
    {
        // SI discriminem per ofertes = si
        // Comerços: TOTS (CONTROL == 1)
        // es cridada per   dialog_seleccioOperacions::carregaLlista_Inici
        missatges_qmessagebox("ieeeeee", "Entrant a les ofertes","");
        if (!queryID->prepare("SELECT hi.id,pr.nom,pr.familia,pr.descripcio,pr.ean,hi.pvp FROM producte as pr INNER JOIN historicpreus AS hi ON hi.fkey_producte = pr.id WHERE hi.ofertes = 'si' ORDER BY pr.nom ASC"))
        {
            missatges_qmessagebox("Alerta FALLO!", "Fallo:  ", db.lastError().text());
        }
        else
        {
            bool queryRes = queryID->exec();
            if (queryID->lastError().type() != QSqlError::NoError || !queryRes)
            {
                missatges_qmessagebox("Alerta!", "No processat!", "");
            }
            else
            {
                while (queryID->next())
                {
                    QVector<QString> tempVector;
                    for (int i=0 ; i< longitudDades; i++)
                    {
                        tempVector.push_back(queryID->value(i).toString());
                    }
                    productesGenera.push_back(tempVector);
                }
            }
        }
        return productesGenera;
    }
    else if (CONTROL != 0 && OFERTES == 0) //OFERTES = NO
    {
        // VALOR_CONTROL, el ID del comerç
        queryID->prepare("SELECT DISTINCT ON (pr.descripcio) hi.id,pr.nom,pr.familia,pr.descripcio,pr.ean,hi.pvp FROM producte AS pr INNER JOIN historicpreus AS hi ON hi.fkey_producte = pr.id INNER JOIN beneficiari AS bf ON bf.id = hi.fkey_beneficiari WHERE bf.id = :VALOR_CONTROL AND hi.ofertes = 'no' AND hi.stock = 't' ORDER BY pr.descripcio,(SELECT NOW()) <-> hi.data ASC");
        queryID->bindValue(":VALOR_CONTROL", CONTROL);

        if (queryID->exec())
        {
            while (queryID->next())
            {
                QVector<QString> tempVector;
                for (int i=0 ; i< longitudDades; i++)
                {
                    tempVector.push_back(queryID->value(i).toString());
                    //if (i==4) qDebug() << queryID->value(i).toString();
                }
                productesGenera.push_back(tempVector);
            }
            //qDebug() << "Productes llistat:\t" << productesGenera;
            return productesGenera;
        }
        else
        {
            if (queryID->lastError().type() != QSqlError::NoError || !queryID->exec())
            {
                missatges_qmessagebox("Alerta!", "No processat!", "");
            }
        }
    }
    else if (CONTROL != 0 && OFERTES == 1) //OFERTES = SI
    {// binValue del magatzemIntec2!!!!
        queryID->prepare("SELECT DISTINCT ON (pr.descripcio) hi.id,pr.nom,pr.familia,pr.descripcio,pr.ean,hi.pvp FROM producte AS pr INNER JOIN historicpreus AS hi ON hi.fkey_producte = pr.id INNER JOIN beneficiari AS bf ON bf.id = hi.fkey_beneficiari WHERE bf.id = :VALOR_CONTROL AND hi.ofertes = 'si' AND hi.stock = 't' ORDER BY pr.descripcio,(SELECT NOW()) <-> hi.data ASC");
        queryID->bindValue(":VALOR_CONTROL", CONTROL);

        if (queryID->exec())
        {
            while (queryID->next())
            {
                QVector<QString> tempVector;
                for (int i=0 ; i< longitudDades; i++)
                {
                    tempVector.push_back(queryID->value(i).toString());
                    //if (i==4) qDebug() << queryID->value(i).toString();
                }
                productesGenera.push_back(tempVector);
            }
            return productesGenera;
        }
        else
        {
            if (queryID->lastError().type() != QSqlError::NoError || !queryID->exec())
            {
                missatges_qmessagebox("Alerta!", "No processat!", "");
            }
        }
    }

    tancaBBDD(&db);
}

void actualitzaPreu(float preu, uint32_t id_Producte, uint16_t beneficiari)
{
    QSqlDatabase db = obrirBBDD();
    uint32_t prod;

    QSqlQuery * queryID = new QSqlQuery(db);
    queryID->setForwardOnly(true);

    QSqlQuery * queryPRODUCTE = new QSqlQuery(db);
    queryPRODUCTE->setForwardOnly(true);

    queryPRODUCTE->prepare("SELECT fkey_producte FROM historicpreus WHERE id = :idGeneral ");
    queryPRODUCTE->bindValue(":idGeneral", id_Producte);
    queryPRODUCTE->exec();
    while (queryPRODUCTE->next())
    {
        //prod = queryPRODUCTE->value(0).toInt();
        prod = queryPRODUCTE->value(0).toInt();
    }

    qDebug() << "aa\t" << prod;

    queryID->prepare("INSERT INTO historicpreus (pvp,data,ofertes,fkey_producte,fkey_beneficiari) SELECT :PREU,(SELECT current_date),'no',:idProducte,:idBeneficiari WHERE NOT EXISTS ( SELECT * FROM historicpreus WHERE pvp = :PREU AND ofertes = 'no' AND fkey_beneficiari = :idBeneficiari AND fkey_producte = :idProducte )");
    queryID->bindValue(":PREU", preu);
    queryID->bindValue(":idProducte", prod);
    queryID->bindValue(":idBeneficiari", beneficiari);
    queryID->exec();

    qDebug() << "Mira els resultats!";

    tancaBBDD(&db);
}

void actualitzaPreuOfertes(float preu, uint32_t id_Producte, uint16_t beneficiari)
{
    QSqlDatabase db = obrirBBDD();
    uint32_t prod;

    QSqlQuery * queryID = new QSqlQuery(db);
    queryID->setForwardOnly(true);

    QSqlQuery * queryPRODUCTE = new QSqlQuery(db);
    queryPRODUCTE->setForwardOnly(true);

    queryPRODUCTE->prepare("SELECT fkey_producte FROM historicpreus WHERE id = :idGeneral ");
    queryPRODUCTE->bindValue(":idGeneral", id_Producte);
    queryPRODUCTE->exec();
    while (queryPRODUCTE->next())
    {
        prod = queryPRODUCTE->value(0).toInt();
    }

    qDebug() << "aa\t" << prod;

    queryID->prepare("INSERT INTO historicpreus (pvp,data,ofertes,fkey_producte,fkey_beneficiari) SELECT :PREU,(SELECT current_date),'si',:idProducte,:idBeneficiari WHERE NOT EXISTS ( SELECT * FROM historicpreus WHERE pvp = :PREU AND ofertes = 'si' AND fkey_beneficiari = :idBeneficiari AND fkey_producte = :idProducte )");
    queryID->bindValue(":PREU", preu);
    queryID->bindValue(":idProducte", prod);
    queryID->bindValue(":idBeneficiari", beneficiari);
    queryID->exec();

    qDebug() << "Mira els resultats!";

    tancaBBDD(&db);
}

// Versió 0:
// Insereix a public.quantitat el producte i la quantitat que no existeixen previament
// Retorna el ID.
//uint32_t creaQuantitat(uint32_t quantitat, uint32_t fkey_producte, QSqlDatabase *db)
//
// Versió 1
// Insereix a public.quantitat el producte, la quantitat i el pes que no existeixen previament
uint32_t creaQuantitat(uint32_t quantitat, uint32_t fkey_producte, float llast, QSqlDatabase *db, uint8_t CONTROL_COMERC)
{
    uint32_t valorID = 0;

    QSqlQuery * crea_id_quantitat = new QSqlQuery(*db);

    crea_id_quantitat->setForwardOnly(true);

    if (CONTROL_COMERC != 4)
    {
        crea_id_quantitat->prepare("SELECT crea_quantitat_normal(:quantitat,:fkey)");
        crea_id_quantitat->bindValue(":quantitat", quantitat);
        crea_id_quantitat->bindValue(":fkey", fkey_producte);
        crea_id_quantitat->exec();

        while (crea_id_quantitat->next())
        {
            //prod = queryPRODUCTE->value(0).toInt();
            valorID = crea_id_quantitat->record().value(0).toInt();
            //qDebug() << "funcio\t" << fkey_producte << "num:\t" << valorID;
        }
    }
    else
    {
        crea_id_quantitat->prepare("SELECT crea_quantitat_ambpes(:quantitat,:fkey, :llast)");
        crea_id_quantitat->bindValue(":quantitat", quantitat);
        crea_id_quantitat->bindValue(":fkey", fkey_producte);
        crea_id_quantitat->bindValue(":llast", llast);
        crea_id_quantitat->exec();

        while (crea_id_quantitat->next())
        {
            //prod = queryPRODUCTE->value(0).toInt();
            valorID = crea_id_quantitat->record().value(0).toInt();
            //qDebug() << "funcio\t" << fkey_producte << "num:\t" << valorID;
        }
    }

    //tancaBBDD(&db);
    return valorID;
}

/*
 SELECT troba_id_quantitat(variable1, variable2);
 Retorna el id o 0 de la taula quantitat. Si és 0, cridem la funció creaQuantitat
*/
//uint32_t troba_id_quantitat(uint32_t quantitat, uint32_t fkey)
std::vector<uint32_t> troba_id_quantitat(QVector<QVector<int>> matrix)
{
    /*
       Per cridar la funció és de la següent manera:

       SELECT troba_id_quantitat(variable1, variable2);
       on:
         variable1 és la quantitat      = matrix[i][0]
         variable2 és la fkey_historicpreus  = matrix[i][1]
    */
    std::vector<uint32_t> id_quantitat;
    int32_t resultatTemporal;
    QSqlDatabase db = obrirBBDD();

    //qDebug() << "\n\n\tLa Matriu:\n\n\t" << matrix << "\n\n";

    //matrix.push_front({0,0});

    int i = 0;

    for (i=0; i< matrix.size(); i++)
    {
        //qDebug() << "I:\t" << i << "\tquantitat:\t" << matrix[i][0] << "\tid:\t" << matrix[i][1];
        //if (i != 0) {
        // Primer busquem el id dins de la taula quantitat.
        QSqlQuery * consulta_id_quantitat = new QSqlQuery(db);
        consulta_id_quantitat->setForwardOnly(true);

        consulta_id_quantitat->prepare("SELECT troba_id_quantitat(:quantitat,:fkey)");
        consulta_id_quantitat->bindValue(":quantitat", matrix[i][0]);
        consulta_id_quantitat->bindValue(":fkey", matrix[i][1]);
        consulta_id_quantitat->exec();

        //while (consulta_id_quantitat->next())
        //{
        resultatTemporal = consulta_id_quantitat->record().value(0).toUInt();

        if (resultatTemporal != 0)
        {
            // Retorna el ID i afegir a la nova
            id_quantitat.push_back(resultatTemporal);
        }
        else if (resultatTemporal == 0)
        {
            // INSERT a quantitat
            uint32_t te = 0;
            te = creaQuantitat(matrix[i][0],matrix[i][1], &db);
            //qDebug() << "te original:\t" << te;
            // NI DE CONYA eliminar aquest if. NI DE CONYA!!!!!
            if (te == 0)
                te = creaQuantitat(matrix[i][0],matrix[i][1], &db);
            //qDebug() << "te nou\t\t" << te;
            id_quantitat.push_back(te);
        }
    }
    //qDebug() << "Vector quantitat:\t" << id_quantitat;

    tancaBBDD(&db);
    uint32_t id_cistell;
    id_cistell = troba_id_cistell(id_quantitat);
    return id_quantitat;
}

uint32_t crea_cistell_compra(std::vector<uint32_t> vector, QSqlDatabase *db)
{
    /*
       funció SQL a BBDD troba_id_cistell
       retorna ID:
       0 -> no existeix cistell
       != 0 -> retorna ID
    */

    uint32_t valorID = 0;
    std::sort(vector.begin(), vector.end());

    QSqlQuery * crea_id_cistell = new QSqlQuery(*db);
    crea_id_cistell->setForwardOnly(true);

    QString cap = "SELECT crea_cistell(";
    QString coma = ",";
    QString final = ")";
    QString unga = ":dades";

    QString cadena;
    cadena.append(cap);

    for(uint32_t i = 0; i < vector.size(); i++)
    {
        cadena.append(unga);
        cadena.append(QString::number(i));
        if (vector.size() > 1 && i < vector.size() -1)
            cadena.append(coma);
    }
    cadena.append(final);

    crea_id_cistell->prepare(cadena);

    for(uint32_t i=0; i < vector.size() ; i++)
    {
        unga.append(QString::number(i));
        crea_id_cistell->bindValue(unga, vector.at(i));
        unga.remove(QRegExp("[0-9]"));
    }

    //mirar: https://doc.qt.io/qt-5/sql-sqlstatements.html#inserting-updating-and-deleting-records
    crea_id_cistell->exec();

    valorID = crea_id_cistell->record().value(0).toInt();

    //qDebug() << "executat:\t" << valorID;

    while (crea_id_cistell->next())
    {
        valorID = crea_id_cistell->record().value(0).toInt();
        //qDebug() << "Identificador cistell:\t" << valorID;
    }


    //tancaBBDD(&db);
    return valorID;
}

uint32_t troba_id_cistell(std::vector<uint32_t> vectorProductes)
{
    QSqlDatabase db = obrirBBDD();
    int32_t resultatTemporal;
    std::vector<uint32_t> vectorTemporal;

    QSqlQuery * consulta_id_cistell = new QSqlQuery(db);
    consulta_id_cistell->setForwardOnly(true);

    QString cap = "SELECT troba_id_cistell(";
    QString coma = ",";
    QString final = ")";
	QString unga = ":dades";

    QString cadena;
	cadena.append(cap);

    //qDebug() << vectorProductes.size();

    for(uint32_t i = 0; i < vectorProductes.size(); i++)
    {
		cadena.append(unga);
        cadena.append(QString::number(i));
        if (vectorProductes.size() > 1 && i < vectorProductes.size() -1)
			cadena.append(coma);
        vectorTemporal.push_back(vectorProductes.at(i));
    }
    cadena.append(final);

    consulta_id_cistell->prepare(cadena);

    for(uint32_t i=0; i < vectorProductes.size() ; i++)
    {
        unga.append(QString::number(i));
        consulta_id_cistell->bindValue(unga, vectorProductes.at(i));
        unga.remove(QRegExp("[0-9]"));
    }

    consulta_id_cistell->exec();

    while (consulta_id_cistell->next())
    {
        resultatTemporal = consulta_id_cistell->record().value(0).toUInt();
    }

    if (resultatTemporal == 0)
    {
        // EL CISTELL NO EXISTEIX
        resultatTemporal = crea_cistell_compra(vectorTemporal, &db);
    }

    tancaBBDD(&db);
    return resultatTemporal;
}

// Retorna la quantitat, descripció i el preu de l'últim tiquet.
QVector<QVector<QString>> mostrar_ultim_tiquet()
{
    QSqlDatabase db = obrirBBDD();
    QSqlQuery * consulta_items_cistell = new QSqlQuery(db);
    consulta_items_cistell->setForwardOnly(true);
    QVector<QVector<QString>> productesGenera;

    consulta_items_cistell->prepare("SELECT quantitat,descripcio,pvp*quantitat AS preu FROM taulatiquets((SELECT MAX(id) FROM cistell_compra)) ");
    //consulta_items_cistell->bindValue(":idGeneral", identificadorCistell);
    consulta_items_cistell->exec();
    while (consulta_items_cistell->next())
    {
        QVector<QString> tempVector;
        for (int i=0 ; i< longitudDades; i++)
        {
            tempVector.push_back(consulta_items_cistell->value(i).toString());
            //if (i==4) qDebug() << queryID->value(i).toString();
        }
        productesGenera.push_back(tempVector);
    }

    //qDebug() << "El cistell " << identificadorCistell << " té " << resultat << " productes.";

    tancaBBDD(&db);
    return productesGenera;
}

// Funció per mostrar el preu del últim tiquet
float mostrar_preu_ultim_tiquet()
{
    float preuFinal;

    QSqlDatabase db = obrirBBDD();
    QSqlQuery * consulta_preu_cistell = new QSqlQuery(db);
    consulta_preu_cistell->setForwardOnly(true);
    consulta_preu_cistell->prepare("SELECT sum(preu)::float FROM (SELECT quantitat*pvp AS preu FROM taulatiquets((SELECT max(id) FROM cistell_compra))) AS preu");
    consulta_preu_cistell->exec();
    while (consulta_preu_cistell->next())
    {
        preuFinal = consulta_preu_cistell->value(0).toFloat();
    }
    qDebug() << "aaaa";

    return preuFinal;
}

uint32_t troba_id_oferta_segona_unitat(uint32_t producte, uint32_t comerc)
{
    QSqlDatabase db = obrirBBDD();
    uint32_t resultatTemporal = 0;

    QSqlQuery * consulta_id_oferta = new QSqlQuery(db);
    consulta_id_oferta->setForwardOnly(true);
    //consulta_id_oferta->prepare("DO $$; DECLARE resultat INTEGER; BEGIN resultat:=(SELECT id FROM historicpreus WHERE ofertes = '2a' AND fkey_producte = (SELECT fkey_producte FROM historicpreus WHERE id = :identificadorProducte) AND fkey_beneficiari = :identificadorComerc ORDER BY (SELECT NOW())<->data LIMIT 1); IF resultat IS NULL THEN resultat:=0; END IF; RAISE NOTICE '%', resultat; END; $$;");
    consulta_id_oferta->prepare("SELECT id FROM historicpreus WHERE ofertes = '2a' AND fkey_producte = (SELECT fkey_producte FROM historicpreus WHERE id = :identificadorProducte) AND fkey_beneficiari = :identificadorComerc ORDER BY (SELECT NOW())<->data LIMIT 1");
    consulta_id_oferta->bindValue(":identificadorProducte", producte);
    consulta_id_oferta->bindValue(":identificadorComerc", comerc);


    if (consulta_id_oferta->exec())
    {
        while (consulta_id_oferta->next())
        {
            resultatTemporal = consulta_id_oferta->record().value(0).toUInt();
            qDebug() << "\n\n\tProducte:\t" << producte << "\n\tEl resultat temporal és:\t" << resultatTemporal << "\n\n";
        }
    }
    else
    {
        if (consulta_id_oferta->lastError().type() != QSqlError::NoError || !consulta_id_oferta->exec())
        {
            missatges_qmessagebox("Error!", "No processat!", "");
        }
    }

    if (resultatTemporal == 0)
    {
        // LA OFERTA NO EXISTEIX
        missatges_qmessagebox("Error en producte", "El producte seleccionat no existeix a la BBDD.", "");
    }

    tancaBBDD(&db);
    return resultatTemporal;
}
