#ifndef DIALOG_ABOUTTIQUETS_H
#define DIALOG_ABOUTTIQUETS_H

#include "parametres_Configuracio.h"

#include <QDialog>
#include <QPixmap>
#include <QGraphicsView>
#include <QGraphicsScene>

namespace Ui {
class dialog_AboutTiquets;
}

class dialog_AboutTiquets : public QDialog
{
    Q_OBJECT

public:
    explicit dialog_AboutTiquets(QWidget *parent = nullptr);
    ~dialog_AboutTiquets();

private:
    Ui::dialog_AboutTiquets *ui;
};

#endif // DIALOG_ABOUTTIQUETS_H
