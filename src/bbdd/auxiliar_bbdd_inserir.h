#ifndef AUXILIAR_BBDD_INSERIR_H
#define AUXILIAR_BBDD_INSERIR_H

#include "parametres_Configuracio.h"

#include <QString>

typedef struct estructura
{
    int h;
}cosesInserir;

void inserirBeneficiari(QString *nomComerc);
void inserirProducte(cosesInserir estructura);

#endif // AUXILIAR_BBDD_INSERIR_H
