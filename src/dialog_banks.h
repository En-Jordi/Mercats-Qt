#ifndef DIALOG_BANKS_H
#define DIALOG_BANKS_H

#include <QDialog>

namespace Ui {
class dialog_banks;
}

class dialog_banks : public QDialog
{
    Q_OBJECT

public:
    explicit dialog_banks(QWidget *parent = nullptr);
    ~dialog_banks();

private:
    Ui::dialog_banks *ui;
};

#endif // DIALOG_BANKS_H
