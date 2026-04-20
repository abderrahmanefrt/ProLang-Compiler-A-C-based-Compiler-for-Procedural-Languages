%{
#include "ts.h"
#include "quad.h"
#include "optim.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

extern int ligne;
extern int colonne;

int  yylex(void);
void yyerror(const char *msg);

char SauvType[20];
char attente[128][15];
int  nb_attente = 0;

/* ---- fonctions semantiques (inchangees) ---- */
static int type_compatible(const char *g, const char *d)
{
    if (strcmp(g, d) == 0) return 1;
    if (strcmp(g,"float")==0 && strcmp(d,"integer")==0) return 1;
    return 0;
}
static void erreur_semantique(const char *msg, const char *entite)
{
    fprintf(stderr, "Erreur semantique : %s, ligne %d, colonne %d, entite %s\n",
            msg, ligne, colonne, entite ? entite : "");
}
static void ajouterAttente(const char *nom)
{
    if (nb_attente < 128) {
        strncpy(attente[nb_attente], nom, 14);
        attente[nb_attente][14] = '\0';
        nb_attente++;
    }
}
static void viderAttente(void) { nb_attente = 0; }
static void appliquerTypeVar(void)
{
    int i;
    for (i = 0; i < nb_attente; i++) {
        if (rechercheType(attente[i]))
            erreur_semantique("double declaration", attente[i]);
        else {
            insererType(attente[i], SauvType);
            mettreAjourCode(attente[i], "var");
        }
    }
}
static void sauvegarder_valeur_expr(const char *nom, ExprInfo *e)
{
    char buf[64];
    if (e == NULL || !e->isConst) return;
    if (e->isInt) sprintf(buf, "%d", e->ival);
    else          sprintf(buf, "%g", (double)e->fval);
    insererValeur(nom, buf);
}
static void verifierDeclaration(const char *nom)
{
    if (!rechercheType(nom))
        erreur_semantique("identificateur non declare", nom);
}
static void verifierVariableSimple(const char *nom)
{
    verifierDeclaration(nom);
    if (estTableau(nom))
        erreur_semantique("tableau utilise sans indice", nom);
}
static void verifierConstanteModifiable(const char *nom)
{
    verifierDeclaration(nom);
    if (estConstante(nom))
        erreur_semantique("modification d'une constante", nom);
}
static void verifierTypeAffectation(const char *nom, ExprInfo *e)
{
    const char *t;
    if (e == NULL) return;
    t = getType(nom);
    if (t == NULL || strlen(t) == 0) return;
    if (!type_compatible(t, e->type))
        erreur_semantique("incompatibilite de types dans affectation", nom);
}
static void verifierTableauIndice(const char *nom, ExprInfo *idx)
{
    verifierDeclaration(nom);
    if (!estTableau(nom)) {
        erreur_semantique("identificateur non tableau utilise avec indice", nom);
        return;
    }
    if (idx == NULL) return;
    if (strcmp(idx->type, "integer") != 0)
        erreur_semantique("indice de tableau non entier", nom);
    if (idx->isConst && idx->isInt)
        if (idx->ival < 0 || idx->ival >= getTaille(nom))
            erreur_semantique("index hors limites", nom);
}
%}

%union {
    int   entier;
    float reel;
    char *str;
    void *expr;
    int   idx;   /* indice de quadruplet pour backpatching */
}

%token mc_beginproject mc_endproject mc_setup mc_run
%token mc_define mc_const mc_if mc_then mc_else mc_endif
%token mc_loop mc_while mc_endloop mc_for mc_to mc_endfor
%token mc_and mc_or mc_non mc_out mc_in err
%token affect infeg supeg egal diff tok_pipe

%token <str>    idf mc_integer mc_float tok_chaine
%token <entier> cst_int
%token <reel>   cst_reel

%left mc_and
%left mc_or
%left '<' '>' infeg supeg egal diff
%left '+' '-'
%left '*' '/'

%type <expr> EXPR VALEUR_LITTERALE CONDITION
%type <idx>  IF_PREFIX WHILE_PREFIX FOR_PREFIX
%type <idx>  IF_ELSE_PART

%start S

%%

S
    : mc_beginproject idf ';' mc_setup ':' DECLARATIONS mc_run ':' '{' INSTRUCTIONS '}' mc_endproject ';'
      {
          ajouterQuad("Fin", "vide", "vide", "vide");
          free($2);
          printf("Analyse syntaxique et semantique reussie.\n");
          YYACCEPT;
      }
    ;

DECLARATIONS
    : /* vide */
    | DECLARATIONS DEC
    ;

DEC
    : DECL_CONST
    | DECL_VARIABLE
    | DECL_TABLEAU
    ;

DECL_CONST
    : mc_const idf ':' TYPE '=' VALEUR_LITTERALE ';'
      {
          if (rechercheType($2)) {
              erreur_semantique("double declaration", $2);
          } else {
              if (!type_compatible(SauvType, ((ExprInfo *)$6)->type))
                  erreur_semantique("incompatibilite de types dans declaration constante", $2);
              mettreAjourCode($2, "const");
              insererType($2, SauvType);
              marquerConstante($2, 1);
              sauvegarder_valeur_expr($2, (ExprInfo *)$6);
          }
          free($2);
          libererExpr((ExprInfo *)$6);
      }
    ;

DECL_VARIABLE
    : mc_define idf ':' TYPE ';'
      {
          if (rechercheType($2))
              erreur_semantique("double declaration", $2);
          else {
              insererType($2, SauvType);
              mettreAjourCode($2, "var");
          }
          free($2);
      }
    | mc_define idf ':' TYPE '=' VALEUR_LITTERALE ';'
      {
          if (rechercheType($2)) {
              erreur_semantique("double declaration", $2);
          } else {
              if (!type_compatible(SauvType, ((ExprInfo *)$6)->type))
                  erreur_semantique("incompatibilite de types dans initialisation", $2);
              insererType($2, SauvType);
              mettreAjourCode($2, "var");
              sauvegarder_valeur_expr($2, (ExprInfo *)$6);
              ajouterQuad(":=", ((ExprInfo *)$6)->place, "vide", $2);
          }
          free($2);
          libererExpr((ExprInfo *)$6);
      }
    | mc_define idf SUITE_IDF ':' TYPE ';'
      {
          ajouterAttente($2);
          appliquerTypeVar();
          viderAttente();
          free($2);
      }
    ;

SUITE_IDF
    : tok_pipe idf              { ajouterAttente($2); free($2); }
    | SUITE_IDF tok_pipe idf    { ajouterAttente($3); free($3); }
    ;

DECL_TABLEAU
    : mc_define idf ':' '[' TYPE ';' cst_int ']' ';'
      {
          if (rechercheType($2)) {
              erreur_semantique("double declaration", $2);
          } else {
              if ($7 <= 0)
                  erreur_semantique("taille tableau positive requise", $2);
              else {
                  insererType($2, SauvType);
                  insererTaille($2, $7);
                  mettreAjourCode($2, "tab");
                  marquerTableau($2, 1);
              }
          }
          free($2);
      }
    ;

TYPE
    : mc_integer { strcpy(SauvType, "integer"); free($1); }
    | mc_float   { strcpy(SauvType, "float");   free($1); }
    ;

INSTRUCTIONS
    : /* vide */
    | INSTRUCTIONS INSTRUCTION
    ;

INSTRUCTION
    : instaff
    | instif
    | instwhile
    | instfor
    | instout
    | instin
    ;

/* ============================================================
   AFFECTATION
   ============================================================ */
instaff
    : idf affect EXPR ';'
      {
          verifierVariableSimple($1);
          verifierConstanteModifiable($1);
          verifierTypeAffectation($1, (ExprInfo *)$3);
          ajouterQuad(":=", ((ExprInfo *)$3)->place, "vide", $1);
          free($1);
          libererExpr((ExprInfo *)$3);
      }
    | idf '[' EXPR ']' affect EXPR ';'
      {
          verifierTableauIndice($1, (ExprInfo *)$3);
          verifierConstanteModifiable($1);
          verifierTypeAffectation($1, (ExprInfo *)$6);
          ajouterQuad("WRITETAB", ((ExprInfo *)$6)->place, ((ExprInfo *)$3)->place, $1);
          free($1);
          libererExpr((ExprInfo *)$3);
          libererExpr((ExprInfo *)$6);
      }
    ;

/* ============================================================
   INSTRUCTION IF   (backpatching style cours)

   Sous-regles pour decouper la grammaire LALR(1) :

   IF_PREFIX  : emet BZ          => retourne idx_bz
   IF_ELSE_PART : emet BR, backpatche le BZ => retourne idx_br
   instif : IF_PREFIX '{' INSTRUCTIONS '}' IF_SUITE

   ============================================================ */

/* R1 : lit la condition et emet BZ vide */
IF_PREFIX
    : mc_if '(' CONDITION ')' mc_then ':'
      {
          if (strcmp(((ExprInfo *)$3)->type, "integer") != 0)
              erreur_semantique("condition if invalide", "if");
          $$ = debut_if((ExprInfo *)$3);   /* retourne idx_bz */
          libererExpr((ExprInfo *)$3);
      }
    ;

/* R2 : rencontre du else => emet BR, backpatche le BZ */
IF_ELSE_PART
    : mc_else
      {
          $$ = partie_else($<idx>0);   /* $<idx>0 = valeur de IF_PREFIX sur la pile */
      }
    ;

instif
    /* if sans else */
    : IF_PREFIX '{' INSTRUCTIONS '}' mc_endif ';'
      {
          fin_if_sans_else($1);   /* backpatch BZ avec qc */
      }
    /* if avec else */
    | IF_PREFIX '{' INSTRUCTIONS '}' IF_ELSE_PART '{' INSTRUCTIONS '}' mc_endif ';'
      {
          fin_if($5);             /* backpatch BR avec qc */
      }
    ;

/* ============================================================
   INSTRUCTION WHILE  (backpatching style cours)

   WHILE_PREFIX : retourne idx_debut (numero avant la condition)
   On a besoin de deux indices : idx_debut et idx_bz.
   On les combine dans un seul entier encode :
       valeur = idx_debut * 10000 + idx_bz
   (valable tant que qc < 10000, ce qui est le cas)
   ============================================================ */

WHILE_PREFIX
    : mc_loop mc_while '(' CONDITION ')'
      {
          int idx_debut = debut_while();   /* = qc avant la condition */
          /* Note : la condition a deja ete evaluee par EXPR,
             mais on doit emettre BZ apres.
             On calcule ici apres que CONDITION est reduit. */
          int idx_bz    = conditionWhile((ExprInfo *)$4);
          libererExpr((ExprInfo *)$4);
          /* encode les deux indices dans un seul entier */
          $$ = idx_debut * 10000 + idx_bz;
      }
    ;

instwhile
    : WHILE_PREFIX '{' INSTRUCTIONS '}' mc_endloop ';'
      {
          int idx_debut = $1 / 10000;
          int idx_bz    = $1 % 10000;
          fin_while(idx_debut, idx_bz);
      }
    ;

/* ============================================================
   INSTRUCTION FOR  (backpatching style cours)

   FOR_PREFIX : initialise var, emet la condition, retourne idx_bz
   fin_for    : incremente, retourne, backpatche
   ============================================================ */

FOR_PREFIX
    : mc_for idf mc_in EXPR mc_to EXPR
      {
          verifierVariableSimple($2);
          verifierConstanteModifiable($2);
          if (strcmp(getType($2), "integer") != 0)
              erreur_semantique("variable du for non entiere", $2);
          if (strcmp(((ExprInfo *)$4)->type,"integer")!=0 ||
              strcmp(((ExprInfo *)$6)->type,"integer")!=0)
              erreur_semantique("bornes du for non entieres", $2);

          $$ = debut_for($2, (ExprInfo *)$4, (ExprInfo *)$6);  /* retourne idx_bz */

          free($2);
          libererExpr((ExprInfo *)$4);
          libererExpr((ExprInfo *)$6);
      }
    ;

instfor
    : FOR_PREFIX '{' INSTRUCTIONS '}' mc_endfor ';'
      {
          /* On a besoin du nom de la variable pour l'incrementation.
             Astuce : le nom est stocke dans quad[idx_bz-2].res (quad :=)
             => on le recupere directement */
          fin_for($1, quad[$1].res);   /* var stocke dans res du BZ par debut_for */
      }
    ;

/* ============================================================
   INPUT / OUTPUT
   ============================================================ */
instout
    : mc_out '('
      { ajouterQuad("BEGIN_OUT", "vide", "vide", "vide"); }
      LIST_ARGS ')' ';'
      { ajouterQuad("END_OUT", "vide", "vide", "vide"); }
    ;

instin
    : mc_in '(' idf ')' ';'
      {
          verifierVariableSimple($3);
          verifierConstanteModifiable($3);
          ajouterQuad("IN", "vide", "vide", $3);
          free($3);
      }
    ;

LIST_ARGS
    : arg
    | LIST_ARGS ',' arg
    ;

arg
    : EXPR
      { ajouterQuad("OUT", ((ExprInfo *)$1)->place, "vide", "vide");
        libererExpr((ExprInfo *)$1); }
    | tok_chaine
      { ajouterQuad("OUT", $1, "vide", "vide"); free($1); }
    ;

/* ============================================================
   EXPRESSIONS
   ============================================================ */
CONDITION : EXPR { $$ = $1; } ;

EXPR
    : EXPR '+' EXPR   { $$ = (void *)exprArith  ((ExprInfo *)$1, (ExprInfo *)$3, '+'); }
    | EXPR '-' EXPR   { $$ = (void *)exprArith  ((ExprInfo *)$1, (ExprInfo *)$3, '-'); }
    | EXPR '*' EXPR   { $$ = (void *)exprArith  ((ExprInfo *)$1, (ExprInfo *)$3, '*'); }
    | EXPR '/' EXPR   { $$ = (void *)exprArith  ((ExprInfo *)$1, (ExprInfo *)$3, '/'); }
    | EXPR '<'   EXPR { $$ = (void *)exprComp   ((ExprInfo *)$1, (ExprInfo *)$3, "<",  1); }
    | EXPR '>'   EXPR { $$ = (void *)exprComp   ((ExprInfo *)$1, (ExprInfo *)$3, ">",  2); }
    | EXPR infeg EXPR { $$ = (void *)exprComp   ((ExprInfo *)$1, (ExprInfo *)$3, "<=", 3); }
    | EXPR supeg EXPR { $$ = (void *)exprComp   ((ExprInfo *)$1, (ExprInfo *)$3, ">=", 4); }
    | EXPR egal  EXPR { $$ = (void *)exprComp   ((ExprInfo *)$1, (ExprInfo *)$3, "==", 5); }
    | EXPR diff  EXPR { $$ = (void *)exprComp   ((ExprInfo *)$1, (ExprInfo *)$3, "!=", 6); }
    | EXPR mc_and EXPR { $$ = (void *)exprLogique((ExprInfo *)$1, (ExprInfo *)$3, "AND", 1); }
    | EXPR mc_or  EXPR { $$ = (void *)exprLogique((ExprInfo *)$1, (ExprInfo *)$3, "OR",  2); }
    | mc_non '(' EXPR ')' { $$ = (void *)exprNon((ExprInfo *)$3); }
    | '(' EXPR ')'        { $$ = $2; }
    | idf
      {
          char place[64];
          verifierVariableSimple($1);
          strncpy(place, $1, 63); place[63] = '\0';
          $$ = (void *)creerExpr(getType($1), place, "BZ", 0, 0, 0, 0.0f);
          free($1);
      }
    | idf '[' EXPR ']'
      {
          char temp[32];
          verifierTableauIndice($1, (ExprInfo *)$3);
          nouveauTemp(temp);
          ajouterQuad("READTAB", $1, ((ExprInfo *)$3)->place, temp);
          $$ = (void *)creerExpr(getType($1), temp, "BZ", 0, 0, 0, 0.0f);
          free($1);
          libererExpr((ExprInfo *)$3);
      }
    | cst_int
      {
          char place[64];
          sprintf(place, "%d", $1);
          $$ = (void *)creerExpr("integer", place, "BZ", 1, 1, $1, (float)$1);
      }
    | cst_reel
      {
          char place[64];
          sprintf(place, "%g", (double)$1);
          $$ = (void *)creerExpr("float", place, "BZ", 1, 0, 0, $1);
      }
    ;

VALEUR_LITTERALE
    : cst_int
      {
          char place[64];
          sprintf(place, "%d", $1);
          $$ = (void *)creerExpr("integer", place, "BZ", 1, 1, $1, (float)$1);
      }
    | cst_reel
      {
          char place[64];
          sprintf(place, "%g", (double)$1);
          $$ = (void *)creerExpr("float", place, "BZ", 1, 0, 0, $1);
      }
    ;

%%

void yyerror(const char *msg)
{
    fprintf(stderr, "Erreur syntaxique : %s | Ligne %d, Colonne %d\n",
            msg, ligne, colonne);
}

int main(void)
{
    int r = yyparse();
    if (r == 0) {
        afficherQuads();
        optimiser();
        afficherQuadsOptimises();
        afficher();
    }
    liberer();
    return 0;
}
