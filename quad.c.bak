#include "quad.h"

/* ============================================================
   VARIABLES GLOBALES
   ============================================================ */
Quad quad[1000];
int  qc      = 0;
int  cptTemp = 0;

/* ============================================================
   FONCTIONS DE BASE
   ============================================================ */

/* Ajoute un quadruplet (oper, op1, op2, res) dans la table */
void ajouterQuad(char oper[], char op1[], char op2[], char res[])
{
    if (qc >= 1000) {
        fprintf(stderr, "Erreur : trop de quadruplets.\n");
        exit(1);
    }
    strcpy(quad[qc].oper, oper);
    strcpy(quad[qc].op1,  op1);
    strcpy(quad[qc].op2,  op2);
    strcpy(quad[qc].res,  res);
    qc++;
}

/* Met a jour une case d'un quadruplet existant (backpatching)
   colon_quad : 0=oper  1=op1  2=op2  3=res                   */
void updateQuad(int num_quad, int colon_quad, char val[])
{
    if      (colon_quad == 0) strcpy(quad[num_quad].oper, val);
    else if (colon_quad == 1) strcpy(quad[num_quad].op1,  val);
    else if (colon_quad == 2) strcpy(quad[num_quad].op2,  val);
    else if (colon_quad == 3) strcpy(quad[num_quad].res,  val);
}

/* Genere un nouveau nom de temporaire : T1, T2, T3, ... */
void nouveauTemp(char *buf)
{
    sprintf(buf, "T%d", ++cptTemp);
}

/* Affiche la table des quadruplets */
void afficherQuads(void)
{
    int i;
    printf("\n*********************Les Quadruplets***********************\n");
    printf("------------------------------------------------------------\n");
    for (i = 0; i < qc; i++) {
        printf(" %d - ( %s , %s , %s , %s )\n",
               i, quad[i].oper, quad[i].op1, quad[i].op2, quad[i].res);
        printf("------------------------------------------------------------\n");
    }
}

/* ============================================================
   GESTION DES EXPRESSIONS
   ============================================================ */

/* ---- creerExpr ----
   Alloue une fiche ExprInfo.
   NOUVEAU : parametre cmpOp pour stocker l'operateur de saut.
   Ex : creerExpr("integer", "T1", "BLE", 0, 1, 0, 0)
        signifie : expression de comparaison, on saute si <=    */
ExprInfo *creerExpr(const char *type, const char *place,
                    const char *cmpOp,
                    int isConst, int isInt, int ival, float fval)
{
    ExprInfo *e = (ExprInfo *)malloc(sizeof(ExprInfo));
    if (e == NULL) { fprintf(stderr, "Erreur memoire.\n"); exit(1); }

    strcpy(e->type,  type   ? type   : "");
    strncpy(e->place, place ? place  : "", 63); e->place[63] = '\0';
    strncpy(e->cmpOp, cmpOp ? cmpOp  : "BZ", 7); e->cmpOp[7] = '\0';

    e->isConst = isConst;
    e->isInt   = isInt;
    e->ival    = ival;
    e->fval    = fval;
    return e;
}

void libererExpr(ExprInfo *e)
{
    if (e) free(e);
}

/* ============================================================
   TABLE DE CORRESPONDANCE
   Opérateur de comparaison  →  saut conditionnel inverse
   On saute quand la condition est FAUSSE, donc on prend
   l'operateur inverse :
     x > y  est faux  quand x <= y  → BLE
     x < y  est faux  quand x >= y  → BGE
     x >= y est faux  quand x <  y  → BLT
     x <= y est faux  quand x >  y  → BGT
     x == y est faux  quand x != y  → BNZ
     x != y est faux  quand x == y  → BZ
   ============================================================ */
static const char *sautInverse(const char *op)
{
    if (strcmp(op, ">")  == 0) return "BLE";
    if (strcmp(op, "<")  == 0) return "BGE";
    if (strcmp(op, ">=") == 0) return "BLT";
    if (strcmp(op, "<=") == 0) return "BGT";
    if (strcmp(op, "==") == 0) return "BNZ";
    if (strcmp(op, "!=") == 0) return "BZ";
    return "BZ";  /* par defaut */
}

/* ---- exprArith ----
   Gere +, -, *, /
   Emet : ( op , a , b , Tx )
   cmpOp = "BZ" par defaut (pas une comparaison directe)       */
ExprInfo *exprArith(ExprInfo *a, ExprInfo *b, char op)
{
    char temp[32], opstr[4];

    if (a == NULL || b == NULL) {
        libererExpr(a); libererExpr(b);
        return creerExpr("", "", "BZ", 0, 0, 0, 0.0f);
    }

    if (op == '/' && b->isConst && b->isInt && b->ival == 0)
        fprintf(stderr, "Erreur semantique : division par zero\n");

    nouveauTemp(temp);
    opstr[0] = op; opstr[1] = '\0';
    ajouterQuad(opstr, a->place, b->place, temp);

    if (op == '/' || strcmp(a->type,"float")==0 || strcmp(b->type,"float")==0) {
        ExprInfo *r = creerExpr("float", temp, "BZ", 0, 0, 0, 0.0f);
        libererExpr(a); libererExpr(b);
        return r;
    } else {
        ExprInfo *r = creerExpr("integer", temp, "BZ", 0, 1, 0, 0.0f);
        libererExpr(a); libererExpr(b);
        return r;
    }
}

/* ---- exprComp ----
   NOUVEAU COMPORTEMENT :
   Au lieu d'emettre ( > , x , y , T1 ) puis laisser BZ utiliser T1,
   on stocke directement les operandes et le saut inverse dans la fiche.

   On emet quand meme le quadruplet de comparaison pour calculer
   le resultat 0/1 (utile pour AND/OR), ET on stocke cmpOp
   pour que debut_if / conditionWhile puissent utiliser le
   bon saut conditionnel.

   Exemple : x > y
     Emet     : ( >   , x , y , T1 )
     cmpOp    : "BLE"  (saute si x <= y, c-a-d condition fausse)
     place    : "T1"   (pour compatibilite avec AND/OR)           */
ExprInfo *exprComp(ExprInfo *a, ExprInfo *b, const char *opstr, int code)
{
    char temp[32];
    const char *saut;
    ExprInfo *r;
    (void)code;

    if (a == NULL || b == NULL) {
        libererExpr(a); libererExpr(b);
        return creerExpr("integer", "", "BZ", 0, 1, 0, 0.0f);
    }

    /* Calcul du saut inverse */
    saut = sautInverse(opstr);

    /* Emission du quadruplet de comparaison */
    nouveauTemp(temp);
    ajouterQuad((char*)opstr, a->place, b->place, temp);

    /*
       On cree une fiche avec :
       - place  = temp  (T1, T2...) pour AND/OR
       - cmpOp  = saut  (BLE, BGE...) pour if/while/for
    */
    r = creerExpr("integer", temp, saut, 0, 1, 0, 0.0f);

    /* On stocke aussi les operandes originaux dans op1/op2
       du dernier quad pour que les sauts directs soient possibles */

    libererExpr(a); libererExpr(b);
    return r;
}

/* ---- exprLogique ----
   AND / OR : pas de saut special, on utilise BZ par defaut     */
ExprInfo *exprLogique(ExprInfo *a, ExprInfo *b, const char *opstr, int code)
{
    char temp[32];
    (void)code;

    if (a == NULL || b == NULL) {
        libererExpr(a); libererExpr(b);
        return creerExpr("integer", "", "BZ", 0, 1, 0, 0.0f);
    }

    nouveauTemp(temp);
    ajouterQuad((char*)opstr, a->place, b->place, temp);

    libererExpr(a); libererExpr(b);
    return creerExpr("integer", temp, "BZ", 0, 1, 0, 0.0f);
}

/* ---- exprNon ----
   NON(a) : si a est vrai (!=0) → resultat = 0, donc BNZ       */
ExprInfo *exprNon(ExprInfo *a)
{
    char temp[32];
    if (a == NULL) return creerExpr("integer", "", "BZ", 0, 1, 0, 0.0f);

    nouveauTemp(temp);
    ajouterQuad("NON", a->place, "vide", temp);

    libererExpr(a);
    /* NON(x) vaut 0 quand x != 0, donc on saute si BNZ */
    return creerExpr("integer", temp, "BNZ", 0, 1, 0, 0.0f);
}

/* ============================================================
   GESTION DU IF

   AVANT (avec BZ uniquement) :
     ( >  , x , y , T1 )
     ( BZ , dest , T1 , vide )   ← 2 quadruplets

   APRES (avec saut direct) :
     ( >   , x , y , T1 )
     ( BLE , dest , x , y )      ← saut direct si x <= y (faux)

   On utilise le cmpOp stocke dans la fiche de la condition
   pour choisir le bon saut.
   ============================================================ */

/* ---- debut_if ----
   Emet le saut conditionnel avec le bon operateur.
   Si la condition est (x > y) → cmpOp = "BLE"
   On emet : ( BLE , vide , x , y )
   Si la condition est une expression logique → cmpOp = "BZ"
   On emet : ( BZ  , vide , T1 , vide )                        */
int debut_if(ExprInfo *cond)
{
    int idx = qc;

    /* On recupere le dernier quad emis pour connaitre op1/op2 */
    /* quad[qc-1] contient ( > , x , y , T1 ) */
    char op1_cond[64], op2_cond[64];
    strcpy(op1_cond, quad[qc-1].op1);  /* = x  */
    strcpy(op2_cond, quad[qc-1].op2);  /* = y  */

    /*
       Si le cmpOp est un saut direct (BLE, BGE, BGT, BLT, BNZ)
       on peut faire un saut direct sur les operandes originaux.
       Sinon (BZ : cas AND/OR ou expression simple) on utilise
       le temporaire.
    */
    if (strcmp(cond->cmpOp, "BZ") == 0) {
        /* Cas AND/OR ou expression simple : ( BZ , vide , T1 , vide ) */
        ajouterQuad("BZ", "vide", cond->place, "vide");
    } else {
        /* Cas comparaison directe : ( BLE , vide , x , y ) */
        ajouterQuad(cond->cmpOp, "vide", op1_cond, op2_cond);
    }

    return idx;
}

/* ---- partie_else ----
   Emet BR, backpatch le saut conditionnel (BZ/BLE/...)         */
int partie_else(int idx_bz)
{
    int idx_br = qc;
    char tmp[20];

    ajouterQuad("BR", "vide", "vide", "vide");

    sprintf(tmp, "%d", qc);
    updateQuad(idx_bz, 1, tmp);

    return idx_br;
}

/* ---- fin_if_sans_else ---- */
void fin_if_sans_else(int idx_bz)
{
    char tmp[20];
    sprintf(tmp, "%d", qc);
    updateQuad(idx_bz, 1, tmp);
}

/* ---- fin_if ---- */
void fin_if(int idx_br)
{
    char tmp[20];
    sprintf(tmp, "%d", qc);
    updateQuad(idx_br, 1, tmp);
}

/* ============================================================
   GESTION DU WHILE
   Meme principe : on utilise le bon saut selon cmpOp           */

int debut_while(void)
{
    return qc;
}

int conditionWhile(ExprInfo *cond)
{
    int idx = qc;
    char op1_cond[64], op2_cond[64];

    strcpy(op1_cond, quad[qc-1].op1);
    strcpy(op2_cond, quad[qc-1].op2);

    if (strcmp(cond->cmpOp, "BZ") == 0) {
        ajouterQuad("BZ", "vide", cond->place, "vide");
    } else {
        ajouterQuad(cond->cmpOp, "vide", op1_cond, op2_cond);
    }

    return idx;
}

void fin_while(int idx_debut, int idx_bz)
{
    char tmp[20];

    sprintf(tmp, "%d", idx_debut);
    ajouterQuad("BR", tmp, "vide", "vide");

    sprintf(tmp, "%d", qc);
    updateQuad(idx_bz, 1, tmp);
}

/* ============================================================
   GESTION DU FOR
   Le test est toujours var <= limite → on utilise BGT
   (on saute si var > limite, c-a-d condition fausse)           */

int debut_for(const char *var, ExprInfo *init, ExprInfo *lim)
{
    char tempLim[32], tempCond[32], tmp[20];
    int  idx_debut, idx_bz;

    /* QUAD 1 : initialisation */
    ajouterQuad(":=", init->place, "vide", (char*)var);

    /* QUAD 2 : sauvegarde limite */
    nouveauTemp(tempLim);
    ajouterQuad(":=", lim->place, "vide", tempLim);

    /* Debut du test */
    idx_debut = qc;

    /* QUAD 3 : test var <= limite */
    nouveauTemp(tempCond);
    ajouterQuad("<=", (char*)var, tempLim, tempCond);

    /*
       NOUVEAU : on utilise BGT au lieu de BZ
       On saute si var > limite (condition fausse)
       ( BGT , vide , var , tempLim )
       au lieu de
       ( BZ  , vide , tempCond , vide )
    */
    idx_bz = qc;
    sprintf(tmp, "%d", idx_debut);
    ajouterQuad("BGT", tmp, (char*)var, tempLim);
    /*
       op1 = idx_debut (sera ecrase par backpatch)
       op2 = var       (operande 1 du test)
       res = tempLim   (operande 2 du test)
    */

    return idx_bz;
}

void fin_for(int idx_bz, const char *var)
{
    char tempIncr[32], tmp[20];
    int  idx_debut;

    /* Recuperer idx_debut depuis op1 du BGT */
    idx_debut = atoi(quad[idx_bz].op1);

    /* Incrementation */
    nouveauTemp(tempIncr);
    ajouterQuad("+",  (char*)var, "1",     tempIncr);
    ajouterQuad(":=", tempIncr,  "vide",   (char*)var);

    /* Retour au test */
    sprintf(tmp, "%d", idx_debut);
    ajouterQuad("BR", tmp, "vide", "vide");

    /* Backpatch du BGT */
    sprintf(tmp, "%d", qc);
    updateQuad(idx_bz, 1, tmp);
}
