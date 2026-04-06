%{
#include "ts.h"
#include "quad.h"
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

static int type_compatible(const char *gauche, const char *droite)
{
    if (strcmp(gauche, droite) == 0) return 1;
    if (strcmp(gauche,"float")==0 && strcmp(droite,"integer")==0) return 1;
    return 0;
}

static void erreur_semantique(const char *msg, const char *entite)
{
    fprintf(stderr,
        "Erreur semantique : %s, ligne %d, colonne %d, entite %s\n",
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

%start S

%%

S
    : mc_beginproject idf ';' mc_setup ':' DECLARATIONS mc_run ':' '{' INSTRUCTIONS '}' mc_endproject ';'
      {
          ajouterQuad("HALT", "", "", "");
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
          liberer_expr((ExprInfo *)$6);
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
              ajouterQuad(":=", ((ExprInfo *)$6)->place, "", $2);
          }
          free($2);
          liberer_expr((ExprInfo *)$6);
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
    : tok_pipe idf
      { ajouterAttente($2); free($2); }
    | SUITE_IDF tok_pipe idf
      { ajouterAttente($3); free($3); }
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

instaff
    : idf affect EXPR ';'
      {
          verifierVariableSimple($1);
          verifierConstanteModifiable($1);
          verifierTypeAffectation($1, (ExprInfo *)$3);
          ajouterQuad(":=", ((ExprInfo *)$3)->place, "", $1);
          free($1);
          liberer_expr((ExprInfo *)$3);
      }
    | idf '[' EXPR ']' affect EXPR ';'
      {
          verifierTableauIndice($1, (ExprInfo *)$3);
          verifierConstanteModifiable($1);
          verifierTypeAffectation($1, (ExprInfo *)$6);
          ajouterQuad("WRITETAB", ((ExprInfo *)$6)->place, ((ExprInfo *)$3)->place, $1);
          free($1);
          liberer_expr((ExprInfo *)$3);
          liberer_expr((ExprInfo *)$6);
      }
    ;

IF_PREFIX
    : mc_if '(' CONDITION ')' mc_then ':'
      {
          if (strcmp(((ExprInfo *)$3)->type, "integer") != 0)
              erreur_semantique("condition if invalide", "if");
          debut_if((ExprInfo *)$3);
          liberer_expr((ExprInfo *)$3);
      }
    ;

IF_SUITE
    : mc_endif ';'
      { fin_if_sans_else(); }
    | mc_else
      { partie_else(); }
      '{' INSTRUCTIONS '}' mc_endif ';'
      { fin_if_avec_else(); }
    ;

instif  : IF_PREFIX '{' INSTRUCTIONS '}' IF_SUITE ;

WHILE_PREFIX
    : mc_loop mc_while
      { debut_while(); }
      '(' CONDITION ')'
      {
          if (strcmp(((ExprInfo *)$5)->type, "integer") != 0)
              erreur_semantique("condition while invalide", "while");
          condition_while((ExprInfo *)$5);
          liberer_expr((ExprInfo *)$5);
      }
    ;

instwhile
    : WHILE_PREFIX '{' INSTRUCTIONS '}' mc_endloop ';'
      { fin_while(); }
    ;

FOR_PREFIX
    : mc_for idf mc_in EXPR mc_to EXPR
      {
          verifierVariableSimple($2);
          verifierConstanteModifiable($2);
          if (strcmp(getType($2), "integer") != 0)
              erreur_semantique("variable du for non entiere", $2);
          if (strcmp(((ExprInfo *)$4)->type, "integer") != 0 ||
              strcmp(((ExprInfo *)$6)->type, "integer") != 0)
              erreur_semantique("bornes du for non entieres", $2);
          verifierTypeAffectation($2, (ExprInfo *)$4);
          debut_for($2, (ExprInfo *)$4, (ExprInfo *)$6);
          free($2);
          liberer_expr((ExprInfo *)$4);
          liberer_expr((ExprInfo *)$6);
      }
    ;

instfor
    : FOR_PREFIX '{' INSTRUCTIONS '}' mc_endfor ';'
      { fin_for(); }
    ;

instout
    : mc_out '('
      { ajouterQuad("BEGIN_OUT", "", "", ""); }
      LIST_ARGS ')' ';'
      { ajouterQuad("END_OUT", "", "", ""); }
    ;

instin
    : mc_in '(' idf ')' ';'
      {
          verifierVariableSimple($3);
          verifierConstanteModifiable($3);
          ajouterQuad("IN", "", "", $3);
          free($3);
      }
    ;

LIST_ARGS
    : arg
    | LIST_ARGS ',' arg
    ;

arg
    : EXPR
      { ajouterQuad("OUT", ((ExprInfo *)$1)->place, "", ""); liberer_expr((ExprInfo *)$1); }
    | tok_chaine
      { ajouterQuad("OUT", $1, "", ""); free($1); }
    ;

CONDITION : EXPR { $$ = $1; } ;

EXPR
    : EXPR '+' EXPR
      {
          ExprInfo *a = (ExprInfo *)$1, *b = (ExprInfo *)$3;
          if (strcmp(a->type,"integer")!=0 && strcmp(a->type,"float")!=0)
              erreur_semantique("operande gauche non numerique", "+");
          if (strcmp(b->type,"integer")!=0 && strcmp(b->type,"float")!=0)
              erreur_semantique("operande droit non numerique", "+");
          $$ = (void *)expr_arith(a, b, '+');
      }
    | EXPR '-' EXPR
      {
          ExprInfo *a = (ExprInfo *)$1, *b = (ExprInfo *)$3;
          if (strcmp(a->type,"integer")!=0 && strcmp(a->type,"float")!=0)
              erreur_semantique("operande gauche non numerique", "-");
          if (strcmp(b->type,"integer")!=0 && strcmp(b->type,"float")!=0)
              erreur_semantique("operande droit non numerique", "-");
          $$ = (void *)expr_arith(a, b, '-');
      }
    | EXPR '*' EXPR
      {
          ExprInfo *a = (ExprInfo *)$1, *b = (ExprInfo *)$3;
          if (strcmp(a->type,"integer")!=0 && strcmp(a->type,"float")!=0)
              erreur_semantique("operande gauche non numerique", "*");
          if (strcmp(b->type,"integer")!=0 && strcmp(b->type,"float")!=0)
              erreur_semantique("operande droit non numerique", "*");
          $$ = (void *)expr_arith(a, b, '*');
      }
    | EXPR '/' EXPR
      {
          ExprInfo *a = (ExprInfo *)$1, *b = (ExprInfo *)$3;
          if (strcmp(a->type,"integer")!=0 && strcmp(a->type,"float")!=0)
              erreur_semantique("operande gauche non numerique", "/");
          if (strcmp(b->type,"integer")!=0 && strcmp(b->type,"float")!=0)
              erreur_semantique("operande droit non numerique", "/");
          if (b->isConst && ((b->isInt && b->ival==0)||((!b->isInt)&&b->fval==0.0f)))
              erreur_semantique("division par zero", "/");
          $$ = (void *)expr_arith(a, b, '/');
      }
    | EXPR '<' EXPR
      {
          ExprInfo *a = (ExprInfo *)$1, *b = (ExprInfo *)$3;
          if (strcmp(a->type,"integer")!=0 && strcmp(a->type,"float")!=0)
              erreur_semantique("comparaison : operande non numerique", "<");
          if (strcmp(b->type,"integer")!=0 && strcmp(b->type,"float")!=0)
              erreur_semantique("comparaison : operande non numerique", "<");
          $$ = (void *)expr_comp(a, b, "<", 1);
      }
    | EXPR '>' EXPR
      {
          ExprInfo *a = (ExprInfo *)$1, *b = (ExprInfo *)$3;
          if (strcmp(a->type,"integer")!=0 && strcmp(a->type,"float")!=0)
              erreur_semantique("comparaison : operande non numerique", ">");
          if (strcmp(b->type,"integer")!=0 && strcmp(b->type,"float")!=0)
              erreur_semantique("comparaison : operande non numerique", ">");
          $$ = (void *)expr_comp(a, b, ">", 2);
      }
    | EXPR infeg EXPR
      {
          ExprInfo *a = (ExprInfo *)$1, *b = (ExprInfo *)$3;
          if (strcmp(a->type,"integer")!=0 && strcmp(a->type,"float")!=0)
              erreur_semantique("comparaison : operande non numerique", "<=");
          if (strcmp(b->type,"integer")!=0 && strcmp(b->type,"float")!=0)
              erreur_semantique("comparaison : operande non numerique", "<=");
          $$ = (void *)expr_comp(a, b, "<=", 3);
      }
    | EXPR supeg EXPR
      {
          ExprInfo *a = (ExprInfo *)$1, *b = (ExprInfo *)$3;
          if (strcmp(a->type,"integer")!=0 && strcmp(a->type,"float")!=0)
              erreur_semantique("comparaison : operande non numerique", ">=");
          if (strcmp(b->type,"integer")!=0 && strcmp(b->type,"float")!=0)
              erreur_semantique("comparaison : operande non numerique", ">=");
          $$ = (void *)expr_comp(a, b, ">=", 4);
      }
    | EXPR egal EXPR
      {
          ExprInfo *a = (ExprInfo *)$1, *b = (ExprInfo *)$3;
          if (strcmp(a->type,"integer")!=0 && strcmp(a->type,"float")!=0)
              erreur_semantique("comparaison : operande non numerique", "==");
          if (strcmp(b->type,"integer")!=0 && strcmp(b->type,"float")!=0)
              erreur_semantique("comparaison : operande non numerique", "==");
          $$ = (void *)expr_comp(a, b, "==", 5);
      }
    | EXPR diff EXPR
      {
          ExprInfo *a = (ExprInfo *)$1, *b = (ExprInfo *)$3;
          if (strcmp(a->type,"integer")!=0 && strcmp(a->type,"float")!=0)
              erreur_semantique("comparaison : operande non numerique", "!=");
          if (strcmp(b->type,"integer")!=0 && strcmp(b->type,"float")!=0)
              erreur_semantique("comparaison : operande non numerique", "!=");
          $$ = (void *)expr_comp(a, b, "!=", 6);
      }
    | EXPR mc_and EXPR
      {
          ExprInfo *a = (ExprInfo *)$1, *b = (ExprInfo *)$3;
          if (strcmp(a->type, "integer") != 0)
              erreur_semantique("operande AND non entier", "AND");
          if (strcmp(b->type, "integer") != 0)
              erreur_semantique("operande AND non entier", "AND");
          $$ = (void *)expr_logique(a, b, "AND", 1);
      }
    | EXPR mc_or EXPR
      {
          ExprInfo *a = (ExprInfo *)$1, *b = (ExprInfo *)$3;
          if (strcmp(a->type, "integer") != 0)
              erreur_semantique("operande OR non entier", "OR");
          if (strcmp(b->type, "integer") != 0)
              erreur_semantique("operande OR non entier", "OR");
          $$ = (void *)expr_logique(a, b, "OR", 2);
      }
    | mc_non '(' EXPR ')'
      {
          ExprInfo *a = (ExprInfo *)$3;
          if (strcmp(a->type, "integer") != 0)
              erreur_semantique("operande NON non entier", "NON");
          $$ = (void *)expr_non(a);
      }
    | '(' EXPR ')'        { $$ = $2; }
    | idf
      {
          char place[64];
          verifierVariableSimple($1);
          strncpy(place, $1, 63); place[63] = '\0';
          $$ = (void *)creer_expr(getType($1), 0, 0, 0, 0.0f, place);
          free($1);
      }
    | idf '[' EXPR ']'
      {
          char temp[32];
          verifierTableauIndice($1, (ExprInfo *)$3);
          nouveauTemp(temp);
          ajouterQuad("READTAB", $1, ((ExprInfo *)$3)->place, temp);
          $$ = (void *)creer_expr(getType($1), 0, 0, 0, 0.0f, temp);
          free($1);
          liberer_expr((ExprInfo *)$3);
      }
    | cst_int
      {
          char place[64];
          sprintf(place, "%d", $1);
          $$ = (void *)creer_expr("integer", 1, 1, $1, (float)$1, place);
      }
    | cst_reel
      {
          char place[64];
          sprintf(place, "%g", (double)$1);
          $$ = (void *)creer_expr("float", 1, 0, 0, $1, place);
      }
    ;

VALEUR_LITTERALE
    : cst_int
      {
          char place[64];
          sprintf(place, "%d", $1);
          $$ = (void *)creer_expr("integer", 1, 1, $1, (float)$1, place);
      }
    | cst_reel
      {
          char place[64];
          sprintf(place, "%g", (double)$1);
          $$ = (void *)creer_expr("float", 1, 0, 0, $1, place);
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
        
        afficher();
    }
    liberer();
    return 0;
}
