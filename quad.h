#ifndef QUAD_H
#define QUAD_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Quad {
    char op[16];
    char arg1[64];
    char arg2[64];
    char res[64];
} Quad;

typedef struct ExprInfo {
    char  type[10];
    int   isConst;
    int   isInt;
    int   ival;
    float fval;
    char  place[64];
} ExprInfo;

typedef struct IfCtx    { char faux[32]; char fin[32]; int aElse; }                      IfCtx;
typedef struct WhileCtx { char debut[32]; char fin[32]; }                                WhileCtx;
typedef struct ForCtx   { char var[15]; char limite[64]; char debut[32]; char fin[32]; } ForCtx;

extern Quad quads[1000];
extern int  qc;
extern int  cptTemp;

ExprInfo *creer_expr(const char *type, int isConst, int isInt,
                     int ival, float fval, const char *place);
void      liberer_expr(ExprInfo *e);

void ajouterQuad(const char *op, const char *arg1,
                 const char *arg2, const char *res);
void nouveauTemp(char *buf);
void afficherQuads(void);

ExprInfo *expr_arith  (ExprInfo *a, ExprInfo *b, char op);
ExprInfo *expr_comp   (ExprInfo *a, ExprInfo *b, const char *opstr, int code);
ExprInfo *expr_logique(ExprInfo *a, ExprInfo *b, const char *opstr, int code);
ExprInfo *expr_non    (ExprInfo *a);

void debut_if        (ExprInfo *cond);
void partie_else     (void);
void fin_if_sans_else(void);
void fin_if_avec_else(void);

void debut_while    (void);
void condition_while(ExprInfo *cond);
void fin_while      (void);

void debut_for(const char *var, ExprInfo *init, ExprInfo *lim);
void fin_for  (void);

#endif
