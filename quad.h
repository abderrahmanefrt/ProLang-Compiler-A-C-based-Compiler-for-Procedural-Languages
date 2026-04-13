#ifndef QUAD_H
#define QUAD_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ============================================================
   STRUCTURE D'UN QUADRUPLET
   Chaque quadruplet = (operateur, operande1, operande2, resultat)
   ============================================================ */
typedef struct {
    char oper[100];
    char op1[100];
    char op2[100];
    char res[100];
} Quad;

/* ============================================================
   STRUCTURE ExprInfo
   Transporte le type et le "place" (nom de variable/temporaire)
   d'une expression entre les regles Bison.
   ============================================================ */
typedef struct {
    char type[10];   /* "integer" ou "float"          */
    char place[64];  /* nom de la variable ou Tx       */
    int  isConst;    /* 1 si valeur connue a la compil */
    int  isInt;      /* 1 si constante entiere         */
    int  ival;       /* valeur si entier constant      */
    float fval;      /* valeur si reel constant        */
} ExprInfo;

/* ============================================================
   TABLE DES QUADRUPLETS  (max 1000 quadruplets)
   ============================================================ */
extern Quad quad[1000];
extern int  qc;          /* compteur de quadruplets      */
extern int  cptTemp;     /* compteur de temporaires (T1, T2, ...) */

/* ============================================================
   FONCTIONS DE BASE
   ============================================================ */

/* Ajoute un quadruplet et incremente qc */
void ajouterQuad(char oper[], char op1[], char op2[], char res[]);

/* Met a jour une case d'un quadruplet existant :
   colon = 0:oper  1:op1  2:op2  3:res              */
void updateQuad(int num_quad, int colon_quad, char val[]);

/* Cree un nouveau nom de temporaire dans buf : T1, T2, ... */
void nouveauTemp(char *buf);

/* Affiche tous les quadruplets generes */
void afficherQuads(void);

/* ============================================================
   GESTION DES EXPRESSIONS
   ============================================================ */
ExprInfo *creerExpr(const char *type, const char *place,
                    int isConst, int isInt, int ival, float fval);
void      libererExpr(ExprInfo *e);

ExprInfo *exprArith  (ExprInfo *a, ExprInfo *b, char op);
ExprInfo *exprComp   (ExprInfo *a, ExprInfo *b, const char *opstr, int code);
ExprInfo *exprLogique(ExprInfo *a, ExprInfo *b, const char *opstr, int code);
ExprInfo *exprNon    (ExprInfo *a);

/* ============================================================
   GESTION DU IF / ELSE  (backpatching)
   - debut_if   : emet BZ avec destination vide, retourne l'indice du BZ
   - partie_else: emet BR (saut fin), retourne l'indice du BR,
                  met a jour le BZ avec la bonne destination
   - fin_if     : met a jour le BR avec la destination finale
   ============================================================ */
int  debut_if   (ExprInfo *cond);
int  partie_else(int idx_bz);
void fin_if     (int idx_br);
void fin_if_sans_else(int idx_bz);

/* ============================================================
   GESTION DU WHILE  (backpatching)
   - debut_while    : retourne le numero du LABEL debut
   - condition_while: emet BZ, retourne l'indice du BZ
   - fin_while      : emet BR vers debut, met a jour le BZ
   ============================================================ */
int  debut_while    (void);
int  conditionWhile (ExprInfo *cond);
void fin_while      (int idx_debut, int idx_bz);

/* ============================================================
   GESTION DU FOR  (backpatching)
   ============================================================ */
int  debut_for(const char *var, ExprInfo *init, ExprInfo *lim);
void fin_for  (int idx_bz, const char *var);

#endif
