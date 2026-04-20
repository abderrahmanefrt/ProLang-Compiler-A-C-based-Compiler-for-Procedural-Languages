#ifndef OPTIM_H
#define OPTIM_H

#include "quad.h"



void optimiser(void);

void afficherQuadsOptimises(void);

int propagationCopie      (void);
int propagationExpression (void);
int eliminationRedondantes(void);
int simplificationAlgebrique(void);
int eliminationCodeInutile(void);

#endif
