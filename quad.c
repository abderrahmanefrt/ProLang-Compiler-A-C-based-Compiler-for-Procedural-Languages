#include "quad.h"


Quad quad[1000];
int  qc      = 0;
int  cptTemp = 0;



/* Ajoute un quadruplet (oper, op1, op2, res)*/
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


ExprInfo *creerExpr(const char *type, const char *place,
                    int isConst, int isInt, int ival, float fval)
{
    ExprInfo *e = (ExprInfo *)malloc(sizeof(ExprInfo));
    if (e == NULL) { fprintf(stderr, "Erreur memoire.\n"); exit(1); }
    strcpy(e->type,  type  ? type  : "");
    strncpy(e->place, place ? place : "", 63);
    e->place[63] = '\0';
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

/* ---- Expression arithmetique : a op b ---- */
ExprInfo *exprArith(ExprInfo *a, ExprInfo *b, char op)
{
    char temp[32], opstr[4];

    if (a == NULL || b == NULL) {
        libererExpr(a); libererExpr(b);
        return creerExpr("", "", 0, 0, 0, 0.0f);
    }

    /* Detection division par zero */
    if (op == '/' && b->isConst && b->isInt && b->ival == 0)
        fprintf(stderr, "Erreur semantique : division par zero\n");

    /* Generation du quadruplet */
    nouveauTemp(temp);
    opstr[0] = op; opstr[1] = '\0';
    ajouterQuad(opstr, a->place, b->place, temp);

    /* Determination du type resultat */
    if (op == '/' || strcmp(a->type,"float")==0 || strcmp(b->type,"float")==0) {
        ExprInfo *r = creerExpr("float", temp, 0, 0, 0, 0.0f);
        libererExpr(a); libererExpr(b);
        return r;
    } else {
        ExprInfo *r = creerExpr("integer", temp, 0, 1, 0, 0.0f);
        libererExpr(a); libererExpr(b);
        return r;
    }
}

/* ---- Expression de comparaison : a opstr b  (resultat dans Tx) ---- */
ExprInfo *exprComp(ExprInfo *a, ExprInfo *b, const char *opstr, int code)
{
    char temp[32];
    (void)code; 

    if (a == NULL || b == NULL) {
        libererExpr(a); libererExpr(b);
        return creerExpr("integer", "", 0, 1, 0, 0.0f);
    }

    nouveauTemp(temp);
    ajouterQuad((char*)opstr, a->place, b->place, temp);

    libererExpr(a); libererExpr(b);
    return creerExpr("integer", temp, 0, 1, 0, 0.0f);
}

/* ---- Expression logique : a opstr b ---- */
ExprInfo *exprLogique(ExprInfo *a, ExprInfo *b, const char *opstr, int code)
{
    char temp[32];
    (void)code;

    if (a == NULL || b == NULL) {
        libererExpr(a); libererExpr(b);
        return creerExpr("integer", "", 0, 1, 0, 0.0f);
    }

    nouveauTemp(temp);
    ajouterQuad((char*)opstr, a->place, b->place, temp);

    libererExpr(a); libererExpr(b);
    return creerExpr("integer", temp, 0, 1, 0, 0.0f);
}

/* ---- NON(a) ---- */
ExprInfo *exprNon(ExprInfo *a)
{
    char temp[32];

    if (a == NULL) return creerExpr("integer", "", 0, 1, 0, 0.0f);

    nouveauTemp(temp);
    ajouterQuad("NON", a->place, "vide", temp);

    libererExpr(a);
    return creerExpr("integer", temp, 0, 1, 0, 0.0f);
}



int debut_if(ExprInfo *cond)
{
    int idx = qc;
    ajouterQuad("BZ", "vide", cond->place, "vide");
    return idx;
}


int partie_else(int idx_bz)
{
    int idx_br = qc;
    char tmp[20];

    ajouterQuad("BR", "vide", "vide", "vide");

    sprintf(tmp, "%d", qc);
    updateQuad(idx_bz, 1, tmp);

    return idx_br;
}


void fin_if(int idx_br)
{
    char tmp[20];
    sprintf(tmp, "%d", qc);
    updateQuad(idx_br, 1, tmp);
}

void fin_if_sans_else(int idx_bz)
{
    char tmp[20];
    sprintf(tmp, "%d", qc);
    updateQuad(idx_bz, 1, tmp);
}



int debut_while(void)
{
    return qc;
}

int conditionWhile(ExprInfo *cond)
{
    int idx = qc;
    ajouterQuad("BZ", "vide", cond->place, "vide");
    return idx;
}

void fin_while(int idx_debut, int idx_bz)
{
    char tmp[20];

    /* Retour au debut */
    sprintf(tmp, "%d", idx_debut);
    ajouterQuad("BR", tmp, "vide", "vide");

    sprintf(tmp, "%d", qc);
    updateQuad(idx_bz, 1, tmp);
}


int debut_for(const char *var, ExprInfo *init, ExprInfo *lim)
{
    char tempLim[32], tempCond[32], tmp[20];
    int  idx_debut, idx_bz;

    /* Affectation de la valeur initiale */
    ajouterQuad(":=", init->place, "vide", (char*)var);

    nouveauTemp(tempLim);
    ajouterQuad(":=", lim->place, "vide", tempLim);

    /* Debut de la boucle = prochain numero */
    idx_debut = qc;

    /* Test de la condition : var <= limite */
    nouveauTemp(tempCond);
    ajouterQuad("<=", (char*)var, tempLim, tempCond);

    idx_bz = qc;
    sprintf(tmp, "%d", idx_debut);
    ajouterQuad("BZ", tmp, tempCond, (char*)var);


    return idx_bz;
}


void fin_for(int idx_bz, const char *var)
{
    char tempIncr[32], tmp[20];
    int  idx_debut;

    /* Recuperer idx_debut et var depuis le quadruplet BZ */
    idx_debut = atoi(quad[idx_bz].op1);
    /* var peut etre passe en parametre OU lu via quad[idx_bz].res */

    /* Incrementation : var = var + 1 */
    nouveauTemp(tempIncr);
    ajouterQuad("+",  (char*)var, "1",    tempIncr);
    ajouterQuad(":=", tempIncr,  "vide",  (char*)var);

    /* Saut retour au debut de la boucle */
    sprintf(tmp, "%d", idx_debut);
    ajouterQuad("BR", tmp, "vide", "vide");

    /* Backpatching du BZ : sa destination reelle = apres le BR */
    sprintf(tmp, "%d", qc);
    updateQuad(idx_bz, 1, tmp);
}
