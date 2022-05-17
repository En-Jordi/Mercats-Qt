#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "parametres_Configuracio.h"

#include "dialog_banks.h"
#include "dialog_abouttiquets.h"
#include "dialog_beneficiaris.h"
#include "dialog_discernirbancs.h"
#include "dialog_selecciooperacions.h"

#include "qframe_extended.h"

#include <QVBoxLayout>

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionAbout_programa_triggered();

    void on_pushButton_Comptes_clicked();

    void on_actionBanc_triggered();

    void on_pushButton_Operacions_clicked();

    void on_actionBeneficiari_triggered();

private:
    Ui::MainWindow *ui;

    dialog_banks *nouDialog_Banks;
    dialog_discernirBancs *triaBanks;
    dialog_AboutTiquets *nouDialog_About;
    dialog_beneficiaris *nouDialog_Beneficiaris;
    dialog_selecciooperacions *nouDialog_Operacions;

    QFrameExtended *frames[42];
    QFrameExtended *createQFrameExtended(Date *date);

    QVBoxLayout *layout;
};
#endif // MAINWINDOW_H
