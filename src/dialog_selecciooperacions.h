#ifndef DIALOG_SELECCIOOPERACIONS_H
#define DIALOG_SELECCIOOPERACIONS_H

#include <QMainWindow>

#include "bbdd/auxiliar_bbdd.h"

#include <QDialog>
#include <QTableWidgetItem>

// Per mostrar els comerços
#include "dialog_beneficiaris.h"

// Per poder inserir productes
#include "dialog_inserirproductes.h"

// Per a poder consultar els productes
#include "dialog_consultaproductes.h"

namespace Ui {
class dialog_selecciooperacions;
}

class dialog_selecciooperacions : public QMainWindow
{
    Q_OBJECT

public:
    explicit dialog_selecciooperacions(QWidget *parent = nullptr);
    ~dialog_selecciooperacions();

private slots:
    void carregaLlistaInici();
    void carregaLlista();
    void discriminadorOfertes();
    //void on_tableWidget_Productes_doubleClicked(const QModelIndex &index);

    void on_tableWidget_Productes_itemDoubleClicked(QTableWidgetItem *item);

    void canviaData();
    void canviaEstatDescomptes_lineEdit();
    void edicioPreus_TableWidget();

    void posicioProducte();//QTableWidgetItem *item);

    void on_pushButton_eliminaProducte_clicked();

    void on_pushButton_crearTiquet_clicked();//QTableWidgetItem *item);

    void on_actionConsulta_triggered();

    void on_actionInserir_triggered();

    void on_actionProducte_general_triggered();

    void on_pushButton_clicked();

    void on_actionConsulta_Productes_triggered();

private:
    Ui::dialog_selecciooperacions *ui;
    QAbstractItemView::EditTriggers default_trigger;

    // Declarem "dialogs" (finestres) on farem altres operacions
    dialog_beneficiaris *nouDialog_Beneficiaris;
    dialog_inserirproductes *nouDialog_Productes;
    dialog_consultaProductes *nouDialog_consultaProductes;
};

#endif // DIALOG_SELOPERACIONS_H
